.PHONY: clean
all: Reactor.o EventHandle.o EventDemultiplex.o main.o
	g++ -o reactorepoll Reactor.o EventHandle.o EventDemultiplex.o main.o
main.o:
	g++ -c main.cpp -o main.o
EventHandle.o: EventHandle.cpp EventHandle.h
	g++ -c EventHandle.cpp -o EventHandle.o
EventDemultiplex.o: EventDemultiplex.cpp EventDemultiplex.h
	g++ -c EventDemultiplex.cpp -o EventDemultiplex.o
Reactor.o: Reactor.cpp Reactor.h
	g++ -c Reactor.cpp -o Reactor.o
clean:
	rm Reactor.o EventHandle.o EventDemultiplex.o main.o
