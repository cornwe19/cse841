all: vectors y_area z_area listing image dn

dn: dn.cpp settings.h listing image y_area vectors
	g++ dn.cpp -Wall bin/* -o dn

listing: mkdirs listing.h listing.cpp
	g++ -c listing.cpp -g -Wall -o bin/listing.o

image: mkdirs image.h image.cpp
	g++ -c image.cpp -g -Wall -o bin/image.o

y_area: mkdirs y_area.h y_area.cpp
	g++ -c y_area.cpp -g -Wall -o bin/y_area.o

z_area: mkdirs z_area.h z_area.cpp
	g++ -c z_area.cpp -g -Wall -o bin/z_area.o

vectors: mkdirs vectors.h vectors.cpp
	g++ -c vectors.cpp -g -Wall -o bin/vectors.o

mkdirs:
	mkdir -p bin
	mkdir -p out

clean:
	rm -rf pcanet
	rm -rf bin/*
