/* CursedEditor.cc -- a very basic Emacs-like curses-based editor  */

#include "CursedEditor.h"
#include "Cons.h"
#include <curses.h>
#include <csignal>
#include <sstream>
#include <iostream>
#include <string>

#define xstr(s) str (s)
#define str(s) #s

string CursedEditor::version_string = "CursedEditor " xstr (VERSION);
int CursedEditor::initial_cursor_state = 0;

void
finish (int signal)
{
  curs_set (CursedEditor::initial_cursor_state); /* Restore cursor.  */
  endwin ();

  exit (EXIT_SUCCESS);
}

int
CursedEditor::run (string file_name)
{
  /* Make sure stdin is a terminal. */
  if (isatty (STDIN_FILENO) == 0)
	{
	  fprintf (stderr, "Must be run from a terminal.\n");
	  return 0;
	}

  initscr ();
  keypad (stdscr, TRUE);		/* Enable keyboard mapping.  */
  intrflush (stdscr, FALSE);  /* Else interrupts flushed on input.  */
  nonl ();						/* Turn off NL->CR/NL on output.  */
  //halfdelay (10 /* tenths of a sec */);
  raw ();
  noecho ();
  meta (stdscr, TRUE);
  typeahead (STDIN_FILENO);

  signal (SIGINT, finish);
  signal (SIGABRT, finish);
  initial_cursor_state = curs_set (0); /* Hide cursor. */

  if (has_colors ())
	{
	  start_color ();
	  init_pair (1, COLOR_RED, COLOR_BLACK);	/* Point, wrap.  */
	  init_pair (2, COLOR_YELLOW, COLOR_BLACK); /* Mode line.  */
	  init_pair (3, COLOR_BLACK, COLOR_YELLOW); /* Mark.  */
	}

  // FIX
#if 0
  buffers.push_back (*new buffer::Buffer (file_name));
  buffer = buffers.begin ();
#else
#ifdef CERN
  buffer = new buffer::CERNBuffer (file_name);
#else
  buffer = new buffer::Buffer (file_name);
#endif
  //buffers.push_back (*buffer);
#endif
  buffer->tab_width = tab_width;

#if 0
  windows.push_back (*new SupplyWindow (buffer,
										buffer->point,
										scroll_margin,
										/* Account for wrap char.  */
										COLS - 1,
										LINES));
  window = windows.begin ();
#else
  window = new SupplyWindow (buffer,
							 buffer->point,
							 scroll_margin,
							 /* Account for wrap char.  */
							 COLS - 1,
							 LINES);
  //windows.push_back (*window);
#endif
  /* %t^50Ec  %tHh%tM  %% */
  window->mode_line_format = "%m    %f    %tc    L%l  C%c  %-";
  window->editor = this;

  display (true);
  display_loop ();

  curs_set (initial_cursor_state); /* Restore cursor.  */
  endwin ();
  // FIX check ERR

  return 1;
}

bool
CursedEditor::display (bool force)
{
  int col, lines, cols, tab_width, echo_size;
  std::string mode_line;

  if (window->prepare () == false
	  && force == false
	  && echo.size () == 0)
	return false;

  tab_width = window->get_tab_width ();

  erase ();

  echo_size = 0;

  if (echo.size ())
	{
	  cons::Cons *tem;

	  tem = cons::string_to_list<void> (echo);
	  echo_size = buffer::window_line_distance (tem,
												NULL,
												window->width);
	  delete tem;

	  window->height -= echo_size;
	  move (LINES - echo_size, 0);
	  addstr (echo.c_str ());
	  move (0, 0);
	}

  lines = window->height;

  mode_line = window->get_mode_line ();
  if (mode_line.size ())
	{
	  move (LINES - echo_size - 1, 0);
	  //attron (A_REVERSE);
	  attron (COLOR_PAIR (2));
	  addstr (mode_line.c_str ());
	  attroff (COLOR_PAIR (2));
	  //attroff (A_REVERSE);
	  move (0, 0);
	  lines--;
	}

  cols = window->width;
  col = 0;

  /* Add the characters.  */
  while (lines && window->more_chars)
	{
	  char ch;

	  ch = window->next_char ();

	  if (ch == '\t')
		{
		  std::string tem;

		  tem = "";

		  if (tab_width > cols - col)
			{
			  /* Tab across end of window line.  */

			  if (window->one_passed_point)
				{
				  attron (COLOR_PAIR (1));
				  attron (A_REVERSE);
				}
			  else if (window->one_passed_mark)
				attron (COLOR_PAIR (3));
			  else
				attron (COLOR_PAIR (1));

			  tem.replace (0, 0, cols - col, ' ');
			  addstr (tem.c_str ());

			  if (window->one_passed_point)
				attroff (A_REVERSE);
			  else if (window->one_passed_mark)
				{
				  attroff (COLOR_PAIR (3));
				  attron (COLOR_PAIR (1));
				}

			  addch (wrap_indicator);
			  attroff (COLOR_PAIR (1));

			  col = 1;
			  lines--;
			}
		  else
			{
			  if (window->one_passed_point)
				{
				  attron (COLOR_PAIR (1));
				  attron (A_REVERSE);
				}
			  else if (window->one_passed_mark)
				attron (COLOR_PAIR (3));
			  tem.replace (0, 0, tab_width, ' ');
			  col += tab_width;
			  addstr (tem.c_str ());
			  if (window->one_passed_point)
				{
				  attroff (A_REVERSE);
				  attroff (COLOR_PAIR (1));
				}
			  else if (window->one_passed_mark)
				attroff (COLOR_PAIR (3));
			}
		}
	  else
		{
		  col++;

		  if (window->one_passed_point
			  || window->one_passed_mark)
			{
			  /* Character at point or mark.  */

			  if (window->one_passed_point)
				{
				  attron (COLOR_PAIR (1));
				  attron (A_REVERSE);
				}
			  else
				attron (COLOR_PAIR (3));
			  if (ch == '\r' || ch == '\n')
				{
				  lines--;
				  addstr (" \n");
				  col = 0;
				  if (window->one_passed_point)
					attroff (A_REVERSE);
				}
			  else
				{
				  addch (ch);
				  if (window->one_passed_point)
					attroff (A_REVERSE);
				  if (col == cols)
					{
					  col = 0;
					  lines--;
					  addch (wrap_indicator);
					}
				}
			  if (window->one_passed_point)
				attroff (COLOR_PAIR (1));
			  else
				attroff (COLOR_PAIR (3));
			}
		  else
			if (ch == '\r' || ch == '\n')
			  {
				lines--;
				addch ('\n');
				col = 0;
			  }
			else
			  {
				addch (ch);
				if (col >= cols)
				  {
					col = 0;
					lines--;
					attron (COLOR_PAIR (1));
					addch (wrap_indicator);
					attroff (COLOR_PAIR (1));
				  }
			  }
		}
	}

  refresh ();

  if (echo_size)
	window->height += echo_size;

  return true;
}

void
CursedEditor::clear_display ()
{
  clear ();
}

string
CursedEditor::version ()
{
  return version_string;
}
