/* bindables.cc -- functions which can be bound to keys  */

#include "bindables.h"
#include "Editor.h"
#include "Cons.h"

using cons::Cons;
using buffer::hil;

int
insert_last_input_char ()
{
  ed->buffer->insert_char ((void*) ed->input_buffer[0]);
  return 1;
}

int
bol ()
{
  ed->buffer->point = ed->buffer->beginning_of_line ();
  return 1;
}

int
eol ()
{
  ed->buffer->point = ed->buffer->end_of_line ();
  return 1;
}

int
set_mark ()
{
  ed->buffer->mark = ed->buffer->point;
  ed->echo += "Mark set\n";
  return 1;
}

int
forward_char ()
{
  ed->buffer->forward_chars ();
  return 1;
}

int
backward_char ()
{
  ed->buffer->forward_chars (-1);
  return 1;
}

int
clip_char ()
{
  ed->buffer->clip_char (0);
  return 1;
}

int
clip_line ()
{
  Cons *tem;

  if (ed->buffer->point
	  && (ed->buffer->point->car ()
		  == (void*) (int) '\n'))
	/* At end of line.  */
	tem = ed->buffer->point->cdr ();
  else
	tem = ed->buffer->end_of_line ();
  /* FIX A better approach may be needed for multiple
	 buffers and windows.  */
  if (ed->window->start == ed->buffer->point)
	ed->window->start = tem;

  tem = ed->buffer->clip_to (tem);
  if (tem)
	ed->add_clip (tem);

  return 1;
}

int
center_window ()
{
  ed->clear_display ();
  ed->window->center ();
  return 1;
}

int
insert_newline ()
{
  ed->buffer->insert_char ((void*) (int) '\n');
  return 1;
}

int
forward_line ()
{
  ed->buffer->forward_line ();
  return 1;
}

int
backward_line ()
{
  ed->buffer->backward_line ();
  return 1;
}

int
forward_window_page ()
{
  ed->window->forward_window_page ();
  ed->window->start_already_adjusted = true;
  return 1;
}

int
backward_window_page ()
{
  ed->window->backward_window_page ();
  ed->window->start_already_adjusted = true;
  return 1;
}

int
clip_region ()
{
  if (ed->buffer->mark == hil)
	ed->echo += "Need mark to clip the region\n";
  else
	{
	  Cons *tem;

	  /* FIX A better approach may be needed for
		 multiple buffers and windows.  */
	  if (ed->window->start == ed->buffer->point)
		ed->window->start = ed->buffer->mark;
	  else if (ed->window->start == ed->buffer->mark)
		ed->window->start = ed->buffer->point;

	  tem = ed->buffer->clip_to (ed->buffer->mark);
	  if (tem)
		ed->add_clip (tem);
	}
  return 1;
}

int
write_buffer ()
{
  if (ed->buffer->write () < 1)
	ed->echo += "Save failed\n";
  else
	ed->echo += "Saved\n";
  return 1;
}

int
return_break ()
{
  return 2;
}

int
evaluate_file ()
{
  ed->eval_file ();
  return 1;
}

int
evaluate_region ()
{
  ed->eval_region ();
  return 1;
}

int
swap_positions ()
{
  if (ed->buffer->mark == hil)
	ed->echo += "Need mark to swap mark and point\n";
  else
	ed->buffer->swap_positions ();
  return 1;
}

int
yank ()
{
  if (ed->clip)
	{
	  Cons *tem;
	  tem = ed->buffer->insert (cons::copy_list<void> (ed->clip->car ()));
	  if (tem)
		ed->window->start = tem;
	}
  return 1;
}

int
clip_char_back ()
{
  ed->buffer->clip_char (-1);
  return 1;
}

int
bob ()
{
  /* Move to beginning of buffer.  */
  ed->window->start = ed->buffer->point = ed->buffer->content;
  ed->window->start_already_adjusted = true;
  return 1;
}

int
eob ()
{
  /* Move to end of buffer.  */
  ed->buffer->point = NULL;
  /* Long function call over two lines.  */
  ed->window->start = ed->buffer->next_window_line
	(- ed->window->height
	 + (ed->window->mode_line.size () ? 1 : 0)
	 + 1,
	 ed->window->width);
  ed->window->start_already_adjusted = true;
  return 1;
}

int
backward_word ()
{
  // FIX include chars w/ carets, ...
  /* Move passed leading chars.  */
  do
	{
	  ed->buffer->forward_chars (-1);
	}
  while (ed->buffer->point != ed->buffer->content
		 && (is_alphanum (ed->buffer->point->car ())
			 == false));

  /* Move to beginning of word.  */
  while (ed->buffer->point != ed->buffer->content
		 && is_alphanum (ed->buffer->point->car ()))
	ed->buffer->forward_chars (-1);

  if (ed->buffer->point != ed->buffer->content)
	ed->buffer->forward_chars (1);

  return 1;
}

int
forward_word ()
{
  /* Move passed leading chars.  */
  while (ed->buffer->point
		 && (is_alphanum (ed->buffer->point->car ())
			 == false))
	ed->buffer->forward_chars (1);

  /* Move to end of word.  */
  do
	{
	  ed->buffer->forward_chars (1);
	}
  while (ed->buffer->point
		 && is_alphanum (ed->buffer->point->car ()));

  return 1;
}

int
copy_region ()
{
  if (ed->buffer->mark == hil)
	ed->echo += "Need mark to copy the region\n";
  else
	{
	  Cons* tem;

	  tem = ed->buffer->copy_to (ed->buffer->mark);
	  if (tem)
		ed->add_clip (tem);
	}
  return 1;
}

int
random_fill ()
{
  ed->buffer->random_fill (1000);
  return 1;
}

int
graph_pairs ()
{
  ed->graph_float_pairs ();
  return 1;
}

int
eval_mpi_expr ()
{
  ed->eval_mpi_line ();
  return 1;
}

int
ppl_buffer ()
{
  ed->ppl ();
  return 1;
}
