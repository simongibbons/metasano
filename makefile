CXX=g++
CFLAGS=-std=c++11 -g -O2 -Wall -Wextra -I.

UTILSOURCES = $(wildcard util/*.cc)
UTILOBJECTS = $(UTILSOURCES:.cc=.o)

PROGSOURCES = $(wildcard solutions/**/*.cc) $(wildcard solutions/*.cc)
PROGEXES = $(PROGSOURCES:.cc=.exe)

all: $(PROGEXES)

libutil.a: $(UTILOBJECTS)
	ar -cvq libutil.a $(UTILOBJECTS)

%.o: %.cc
	$(CXX) -c $(CFLAGS) $< -o $@

%.exe: %.cc libutil.a
	$(CXX) $(CFLAGS) -L. $< -lssl -lutil -o $@

clean:
	rm -rf $(UTILOBJECTS)
	rm -rf libutil.a
	rm -rf $(PROGEXES)

