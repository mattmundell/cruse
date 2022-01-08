include ../Makefile.config

VPATH = ../src ../include

.PHONY: test clean

test: test-Cons test-Buffer
	test-Cons
	test-Buffer

test-Buffer: test-Buffer.cc Buffer.o Cons.h Buffer.h printCons.h
	$(CXX) $(CXXFLAGS) $< ../src/Buffer.o -o $@

test-Cons: test-Cons.cc Cons.h printCons.h
	$(CXX) $(CXXFLAGS) $< -o $@

# simple test of c-py interoperation: test-inter ../python/test_inter.py
test-inter: test-inter.cc $(OBJECTS)
	$(CXX) $(CXXFLAGS) $< -lncurses $(PYFLAGS) -isystem /usr/include/python$(PYVERSION) \
		$(shell ../python/get_flags.py) $(OBJECTS) -o $@

clean:
	-$(RM) $(OBJECTS) test-Cons test-Buffer test-inter
