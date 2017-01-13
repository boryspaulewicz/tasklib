OBJS:= $(shell ls *.cpp)

all: $(OBJS:cpp=o)

COMMON= Common.hpp Utils.hpp Log.hpp

*.o: $(COMMON)

Task.o: Task.cpp *.hpp

Media.o: Timer.hpp PType.hpp

DataExchange.o: Database.hpp

Database.o: PType.hpp

Settings.o: Task.hpp

%.o: %.cpp %.hpp Utils.hpp
	g++ -o $@ -c $< $(CXXFLAGS)

