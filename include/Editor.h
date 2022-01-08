/* Editor.h -- abstract header for Emacs-like editor  */

#ifndef _Editor_h
#define _Editor_h

#include "Cons.h"
#ifdef CERN
#include "CERNBuffer.h"
#else
#include "Buffer.h"
#endif
#include "SupplyWindow.h"
#include "bindables.h"
#include <string>
#include <vector>
#include <curses.h>  /* For KEY_MAX.  */

#define KEY_TABLE_SIZE KEY_MAX
#define INPUT_BUFFER_SIZE 5

using std::vector;
using std::string;

extern Editor *ed;  /* For python.  */

/* Run the function bound, in KEY_MAP, to the last key read.
   Cater for bindings to key maps.  Return 1, or 0 on error.  */
int interpret_last_input (void* key_map);

typedef struct _key_binding
{
  void *func;
  void *param;
} key_binding;

typedef key_binding key_map[KEY_TABLE_SIZE];

class Editor
{
private:
  static string version_string;

public: // protected:  (public for Python)

  int input_buffer[INPUT_BUFFER_SIZE]; /* Last characters read.  */

  vector<buffer::Buffer> buffers;
  //vector<buffer::Buffer>::iterator buffer; /* Current buffer.  */
  buffer::Buffer *buffer;		/* For Python.  */

  vector<SupplyWindow> windows;
  //<SupplyWindow>::iterator window; /* Current window.  */
  SupplyWindow *window;			/* For Python.  */

  cons::ConsT<cons::Cons> *clips; /* List of char cons lists.  */
  cons::ConsT<cons::Cons> *clip; /* Current position in clips.  */
  /* Add CLIP to clips at clip.  */
  void add_clip (cons::Cons *clip, bool append = false);
  /* Flag for appending the next clip to `clips'.  */
  bool append_next_clip;
  bool old_append_next_clip;  /* Shadow for add_clip to check.  */

  /* A table mapping keys to functions.  */
  key_map *key_bindings;

  /* Redraw the display.  Return true if redrawn, else false.  */
  virtual bool display () { return display (false); }
  /* As above.  FORCE forces the redraw to take place.  */
  virtual bool display (bool force) = 0;

  /* Loop reading input and redrawing the display.  Return 1.  */
  int display_loop ();

  virtual void clear_display () = 0; /* Clear the display.  */

  /* Read and return a character.  */
  virtual bool read_char (int *ch) = 0;

  /* Evaluate current buffer's file.  Return 1, or 0 on error. */
  virtual int eval_file () { return 1; }

  /* Evaluate the region.  Return 1, or 0 on error. */
  virtual int eval_region () { return 1; }

  /* Initialise the key bindings.
   Any references in `key_bindings' will be lost.  */
  void init_key_bindings ();


public:

  /* String to display in the echo area.
	 Only lines in the string which end in a newline are displayed.
	 Public for tracing.  */
  string echo;

  int tab_width;				/* Number of spaces per tab.  */
  void set_tab_width (int width) { tab_width = width; }

  /* Size in lines of scroll margins.  Moving into the scroll margin
	 at the top or bottom of the window causes the window to scroll up
	 or down, respectively.  */
  int scroll_margin;

  /* Character displayed to indicate line wrapping.  */
  char wrap_indicator;

  Editor ();
  virtual ~Editor () { delete clips; }

  /* Edit file named FILE_NAME.  Returns 1, or 0 on error.  */
  virtual int run (string file_name) = 0;

  /* Start the editor.  Returns 1, or 0 on error.  */
  int run () { return run (NULL); }


  /* The key binding system assumes that bindings to functions with a
	 parameter are only made for keymaps and extension functions.  */

  /* Bind KEY to FUNCTION.
	 If the parameter previously bound to KEY is an extension function
	 then return it, else return NULL.  */
  virtual void* bind_key (int key, void *function);

  /* Bind KEY to FUNCTION, which takes PARAM.
	 If the parameter previously bound to KEY is an extension function
	 then return it, else return NULL.  */
  virtual void* bind_key (int key, void *function, void *param) = 0;

  /* Bind KEY to FUNCTION in KEYS.
	 If the parameter previously bound to KEY is an extension function
	 then return it, else return NULL.  */
  virtual void* bind_key_in_map (int key,
								 void *function,
								 key_map *KEYS);

  /* Bind KEY to FUNCTION, which takes PARAM, in KEYS.
	 If the parameter previously bound to KEY is an extension function
	 then return it, else return NULL.  */
  virtual void* bind_key_in_map (int key,
								 void *function,
								 void *param,
								 key_map *KEYS);

  /* Bind KEY to KEYS. Return 1, or 0 on error.
	 KEY_MAP will be freed if the binding is replaced.  */
  virtual int bind_key_to_map (int key, key_map *KEYS);

  /* Bind KEY to KEYS in BINDINGS (another key map).
	 Return 1, or 0 on error.
	 KEY_MAP will be freed if the binding is replaced.  */
  virtual int bind_key_to_map_in_map (int key,
									  key_map *keys,
									  key_map *bindings);

  /* Bind SEQUENCE to FUNCTION. Return 1, or 0 on error.
	 Creates any required key maps.  */
  virtual int bind_sequence (char *sequence, void *function);

  /* Bind SEQUENCE to FUNCTION, which takes PARAM.
	 Return 1, or 0 on error.
	 Creates any required key maps.  */
  virtual int bind_sequence (char *sequence,
							 void *function,
							 void *param);


  /* Create a graph from the pairs of floats at point.  */
  void graph_float_pairs ();

  /* Evaluate multiple precision int expression on current line.  */
  void eval_mpi_line ();

  /* Load polyhedron from buffer, then echo description of it.  */
  void ppl ();

  static string version ();		/* Return version string.  */

  friend int interpret_last_input (void* key_map);
  friend int insert_last_input_char ();
  friend int clip_line ();
};

/* If CH is an alphanumeric char return true, else return false.  */
bool is_alphanum (void* ch);

/* Return true address ONE equals address TWO, else return false.  */
bool eq (void* one, void* two);

#endif /* not _Editor_h */
