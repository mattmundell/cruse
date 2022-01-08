/* SupplyWindow.h -- header for text-supplying Window  */

#ifndef _SupplyWindow_h
#define _SupplyWindow_h

#include "Window.h"
#include <vector>

using std::vector;

class SupplyWindow : public Window
{
protected:

  cons::Cons *content;			/* Chars remaining in the window.  */

public:

  /* True if changes to point have been accounted for in the window
	 start.  */
  bool start_already_adjusted;

  /* Indicates whether there are more chars available.  */
  bool more_chars;

  /* Flag whether the last char read was at point and/or mark.  */
  bool one_passed_point;
  bool one_passed_mark;

  SupplyWindow () : start_already_adjusted (true), more_chars (false) {}

  SupplyWindow (//vector<buffer::Buffer>::iterator buffer,
				buffer::Buffer *buffer,
				cons::Cons *buffer_start,
				int scroll_margin,
				int width,
				int height)
	: Window (buffer, buffer_start, scroll_margin, width, height),
	  start_already_adjusted (true), more_chars (false) {}

  ~SupplyWindow () {}

  /* Prepare to supply the chars in the window.
	 Return true if the window has changed, else false. (Currently
	 always returns true.)  */
  bool prepare ();

  /* Return the next char in the window.  */
  char next_char ();
};

#endif /* not _SupplyWindow_h */
