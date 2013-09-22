all: city findRoute

findRoute: findRoute.cpp city.cpp
	g++ findRoute.cpp bin/city.o -g -o findRoute

city: mkbin city.cpp
	g++ -c city.cpp -g -o bin/city.o

mkbin:
	mkdir -p bin

clean:
	rm -rf findRoute
	rm -rf findRoute.dSYM
	rm -rf bin/*
