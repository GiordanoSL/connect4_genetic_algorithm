# Default target
all: connect4

connect4: main.o game.o graphics.o AG.o
	g++ -o connect4 main.o game.o graphics.o AG.o -march=native -O3 -lglut -lGLU -lGL -Wall -lm -Wall -std=c++17 

main.o: main.cpp
	g++ -Wall -std=c++17 -lglut -lGLU -lGL -Wall -lm -c main.cpp -march=native -O3

game.o: game.cpp
	g++ -Wall -std=c++17 -c game.cpp -march=native -O3

AG.o: AG.cpp
	g++ -Wall -std=c++17 -c AG.cpp -march=native -O3

graphics.o: graphics.cpp
	g++ -Wall -std=c++17 -lglut -lGLU -lGL -Wall -lm -c graphics.cpp -march=native -O3

clean:
	rm *.0 connect4

run:
	./connect4
