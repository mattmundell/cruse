/* Buffer.cc -- file buffer  */

#include "Buffer.h"
#include <fstream>
#include <sstream>

#define LINEFEED ((void*) 10)

namespace buffer
{
  cons::Cons *hil = new cons::Cons ();

  Buffer::Buffer (string file_name) : tab_width (INIT_TAB_WIDTH)
  {
	this->file_name = file_name;
	modified = false;
	content = NULL;
	mark = hil;

	if (this->file_name.size ())
	  {
		std::ifstream ifile (this->file_name.c_str ());

		if (ifile.good ())
		  {
			char ch;

			ifile.read (&ch, 1);
			if (ifile.eof () == false)
			  {
				point = new Cons ((void*) (int) ch, NULL);
				content = point;
				ifile.read (&ch, 1);
			  }
			while (ifile.eof () == false)
			  {
				point = point->setcdr (new Cons ((void*) (int) ch,
												 NULL));
				ifile.read (&ch, 1);
			  }
		  }
	  }

	point = content;
  }

  int
  Buffer::write ()
  {
	// FIX Check if file has changed on disk, and if buffer modified.
	if (file_name.size ())
	  {
		std::ofstream ofile (file_name.c_str ());

		if (ofile.good ())
		  {
			Cons *tem;

			tem = content;
			//while (tem > NULL)
			while (tem != 0)
			  {
				ofile << (char) (int) tem->car ();
				tem = tem->cdr ();
			  }

			modified = false;
			return 1;
		  }
		else
		  return -1;
	  }
	return 0;
  }

  int
  Buffer::current_line ()
  {
	return line_distance (content, point);
  }

  int
  Buffer::current_column ()
  {
	Cons *tem;
	int count;

	tem = beginning_of_line ();
	count = 0;

	while (tem)
	  {
		if (tem == point)
		  return count;

		if (tem->car () == (void*) (int) '\011' /* tab */)
		  count += tab_width;
		else
		  count++;
		tem = tem->cdr ();
	  }

	return count;
  }

  Cons*
  Buffer::insert_char (void *car)
  {
	if (point)
	  {
		Cons *cons;

		cons = new Cons (point->car (), point->cdr ());
		point->setcdr (cons);
		point->setcar (car);
		point = cons;
	  }
	else
	  if (content)
		last (content)->setcdr (new Cons (car, NULL));
	  else
		content = new Cons (car, NULL);

	modified = true;

	return point;
  }

  Cons*
  Buffer::insert (Cons *list)
  {
	if (list)
	  {
		Cons *ret;

		/* Empty buffer. */
		if (content == NULL)
		  {
			content = list;
			return point;
		  }

		ret = point;
		last (list)->setcdr (point);

		/* Start of buffer. */
		if (point == content)
		  return content = list;

		/* FIX Instead of heavy forward_chars use point as first in
		   list and use first in list as point (move to end of
		   list).  */
		forward_chars (-1);
		point->setcdr (list);

		point = ret;
	  }
	return NULL;
  }

  Cons*
  Buffer::clip_to (Cons* cons)
  {
	Cons *swap, *ret, *tem;
	bool holds_mark;

	if (point == cons)
	  return NULL;

	if (point == NULL)
	  goto swap;

	swap = NULL;

  clip_to:

	holds_mark = false;
	ret = tem = point;
	/* Get the cell before CONS.  */
	while (tem && tem->cdr () != cons)
	  {
		if (tem == mark)
		  holds_mark = true;
		tem = tem->cdr ();
	  }
	/* Check that CONS was found.  */
	if (tem == NULL)
	  if (swap)
		{
		  point = swap;
		  return NULL;
		}
	  else
		{
		swap:
		  /* Try with point and CONS swapped.  */
		  swap = point;
		  point = cons;
		  cons = swap;
		  goto clip_to;
		}

	/* Terminate the list for return.  */
	tem->setcdr (NULL);

	modified = true;

	if (content == point)
	  /* At beginning of buffer.  */
	  content = cons;
	else
	  {
		forward_chars (-1);
		point->setcdr (cons);
	  }
	if (holds_mark)
	  mark = cons;
	point = cons;

	return ret;
  }

