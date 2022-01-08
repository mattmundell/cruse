/* CursedEditor.h -- header for Emacs-like curses-based editor  */

#ifndef _CursedEditor_h
#define _CursedEditor_h

#include "PythonedEditor.h"
#include <string>
#include <curses.h>

using std::string;

class CursedEditor : public PythonedEditor
{
private:

  static string version_string;

protected:

  /* Redraw the display.  Return true if redrawn, else false.  */
  bool display (bool force);

public:

  static int initial_cursor_state;

  CursedEditor () {}
  CursedEditor (int argc, char **argv)
	: PythonedEditor (argc, argv) {}
  ~CursedEditor () {}

  /* Edit file named FILE_NAME.  Returns 1, or 0 on error.  */
  int run (string file_name);

  void clear_display ();		/* Clear the display.  */

  /* Read a character into CH.
     On success return true, else return false.  */
  bool read_char (int *ch)
  {
	if ((*ch = getch ()) == ERR)
	  return false;
	return true;
  }

  static string version ();		/* Return version string.  */
};

#endif /* not _CursedEditor_h */
