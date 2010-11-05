CXXFLAGS = -Wall -g
LIB = fwk/BaseNotifiee.o fwk/Exception.o

OBJECTS = Instance.o Engine.o entities.o

default:	test

test:	test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f test1 test1.o $(OBJECTS) *~

Instance.o: Instance.cpp Instance.h PtrInterface.h Ptr.h Engine.h
EngineManager.o: EngineManager.h Engine.h
entities.o: entities.cpp entities.h
Engine.o: Engine.h Engine.cpp types.h error.h
test.o: test1.cpp Instance.h PtrInterface.h Ptr.h
test1.o: test1.cpp Instance.h PtrInterface.h Ptr.h
