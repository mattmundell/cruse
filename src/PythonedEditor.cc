/* PythonedEditor.cc -- Python-extensible Editor  */

#include "PythonedEditor.h"
#include "Cons.h"
#include "Simple.h"
#include "CursedEditor_wrap.h"
#include <Python.h>
#include <cstdio>

Editor *ed;

#ifdef TEST_WRAP
std::string cons_wrap_test ();
std::string Simple_wrap_test (PythonedEditor *py_ed);
#endif

extern "C" {
  void init_cons (void);
  void init_Simple (void);
}

PythonedEditor::PythonedEditor ()
{
  _args = std::pair<int,char**> (-1, NULL);
}

PythonedEditor::PythonedEditor (int argc, char** argv)
{
  Py_SetProgramName (argv[0]);
  _args = std::pair<int,char**> (argc, argv);
}

PythonedEditor::~PythonedEditor ()
{
  if (Py_IsInitialized ())
	Py_Finalize();
}

int
PythonedEditor::init_python ()
{
  FILE *file;

  Py_Initialize ();

  if (_args.first > -1)
	PySys_SetArgv (_args.first, _args.second);

#ifdef TEST_WRAP
  init_Simple ();
  init_cons ();
  PyRun_SimpleString ("import Simple\n");
  PyRun_SimpleString ("import cons\n");
  echo += Simple_wrap_test (this) + "\n";
  echo += cons_wrap_test () + "\n";
#endif

  init_cursed_editor ();

  //echo += export_this () + "\n";
  ed = this;

  PyRun_SimpleString ("import cursed_editor\n");
  PyRun_SimpleString ("from cursed_editor import *\n");
  PyRun_SimpleString ("__builtins__.this = cvar.ed\n");
  PyRun_SimpleString ("__builtins__.hil = cvar.hil\n");

  /* Extension initialisation.  */
  file = fopen ("../python/init.py", "r");
  if (file)
	{
	  // FIX stdout,stderr
	  if (PyRun_SimpleFile (file, "../python/init.py"))
		{
		  fclose (file);
		  return 0;
		}
	  fclose (file);
	}
  return 1;
}

int
PythonedEditor::eval_file ()
{
  FILE *file, *ostream;
  const char *name;
  PyObject *py_file, *py_mod, *py_dict;

  char *stream_buffer;
  size_t stream_buffer_size;

  if (Py_IsInitialized () == 0)
	{
	  echo += "eval_file requires an initialised Python\n";
	  return 0;
	}

  ostream = open_memstream (&stream_buffer, &stream_buffer_size);
  py_file = PyFile_FromFile (ostream, "eval_file_ostream",
							 "w+", NULL /* FIX? */);

  py_mod = PyImport_AddModule ("sys");
  if (py_mod == NULL)
	{
	  echo += "Failed to add module sys\n";
	  goto err;
	}
  py_dict = PyModule_GetDict (py_mod); /* Borrow reference.  */

  if (PyDict_SetItemString (py_dict, "stdout", py_file))
	{
	  echo += "Failed to set stdout\n";
	  goto err;
	}

  if (PyDict_SetItemString (py_dict, "stderr", py_file))
	{
	  echo += "Failed to set stderr\n";
	  goto err;
	}


  name = buffer->file_name.c_str ();
  file = fopen (name, "r");
  if (file)
	{
	  if (buffer->modified)
		buffer->write ();

	  PyRun_SimpleFile (file, name);

	  fclose (ostream);
	  if (stream_buffer_size)
		{
		  echo += "Output from " + buffer->file_name + ":\n";
		  echo += stream_buffer;
		}
	  else
		echo += "Evaluated " + buffer->file_name + "\n";
	  if (stream_buffer)
		free (stream_buffer);

	  return 1;
	}

  echo += "Failed to open " + buffer->file_name + "\n";

 err:
  fclose (ostream);
  if (stream_buffer)
	free (stream_buffer);

  return 0;
}

