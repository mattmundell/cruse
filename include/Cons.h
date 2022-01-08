/* Cons.h -- header for a Lisp-like construct cell  */

#ifndef _Cons_h
#define _Cons_h

#include <cstddef>  /* For NULL.  */
#include <string>
#include <iostream>

namespace cons
{
  template <class T> class ConsT;
  template <class T> void print_cons (std::ostream &out, ConsT<T> *cons);

  template <class T>
  class ConsT
  {
	/* The cdr is freed when the cons is destructed.  Sharing these
	   cells among lists may require care.  */

  protected:

	T *_car;					/* First element in cell.  */
	ConsT<T> *_cdr;				/* Other element in cell.  */


  public:

	/* Flag for cdr freeing.  */
	bool free_cdr;

	/* Create.  */
	ConsT<T> ()
	  : _car (NULL), _cdr (NULL), free_cdr (true)
	{}

	/* Create, setting car to CAR and cdr to CDR.  */
	ConsT<T> (T *car, ConsT<T> *cdr)
	  : _car (car), _cdr (cdr), free_cdr (true)
	{}

	/* Copy constructor.  */
	ConsT<T> (const ConsT<T>& cons)
	  : _car (cons._car),
		_cdr (cons._cdr),
		free_cdr (cons.free_cdr)
	{}

	/* Free cons.  */
	virtual ~ConsT<T> () { if (free_cdr) delete _cdr; }

	/* Return the car. */
	T* car () { return _car; }

	/* Return the cdr. */
	ConsT<T>* cdr () { return _cdr; }

	/* Set the car to CAR.  Return the car. */
	T* setcar (T *car) { return _car = car; }

	/* Set the cdr to CDR.  Return the cdr. */
	ConsT<T>* setcdr (ConsT<T> *cdr) { return _cdr = cdr; }

	friend void print_cons<> (std::ostream &out, ConsT<T> *cons);
  };

  /* Return the length of the list headed by CONS.  */
  template <class T>
  int
  length (ConsT<T>* cons)
  {
	int len;

	len = 0;
	while (cons)
	  {
		cons = cons->cdr ();
		len++;
	  }

	return len;
  }

  /* Return the last element of the list headed by CONS.  */
  template <class T>
  ConsT<T>*
  last (ConsT<T> *cons)
  {
	//while (cons->cdr () > NULL)
	while (cons->cdr () != 0)
	  cons = cons->cdr ();

	return cons;
  }

  /* Return the element COUNT elements before CONS in LIST.  */
  template <class T>
  ConsT<T>*
  find_but (ConsT<T> *list, ConsT<T> *cons, int *count)
  {
	ConsT<T> *last;

	if (*count == 0)
	  return cons;

	last = list;

	while (list != cons)
	  {
		list = list->cdr ();
		if (*count)
		  (*count)--;
		else
		  last = last->cdr ();
	  }

	return last;
  }

  /* Return the last but COUNT element of the list headed by CONS.  */
  template <class T>
  ConsT<T>*
  last (ConsT<T> *cons, int *count)
  {
	return find_but<T> (cons, NULL, count);
  }

  /* Return the distance in elements between ONE and TWO.

  Return -1 on failure (e.g. if elements are in different lists).
  Return 0 if they are the same Cons, 1 if they are adjacent...

  Faster if ONE comes before TWO.  */
  template <class T>
  int
  distance (ConsT<T> *one, ConsT<T> *two)
  {
	ConsT<T> *tem;
	int count;

	tem = one;
	count = 0;

	while (tem)
	  {
		if (tem == two)
		  return count;

		tem = tem->cdr ();
		count++;
	  }

	tem = two;
	count = 1;

	while (tem)
	  {
		tem = two->cdr ();

		if (tem == one)
		  return count;

		count++;
	  }

	return -1;
  }

  /* Copy the list headed by CONS into a new list.  */
  template <class T>
  ConsT<T>*
  copy_list (ConsT<T> *cons)
  {
	if (cons)
	  return new ConsT<T> (cons->car (),
						   cons::copy_list<T> (cons->cdr ()));
	else
	  return NULL;
  }

  /* Create a list of the characters in STR.  */
  template <class T>
  ConsT<T>*
  string_to_list (std::string str)
  {
	std::string::size_type pos;
	ConsT<T> *tem;

	tem = NULL;
	pos = str.size ();
	while (pos--)
	  tem = new ConsT<T> ((T*) /* FIX: */ (int) str[pos], tem);

	return tem;
  }

  /* Create a list of the characters in STR.  */
  template <class T>
  std::string
  list_to_string (ConsT<T> *list)
  {
	std::string str;

	str = "";
	while (list)
	  {
		str += /* FIX: */ (char) list->car ();
		list = list->cdr ();
	  }

	return str;
  }

  template <class T>
  void
  print_cons (std::ostream &out, ConsT<T> *cons)
  {
	/* Use the protected members, as a friend demo.  */
	out << cons->_car;
	if (cons->_cdr)
	  {
		out << ' ';
		print_cons (out, cons->_cdr);
	  }
  }

  template <class T>
  std::ostream &
  operator<< (std::ostream &out, ConsT<T> &cons)
  {
	out << '(';
	print_cons<T> (out, &cons);
	out << ')';
	out.flush ();
	return out;
  }

  typedef ConsT<void> Cons;


  class BasicCons : public Cons
  {
	/* Freeing of the cdr is left to the caller.  */

  public:

	/* Create.  */
	BasicCons () { free_cdr = false; }

	/* Create, setting car to CAR and cdr to CDR.  */
	BasicCons (void *car, Cons *cdr) : Cons (car, cdr)
	{ free_cdr = false; }

	/* Copy constructor.  */
	BasicCons (const BasicCons& cons) : Cons (cons) { }

	/* Free cons.  */
	~BasicCons () { }
  };
}

#endif /* not _Cons_h */
