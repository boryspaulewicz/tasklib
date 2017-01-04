OBJS:= $(shell ls *.cpp)

all: $(OBJS:cpp=o)

Task.o: Task.cpp *.hpp

Media.o: Timer.o

%.o: %.cpp %.hpp
	g++ -o $@ -c $< $(CXXFLAGS)

