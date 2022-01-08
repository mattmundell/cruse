/* GTKEditor.h -- header for basic Emacs-like GTK-based editor  */

#ifndef _GTKEditor_h
#define _GTKEditor_h

#include "Editor.h"
#include <gtkmm.h>
#include <string>

class GTKEditor : public Editor, public Gtk::Window
{
protected:

  void display () {}			/* Redraw the display.  */

public:

  static int initial_cursor_state;

  GTKEditor ()
  {
	set_title("gdit");
	set_border_width (0);
  }
  virtual ~GTKEditor () {}

  /* Edit file named FILE_NAME.  Returns 1, or 0 on error.  */
  int run (std::string file_name)
  {
	Gtk::Main::run (*this);
	return 1;
  }

  void clear () {}				/* Clear the display.  */
};

#endif /* not _GTKEditor_h */
