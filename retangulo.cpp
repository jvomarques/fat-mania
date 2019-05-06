#include "retangulo.h"

Retangulo::Retangulo(ALLEGRO_BITMAP *imagem){
	this->imagem = imagem;
};

void Retangulo::Start (string nom){
		this->nome = nome;
};

void Retangulo::Create (float posx1, float posy1, float posx2, float posy2)
{
		this->posicao_x1 = posx1;
		this->posicao_y1 = posy1;
		this->posicao_x2 = posx2;
		this->posicao_y2 = posy2;
};

float Retangulo::getPosicao_x1(){
		return this->posicao_x1;
}
float Retangulo::getPosicao_y1(){
	return this->posicao_y1;
}

float Retangulo::getPosicao_x2(){
		return this->posicao_x2;
}
float Retangulo::getPosicao_y2(){
	return this->posicao_y2;
}

void Retangulo::Render(){
		if (this->imagem == nullptr)
		{
			al_draw_filled_rectangle(this->getPosicao_x1(), this->getPosicao_y1(), this->getPosicao_x2(), this->getPosicao_y2(), al_map_rgb(255, 255, 255));
		}
		else
		{
			al_draw_scaled_bitmap(this->imagem, 0, 0, al_get_bitmap_width(this->imagem), al_get_bitmap_height(this->imagem), this->getPosicao_x1(), this->getPosicao_y1(), this->getPosicao_x2() - this->getPosicao_x1(), this->getPosicao_y2() - this->getPosicao_y1(), 0);

		}
};