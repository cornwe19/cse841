all: vectors listing image dn

dn: dn.cpp settings.h
	g++ dn.cpp -Wall bin/* -o dn

listing: mkdirs listing.h
	g++ -c listing.cpp -g -Wall -o bin/listing.o

image: mkdirs image.h
	g++ -c image.cpp -g -Wall -o bin/image.o

vectors: mkdirs vectors.h
	g++ -c vectors.cpp -g -Wall -o bin/vectors.o

mkdirs:
	mkdir -p bin
	mkdir -p out

clean:
	rm -rf pcanet
	rm -rf bin/*
