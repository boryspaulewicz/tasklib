OBJS:= $(shell ls *.cpp)

all: $(OBJS:cpp=o)

Task.o: Task.cpp *.hpp

%.o: %.cpp %.hpp
	g++ -o $@ -c $< $(CXXFLAGS)

