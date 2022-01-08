/* Editor.cc -- a very basic Emacs-like editor  */

#ifdef WITH_PPL
#include <ppl.hh>
namespace PPL = Parma_Polyhedra_Library;
#endif

#include "Editor.h"
#include "Buffer.h"
#include "Cons.h"
#include <curses.h>  /* Key name hack.  Replace with key bindings.  */
#include <sstream>
#include <cassert>
#include <string.h>

#ifdef GMP
#include <cstdio>
#include <cstdlib>
#include <gmp.h>
extern "C" {
#include <expr.h>
}
#endif

#ifdef CERN

#include <cfortran.h>
#include <graflib.h>
#include <packlib.h>
#include <kernlib.h>

#if defined(__hpux) || defined(_IBMR2)
#define extname
#endif

#define PAWC_SIZE 50000

typedef struct { float PAW[PAWC_SIZE]; } PAWC_DEF;
#define PAWC COMMON_BLOCK (PAWC,pawc)
COMMON_BLOCK_DEF (PAWC_DEF,PAWC);

#endif /* CERN */

using cons::Cons;
using cons::ConsT;
using buffer::hil;

std::string Editor::version_string = "Editor 0.01";

Editor::Editor ()
{
  tab_width = 4;
  scroll_margin = 5;
  wrap_indicator = '\\';
  //buffer = vector<buffer::Buffer>::iterator ();
  buffer = NULL;
  //window = vector<SupplyWindow>::iterator ();
  window = NULL;
  echo.clear ();
  clips = clip = NULL;
  init_key_bindings ();
  memset (input_buffer, 0, INPUT_BUFFER_SIZE * sizeof (int));
}

/* Push CH onto `input_buffer', shifting rest along, losing last.  */
void
push_input (int ch, int input_buffer[])
{
  int size;

  size = INPUT_BUFFER_SIZE - 1;
  /* FIX memcpy may be better.  */
  while (size)
	input_buffer[size] = input_buffer[--size];
  input_buffer[0] = ch;
}

int
Editor::bind_sequence (char *sequence, void *func)
{
  return bind_sequence (sequence, func, NULL);
}

int
Editor::bind_sequence (char *sequence, void *func, void *param)
{
  int tem, size;
  key_map *table;

  size = strlen (sequence) - 1;
  if (size == -1)
	return 1;

  tem = -1;
  table = key_bindings;

  /* Prefix characters.  */
  while (++tem < size)
	if (table[sequence[tem]]->func)
	  {
		if (table[sequence[tem]]->func != (void*) interpret_last_input)
		  {
			echo += "Part of prefix already bound to a function\n";

			std::stringstream str;
			str << sequence[tem];
			echo += "(last char: "
			  + std::string (1, (char) sequence[tem]);
			echo += " " + str.str () + ")\n";

			return 0;
		  }
		table = (key_map*) table[sequence[tem]]->param;
	  }
	else
	  {
		key_map *new_table;
		new_table = (key_map*) calloc (KEY_TABLE_SIZE, sizeof (key_map));
		if (new_table == NULL)
		  {
			echo += "Failed to allocate key map\n";
			return 0;
		  }
		if (bind_key_to_map_in_map (sequence[tem], new_table, table)
			== 0)
		  {
			echo += "Failed to bind key to new key map\n";
			return 0;
		  }
		table = new_table;
	  }

  /* Final character.  */
  bind_key_in_map (sequence[size], func, param, table);

  return 1;
}

void*
Editor::bind_key (int key, void *func)
{
  return bind_key_in_map (key, func, key_bindings);
}

void*
Editor::bind_key (int key, void *func, void *param)
{
  return bind_key_in_map (key, func, param, key_bindings);
}

void
free_key_map (key_map *keys)
{
  int tem;
  tem = KEY_TABLE_SIZE - 1;
  while (tem--)
	if (keys[tem]->func == (void*) interpret_last_input)
	  free_key_map ((key_map*) keys[tem]->param);
  free (keys);
}

