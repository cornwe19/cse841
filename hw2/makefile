all: listing image pcanet

pcanet: pcanet.cpp settings.h
	g++ pcanet.cpp -Wall bin/* -o pcanet

listing: mkdirs listing.h
	g++ -c listing.cpp -g -Wall -o bin/listing.o

image: mkdirs image.h
	g++ -c image.cpp -g -Wall -o bin/image.o

mkdirs:
	mkdir -p bin
	mkdir -p out

clean:
	rm -rf pcanet
	rm -rf bin/*
