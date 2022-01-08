include ../Makefile.config

VPATH = ../src ../include

MOST_HEADERS = Editor.h CursedEditor.h Window.h SupplyWindow.h \
		  	   Buffer.h Cons.h PythonedEditor.h Simple_wrap.h \
		  	   Cons_wrap.h
HEADERS = $(MOST_HEADERS) CursedEditor_wrap.h

ifdef CERN
CERN_LIBS = -lkernlib -lmathlib -lgraflib
HEADERS += CERNBuffer.h
endif

ifdef GMP
GMP_LIBS = -lgmpxx -lgmp -lexpr
endif

ifdef WITH_PPL
PPL_LIBS = -lppl
ifndef GMP
PPL_LIBS += -lgmpxx -lgmp
endif
endif

.PHONY: all clean

all: cruse _cursed_editor.so # _Simple.so _cons.so

cruse: cruse.cc $(OBJECTS) Cruse.h CommandLine.h
	$(CXX) $(CXXFLAGS) $< -lncurses \
		$(PYFLAGS) -isystem /usr/include/python$(PYVERSION) \
		$(shell python-config --cflags) $(shell python-config --ldflags) $(OBJECTS) $(GMP_LIBS) $(PPL_LIBS) \
		 -o $@ -DVERSION=`cat ../etc/VERSION` $(CERN) $(CERN_LIBS)

PythonedEditor.o: PythonedEditor.cc CursedEditor.o PythonedEditor.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ -isystem /usr/include/python$(PYVERSION) $(CERN)

CursedEditor.o: CursedEditor.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@ -isystem /usr/include/python$(PYVERSION) \
		-DVERSION=`cat ../etc/VERSION` $(CERN)

Editor.o: Editor.cc $(HEADERS)
	$(CXX) $(CXXFLAGS) $(GMP) $(WITH_PPL) -c $< -o $@ $(CERN)

%.o: %.cc %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(CERN)

### wrappers for Python

CursedEditor_wrap.h CursedEditor_wrap.cxx: CursedEditor.i Cons.i $(MOST_HEADERS)
	$(SWIG) $(SWIGFLAGS)  -o $@ $<

_cons.so: Cons_wrap.o
	$(CXX) $(CXXFLAGS) $(shell ../python/get_flags.py) -shared -o $@ $<

Cons_wrap.o: Cons_wrap.cxx
	$(CXX) $(CXXFLAGS) -c -isystem /usr/include/python$(PYVERSION) $< -o $@

Cons_wrap.cxx Cons_wrap.h: Cons.i
	$(SWIG) $(SWIGFLAGS) -o $(notdir $@) $<

## module loadable from the Python interpreter

_cursed_editor.so: $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(shell ../python/get_flags.py) -shared -o $@ \
	    $(CERN_LIBS) $(OBJECTS)

_Simple.so: Simple_wrap.o
	$(CXX) $(CXXFLAGS) -shared $< -o $@

## tests of Python wrapping
#  to use define TEST_WRAP for PythonedEditor.cc, include objects in OBJECTS

py_simple_wrap_test.o: py_simple_wrap_test.cc Simple_wrap.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ -isystem /usr/include/python$(PYVERSION)

py_cons_wrap_test.o: py_cons_wrap_test.cc Cons_wrap.h
	$(CXX) $(CXXFLAGS) -c $< -o $@ -isystem /usr/include/python$(PYVERSION)

Simple_wrap.o: Simple_wrap.cxx
	$(CXX) $(CXXFLAGS) -c -isystem /usr/include/python$(PYVERSION) $< -o $@

Simple_wrap.cxx Simple_wrap.h: Simple.h
	$(SWIG) $(SWIGFLAGS) -module Simple -o $(notdir $@) $<

### admin

clean:
	-$(RM) $(OBJECTS) cruse Simple_wrap.o \
		Simple_wrap.cxx Simple_wrap.h Cons_wrap.cxx Cons_wrap.h \
	    _Simple.so _cons.so _cursed_editor.so CursedEditor_wrap.h \
		CursedEditor_wrap.cxx Cons_wrap.o CursedEditor_wrap.o \
		*.pyc Simple.py cons.py cursed_editor.py
