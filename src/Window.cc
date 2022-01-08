/* Window.h -- screen sectioning  */

#include "Window.h"
#include "Buffer.h"
#include "Cons.h"
#include <string>
#include <sstream>

using std::string;

string
Window::get_mode_line ()
{
  int pos, size;

  pos = 0;
  size = mode_line_format.size ();
  mode_line = "";
  while (pos < size
		 && ((int) mode_line.size ()) < width)
	{
	  if (mode_line_format[pos] == '%')
		{
		  if (mode_line_format[++pos] == '%')
			mode_line += '%';
		  else if (mode_line_format[pos] == 'm')
			if (buffer->modified == true)
			  mode_line += "**";
			else
			  mode_line += "--";
		  else if (mode_line_format[pos] == 'f')
			mode_line += buffer->file_name;
		  else if (mode_line_format[pos] == 't')
			{
			  /* Time and date.  */
#define	SIZE 256
			  char buffer[SIZE];
			  string time_spec;
			  time_t current_time;
			  struct tm *local_time;

			  time_spec = "%";

			  /* Optional flag (GNU extension).  */
			  if (string ("_-0^").find (mode_line_format[++pos])
				  < 4)
				time_spec += mode_line_format[pos++];

			  /* Optional width.  FIX fractions?  */
			  while (mode_line_format[pos] >= '0'
					 && mode_line_format[pos] <= '9')
				time_spec += mode_line_format[pos++];

			  /* Optional modifier.  */
			  if (string ("EO").find (mode_line_format[pos])
				  < 2)
				time_spec += mode_line_format[pos++];

			  time_spec += mode_line_format[pos];

			  current_time = time (NULL);
			  /* Local representation. */
			  local_time = localtime (&current_time);

			  strftime (buffer, SIZE,
						time_spec.c_str (),
						local_time);
			  mode_line += buffer;
			}
		  else if (mode_line_format[pos] == 'l')
			{
			  std::stringstream tem;
			  //tem << current_line;
			  //tem << "3";
			  mode_line += "3";//tem.str ();
			}
		  else if (mode_line_format[pos] == 'c')
			{
			  std::stringstream tem;
			  tem << current_column;
			  mode_line += tem.str ();
			}
		  else if (mode_line_format[pos] == '-')
			{
			  if (((int) mode_line.size ()) < width)
				mode_line.replace (mode_line.size (),
								   0,
								   width - mode_line.size (),
								   '-');
			  /* Ends the effect of the format.  */
			  break;
			}
		  else
			{
			  mode_line += "%";
			  mode_line += mode_line_format[pos];
			}
		}
	  else
		mode_line += mode_line_format[pos];
	  pos++;
	}

  mode_line.erase (width);		/* Truncate to Window width.  */

  return mode_line;
}

void
Window::backward_window_page ()
{
  int count, text_height;

  count = buffer::line_distance (start, buffer->point);
  text_height = height - (mode_line.size () ? 1 : 0);
  // FIX expl why - 1
  buffer->point = buffer->next_window_line (-text_height - 1,
											width);
  /* Adjust window start.  */
  start = buffer->next_window_line (-count, width);
}

void
Window::forward_window_page ()
{
  int count;

  count = buffer::line_distance (start, buffer->point);
  buffer->point = buffer->next_window_line (height, width);
  if (buffer::line_distance (start, buffer->point)
	  >= height - (mode_line.size () ? 1 : 0))
	/* Adjust window start.  */
	start = buffer->next_window_line (-count, width);
}
