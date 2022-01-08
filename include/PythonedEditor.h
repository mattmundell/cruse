/* PythonedEditor.h -- header for Python-extensible Editor  */

#ifndef _PythonedEditor_h
#define _PythonedEditor_h

#include "Editor.h"
#include <string>
#include <utility>
#include <Python.h>

class PythonedEditor : public Editor
{
protected:
  std::pair<int,char**> _args;	/* For argument passing.  */

public:

  PythonedEditor ();
  PythonedEditor (int argc, char** argv);
  virtual ~PythonedEditor ();

  /* Initialise Python.  Must be called before the methods below.
	 Return 1, or 0 on error. */
  int init_python ();

  /* Evaluate current buffer's file.  Return 1, or 0 on error. */
  int eval_file ();

  /* Evaluate the current region.  Return 1, or 0 on error. */
  int eval_region ();

  /* Export the current instance to Python.
     Return empty string on success, else an error message.
     (Currently a lesser equivalent of this is done instead, using the
     ed global variable). */
  string export_this ();

  /* Bind KEY to Python function FUNCTION, which takes PARAM.
	 Return 1.  */
  virtual void* bind_key (int key, void *function, void *param);

  /* Bind KEY in KEYS to Python function FUNCTION, which takes PARAM.
	 Return 1.  */
  virtual void* bind_key_in_map (int key, void *func,
								 void *param, key_map *keys);
};

/* Set tab width in BUFFER to WIDTH.  Return the width.
   For testing.  */
int
set_tab_width (vector<buffer::Buffer>::iterator buffer, int width);

/* Return the value of CH as an address, for Python.  */
void*
char_to_pointer (char ch);

/* Return a copy of the string in PY_STR, or NULL on error.
   Freeing of the returned string is left to the caller.  */
char*
c_str (PyObject *py_str);

/* Evaluate Python function FUNCTION.  Return 1, or 0 on error.  */
int eval_py_function (void *function);

/* Bind KEY to Python function FUNCTION.  */
void bind_key (int key, PyObject *function);

/* Bind SEQUENCE to Python function FUNCTION.  */
void bind_sequence (char *sequence, PyObject *function);

#endif /* not _PythonedEditor_h */
