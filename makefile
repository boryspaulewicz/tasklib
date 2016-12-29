include makefile.mak

TASKS:= $(shell ls ../ | sed /tasklib/d)

UTILS= project guitest

all: $(UTILS) $(TASKS)

show:
	echo "$(TASKS)"

clear:
	rm -f $(OBJS) $(UTILS) $(addsufix /start.tgz,$(addprefix ../,$(TASKS))) $(addsufix /start,$(addprefix ../,$(TASKS)))

## Zale¿no¶æ
$(OBJS:.o=.cpp): $(OBJS:.o=.hpp)

$(OBJS): $(OBJS:.o=.cpp)
	$(foreach var, $@, g++ -c $(var:.o=.cpp) -o $(var) ${CPPFLAGS})

project: project.cpp $(OBJS)
	g++ $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

guitest: guitest.cpp $(OBJS)
	g++ $^ -o $@ $(CPPFLAGS) $(LDFLAGS)

$(TASKS): $(OBJS)
	git commit -a -m "makefile commit" || true
	$(foreach var,$@, git rev-parse HEAD > ../$(var)/.lib_sha; \
	cd ../$(var)/; \
	git commit -a -m "makefile commit" || true; \
	git rev-parse HEAD > .project_sha; \
	make; \
	rm -f start.tgz; \
	tar -czf start.tgz ./* --exclude-from ../tasklib/tgz_exclude; \
	export TASKLIB=task; \
	../tasklib/project register $(var))
