/* Buffer.h -- file buffer header  */

#ifndef _Buffer_h
#define _Buffer_h

#include "Cons.h"
#include <string>

#define INIT_TAB_WIDTH 4

namespace buffer
{
  using namespace cons;
  using std::string;

  extern Cons *hil;				/* hil is.  */

  class Buffer
  {
  public:

	/* Name of the file read into the buffer.  */
	string file_name;

	Cons *point;				/* Current position in buffer.  */
	Cons *mark;					/* A secondary position in buffer.  */
	Cons *content;				/* Beginning of buffer.  */
	bool modified;				/* Modification flag.  */

	int tab_width;				/* Number of spaces per tab.  */

	Buffer () : tab_width (INIT_TAB_WIDTH)
	{ content = point = NULL; mark = hil; modified = false; }
	/* Read the file named FILE_NAME into the buffer.  */
	Buffer (string file_name);

	virtual ~Buffer () { delete content; }

	/* Write buffer back to file.
	   Return 1 on success, 0 on failure, -1 on error.  */
	int write ();

	/* Return the number of the line point is on.  */
	int current_line ();

	/* Return the number of the column point is on.  */
	int current_column ();

	/* Insert CAR at point.  Return point.  */
	Cons* insert_char (void *car);

	/* Insert LIST at point.
	   Return content if it has changed, else NULL.  */
	Cons* insert (Cons *list);

	/* Clip the char COUNT chars (pos/neg) from point.
	   Return point.  */
	Cons* clip_char (int count);

	/* Clip all chars from point to the char before CONS.
	   Return a list of the clipped chars.  */
	Cons* clip_to (Cons* cons);

	/* Return copy of char list from point to char before CONS.  */
	Cons* copy_to (Cons* cons);

	/* Move point COUNT characters forward.

	Return point.  COUNT can be negative or positive.  Leave point in
	place if it's at the beginning of the buffer.  */
	Cons* forward_chars (int count = 1);

	/* Move point *COUNT characters forward.

	Return point.  *COUNT can be negative or positive.  Leave point
	in place if requested to move out of the buffer.  */
	Cons* forward_chars (int *count);

	/* Return beginning of the line COUNT lines after current line.

	COUNT can be positive or negative.
	..->next_line (0); returns the current line.
	..->next_line (1); returns the next line.
	..->next_line (-1); returns the previous line.  */
	Cons* next_line (int count);

	/* Return start of COUNTth-from-current window line.

	FIX Assume point is in the first column.

	COLS is the character width of the window.  A window line is a line
	terminated either by a newline character or the edge of the window.

	COUNT can be positive or negative.
	..->next_line (0); returns the current window line.
	..->next_line (1); returns the next window line.
	..->next_line (-1); returns the previous window line.  */
	Cons* next_window_line (int count, const int cols);

	/* Move point forward one line.  Return point.  */
	Cons* forward_line ();

	/* Move point backward one line.

	Return point.  When point is on the first line move it to the
	beginning of that line.  */
	Cons* backward_line ();

	/* Return the beginning of the line point is on.  */
	Cons* beginning_of_line ();

	/* Return the end of the line point is on.  */
	Cons* end_of_line ();

	/* Swap point and mark.  */
	inline void swap_positions ();

	/* Insert N pairs of random float values at point.  */
	virtual void random_fill (int n) {}

	/* Return the list from point to the end of line as a string.
	 Leave point at the beginning of the next line.  */
	string get_line_as_string ();

	/* Return the buffer as a string, leaving point in place.  */
	string get_buffer_as_string ();
  };

  /* Return the number of lines in the list HEADED by cons.  */
  int lines (Cons *cons);

  /* Return distance, in line beginnings, between conses ONE and TWO.

  Return 0 if they are on the same line, 1 if they are on adjacent
  lines, etc.  Returns -1 on failure (e.g. if elements are in
  different lists).

  Faster if one comes before two.  */
  int line_distance (Cons *one, Cons *two);

  /* Return window lines from ONE to TWO, given a COLS-widthed screen.

  FIX Assume the first (in list) of ONE and TWO is in the first column.

  A window line is a line terminated by a newline character or the edge
  of the screen.

  Return 0 on failure (i.e. if elements are in different
  lists). Return 0 if ONE and TWO are on the same line, 1 if TWO is on
  the line following ONE, -1 if ONE is on the line following TWO, etc.

  Faster if ONE comes before TWO.  */
  int window_line_distance (Cons *one, Cons *two, int cols);

  inline void
  Buffer::swap_positions ()
  {
	Cons *tem;

	if (mark == hil)
	  return;

	tem = point;
	point = mark;
	mark = tem;
  }
}

#endif /* not _Buffer_h */
