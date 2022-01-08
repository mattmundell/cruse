/* cruse.cc -- execute very basic curses-based text editor  */

#include "Cruse.h"

int
main (int argc, char** argv)
{
  Cruse editor (argc, argv);
  return editor.start ();
}
