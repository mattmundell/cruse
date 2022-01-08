/* test-Cons.cc -- tests of Buffer  */

#include "Buffer.h"
#include "printCons.h"
#include <iostream>

using std::cout;
using buffer::Buffer;

#define PRINT(x)								\
  do {											\
	if (x)										\
	  print (x);								\
	else										\
	  cout << (x);								\
  } while (0)

int
main ()
{
  Buffer buf = Buffer ();
  Buffer *buf2 = new Buffer ("test.data");
  Buffer *ebuf = new Buffer ("test-empty.data");

  cout << "Testing Buffer...\n";

  cout << "\nbuffer.content: ";
  //cout << buf.content;
  print (buf2->content);

  cout << "\nbuffer2.content: ";
  //cout << buf.content;
  print (buf2->content);

  cout << "\nbuf2->forward_chars (20)\npoint: ";
  buf2->forward_chars (20);
  print (buf2->point);
  //cout << buf.content;

  cout << "\nbuf2->insert ('\\n')\ncontent: ";
  buf2->insert_char ((void*) (int) '\n');
  print (buf2->content);
  cout << "\npoint: ";
  print (buf2->point);

  cout << "\nbuff2->end_of_line (): ";
  print (buf2->end_of_line ());

  cout << "\nbuff2->beginning_of_line (): ";
  print (buf2->beginning_of_line ());

  cout << "\nbuff2->forward_line (): ";
  buf2->forward_line ();
  print (buf2->point);

  cout << "\nbuff2->forward_line (): ";
  buf2->forward_line ();
  print (buf2->point);

  cout << "\nbuf2->next_window_line (-1, 2): ";
  buf2->point = buf2->next_window_line (-1, 2);
  print (buf2->point);

#if 0
  cout << "\nbuf2->backward_line (): ";
  buf2->backward_line ();
  print (buf2->point);
#endif

#if 0
  cout << "\nbuf2->beginning_of_line (): ";
  print (buf2->beginning_of_line ());
#endif

  cout << "\nbuf2->next_window_line (-1, 4): ";
  buf2->point = buf2->next_window_line (-1, 4);
  print (buf2->point);

  cout << "\nbuf2->next_window_line (-1, 4): ";
  buf2->point = buf2->next_window_line (-1, 4);
  print (buf2->point);

  cout << "\nbuf2->next_window_line (-1, 4): ";
  buf2->point = buf2->next_window_line (-1, 4);
  print (buf2->point);

  cout << "\nwindow_line_distance (buf2->point, NULL): "
	   << buffer::window_line_distance (buf2->point, NULL, 4)
	   << std::endl;

  cout << "\nwindow_line_distance (buf2->content, buf2->point): "
	   << buffer::window_line_distance (buf2->content, buf2->point, 4)
	   << std::endl;


  /* Empty Buffer.  */

  cout << "\nebuf->content: ";
  print (ebuf->content);
  cout << "\nebuf->point: ";
  print (ebuf->point);

  cout << "\nebuf->insert ('a')\nebuf->insert ('\n')\ncontent: ";
  ebuf->insert_char ((void*) (int) 'a');
  ebuf->insert_char ((void*) (int) '\n');

  print (ebuf->content);
  cout << "\npoint: ";
  print (ebuf->point);

  delete buf2;
  delete ebuf;

  cout << "\ndone.\n";

  return (EXIT_SUCCESS);
}
