OBJS:= $(shell ls *.cpp)

all: $(OBJS:cpp=o)

COMMON= Common.hpp Utils.hpp Log.hpp

*.o: $(COMMON)

Task.o: Task.cpp *.hpp

Media.o: Timer.hpp Ptype.hpp

Datasaver.o: Database.hpp

Database.o: Ptype.hpp

Settings.o: Task.o

%.o: %.cpp %.hpp Utils.hpp
	g++ -o $@ -c $< $(CXXFLAGS)

