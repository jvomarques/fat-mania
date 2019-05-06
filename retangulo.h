#ifndef __RETANGULO_H_
#define __RETANGULO_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <string>

using namespace std;

class Retangulo
{
	string nome;
	float posicao_x1, posicao_y1,posicao_x2, posicao_y2;
	ALLEGRO_BITMAP *imagem;

	public:

	//Construtor - cria o retângulo e recebe ou não uma imagem por parâmetro
	Retangulo(ALLEGRO_BITMAP *imagem = nullptr);

	//Inicia o objeto adicionando um nome
	void Start (string nom);
	
	//Cria e define posições do retângulo
	void Create (float posx1, float posy1, float posx2, float posy2);

	//Retorna posições das pontas do retângulo
	float getPosicao_x1();
	float getPosicao_y1();
	float getPosicao_x2();
	float getPosicao_y2();

	//Renderiza o retângulo de acordo com suas propriedades, podendo ter ou não imagem
	void Render();

};

#endif