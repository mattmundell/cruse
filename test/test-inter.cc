/* test-inter -- test C++ Python interaction  */

#include "CursedEditor.h"
#include <Python.h>
#include <stdio.h>
#include <iostream>

int
main (int argc, char *argv[])
{
  CursedEditor etor;
  //PyObject *pSimple;
  FILE* file;

  if (argc != 2)
	{
	  fprintf(stderr,
			  "Usage: %s pythonfile file_name\n",
			  program_invocation_short_name);
	  return 1;
	}

  ed = &etor;
  std::cout << "ed->tab_width: " << ed->tab_width << std::endl;

  etor.init_python ();

#if 1
  //PyRun_SimpleString("print sys.builtin_module_names\n");

  if ((file = fopen (argv[1], "r")) == NULL)
	{
	  fprintf (stderr, "Failed to open stream %s\n", argv[1]);
	  return 1;
	}

  if (PyRun_SimpleFile(file, argv[1]))
	{
	  PyErr_Print ();
	  fprintf (stderr, "Failed to run file %s\n", argv[1]);
	  return 1;
	}

  //PyRun_SimpleString ("print \"multiply (3,3): \", multiply (3,3)");

  std::cout << "ed->tab_width: " << ed->tab_width << std::endl;

  Py_Exit (0);
#endif
  return 0;
}
