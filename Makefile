CXX = g++
CXXFLAGS= -Wall -g -O3 #-O3 #-pg -g #-Wall #-O3
#CXXFLAGS=		-g -Wall -O2
LINKPATH= 
LINKFLAGS = -lz 
DEBUG=
OBJECTS =  #BaseReads.o Alignment.o 
HEADERS = *.hpp

#asan=1
ifneq ($(asan),)
	CXXFLAGS+=-fsanitize=address -g
	LDFLAGS+=-fsanitize=address -ldl -g
endif

all: dustmasker 

dustmasker: dustmasker.o $(OBJECTS)
	$(CXX) -o $@ $(LINKPATH) $(CXXFLAGS) $< $(OBJECTS) $(LINKFLAGS)

dustmasker.o: dustmasker.cpp $(HEADERS) 

clean:
	rm -f *.o *.gch dustmasker
