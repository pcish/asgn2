CXXFLAGS = -Wall -g
LIB = fwk/BaseNotifiee.o fwk/Exception.o

OBJECTS = Instance.o Engine.o

default:	test1

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
Engine.o: Engine.h Engine.cpp types.h error.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