  Cons*
  Buffer::copy_to (Cons* cons)
  {
	Cons *swap, *ret, *tem, *cdr;

	if (point == cons)
	  return NULL;

	if (point == NULL)
	  goto swap;

	swap = NULL;
	tem = point;

  copy_to:

	/* Get the cell before CONS.  */
	while (tem && tem->cdr () != cons)
	  tem = tem->cdr ();
	/* Check that CONS was found.  */
	if (tem == NULL)
	  if (swap)
		return NULL;
	  else
		{
		swap:
		  /* Try with tem and CONS swapped.  */
		  swap = tem = cons;
		  cons = point;
		  goto copy_to;
		}

	/* Terminate the list for copying.  */
	cdr = tem->cdr ();
	tem->setcdr (NULL);

	ret = copy_list (swap ? swap : point);

	/* Restore the buffer.  */
	tem->setcdr (cdr);

	return ret;
  }

  Cons*
  Buffer::clip_char (int count)
  {
	if (count)
	  forward_chars (&count);

	if (count)
	  /* COUNT referenced a position outside the buffer.  */
	  return point;

	if (point)
	  {
		Cons *cons;

		cons = point->cdr ();
		if (cons)
		  {
			point->setcdr (cons->cdr ());
			point->setcar (cons->car ());
			cons->setcdr (NULL);
			if (mark == cons)
			  mark = point;
			delete cons;
		  }
		else
		  {
			if (content == point)
			  /* Only char in buffer.  */
			  content = NULL;
			else
			  {
				/* Last char in buffer.  */
				cons = point;
				forward_chars (-1);
				point->setcdr (NULL);
			  }
			point = NULL;
			mark == hil || (mark = NULL);
			delete cons;
		  }

		modified = true;
	  }

	return point;
  }

  Cons*
  Buffer::forward_chars (int count)
  {
	int tem = count;
	return forward_chars (&tem);
  }

  Cons*
  Buffer::forward_chars (int *count)
  {
	Cons *cons;

	if (point == content) /* beginning */
	  {
		if (*count < 0)
		  return point;
	  }
	else if (point == NULL) /* end */
	  if (*count > 0)
		return point;

	if (*count < 0)
	  {
		*count = -(*count);
		return point = find_but (content, point, count);
	  }

	cons = point;
	while (cons && (*count)--)
	  cons = cons->cdr ();

	return point = cons;
  }

  Cons*
  Buffer::next_line (int count)
  {
	Cons *tem, *line;

	tem = point;

	point = beginning_of_line ();

	if (count >= 0)
	  {
		line = point;
		while (line && count)
		  {
			if (line->car () == LINEFEED)
			  count--;
			line = line->cdr ();
		  }
	  }
	else
	  while (count++)
		line = backward_line ();

	point = tem;

	return line;
  }

  Cons*
  Buffer::next_window_line (int count, const int cols)
  {
	Cons *tem, *line;
	int col;

	tem = point;

	//point = beginning_of_window_line ();

	if (count >= 0)
	  {
		col = cols;
		line = point;
		while (line && count)
		  {
			if (col-- == 0
				|| line->car () == LINEFEED)
			  {
				count--;
				col = cols;
			  }
			line = line->cdr ();
		  }
	  }
	else
	  {
		bool first;

		first = true;
		while (count)
		  {
			Cons *pnt, *tem /* NOTE shadowed.  */;
			int edges;

			pnt = point;
			if (first)
			  {
				tem = line = beginning_of_line ();
				first = false;
			  }
			else
			  tem = line = backward_line ();
			if (tem == content)
			  break;
			edges = 0;
			col = 0;
			/* Count window lines on this line.  */
			while (tem != pnt)
			  {
				if (tem->car () == LINEFEED)
				  {
					edges++;
					break;
				  }
				if (++col == cols)
				  {
					edges++;
					col = 0;
				  }
				tem = tem->cdr ();
			  }
			if (edges > -count)
			  {
				/* This line too far backward.  Move return
				   forward.  */
				edges += count;
				while (edges--)
				  {
					col = 0;
					while (col++ < cols)
					  line = line->cdr ();
				  }
				break;
			  }
			else
			  if (first == false)
				count += edges;
		  }
	  }

	point = tem;

	return line;
  }

  Cons*
  Buffer::forward_line ()
  {
	Cons *tem;
	int len, count;

	len = 1;
	tem = beginning_of_line ();
	while (tem != point)
	  {
		len++;
		tem = tem->cdr ();
	  }
	point = end_of_line ();

	if (point == NULL)
	  return point;

	/* Count chars in next line.  */
	tem = point;
	count = 0;
	while (tem)
	  {
		tem = tem->cdr ();
		count++;
		if (tem && tem->car () == LINEFEED)
		  break;
	  }

	return forward_chars (len > count ? &count : &len);
  }

