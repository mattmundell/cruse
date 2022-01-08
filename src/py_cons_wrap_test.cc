/* py_cons_wrap_test.cc -- test wrapping a local Cons, for Python */

#include "Cons.h"
#include "Cons_wrap.h"
#include <string>
#include <Python.h>

std::string
cons_wrap_test ()
{
  cons::Cons *cons;
  PyObject *py_cons, *py_mod, *py_dict;
  std::string ret;

  cons = new cons::Cons ();
  ret = "";

  // FIX check obj refs

  /* A C equiv of: cons = Cons (), which wraps the Cons allocated
	 above.  */
  py_cons = SWIG_NewPointerObj (cons, SWIGTYPE_p_cons__Cons, 1);

  if (PyRun_SimpleString ("cc = cons.Cons ()")
	  /* Change cdr to confirm that the Cons in `cons' used.  */
	  || PyRun_SimpleString ("cc.setcdr (cons.Cons ())")
	  || PyRun_SimpleString ("_cc = 0"))
	{
	  ret = "Failed to create or set cc";
	  goto err;
	}

  py_mod = PyImport_AddModule ("__main__");
  if (py_mod == NULL)
	{
	  ret += "Failed to get module";
	  goto err;
	}
  py_dict = PyModule_GetDict (py_mod); /* Borrow reference.  */

  if (PyDict_SetItemString (py_dict, "_cc", py_cons))
	{
	  ret += "Failed to set _cc";
	  goto err;
	}

  if (PyRun_SimpleString ("Simple._swig_setattr (cc, cons.Cons, 'this', _cc)"))
	ret += "Failed to _swig_setattr";

  if (PyRun_SimpleString ("del _cc"))
	ret += "Failed to free _cc";

 err:
  return ret;
}
