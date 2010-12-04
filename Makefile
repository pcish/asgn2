CXXFLAGS = -Wall -g
LIB = fwk/BaseNotifiee.o fwk/Exception.o

OBJECTS = Instance.o ShippingNetwork.o entities.o Log.o ActivityImpl.o

default:	test1

test:	test.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

test1:	test1.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

experiment: experiment.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

verification: verification.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIB)

clean:
	rm -f test1 *.o *.exe $(OBJECTS) *~

entities.o: entities.cpp entities.h Ptr.h WeakPtr.h PtrInterface.h Instance.h Nominal.h types.h
ShippingNetwork.o: ShippingNetwork.cpp ShippingNetwork.h entityReactor.h entities.o routing/*
Instance.o: Instance.cpp Instance.h ShippingNetwork.o entities.o
test.o: test1.cpp Instance.o ShippingNetwork.o entities.o
test1.o: test1.cpp Instance.o ShippingNetwork.o entities.o
experiment.o: experiment.cpp Instance.o ShippingNetwork.o entities.o
verification.o: verification.cpp Instance.o ShippingNetwork.o entities.o
