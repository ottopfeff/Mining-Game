all:
	g++ -c World.h World.cpp 
	g++ -c Player.h Player.cpp 
	g++ -c ConsoleRenderer.h ConsoleRenderer.cpp World.h Player.h 
	g++ -c WindowRenderer.h WindowRenderer.cpp World.h Player.h
	g++ -c InputManager.h InputManager.cpp World.h Player.h 
	g++  -o Main.exe Main.cpp World.o Player.o Renderer.h ConsoleRenderer.o WindowRenderer.o InputManager.o -I ./src/include -L ./src/lib -lmingw32 -lSDL2main -lSDL2 