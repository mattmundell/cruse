/* Simple.h -- A simple class, for testing  */

#ifdef SWIG
%module Simple
%{
#include "Simple.h"
%}
#endif

#ifndef _BasicSimple_h
#define _BasicSimple_h

class BasicSimple
{
public:
  int val;

  BasicSimple () : val (33) {};
};

#endif
