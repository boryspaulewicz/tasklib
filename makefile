## make tgz=test utworzy archiwum projektu test z plikami sha.
tgz=

CXXFLAGS= -O3 -finline-functions -std=c++11 -O3 -I../tasklib/src `pkg-config gtkmm-3.0 --cflags`

LDFLAGS= -L../tasklib `pkg-config sfml-all --libs` `pkg-config gtkmm-3.0 --libs` -lmysqlcppconn -pthread

OBJS= Task.o Conditions.o Scenario.o Database.o Gui.o Media.o
OBJS:= $(addprefix ./src/,$(OBJS))

export CXXFLAGS LDFLAGS

TASKS= posner $(tgz) ## wersja spakowana musi byæ najpierw zaktualizowana

.PHONY: $(TASKS) tgzs

UTILS= project tests

all: libtask.a $(UTILS) tasks tgzs

clear:
	rm -f libtask.a $(OBJS) $(UTILS) \

libtask.a: $(OBJS:o=cpp) $(OBJS:o=hpp)
	cd src; make
	ar rv $@ $(OBJS)

project: project.cpp libtask.a

tests: tests.cpp libtask.a

tasks: $(TASKS)
	$(foreach var,$^, cd ../$(var)/; make)

tgzs: $(tgz)
	$(foreach var,$^, \
	git commit -a -v || true; \
	git rev-parse HEAD > ../$(var)/lib_sha; \
	cd ../$(var)/; \
	git commit -a -v || true; \
	git rev-parse HEAD > project_sha; \
	rm -f start.tgz; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $(var))
