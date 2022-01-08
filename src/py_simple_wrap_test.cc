/* py_simple_wrap_test.cc -- test wrap local BasicSimple for Python */

#include "PythonedEditor.h"
#include "Simple_wrap.h"
#include <string>
#include <Python.h>

std::string
Simple_wrap_test (PythonedEditor *py_ed)
{
  BasicSimple *bs;
  PyObject *py_bs, *py_mod, *py_dict;
  std::string ret;

  bs = new BasicSimple ();
  ret = "";

  // FIX check obj refs

  /* A C equiv of: bs = Simple (), where bs wraps the BasicSimple
	 allocated above.  */

  py_bs = SWIG_NewPointerObj(bs, SWIGTYPE_p_BasicSimple, 1);

  if (PyRun_SimpleString ("bs = Simple.BasicSimple ()")
	  /* Change val to confirm that the C BasicSimple is used.  */
	  || PyRun_SimpleString ("bs.val = 10")
	  || PyRun_SimpleString ("_bs = 0"))
	{
	  ret = "Failed to create or set bs";
	  goto err;
	}

  py_mod = PyImport_AddModule ("__main__");
  if (py_mod == NULL)
	{
	  ret += "Failed to get module";
	  goto err;
	}
  py_dict = PyModule_GetDict (py_mod); /* Borrow reference.  */

  if (PyDict_SetItemString (py_dict, "_bs", py_bs))
	{
	  ret += "Failed to set _bs";
	  goto err;
	}

  if (PyRun_SimpleString ("Simple._swig_setattr (bs, Simple.BasicSimple, 'this', _bs)"))
	ret += "Failed to _swig_setattr";

  // FIX del _bs

 err:
  return ret;
}
