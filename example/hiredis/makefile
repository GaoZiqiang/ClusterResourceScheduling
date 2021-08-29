all:main

main:main.cpp headers/* sources/*
	g++ main.cpp headers/* sources/* -o main -L/usr/local/lib/ -lhiredis

clean:
	rm -rf main
