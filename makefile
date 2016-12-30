include makefile.mak

tgz=

TASKS= test
.PHONY: $(TASKS) $(tgz)

UTILS= project guitest

all: $(UTILS) tasks

show:
	echo "$(TASKS)"

clear:
	rm -f $(OBJS) $(UTILS) $(addsufix /start.tgz,$(addprefix ../,$(TASKS))) $(addsufix /start,$(addprefix ../,$(TASKS)))

## Zale¿no¶æ
$(OBJS:.o=.cpp): $(OBJS:.o=.hpp)

$(OBJS): $(OBJS:.o=.cpp)
	## $(foreach var, $@, g++ -c $(var:.o=.cpp) -o $(var) ${CXXFLAGS})

project: project.cpp $(OBJS)
	## g++ $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

guitest: guitest.cpp $(OBJS)
	## g++ $^ -o $@ $(CXXFLAGS) $(LDFLAGS)

tasks: $(TASKS) $(tgz)
	$(foreach var,$+, cd ../$(var)/; make)
	$(foreach var,$^, \
	git commit -a -m "makefile commit" || true; \
	cd ../$(var)/; \
	git rev-parse HEAD > project_sha; \
	rm -f start.tgz; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $(var))
