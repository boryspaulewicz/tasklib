OBJS:= $(shell ls *.cpp)

all: $(OBJS:cpp=o)

Task.o: Task.cpp *.hpp

Media.o: States.hpp Timer.hpp

Datasaver.o: Database.hpp

Database.o: Ptype.hpp

%.o: %.cpp %.hpp Utils.hpp
	g++ -o $@ -c $< $(CXXFLAGS)

