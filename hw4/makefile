dntext: dntext.cpp settings.h x_area y_area z_area pgm vocabulary vectors 
	g++ dntext.cpp -Wall bin/* -o dntext

x_area: mkdirs x_area.h x_area.cpp
	g++ -c x_area.cpp -Wall -o bin/x_area.o

y_area: mkdirs y_area.h y_area.cpp
	g++ -c y_area.cpp -Wall -o bin/y_area.o

z_area: mkdirs z_area.h z_area.cpp
	g++ -c z_area.cpp -Wall -o bin/z_area.o

vectors: mkdirs vectors.h vectors.cpp
	g++ -c vectors.cpp -Wall -o bin/vectors.o

pgm: mkdirs pgm.h pgm.cpp
	g++ -c pgm.cpp -Wall -o bin/pgm.o

vocabulary: mkdirs vocabulary.h vocabulary.cpp
	g++ -c vocabulary.cpp -Wall -o bin/vocabulary.o

mkdirs:
	mkdir -p bin

clean:
	rm -rf dntext 
	rm -rf bin/*
