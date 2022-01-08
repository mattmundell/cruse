/* Window.h -- screen sectioning header  */

#ifndef _Window_h
#define _Window_h

#include "Buffer.h"
#include "Cons.h"
#include <vector>
#include <string>
#include <sstream>
#include <ctime>

using std::vector;
using std::string;

class Editor;  /* FIX tracing.  */

class Window
{
public:

  /* Size in lines of scroll margins.  Moving into the scroll margin
	 at the top or bottom of the window causes the window to scroll up
	 or down, respectively.  */
  int scroll_margin;

  /* Dimensions, in characters.  */
  int height;
  int width;

  /* The descriptive final line in the window.  */
  string mode_line;  /* FIX mode area */
  /* Format of the mode line.  */
  string mode_line_format;

  /* The buffer currently displayed in the window.  */
  //vector<buffer::Buffer>::iterator buffer;
  buffer::Buffer *buffer;
  int current_line;
  int current_column;

  /* The beginning of the portion of the buffer on display.  */
  cons::Cons *start;

  /* The Editor holding the window, for tracing.  */
  Editor *editor;

  Window ()
  {
	buffer = NULL; // vector<buffer::Buffer>::iterator ();
	start = NULL;
	height = width = 0;
	current_line = current_column = 0;
  }

  Window (//vector<buffer::Buffer>::iterator buffer,
		  buffer::Buffer *buffer,
		  cons::Cons *start,
		  int scroll_margin,
		  int width,
		  int height)
  {
	this->buffer = buffer;
	this->start = start;
	this->scroll_margin = scroll_margin;
	this->width = width;
	this->height = height;
	current_line = current_column = 0;
  }

  ~Window () {}

  /* Return current tab width.  */
  int get_tab_width () { return buffer->tab_width; }

  /* Generate and return the mode line.  */
  string get_mode_line ();

  /* Set the start of the window so that point is centered in the
	 window.  */
  inline void center ();

  /* Move point and start one page backward.  */
  void backward_window_page ();

  /* Move point and start one page forward.  */
  void forward_window_page ();
};

inline void
Window::center ()
{
  start = buffer->next_window_line (-(height/2), width);
}

#endif /* not _Window_h */