  Cons*
  Buffer::backward_line ()
  {
	Cons *tem, *begin;
	int len;

	len = 0;
	tem = beginning_of_line ();
	if (tem == content)
	  return point = tem;
	begin = tem;

	/* Count point's distance from beginning of line.  */
	while (tem != point)
	  {
		len++;
		tem = tem->cdr ();
	  }

	/* Move to the beginning of the previous line.  */
	point = begin;
	forward_chars (-1);
	point = beginning_of_line ();

	/* Move point back off the beginning of the previous line.  */
	while (len--)
	  {
		if (point->car () == LINEFEED)
		  break;
		point = point->cdr ();
	  }

	return point;
  }

  Cons*
  Buffer::beginning_of_line ()
  {
	Cons *tem, *bol;

	bol = tem = content;

	while (tem != point)
	  if (tem->car () == LINEFEED)
		bol = tem = tem->cdr ();
	  else
		tem = tem->cdr ();

	return bol;
  }

  Cons*
  Buffer::end_of_line ()
  {
	Cons *tem;

	tem = point;

	while (tem != NULL
		   && tem->car () != LINEFEED)
	  tem = tem->cdr ();

	return tem;
  }

  string
  Buffer::get_line_as_string ()
  {
	std::stringstream str;
	str.str ("");
	while (point && point->car () != (void*) (int) '\n')
	  {
		str << (char) point->car ();
		point = point->cdr ();
	  }
	if (point)
	  point = point->cdr ();
	return str.str ();
  }

  string
  Buffer::get_buffer_as_string ()
  {
	std::string str, line;
	Cons *tem;

	str = "";
	tem = point;
	point = content;
	line = get_line_as_string ();
	while (true)
	  {
		/* FIX may return "\n" for empty buffer.  */
		str += line;
		str += "\n";
		if (point == NULL) /* end of buffer */
		  break;
		line = get_line_as_string ();
	  }
	point = tem;

	return str;
  }

  int
  line_distance (Cons *one, Cons *two)
  {
	Cons *tem;
	int count;
	char last;

	if (two == NULL)
	  return lines (one);
	if (one == NULL)
	  return lines (two);

	tem = one;
	count = 0;
	last = (char) (int) tem->car ();

	while (1)
	  {
		if (tem == two)
		  return count;

		tem = tem->cdr ();

		if (tem == NULL)
		  break;

		if (last == '\n')
		  count++;
		last = (char) (int) tem->car ();
	  }

	tem = two;
	count = 1;
	last = (char) (int) tem->car ();

	while (tem)
	  {
		tem = two->cdr ();

		if (tem == one)
		  return last == '\n' ? count + 1 : count;

		if (tem == NULL)
		  break;

		if (last == '\n')
		  count++;
		last = (char) (int) tem->car ();
	  }

	return -1;
  }

  int
  window_line_distance (Cons *one, Cons *two, int cols)
  {
	Cons *tem;
	int count;
	int col;
	char last;

	if (one == NULL)
	  goto two;

	tem = one;
	count = 0;
	last = (char) (int) tem->car ();
	col = cols;

	while (1)
	  {
		if (tem == two)
		  return count;

		tem = tem->cdr ();

		if (tem == NULL)
		  if (two == NULL)
			/* Increment for line displaying terminating NULL.  */
			return ++count;
		  else
			break;

		if (--col == 0
			|| last == '\n')
		  {
			col = cols;
			count++;
		  }
		last = (char) (int) tem->car ();
	  }

  two:
	if (two == NULL)
	  return 0;

	tem = two;
	count = 0;
	last = (char) (int) tem->car ();
	col = cols;

	/* Same loop as above.  */
	while (1)
	  {
		if (tem == one)
		  return -count;

		if (tem == NULL)
		  if (one == NULL)
			/* Increment for line displaying terminating NULL.  */
			return count;
		  else
			break;

		tem = tem->cdr ();

		if (--col == 0
			|| last == '\n')
		  {
			col = cols;
			count++;
		  }
		last = (char) (int) tem->car ();
	  }

	return 0;
  }

  int
  lines (Cons *cons)
  {
	int count;

	count = 0;

	//while (cons > NULL)
	while (cons != 0)
	  {
		if (cons->car () == LINEFEED)
		  count++;
		cons = cons->cdr ();
	  }

	return count;
  }
}
