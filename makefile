## make tgz=test utworzy archiwum projektu test z plikami sha.
tgz=

CXXFLAGS= -O3 -finline-functions -std=c++11 -g -O3 -I../tasklib/src `pkg-config gtkmm-3.0 --cflags`

LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread

OBJS= Log.o Utils.o Common.o Ptype.o Conditions.o Scenario.o Gui.o Database.o Datasaver.o Timer.o Media.o Task.o
OBJS:= $(addprefix ./src/,$(OBJS))

export CXXFLAGS LDFLAGS

## $(tgz) na koñcu, bo wersja spakowana musi byæ najpierw
## zaktualizowana
TASKS:= ablink $(tgz)

.PHONY: $(TASKS) $(tgz)

UTILS= project tests

all: libtask.a $(UTILS) $(TASKS) $(tgz)

clear:
	rm -f libtask.a $(OBJS) $(UTILS) \

libtask.a: $(OBJS:o=cpp) $(OBJS:o=hpp)
	cd src; make
	ar rv $@ $(OBJS)

project: project.cpp libtask.a

tests: tests.cpp libtask.a

$(TASKS):
	cd ../$@; make start

$(tgz):
	git commit -a -v || true; \
	git rev-parse HEAD > ../$@/lib_sha; \
	cd ../$@/; \
	git commit -a -v || true; \
	git rev-parse HEAD > project_sha; \
	rm -f start.tgz; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $@
