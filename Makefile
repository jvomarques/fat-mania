circulo.o: circulo.h circulo.cpp
	g++ --std=c++0x circulo.cpp -c

retangulo.o: retangulo.h retangulo.cpp
	g++ retangulo.cpp -c --std=c++0x

list.o: list.h list.cpp
	g++ list.cpp -c -std=c++0x

fatmania.o: fatmania.cpp
	g++ fatmania.cpp -c -std=c++0x

fatmania: circulo.o retangulo.o list.o fatmania.o 
	g++  -std=c++0x fatmania.cpp circulo.o retangulo.o list.o -o fatmania -lallegro -lallegro_dialog -lallegro_font -lallegro_audio -lallegro_acodec -lallegro_ttf -lallegro_primitives -lm -lallegro_main -lallegro_image

limpar: circulo.o retangulo.o list.o fatmania.o fatmania
	rm circulo.o retangulo.o list.o fatmania.o fatmania