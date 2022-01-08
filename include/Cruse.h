/* Cruse.h -- very simple curses-based text editor  */

#ifndef _Cruse_h
#define _Cruse_h

#include "CommandLine.h"
#include "CursedEditor.h"
#include <string>
#include <iostream>
#include <sstream>

using std::string;

class Cruse : public CommandLine<Cruse>, public CursedEditor
{
public:

  Cruse (int argc, char **argv)
	: CommandLine<Cruse> (argc, argv), CursedEditor (argc, argv)
  {
	args_expected = 1;

	usage = "\
Usage: %s [OPTION]... FILE_NAME\n\
Edit file named FILE_NAME.\n\n\
OPTION can be:\n\
  -h           display this message and exit\n\
  -s LINES     set size of scroll margin\n\
  -t CHARS     set display width of tab\n\
  -v           display version string and exit.\n";

	/* Per-argument callbacks.  */
	handlers["h"] = &Cruse::print_usage_and_exit;
	handlers["v"] = &Cruse::print_version;
	handlers["t:"] = &Cruse::set_tab_width;
	handlers["s:"] = &Cruse::set_scroll_margin;
  }

  virtual ~Cruse () {}

  /* Run the editor.  Return 1, or 0 on error.  */
  int start ()
  {
	process_args (argc, argv);
	if (init_python ()
		&& run (next_arg ()))
	  return EXIT_SUCCESS;

	return EXIT_FAILURE;
  }

  /* Print version string to cout, and exit with EXIT_SUCCESS.  */
  void
  print_version (string arg)
  {
#define xstr(s) str (s)
#define str(s) #s
	std::cout << "Cruse " << xstr (VERSION)
			  << " (extends " << CursedEditor::version () << ")\n";
	exit (EXIT_SUCCESS);
  }

  /* Set default scroll margin size to the number in ARG.  */
  void
  set_scroll_margin (string arg)
  {
	std::stringstream ss (arg);
	int margin;
	ss >> margin;
	scroll_margin = margin;
  }

  /* Set default tab width to the number in ARG.  */
  void
  set_tab_width (string arg)
  {
	std::stringstream ss (arg);
	int width;
	ss >> width;
	tab_width = width;
  }
};

#endif /* not _Cruse_h */
