/* SupplyWindow.cc -- Window which can supply the visible text  */

#include "SupplyWindow.h"

bool
SupplyWindow::prepare ()
{
  int lines;
  bool modified;

  modified = buffer->modified;
  more_chars = true;
  one_passed_point = false;
  one_passed_mark = false;

  if (start == NULL)
	start = buffer->content;

  if (buffer->content == NULL)
	start = NULL;

  /* May have inserted with point at beginning of buffer.  */
  if (buffer->content == buffer->point)
	{
	  start = buffer->point;
	  start_already_adjusted = true;
	}

  current_line = buffer->current_line ();

  current_column = buffer->current_column ();

  lines = buffer::window_line_distance (start,
										buffer->point,
										width);

  if (lines <= scroll_margin - 1)
	{
	  if (start_already_adjusted == false || lines < 0)
		{
		  cons::Cons *tem;

		  /* Move window start backward one window line.  */
		  tem = buffer->point;
		  buffer->point = start;
		  if (lines < 0)
			start = buffer->next_window_line (lines, width);
		  else
			start = buffer->next_window_line (-1, width);
		  buffer->point = tem;
		  modified = true;
		}
	}
  else if (lines
		   // FIX using mode_line_size before prep'd mode line
		   >= height - (mode_line.size () ? 1 : 0) - scroll_margin)
	{
	  if (start_already_adjusted == false
		  || lines >= height - (mode_line.size () ? 1 : 0))
		{
		  cons::Cons *tem;

		  /* Move window start forward one window line.  */
		  tem = buffer->point;
		  buffer->point = start;
		  // FIX this needs work
		  if (lines >= height - (mode_line.size () ? 1 : 0))
			{
			  start = buffer->next_window_line (lines
												- (height
												   - (mode_line.size ()
													  ? 1 : 0))
												+ 1,
												width);
			  start_already_adjusted = true;
			}
		  else
			start = buffer->next_window_line (1, width);
		  buffer->point = tem;
		  modified = true;
		}
	}
  else
	start_already_adjusted = false;

  content = start;

  /* FIX Perhaps will be useful later?  Need to
	 - check for change in mode line
	 - check for change in point!
	 return modified;  */
  return true;
}

char
SupplyWindow::next_char ()
{
  one_passed_point = content == buffer->point;
  one_passed_mark = content == buffer->mark;

  if (content)
	{
	  char ch;

	  ch = (char) (int) content->car ();

	  content = content->cdr ();
	  return ch;
	}
  else
	if (more_chars)
	  {
		more_chars = false;
		return '\n';
	  }
	else
	  // FIX throw error?
	  return '\0';
}
