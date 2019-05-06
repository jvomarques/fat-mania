#ifndef __CIRCULO_H_
#define __CIRCULO_H_

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>
#include <string>
#include <cmath>
#include "retangulo.h"

using namespace std;

class Circulo
{
	string nome;
	float posicao_x, posicao_y, velocidade, raio;
	ALLEGRO_COLOR cor;
	ALLEGRO_BITMAP *imagem;
	int direcao;
	
	public:

	//INICIA CIRCULO COM OU SEM IMAGEM
	Circulo(ALLEGRO_BITMAP *imagem = nullptr);

	//SETA NOME
	void Start (string nome);

	//CRIA DEFININDO DIVERSOS PARAMETROS
	void Create (float posx, float posy, float vel, float rai, int dir, ALLEGRO_COLOR cor);

	//ATUALIZA POSIÇÃO
	void Update (float posx, float posy);

	//SETS E GETS
	float getPosicao_x();
	float getPosicao_y();
	float getVelocidade();
	float getRaio();
	int getDirecao();
	string getNome();

	void setPosicao(float x, float y);
	void setVelocidade(float vel);
	void setRaio(float raio);
	void setCor(ALLEGRO_COLOR cor);
	void setDirecao(int dir);

	//MOVIMENTOS DO OBJETO
	void MoveLeft();
	void MoveRight();
	void MoveUp();
	void MoveDown();
	
	//VERIFICA COLISÃO COM CÍCULO
	bool colidiu(Circulo circ);

	//VERIFICA COLISAO COM RETANGULO - SOBRECARGA
	bool colidiu(Retangulo ret);

	//REDERIZA ESCALONADA COM OU SEM IMAGEM
	void Render();

	//REDERIZA ROTACIONADA - SOBRECARGA
	void Render(float pi);
	
};

#endif