void*
Editor::bind_key_in_map (int key, void *func, key_map *keys)
{
  return bind_key_in_map (key, func, NULL, keys);
}

void*
Editor::bind_key_in_map (int key, void *func,
						 void *param, key_map *keys)
{
  if (key < KEY_TABLE_SIZE)
	{
	  void* ret;
	  ret = NULL;
	  if (keys[key]->func == (void*) interpret_last_input)
		{
		  /* Bound to a key map.  */
		  assert (keys[key]->param);
		  free_key_map ((key_map*) keys[key]->param);
		}
	  else
		ret = keys[key]->param;
	  keys[key]->func = func;
	  keys[key]->param = param;
	  return ret;
	}
  echo += "Key out of range\n";
  return NULL;
}

int
Editor::bind_key_to_map (int key, key_map *keys)
{
  return bind_key_to_map_in_map (key, keys, key_bindings);
}

int
Editor::bind_key_to_map_in_map (int key, key_map *keys,
								key_map *bindings)
{
  if (key < KEY_TABLE_SIZE)
	{
	  if (bindings[key]->func == interpret_last_input
		  && bindings[key]->param != key_bindings)
		{
		  /* Bound to a keymap.  */
		  assert (bindings[key]->param);
		  free_key_map ((key_map*) bindings[key]->param);
		}
	  bindings[key]->func = (void*) interpret_last_input;
	  bindings[key]->param = (void*) keys;
	  return 1;
	}
  return 0;
}

int
interpret_last_input (void* map)
{
  int ch;
  key_map *tem_map;

  tem_map = (key_map*) map;
  ch = ed->input_buffer[0];
  if (tem_map[ch])
	{
	  void *param;
	  param = tem_map[ch]->param;
	  if (param)
		{
		  int (*func) (void*);

		  func = (int (*) (void*)) tem_map[ch]->func;
		  if (func == interpret_last_input)
			{
			  ed->echo += std::string (1, (char) ch) + "-\n";

			  ed->display ();
			  while (ed->read_char (&ch) == false)
				ed->display ();
			  push_input (ch, ed->input_buffer);
			  ed->echo.clear ();
			}

		  return ((int (*) (void*)) func) (param);
		}
	  else
		{
		  int (*func) ();
		  func = (int (*) ()) tem_map[ch]->func;
		  if (func)
			return func ();
		}
	}

  return 3;
}

