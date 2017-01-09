## make tgz=test utworzy archiwum projektu test z plikami sha.
release=

SHA_FLAGS= -DLIB_SHA="dev" -DPROJECT_SHA="dev"

CXXFLAGS= -O3 -finline-functions -std=c++11 -g -O3 -I../tasklib/src `pkg-config gtkmm-3.0 --cflags`

LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread

OBJS= Log.o Utils.o Common.o Ptype.o Conditions.o Scenario.o Gui.o Database.o Datasaver.o Timer.o Media.o Task.o
OBJS:= $(addprefix ./src/,$(OBJS))

export CXXFLAGS LDFLAGS

## $(release) na koñcu, bo wersja spakowana musi byæ najpierw
## zaktualizowana
TASKS:= ablink $(release)

.PHONY: $(TASKS) $(release)

UTILS= project tests

all: libtask.a $(UTILS) $(TASKS) $(release)

clear:
	rm -f libtask.a $(OBJS) $(UTILS) \

libtask.a: $(OBJS:o=cpp) $(OBJS:o=hpp)
	cd src; make
	ar rv $@ $(OBJS)

project: project.cpp libtask.a
	g++ $^ -o $@ $(CXXFLAGS) $(SHA_FLAGS) $(LDFLAGS)

tests: tests.cpp libtask.a
	g++ $^ -o $@ $(CXXFLAGS) $(SHA_FLAGS) $(LDFLAGS)

$(TASKS):
	cd ../$@; make start CXXFLAGS="$(CXXFLAGS) $(SHA_FLAGS)"

$(release):
	git commit -a -v || true; \
	cd ../$@; rm -f start start.tgz; \
	make start CXXFLAGS="$(CXXFLAGS) -DLIB_SHA=\"$(shell cd ../tasklib; git rev-parse HEAD)\" -DPROJECT_SHA=\"$(shell cd ../$@; git rev-parse HEAD)\""
	git commit -a -v || true; \
	cd ../$@; tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $@
