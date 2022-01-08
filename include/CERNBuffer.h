/* CERNBuffer.h -- header for CERNLIB enhanced file buffer  */

#ifndef _CERNBuffer_h
#define _CERNBuffer_h

#include "Buffer.h"
#include "Cons.h"
#include <string>
#include <sstream>
#include <iostream>
#include <cfortran.h>
#include <kernlib.h>

namespace buffer
{

  class CERNBuffer : public Buffer
  {
  public:

	CERNBuffer () {}
	/* Read the file named FILE_NAME into the buffer.  */
	CERNBuffer (std::string file_name) : Buffer (file_name) {}

	~CERNBuffer () {}

	/* Insert N pairs of random values at point.
	 Leave point at the beginning of the values, and mark at the
	 end.  */
	void random_fill (int n)
	{
	  float x, y;
	  cons::Cons *tem;

	  forward_chars (-1); /* In case point at end.  */
	  tem = point;
	  forward_chars (1);
	  while (n--)
		{
		  std::stringstream str;
		  RANNOR (x,y);
		  str << x << " " << y << std::endl;
		  insert (string_to_list<void> (str.str ()));
		}
	  mark = point;
	  if (tem)
		point = tem->cdr ();
	  else
		point = content;
	}
  };
}

#endif /* not _CERNBuffer_h */
