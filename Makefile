CXXFLAGS = -Wall -g
LIB = fwk/BaseNotifiee.o fwk/Exception.o

OBJECTS = Instance.o Engine.o entities.o Log.o ActivityImpl.o

default:	test1

test:	test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

example: example.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

experiment: experiment.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)
	
clean:
	rm -f test1 test1.o test.o test.exe $(OBJECTS) *~

entities.o: entities.cpp entities.h Ptr.h WeakPtr.h PtrInterface.h Instance.h Nominal.h types.h
Engine.o: Engine.cpp Engine.h entityReactor.h entities.o
Instance.o: Instance.cpp Instance.h Engine.o entities.o
test.o: test1.cpp Instance.o Engine.o entities.o
test1.o: test1.cpp Instance.o Engine.o entities.o
example.o: example.cpp Instance.o Engine.o entities.o
experiment.o: experiment.cpp Instance.o Engine.o entities.o
