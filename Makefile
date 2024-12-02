# Default target
all: connect4

connect4: main.o game.o graphics.o
	g++ -o connect4 main.o game.o graphics.o -march=native -O3 -lglut -lGLU -lGL -Wall -lm -Wall -std=c++17 

main.o: main.cpp
	g++ -Wall -std=c++17 -lglut -lGLU -lGL -Wall -lm -c main.cpp

game.o: game.cpp
	g++ -Wall -std=c++17 -c game.cpp

graphics.o: graphics.cpp
	g++ -Wall -std=c++17 -lglut -lGLU -lGL -Wall -lm -c graphics.cpp

clean:
	rm *.0 connect4

run:
	./connect4