int
PythonedEditor::eval_region ()
{
  FILE *ostream;
  PyObject *py_file, *py_mod, *py_dict;
  string str;

  char *stream_buffer;
  size_t stream_buffer_size;

  if (buffer->mark == buffer::hil)
	{
	  echo += "Set mark to create a region\n";
	  return 0;
	}

  if (Py_IsInitialized () == 0)
	{
	  echo += "eval_file requires an initialised Python\n";
	  return 0;
	}

  str = cons::list_to_string<void> (buffer->copy_to (buffer->mark));

  ostream = open_memstream (&stream_buffer, &stream_buffer_size);
  py_file = PyFile_FromFile (ostream, "eval_file_ostream",
							 "w+", NULL /* FIX? */);

  py_mod = PyImport_AddModule ("sys");
  if (py_mod == NULL)
	{
	  echo += "Failed to add module sys\n";
	  goto err;
	}
  py_dict = PyModule_GetDict (py_mod); /* Borrow reference.  */

  if (PyDict_SetItemString (py_dict, "stdout", py_file))
	{
	  echo += "Failed to set stdout\n";
	  goto err;
	}

  if (PyDict_SetItemString (py_dict, "stderr", py_file))
	{
	  echo += "Failed to set stderr\n";
	  goto err;
	}

  PyRun_SimpleString (str.c_str ());
  fclose (ostream);
  if (stream_buffer_size)
	{
	  echo += "Output from region:\n";
	  echo += stream_buffer;
	}
  else
	echo += "Evaluated region\n";
  if (stream_buffer)
	free (stream_buffer);

  return 1;

 err:
  fclose (ostream);
  if (stream_buffer)
	free (stream_buffer);

  return 0;
}

std::string
PythonedEditor::export_this ()
{
  PyObject *py_ed, *py_mod, *py_dict;
  std::string ret;

  ret = "";

  // FIX check obj refs

  /* Set the object "ed" in Python to a wrapped CursedEditor holding
	 this (the current CursedEditor instance).  */

  py_ed = SWIG_NewPointerObj (this, SWIGTYPE_p_CursedEditor, 1);

  if (PyRun_SimpleString ("ed = CursedEditor ()\n")
	  // FIX
	  || PyRun_SimpleString ("_ed_old = ed.__dict__['this']\n")
	  /* Change tab_width to check that current instance is used.  */
	  || PyRun_SimpleString ("ed.tab_width = 11\n")
	  || PyRun_SimpleString ("_ed = 0\n"))
	{
	  ret = "Failed to create or set _ed";
	  goto err;
	}

  py_mod = PyImport_AddModule ("__main__");
  if (py_mod == NULL)
	{
	  ret += "Failed to add module __main__";
	  goto err;
	}
  py_dict = PyModule_GetDict (py_mod); /* Borrow reference.  */

  if (PyDict_SetItemString (py_dict, "_ed", py_ed))
	{
	  ret += "Failed to set _ed";
	  goto err;
	}

  if (PyRun_SimpleString ("print \"_ed: \", _ed, \", _ed_old = \", _ed_old\n"))
	ret += "Failed to print _ed";

  if (PyRun_SimpleString ("cursed_editor._swig_setattr (ed, CursedEditor, 'this', _ed)\n"))
	ret += "Failed to set _ed into ed";
#if 0
  if (PyRun_SimpleString ("del _ed"))
	ret += "Failed to free _ed";
#endif

 err:
  return ret;
}

void
bind_sequence (char *sequence, PyObject *func)
{
  ed->bind_sequence (sequence, (void*) eval_py_function, (void*) func);
}

void
bind_key (int key, PyObject *func)
{
  ed->bind_key (key, (void*) eval_py_function, (void*) func);
}

void*
PythonedEditor::bind_key_in_map (int key, void *func,
								 void *param, key_map *keys)
{
  void *tem;
  tem = Editor::bind_key_in_map (key, func, param, keys);
  if (param && func != (void*) interpret_last_input)
	/* FIX assumes Ed..::bind_key succeeds
	   (Ed..::bind_key has an ambiguous return) */
	Py_INCREF ((PyObject*) param);
  if (tem)
	/* This is safe as Ed..::bind_key.. only returns extension
	   functions.  */
	Py_DECREF ((PyObject*) tem);
  return tem;
}

void*
PythonedEditor::bind_key (int key, void *func, void *param)
{
  void *tem;
  tem = Editor::bind_key (key, func, param);
  if (param && func != (void*) interpret_last_input)
	/* FIX assumes Ed..::bind_key succeeds
	   (Ed..::bind_key has an ambiguous return) */
	Py_INCREF ((PyObject*) param);
  if (tem)
	/* This is safe as Ed..::bind_key.. only returns extension
	   functions.  */
	Py_DECREF ((PyObject*) tem);
  return tem;
}

int
eval_py_function (void *func)
{
  PyObject* py_func;

  py_func = (PyObject*) func;

  if (PyCallable_Check (py_func))
	{
	  PyObject* py_tup;

	  py_tup = Py_BuildValue ("()");
	  if (PyObject_Call (py_func, py_tup, NULL))
		return 1;
	}

  return 0;
}

int
set_tab_width (vector<buffer::Buffer>::iterator buffer, int width)
{
  return buffer->tab_width = width;
}

void*
char_to_pointer (char ch)
{
  return (void*) (int) ch;
}

char*
c_str (PyObject *py_str)
{
  char *str;

  str = PyString_AsString (py_str);
  if (str)
	return strdup (str);
  return NULL;
}
