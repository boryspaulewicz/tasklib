## make tgz=test utworzy archiwum projektu test z plikami sha.
tgz=

CXXFLAGS= -O3 -finline-functions -std=c++11 -O3 -I../tasklib `pkg-config gtkmm-3.0 --cflags`

LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread

OBJS= Task.o Conditions.o Scenario.o Database.o Gui.o Media.o

export CXXFLAGS LDFLAGS

TASKS= test $(tgz)

.PHONY: $(TASKS) tgzs

UTILS= project guitest

all: libtask.a $(UTILS) tasks tgzs

clear:
	rm -f libtask.a $(OBJS) $(UTILS) \

libtask.a: $(OBJS)
	ar rv $@ $^

$(OBJS): $(OBJS:.o=.hpp)

$(OBJS): ## regu³a implicitna, inaczej zawsze kompiluje wszystkie elementy

project: project.cpp libtask.a

guitest: guitest.cpp libtask.a

tasks: $(TASKS)
	$(foreach var,$^, cd ../$(var)/; make)

tgzs: $(tgz)
	$(foreach var,$^, \
	git commit -a -m "makefile commit" || true; \
	cd ../$(var)/; \
	git rev-parse HEAD > project_sha; \
	rm -f start.tgz; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $(var))