void
Editor::init_key_bindings ()
{
  key_map *cx_key_map, *esc_key_map;

  key_bindings = (key_map*) calloc (KEY_TABLE_SIZE, sizeof (key_map));
  if (key_bindings == NULL)
	{
	  echo += "Failed to allocate root key map";
	  return;
	}

  for (int i = 040; i < 256; i++)
	if (isprint (i))
	  bind_key (i,  (void*) insert_last_input_char);

  bind_key ('\n'   /* NL  */,  (void*) insert_last_input_char);
  bind_key ('\000' /* C-  */,  (void*) set_mark);
  bind_key ('\001' /* C-a */,  (void*) bol);
  bind_key ('\002' /* C-b */,  (void*) backward_char);
  bind_key ('\004' /* C-d */,  (void*) clip_char);
  bind_key ('\005' /* C-e */,  (void*) eol);
  bind_key ('\006' /* C-f */,  (void*) forward_char);
  bind_key ('\011' /* Tab */,  (void*) insert_last_input_char);
  bind_key ('\013' /* C-k */,  (void*) clip_line);
  bind_key ('\014' /* C-l */,  (void*) center_window);
  bind_key ('\015' /* CR  */,  (void*) insert_newline);
  bind_key ('\016' /* C-n */,  (void*) forward_line);
  bind_key ('\020' /* C-p */,  (void*) backward_line);
  bind_key ('\026' /* C-v */,  (void*) forward_window_page);
  bind_key ('\027' /* C-w */,  (void*) clip_region);
  bind_key ('\031' /* C-y */,  (void*) yank);
  bind_key ('\177' /* DEL */,  (void*) clip_char_back);

  bind_key (KEY_BACKSPACE, (void*) clip_char_back);
  bind_key (KEY_NPAGE /* Page down.  */, (void*) forward_window_page);
  bind_key (KEY_PPAGE /* Page up.  */, (void*) backward_window_page);
  bind_key (KEY_HOME, (void*) bob);
  bind_key (KEY_END, (void*) eob);
  /* Arrow keys.  */
  bind_key (KEY_UP, (void*) backward_line);
  bind_key (KEY_DOWN, (void*) forward_line);
  bind_key (KEY_LEFT, (void*) backward_char);
  bind_key (KEY_RIGHT, (void*) forward_char);

  /* These could all use bind_sequence now.  */

  cx_key_map = (key_map*) calloc (KEY_TABLE_SIZE, sizeof (key_map));
  if (cx_key_map)
	{
	  bind_key_in_map ('\023' /* C-s */,  (void*) write_buffer, cx_key_map);
	  bind_key_in_map ('\003' /* C-c */,  (void*) return_break, cx_key_map);
	  bind_key_in_map ('e',               (void*) evaluate_file, cx_key_map);
	  bind_key_in_map ('\005' /* C-e */,  (void*) evaluate_region, cx_key_map);
	  bind_key_in_map ('\030' /* C-x */,  (void*) swap_positions, cx_key_map);

	  bind_key_to_map ('\030' /* C-x */, cx_key_map);
	}
  else
	echo += "Failed to allocate ^X key map";

  esc_key_map = (key_map*) calloc (KEY_TABLE_SIZE, sizeof (key_map));
  if (esc_key_map)
	{
	  bind_key_in_map ('b', (void*) backward_word, esc_key_map);
	  bind_key_in_map ('f', (void*) forward_word, esc_key_map);
	  bind_key_in_map ('v', (void*) backward_window_page, esc_key_map);
	  bind_key_in_map ('w', (void*) copy_region, esc_key_map);
	  bind_key_in_map ('<', (void*) bob, esc_key_map);

	  bind_key_to_map ('\033' /* C-[ (ESC) */, esc_key_map);

	  bind_sequence ("\033>", (void*) eob);  /* Test of bind_sequence.  */
	}
  else
	echo += "Failed to allocate \033 (ESC) key map";

  /* C-c is for user bindings.  It is bound here for demos.  */
#ifdef CERN
  bind_sequence ("\003r" /* C-c r */, (void*) random_fill);
  bind_sequence ("\003g" /* C-c g */, (void*) graph_pairs);
#endif
#ifdef GMP
  bind_sequence ("\003i" /* C-c i */, (void*) eval_mpi_expr);
#endif
#ifdef WITH_PPL
  bind_sequence ("\003p" /* C-c p */, (void*) ppl_buffer);
#endif
}

int
Editor::display_loop ()
{
  int ch;
  while (1)
	try
	  {
		display ();

		if (read_char (&ch))
		  {
			echo.clear ();

			push_input (ch, input_buffer);

			old_append_next_clip = append_next_clip;
			append_next_clip = false;

			switch (interpret_last_input (key_bindings))
			  {
			  case 0:
				echo += "Error in bound function\n";
				break;
			  case 2:
				return 1;
			  case 3:
				std::stringstream str;
				str << input_buffer[0];
				echo += "Empty key sequence (last char: "
				  + std::string (1, (char) input_buffer[0]);
				echo += " " + str.str () + ")\n";
				break;
			  }
		  }
	  }
	catch (std::exception excpt)
	  {
		echo += "Exception caught: ";
		echo += excpt.what ();
		echo += "\n";
	  }
}

