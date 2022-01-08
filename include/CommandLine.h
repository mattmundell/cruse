/* CommandLine.h -- command line argument processing  */

#ifndef _CommandLine_h
#define _CommandLine_h

#include <string>
#include <vector>
#include <map>
#include <iostream>

using std::vector;
using std::string;
using std::map;

char *program_invocation_short_name, *program_invocation_name;

template<class T>
class CommandLine
{
protected:

  /* Option handler mapping.  */
  map<string,void(T::*)(string)> handlers;

  /* The object containing the handlers.  */
  T *handlers_object;

  vector<string> args;			/* Normal arguments.  */
  vector<string>::iterator current_arg;

  int argc;
  char **argv;

  string usage;

  /* Number of normal arguments expected.  */
  int args_expected;

public:

  CommandLine () : argc (0), argv (NULL), handlers_object (this) {};
  CommandLine (int argc, char **argv);
  CommandLine (int argc, char **argv, T *handlers_object);
  virtual ~CommandLine () {}

  /* Process ARGC arguments from ARGV.
     Typically called in subclass constructor.  */
  void process_args (int argc, char **argv);

  /* Return the next normal argument.
	 process_args () must be called before this.  */
  string next_arg ();

  /* Print `usage' to cout.  */
  void print_usage (string arg);

  /* Print `usage' to cout, then exit with EXIT_SUCCESS.  */
  void print_usage_and_exit (string arg);

  /* Run the program.  Return EXIT_SUCCESS or EXIT_FAILURE.  */
  virtual int start () = 0;
};

template <class T>
CommandLine<T>::CommandLine (int argc, char **argv)
{
  string str (argv[0]);
  string::size_type pos;

  pos = str.rfind ('/');
  if (pos == string::npos)
	pos = 0;
  else
	pos++;
  program_invocation_short_name = strdup (str.substr (pos).c_str ());
  program_invocation_name = strdup (argv[0]);
  this->argc = argc;
  this->argv = argv;
  this->handlers_object = (T*) this;
}

template <class T>
CommandLine<T>::CommandLine (int argc, char **argv, T *handlers_object)
{
  string str (argv[0]);
  string::size_type pos;

  pos = str.rfind ('/');
  if (pos == string::npos)
	pos = 0;
  else
	pos++;
  program_invocation_short_name = strdup (str.substr (pos).c_str ());
  program_invocation_name = strdup (argv[0]);
  this->argc = argc;
  this->argv = argv;
  this->handlers_object = handlers_object;
}

template <class T>
void
CommandLine<T>::print_usage (string arg)
{
  std::cout << usage;
}

template <class T>
void
CommandLine<T>::print_usage_and_exit (string arg)
{
  print_usage (arg);
  exit (EXIT_SUCCESS);
}

template <class T>
string
CommandLine<T>::next_arg ()
{
  string ret;
  ret = *current_arg;
  current_arg++;
  return ret;
}

template <class T>
void
CommandLine<T>::process_args (int argc, char **argv)
{
  int tem;
  string::iterator iter;
  string::size_type pos;

  /* Substitute usage abbreviations.  */
  pos = 0;
  while ((pos = usage.find ("%", pos)) != string::npos)
	switch (usage[pos + 1])
	  {
	  case '%':
		usage.erase (pos++ + 1, 1);
		break;
	  case 's':
		usage.replace (pos, 2, program_invocation_short_name);
		pos += strlen (program_invocation_short_name);
		break;
	  case 'l':
		usage.replace (pos, 2, program_invocation_name);
		pos += strlen (program_invocation_name);
		break;
	  default:
		pos++;
	  }

  /* Call handler for each option.  */
  tem = 0;
  while (++tem < argc)
	{
	  typename map<string,void(T::*)(string)>::iterator handler_iter;

	  if (argv[tem][0] == '-')
		{
		  if (argv[tem][1] == '-')
			if (argv[tem][2] == '\0')
			  {
				/* "--" means end of options.  */
				while (++tem < argc)
				  args.push_back (argv[tem]);
				break;
			  }
			else
			  std::cout << "FIX long opt" << std::endl;
		  else
			{
			  /* Possible short option(s).  */
			  string str;

			  str = argv[tem][1];
			  str += ':';
			  handler_iter = handlers.find (str);
			  if (handler_iter != handlers.end ())
				/* An option which takes an argument.  */
				if (argv[tem][2] == '\0')
				  /* Use the next argv element, even if it has a
					 leading '-'.  */
				  (handlers_object->*handler_iter->second) (string (argv[++tem]));
				else
				  (handlers_object->*handler_iter->second) (string (argv[tem] + 1));
			  else
				{
				  int arg;

				  arg = 0;
				  while (argv[tem][++arg] != '\0')
					{
					  str = argv[tem][arg];
					  handler_iter = handlers.find (str);
					  if (handler_iter != handlers.end ())
						(handlers_object->*handler_iter->second) ("");
					  // FIX else deal with option (err msg?)
					}
				}
			}
		}
	  else
		args.push_back (argv[tem]);
	}

  if (args_expected != (int) args.size ())
	print_usage_and_exit ("");

  current_arg = args.begin ();
}

#endif /* not _CommandLine_h */
