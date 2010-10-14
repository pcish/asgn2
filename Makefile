CXXFLAGS = -Wall -g

OBJECTS = Instance.o

default:	test1

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
