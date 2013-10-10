all: listing image pcanet

pcanet: pcanet.cpp settings.h
	g++ pcanet.cpp -Wall bin/* -o pcanet

listing: mkbin listing.cpp
	g++ -c listing.cpp -g -Wall -o bin/listing.o

image: mkbin image.cpp
	g++ -c image.cpp -g -Wall -o bin/image.o

mkbin:
	mkdir -p bin

clean:
	rm -rf pcanet
	rm -rf bin/*
