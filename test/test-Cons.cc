/* test-Cons.cc -- tests of Cons  */

#include "Cons.h"
#include "printCons.h"
#include <iostream>
#include <string>

using std::cout;
using cons::BasicCons;
using cons::Cons;

#define make_number(x) ((void*)(x))

void
test_BasicCons ()
{
  int tem;
  Cons l, *l2, l3, ltem;

  l = BasicCons ();
  l2 = new BasicCons (make_number (3), NULL);

  cout << "Testing BasicCons...\n";

  cout << "l.car () " << l.car ();
  cout << "\nl.free_cdr: " << l.free_cdr;
  cout << "\nl.cdr () " << l.cdr ();
  cout << "\nl2->car () " << l2->car ();
  cout << "\nl2->cdr () " << l2->cdr ();

  cout << "\n\nl.setcar (..4)";
  l.setcar (make_number (4));

  cout << "\nl.setcdr (l2)";
  l.setcdr (l2);

  cout << "\n\nl.car () " << l.car ();
  cout << "\nl.cdr () " << l.cdr ();
  cout << "\nl2->car () " << l2->car ();
  cout << "\nl2->cdr () " << l2->cdr ();

  cout << "\nl: " << l;

  {
	//l3 = l + *l2; // old
	Cons *tem;
	tem = new BasicCons ();
	*tem = *l2;
	l3 = l;
	last (&l3)->setcdr (tem);
	cout << "\n\nequiv of l3 = l + l2\nl3: " << l3;
  }

  cout << "\n\nsetcdr l3 to l\nl3: ";
  l3.setcdr (&l);
  cout << l3 << "\nl: " << l << "\nl2: " << *l2;

  cout << "\n\nPrepend 6 to l";
  ltem = l;
  l = BasicCons (make_number (6), &ltem);
  cout << "\nl: " << l << "\nl3: " << l3 << "\nl2: " << *l2;

  cout << "\n\nlast in l3: " << *last (&l3);

  tem = 2;
  cout << "\n\nfind (last in l3) but 2, in l3: "
	   << *find_but (&l3, last (&l3), &tem)
	   << "\ncount arg afterwards: " << tem;

  tem = 2;
  cout << "\n\nlast but 2 in l3: "
	   << *last (&l3, &tem)
	   << "\nlast's count arg after last: " << tem;

#if 0
  // FIX was in test_Buffer.cc
  Cons *cons;

  tem = 1;
  cout << "\nfindbut (ebuf->content, NULL, 1): ";
  cons = find_but (ebuf->content, NULL, &tem);
  print (cons);
#endif

  cout << "\ndone.\n";
}

int
main ()
{
  Cons *cons, *cdr, *copy;

  test_BasicCons ();

  cout << "\nTesting Cons...\n";

  cdr = new Cons (make_number (7), NULL);
  /* cdr: (7) */
  cout << "\ncdr: " << *cdr;
  cons = new Cons (make_number (6), cdr);
  /* cons: (6 7) */
  cout << "\ncons: " << *cons;
  cons->setcdr (new Cons (make_number (8), NULL));
  /* cons: (6 8), cdr (7) */
  cout << "\ncons: " << *cons
	   << "\ncdr: " << *cdr;

  delete cdr;
  delete cons;
  /* FIX verify that cons.cdr has been freed.  */

  cons = new Cons (make_number (1), NULL);
  cons->setcdr (new Cons (make_number (2), NULL));
  cons->cdr ()->setcdr (new Cons (make_number (3), NULL));
  cout << "\n\ncons: " << *cons;
  copy = cons::copy_list<void> (cons);
  cout << "\ncopy = copy_list<void> (cons)\ncopy: " << *copy;
  cons->setcar (make_number (4));
  cout << "\ncons->setcar(..4)";
  cout << "\ncons: " << *cons;
  cout << "\ncopy: " << *copy;

  delete cons;
  delete copy;

  struct { Cons *cons; char *str; } struct_eg;
  struct_eg.str = "foo";
  cout << "\n\nstruct_eg.str: " << struct_eg.str;

  struct_eg.cons = cons::string_to_list<void> (struct_eg.str);
  cout << "\nstruct_eg.cons = string_to_list<void> (struct_eg.str)"
	   << "\n*struct_eg.cons: " << *struct_eg.cons;

  delete cons;

  Cons one (make_number (7), NULL);
  cout << "\n\none: " << one
	   << "\nCons two = one";
  /* Copy construct.  */
  Cons two = one;
  cout << "\none: " << one
	   << "\ntwo: " << two;

  cout << "\n\ndone.\n";

  return EXIT_SUCCESS;
}
