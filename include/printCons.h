/* printCons.h -- print a Lisp-like linked list  */

#include "Cons.h"
#include <iostream>

using cons::Cons;

/* Print the list headed by CONS to cout.  */
int print (Cons* cons)
{
  Cons *cdr;
  void* car;

  if (cons == NULL)
	{
	  std::cout << "null";
	  return 1;
	}

  // FIX prints "---" for \n, for Buffer tracing

  car = cons->car ();
  if (car == (void*) 10)
	std::cout << '(' << "---";
  else
	std::cout << '(' << car;

  cdr = cons->cdr ();
  while (cdr > NULL)
	{
	  car = cdr->car ();
	  if (car == (void*) 10)
		std::cout << ' ' << "---";
	  else
		std::cout << ' ' << car;
	  cdr = cdr->cdr ();
	}
  std::cout << ')';

  return 1;
}
