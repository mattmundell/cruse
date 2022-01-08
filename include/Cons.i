/* Cons SWIG interface definition. */
%module cons
%{
#include "Cons.h"
%}

%rename(ConsT_Cons) ConsT<Cons>;

namespace cons
{
  class Cons
  {
  protected:

	void *_car;
	Cons *_cdr;

  public:

	bool free_cdr;
	Cons ()
	  : _car (NULL), _cdr (NULL), free_cdr (true);

	Cons (void *car, Cons *cdr);
	virtual ~Cons ();

	void* car ();
	Cons* cdr ();
	void* setcar (void *car);

	Cons* setcdr (Cons *cdr);
  };

#if 0
  class ConsT<Cons>
  {
  protected:

	Cons *_car;
	Cons *_cdr;

  public:

	bool free_cdr;
	ConsT<Cons> ()
	  : _car (NULL), _cdr (NULL), free_cdr (true);

	ConsT<Cons> (Cons *car, Cons *cdr);
	virtual ~Cons ();

	Cons* car ();
	Cons* cdr ();
	Cons* setcar (Cons *car);

	Cons* setcdr (Cons *cdr);
  };
#endif
}
