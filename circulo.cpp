#include "circulo.h"

Circulo::Circulo(ALLEGRO_BITMAP *imagem){
	this->imagem = imagem;
};

void Circulo::Start (string nome){
		this->nome = nome;
};

void Circulo::Create (float posx, float posy, float vel, float rai, int dir, ALLEGRO_COLOR cor){
		this->posicao_x = posx;
		this->posicao_y = posy;
		this->velocidade = vel;
		this->raio = rai;
		this->cor = cor;
		this->direcao;
};

void Circulo::Update (float posx, float posy){
		this->posicao_x = posx;
		this->posicao_y = posy;
};

float Circulo::getPosicao_x(){
		return this->posicao_x;
}

float Circulo::getPosicao_y(){
	return this->posicao_y;
}

float Circulo::getVelocidade(){
	return this->velocidade;
}

float Circulo::getRaio(){
	return this->raio;
}

int Circulo::getDirecao(){
	return this->direcao;
}

string Circulo::getNome(){
	return this->nome;
}

void Circulo::setPosicao(float x, float y){
	this->posicao_x = x;
	this->posicao_y  = y;
}

void  Circulo::setVelocidade(float vel){
	this->velocidade = vel;
}

void  Circulo::setCor(ALLEGRO_COLOR cor){
	this->cor = cor;
}

void  Circulo::setRaio(float raio){
	this->raio = raio;
}

void Circulo::setDirecao(int dir){
	this->direcao = dir;
}

void Circulo::MoveLeft(){
	this->posicao_x -= this->velocidade;
}

void Circulo::MoveRight(){
	this->posicao_x += this->velocidade;
}

void Circulo::MoveUp(){
	this->posicao_y -= this->velocidade;
}

void Circulo::MoveDown(){
	this->posicao_y += this->velocidade;
}

bool Circulo::colidiu(Circulo circ)
{
		if (sqrt(pow(circ.getPosicao_x() - this->getPosicao_x(), 2) + pow(circ.getPosicao_y() - this->getPosicao_y(), 2)) <  this->getRaio() + circ.getRaio())
			return true;
		else
			return false;
}

bool Circulo::colidiu(Retangulo ret)
{
		for (int j = ret.getPosicao_x1(); j <= ret.getPosicao_x2(); j++)
		{
			if (ceil(sqrt(pow(j - this->getPosicao_x(), 2) + pow(ret.getPosicao_y1() - this->getPosicao_y(), 2))) <  this->getRaio())
				return true;
		}
		
		for (int j = ret.getPosicao_x1(); j <= ret.getPosicao_x2(); j++)
		{
			if (ceil(sqrt(pow(j - this->getPosicao_x(), 2) + pow(ret.getPosicao_y2() - this->getPosicao_y(), 2))) <  this->getRaio())
				return true;
		}
		
		for (int j = ret.getPosicao_y1(); j <= ret.getPosicao_y2(); j++)
		{
			if (ceil(sqrt(pow(ret.getPosicao_x1() - this->getPosicao_x(), 2) + pow(j - this->getPosicao_y(), 2))) <  this->getRaio())
				return true;
		}
		
		for (int j = ret.getPosicao_y1(); j <= ret.getPosicao_y2(); j++)
		{
			if (ceil(sqrt(pow(ret.getPosicao_x2() - this->getPosicao_x(), 2) + pow(j - this->getPosicao_y(), 2))) <  this->getRaio())
				return true;
		}

		
		return false;
}

void Circulo::Render(){

		if (this->imagem == nullptr)
			al_draw_filled_circle(this->getPosicao_x(), this->getPosicao_y(), this->getRaio(), this->cor);
		else
	 		al_draw_scaled_bitmap(this->imagem, 0, 0, al_get_bitmap_width(this->imagem), al_get_bitmap_height(this->imagem), this->getPosicao_x() - this->getRaio(), this->getPosicao_y() - this->getRaio(), this->getRaio()*2, this->getRaio()*2, 0);

};

void Circulo::Render(float angulo)
{
		if (this->imagem == nullptr)
			al_draw_filled_circle(this->getPosicao_x(), this->getPosicao_y(), this->getRaio(), this->cor);
		else 
			al_draw_scaled_rotated_bitmap(this->imagem, al_get_bitmap_width(this->imagem)/2, al_get_bitmap_height(this->imagem)/2, this->getPosicao_x(), this->getPosicao_y(), this->getRaio()*2 / al_get_bitmap_width(this->imagem) , this->getRaio()*2 / al_get_bitmap_height(this->imagem), angulo, 0);	
};