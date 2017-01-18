## -*- coding: utf-8 -*-

## make tgz=test utworzy archiwum projektu test z plikami sha.
tgz=

EDITOR=emacs

CXXFLAGS= -std=c++11 -g -I../tasklib/src `pkg-config gtkmm-3.0 --cflags` -O3 -finline-functions

LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread

OBJS= Log.o Utils.o Common.o PType.o Conditions.o Scenario.o Gui.o Database.o DataExchange.o Timer.o Graphics.o Media.o Task.o
OBJS:= $(addprefix ./src/,$(OBJS))

SHA_FLAGS=-DLIB_SHA="dev" -DPROJECT_SHA="dev"

export CXXFLAGS LDFLAGS SHA_FLAGS EDITOR

## $(tgz) na końcu, bo wersja spakowana musi być najpierw
## zaktualizowana
tasks:=$(tgz)

.PHONY: $(tasks) $(tgz) test

UTILS= project tests

all: libtask.a $(UTILS) test $(tasks) $(tgz)

clean:
	rm -f libtask.a $(OBJS) $(UTILS) \

libtask.a: $(OBJS:o=cpp) $(OBJS:o=hpp)
	cd src; make
	ar rv $@ $(OBJS)

project: project.cpp libtask.a
	g++ $^ -o $@ $(CXXFLAGS) $(LDFLAGS) $(SHA_FLAGS)

tests: tests.cpp libtask.a
	g++ $^ -o $@ $(CXXFLAGS) $(LDFLAGS) $(SHA_FLAGS)

test:
	cd test; make

$(tasks):
	cd ../$@; make start CXXFLAGS="$(CXXFLAGS) $(SHA_FLAGS)"

$(tgz):
	rm -f ../$@/start ../$@/start.tgz
	git commit -a -v || true; \
	cd ../$@/; git commit -a -v || true
	cd ../$@; make CXXFLAGS="$(CXXFLAGS) -DPROJECT_SHA=$(shell cd ../$@; git rev-parse HEAD) -DLIB_SHA=$(shell git rev-parse HEAD)"; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $@