void
Editor::add_clip (Cons *clip, bool append)
{
  if (this->clip == NULL)
	{
	  clips = new ConsT<Cons> (clip, NULL);
	  this->clip = clips;
	}
  else
	{
	  if (old_append_next_clip)
		/* Set the cdr of the last cons in the car of the clip ring to
		   the given clip.  */
		last (this->clip->car ())->setcdr (clip);
	  else
		{
		  this->clip->setcdr (new ConsT<Cons> (this->clip->car (),
											   this->clip->cdr ()));
		  this->clip->setcar (clip);
		}
	}
  append_next_clip = true;
}

void
Editor::graph_float_pairs ()
{
#ifdef CERN
  int ch;
  string line;
  Cons *tem;

  HLIMIT (PAWC_SIZE);
  HPLINT (4);

  HBOOK1 (1, "x", 100, -4., 4., 0.);
  HBOOK1 (2, "y", 100, -4., 4., 0.);
  HBOOK1 (3, "x - y", 100, -6., 6., 0.);

  tem = buffer->point;
  line = buffer->get_line_as_string ();
  while (line.size ())
	{
	  float x, y;
	  std::stringstream str (line);

	  str >> x;
	  str >> y;

	  HF1 (1, x, 1.);
	  HF1 (2, y, 1.);
	  HF1 (3, x - y, 1.);

	  line = buffer->get_line_as_string ();
	}

  HPLZON (2, 2, 1, " ");
  HPLOT (1, " ", " ", 0);
  HPLOT (2, " ", " ", 0);
  HPLZON (1, 2, 2, "S");
  HPLOT (3, " ", " ", 0);

  read_char (&ch);

  HPLEND ();
  buffer->point = tem;

  /* Could instead temporarily redirect stdout, or turn off the HIGZ
	 version message.  */
  clear_display ();
#endif
}

void
Editor::eval_mpi_line ()
{
#ifdef GMP

  mpz_t result, foo, bar;		/* Multiple precision integers.  */
  int base, ret;
  char *str;
  string expr;
  Cons *tem;

  base = 0;

  /* Get the expression.  */
  tem = buffer->point;
  buffer->point = buffer->beginning_of_line ();
  expr = buffer->get_line_as_string ();
  buffer->point = tem;

  mpz_init (result);
  /* FIX What are these for?  */
  mpz_init_set_ui (foo, 55L);
  mpz_init_set_ui (bar, 99L);

  /* Evaluate the expression.  */
  ret = mpz_expr (result, base, expr.c_str (), foo, bar, NULL);
  if (ret == MPEXPR_RESULT_OK)
	gmp_asprintf (&str, "%Zd\n", result);
  else
	gmp_asprintf (&str, "Error code %d returned\n", ret);

  echo += str;
  free (str);

  mpz_clear (result);
  mpz_clear (foo);
  mpz_clear (bar);

#endif
}

void
Editor::ppl ()
{
#ifdef WITH_PPL

  PPL::NNC_Polyhedron ph;
  std::stringstream str (buffer->get_buffer_as_string ());

  if (ph.ascii_load (str))
	{
	  if (ph.is_empty ())
		echo += "An empty ";
	  else
		echo += "A ";
	  if (ph.is_topologically_closed ())
		echo += "topologically closed ";
	  /* FIX Is bounded sensible for an (the?) empty polyhedron?  */
	  if (ph.is_bounded ())
		echo += "bounded ";
	  /* FIX Should this be "The ... universe [in x dimensions]"?  */
	  if (ph.is_universe ())
		echo += "universe ";
	  echo += "polyhedron in ";
	  str.str ("");
	  str << ph.space_dimension ();
	  echo += str.str ();
	  if (ph.space_dimension () == 1)
		echo += " dimension\n";
	  else
		echo += " dimensions\n";
	}
  else
	echo += "Loading the buffer into a polyhedron failed\n";

#endif /* WITH_PPL */
}

std::string
Editor::version ()
{
  return version_string;
}

bool
is_alphanum (void* ch)
{
  return isalnum (*((int*) &ch));
}

bool
eq (void* one, void* two)
{
  return one == two;
}
