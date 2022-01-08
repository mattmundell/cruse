/* gdit.cc -- command line interface to very simple GTK editor  */

#include "GTKEditor.h"
#include "GetOpt.h"
#include <iostream>

using std::cout;

void
usage ()
{
  cout << "Usage: ";
  cout << program_invocation_short_name;
  cout << " [OPTION]... FILE\n";
  cout << "Edit FILE.\n\n";
  cout << "OPTIONS can be:\n";
  cout << "  -h           display this message\n";
  cout << "  -s LINES     set size of scroll margin\n";
  cout << "  -t SPACES    set number of spaces per tab.\n";
}

int
main (int argc, char** argv)
{
  if (argc > 1)
	{
	  Gtk::Main kit (argc, argv);
	  GTKEditor editor;
	  GetOpt getopt (argc, argv, "s:t:h");
	  int option, opts;

	  opts = 0;

	  while ((option = getopt ()) != EOF)
		switch (option)
		  {
		  case 'h':
		  case '?':
			usage ();
			return EXIT_SUCCESS;
		  case 's':
			// FIX c++
			editor.scroll_margin = atoi (getopt.optarg);
			opts += 2;
			break;
		  case 't':
			// FIX c++
			editor.tab_width = atoi (getopt.optarg);
			opts += 2;
			break;
		  }

	  if (GetOpt::last_nonopt - GetOpt::first_nonopt == 1)
		{
		  // FIX ?
		  //if (editor.run (getopt.nargv[optind]))
		  //if (editor.run (argv[optind]))
		  if (editor.run (argv[GetOpt::first_nonopt]))
			return EXIT_SUCCESS;
		}
	  else
		usage ();

	  return EXIT_FAILURE;
	}

  usage ();
  return EXIT_FAILURE;
}
