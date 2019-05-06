//BIBLIOTECA ALLEGRO 5.0.11
#include <allegro5/allegro.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

//BIBLIOTECAS PADRÃO UTILIZADAS
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <cmath>

//CLASSES IMPLEMENTADAS
#include "circulo.h"
#include "retangulo.h"
#include "list.h"

using namespace std;

//TAMANHO DA TELA E FPS
#define WIDTH 1220
#define HEIGHT 700
#define FPS 60

//CONSTATE PI
#define PI 3.141592653589793238462643383279502884L

//DEFINE TECLA PRESSIONADA PARA DIREÇÃO DOS PERSONAGENS E USO DE ITENS
enum Dir{ UP, DOWN, LEFT, RIGHT	};
enum Eventos { F1, F2, F3, F4, ESPACO, SEM_EVENTO };

int main (int argc, char const* argv[])
{
	//=========================================INICIALIZA ALLEGRO============================================
	ALLEGRO_DISPLAY *display;

	if(!al_init())
		al_show_native_message_box(NULL, NULL, NULL, "Não foi possível iniciar o Allegro!", NULL, 0);

	display = al_create_display(WIDTH, HEIGHT);

	if(!display)
		al_show_native_message_box(display, NULL, NULL, "Não foi possível iniciar o Display!", NULL, 0);
	
	al_set_window_position(display, 70, 0);
	al_set_window_title(display, "Sumô Mania");

	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_install_keyboard();
	al_install_audio();
	al_init_acodec_addon();
	al_reserve_samples(1);


	//FONTE	
	ALLEGRO_FONT *f_western = al_load_font("fonts/Simpsonfont.ttf", 20, 0);

	//SONS
	ALLEGRO_SAMPLE *som_item = NULL;
	ALLEGRO_SAMPLE *som_colidiu = NULL;
	ALLEGRO_SAMPLE *som_gelo = NULL;
	ALLEGRO_SAMPLE *som_morte = NULL;
	ALLEGRO_SAMPLE *som_perdeu = NULL;
	ALLEGRO_SAMPLE *som_ganhou = NULL;

    som_item = al_load_sample("sounds/som_item.ogg");
    som_colidiu = al_load_sample("sounds/som_colidiu.wav");
 	som_gelo = al_load_sample("sounds/som_gelo.wav");
    som_morte = al_load_sample("sounds/som_morte.wav");
    som_perdeu = al_load_sample("sounds/som_perdeu.wav");
    som_ganhou = al_load_sample("sounds/som_ganhou.wav");

	//========================================FIM INICIALIZA ALLELGRO=========================================

	char nome_fase[14];
	bool ganhou;
	for (int tt = 0; tt < 2; ++tt)
	{	
		if (tt == 0)
		{
			nome_fase[0] = 'f';
			nome_fase[1] = 'a';
			nome_fase[2] = 's';
			nome_fase[3] = 'e';
			nome_fase[4] = '1';
			nome_fase[5] = '.';
			nome_fase[6] = 'f';
			nome_fase[7] = 'a';
			nome_fase[8] = 't';
			nome_fase[9] = 'm';
			nome_fase[10] = 'a';
			nome_fase[11] = 'n';
			nome_fase[12] = 'i';
			nome_fase[13] = 'a';
			nome_fase[14] = '\0';
		}
		else if (tt == 1 && ganhou)
			nome_fase[4] = '2';
		else
			break;

	//========================================EVENTOS E TIMERS===============================================
	ALLEGRO_EVENT_QUEUE *events_queue = al_create_event_queue();
	al_register_event_source(events_queue, al_get_keyboard_event_source());
	
	ALLEGRO_TIMER *timer = al_create_timer(1.0/FPS);
	al_register_event_source(events_queue, al_get_timer_event_source(timer));
	al_start_timer(timer);

	ALLEGRO_TIMER *contador = al_create_timer(1.0);
	al_register_event_source(events_queue, al_get_timer_event_source(contador));

	ALLEGRO_TIMER *item_timer = al_create_timer(5.0);
	al_register_event_source(events_queue, al_get_timer_event_source(item_timer));
	al_start_timer(item_timer);

	ALLEGRO_TIMER *rot_timer = al_create_timer(1.0/FPS);
	al_register_event_source(events_queue, al_get_timer_event_source(rot_timer));
	//=========================================FIM EVENTOS E TIMERS===========================================



	//=========================================CARREGAR IMAGENS===============================================
	ALLEGRO_BITMAP *imagem;
	imagem = al_load_bitmap("images/a.png");

	ALLEGRO_BITMAP *img_tiro;
	img_tiro = al_load_bitmap("images/tiro.png");

	ALLEGRO_BITMAP *img_vida;
	img_vida = al_load_bitmap("images/vida.png");

	ALLEGRO_BITMAP *img_vida_black;
	img_vida_black = al_load_bitmap("images/vida_black.png");

	ALLEGRO_BITMAP *img_lanchonete;
	img_lanchonete = al_load_bitmap("images/lanchonete.png");

	ALLEGRO_BITMAP *img_rosca;
	img_rosca = al_load_bitmap("images/rosca.png");

	ALLEGRO_BITMAP *img_gelo;
	img_gelo = al_load_bitmap("images/gelo.png");

	ALLEGRO_BITMAP *img_gelo_black;
	img_gelo_black = al_load_bitmap("images/gelo_black.png");

	ALLEGRO_BITMAP *img_isaac;
	img_isaac = al_load_bitmap("images/isaac.png");
	//==========================================FIM CARREGA IMAGENS======================================






		//=======================================DECLARAÇÃO DE VARIÁVEIS=====================================
		bool draw;
		bool sai = false;

		int seg = 0;

		float mile = 0.0;
		float tempo = 0.0;

		Dir dir_inimigo = DOWN;
		Dir d_tiro = RIGHT;
		Eventos item_usado = SEM_EVENTO;

		List<Circulo> l_inimigos;//TODOS OS INIMIGOS
		List<Retangulo> l_obstaculos;//TODOS OS OBSTACULOS
		List<Circulo> a; //ITENS INTENS NA TELA
		List<Circulo> mochila;//ITENS RECOLHIDOS NA MOCHILA
		List<Circulo> poderes;//PODERES NA TELA
		List<Circulo> poderes_coletados; //MOCHILA DE PODERES
		List<Circulo> tiros; //TIROS ATIVOS
		List<Circulo> vidas; //VIDAS ATIVAS
		List<Circulo> gelos; //GELOS ATIVOS
		List<Circulo> roscas; //ROSCAS ATIVAS
		//=======================================FIM DECLARAÇÃO DE VARIÁVEIS=================================
		


		//========================================DEFINE ELEMENTOS DO JOGOS==================================

		//HEROI
		Circulo *heroi = new Circulo(imagem);
		heroi->Start("Personagem");
		heroi->Create(180, 25, 5, 25, DOWN ,al_map_rgb(0,0,0));
		heroi->setDirecao(DOWN);
		
		//CARREGAR FASES - DEFINE INIMIGOS E OBSTACULOS
		Circulo *inimigo = new Circulo(img_isaac);
		inimigo->Start("Inimigo");

		Retangulo *obst = new Retangulo();
		obst->Start("Obstaculo");
		
		FILE * arquivo;
		arquivo = fopen(nome_fase,"r");

		int n_inimigos;
		int n_obstaculos;
		int x, y, v, r;
		int v_gelo;

		if (arquivo == NULL)
			cout << "Arquivo Inválido. Pressione qualquer tecla...";
		else
		{
			fscanf(arquivo, "%i\n", &n_inimigos);
			for (int i = 0;i < n_inimigos; i++)
			{
				fscanf(arquivo, "%i ", &x);
				fscanf(arquivo, "%i ", &y);
				fscanf(arquivo, "%i ", &v);
				fscanf(arquivo, "%i\n", &r);

				inimigo->Create(x, y, v, r, DOWN, al_map_rgb(255,0,0));
				l_inimigos.addLast(*inimigo);
			}
			fscanf(arquivo, "%i\n", &v_gelo);

			fscanf(arquivo, "%i\n", &n_obstaculos);
			for (int i = 0; i < n_obstaculos; ++i)
			{
				int x, y, v, r;

				fscanf(arquivo, "%i ", &x);
				fscanf(arquivo, "%i ", &y);
				fscanf(arquivo, "%i ", &v);
				fscanf(arquivo, "%i\n", &r);

				obst->Create(x, y, v, r);
				l_obstaculos.addLast(*obst);
			}	
		} 

		//BARRA DE STATUS
		obst->Create(0, 0, 135, 700);
		l_obstaculos.add(*obst);

		//PAREDE BAIXO
		obst->Create(0, HEIGHT, WIDTH, HEIGHT);
		l_obstaculos.add(*obst);

		//PAREDE CIMA
		obst->Create(0, 0, WIDTH, 0);
		l_obstaculos.add(*obst);
		
		//PAREDE DIREITA
		obst->Create(WIDTH, 0, WIDTH, HEIGHT); 
		l_obstaculos.add(*obst);

		delete obst;
		delete inimigo;

		//TIROS
		Circulo *i_tiro = new Circulo(img_tiro);	
		i_tiro->Start("Tiro");

		i_tiro->Create(800, 20, 7, 15, DOWN, al_map_rgb(126, 41, 225));
		a.addLast(*i_tiro);

		i_tiro->Create(550, 300, 7, 15, DOWN, al_map_rgb(200, 100, 22));
		a.addLast(*i_tiro);

		delete i_tiro;

		//VIDAS
		Circulo *i_vida = new Circulo(img_vida);	
		i_vida->Start("Vida");
		
		i_vida->Create(1000, 100, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		a.addLast(*i_vida);

		i_vida->Create(800, 400, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		a.addLast(*i_vida);

		delete i_vida;

		//ROSCAS
		Circulo *p_rosca = new Circulo(img_rosca);	
		p_rosca->Start("Rosca");

		p_rosca->Create(1000, 400, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		poderes.addLast(*p_rosca);

		p_rosca->Create(800, 200, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		poderes.addLast(*p_rosca);

		delete p_rosca;

		//GELOS
		Circulo *p_gelo = new Circulo(img_gelo);	
		p_gelo->Start("Gelo");
		
		p_gelo->Create(1000, 600, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		poderes.addLast(*p_gelo);
		
		p_gelo->Create(800, 600, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		poderes.addLast(*p_gelo);
		
		p_gelo->Create(700, 500, 7, 15, DOWN, al_map_rgb(172,	11,	44));
		poderes.addLast(*p_gelo);

		delete p_gelo;

		//CASTELO - CHEGADA
		Retangulo *castelo = new Retangulo(img_lanchonete);
		castelo->Start("Castelo");
		castelo->Create(1050, 550, 1220, 700);
		

		//============================================GAME LOOP========================================
		while (sai == false)
		{
			//ESPERA EVENTOS
			ALLEGRO_EVENT events;
			al_wait_for_event(events_queue, &events);

			//DEFINE TECLA SELECIONADA
			if (events.type == ALLEGRO_EVENT_KEY_DOWN)
			{
				if (events.keyboard.keycode == ALLEGRO_KEY_ESCAPE)
				{
					ganhou = false;
					break;
				}
				if (events.keyboard.keycode == ALLEGRO_KEY_DOWN)
					heroi->setDirecao(DOWN);
				if (events.keyboard.keycode == ALLEGRO_KEY_UP)
					heroi->setDirecao(UP);
				if (events.keyboard.keycode == ALLEGRO_KEY_LEFT)
					heroi->setDirecao(LEFT);
				if (events.keyboard.keycode == ALLEGRO_KEY_RIGHT)
					heroi->setDirecao(RIGHT);
				if(events.keyboard.keycode == ALLEGRO_KEY_F1)
					item_usado = F1;
				if(events.keyboard.keycode == ALLEGRO_KEY_F2)
					item_usado = F2;
				if(events.keyboard.keycode == ALLEGRO_KEY_F3)
					item_usado = F3;
				if(events.keyboard.keycode == ALLEGRO_KEY_F4)
					item_usado = F4;
				if(events.keyboard.keycode == ALLEGRO_KEY_SPACE)
					item_usado = ESPACO;
			}
			
			//==========================================INICIO EVENTOS TIMER===========================================
			if (events.type == ALLEGRO_EVENT_TIMER)
			{
				//TIMER PARA MECANICA
				if (events.timer.source == timer)
				{
					//==========================================COLISÃO INIMIGO============================================
					for (int i = 0; i < l_inimigos.getSize(); ++i)
					{
						if(l_inimigos.get(i).colidiu(*heroi) == true && vidas.isEmpty() != true)
						{
							vidas.removeFirst();

							al_play_sample(som_morte, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

							heroi->setPosicao(180, 25);
						}
						else if(l_inimigos.get(i).colidiu(*heroi) == true)
						{
							al_play_sample(som_perdeu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
								al_show_native_message_box(display, "Sumô Mania", "Resultado:","MORREU!!!", NULL, 0);
								

								sai = true;
								ganhou = false;
						}
					}
					//==========================================FIM COLISAO INIMIGO=======================================


					//==========================================MOVIMENTOS INIMIGO=========================================
					for (int i = 0; i < l_inimigos.getSize(); ++i)
					{
						Circulo k = l_inimigos.get(i);				

						if (abs(heroi->getPosicao_x() - l_inimigos.get(i).getPosicao_x()) > heroi->getRaio() + l_inimigos.get(i).getRaio()) 
						{	
							if(heroi->getPosicao_x() < l_inimigos.get(i).getPosicao_x())
							{	
								k.MoveLeft();
								k.setDirecao(LEFT);
							}
							else
							{
								k.MoveRight();
								k.setDirecao(RIGHT);
							}
						}
						else
						{
							if(heroi->getPosicao_y() < l_inimigos.get(i).getPosicao_y())
							{
								k.MoveUp();
								k.setDirecao(UP);
							}
							else
							{
								k.MoveDown();
								k.setDirecao(DOWN);
							}
						}

						if (i == 0)
							l_inimigos.removeFirst();
						else if (i == l_inimigos.getSize()-1)
							l_inimigos.removeLast();
						else
							l_inimigos.remove(i);

						l_inimigos.add(k);
						
					}
					for (int i = 0; i < l_inimigos.getSize(); ++i)
					{
						for (int j = 0; j < l_obstaculos.getSize(); ++j)
						{
							Circulo k = l_inimigos.get(i);

							if(l_inimigos.get(i).colidiu(l_obstaculos.get(j)) == true)
							{
								if(l_inimigos.get(i).getDirecao() == UP)
								{ 
									k.MoveDown();
									if (!l_inimigos.get(i).colidiu(l_obstaculos.get(j)))
										k.setDirecao(DOWN);
								}
								else if(l_inimigos.get(i).getDirecao() == DOWN)
								{
									k.MoveUp();
									if (!l_inimigos.get(i).colidiu(l_obstaculos.get(j)))
										k.setDirecao(UP);
								}			
								else if(l_inimigos.get(i).getDirecao() == LEFT)
								{
									k.MoveRight();
									if (!l_inimigos.get(i).colidiu(l_obstaculos.get(j)))
										k.setDirecao(RIGHT);
								}
								else if(l_inimigos.get(i).getDirecao() == RIGHT)
								{
									k.MoveLeft();
									if (!l_inimigos.get(i).colidiu(l_obstaculos.get(j)))
										k.setDirecao(LEFT);
								}

								if (i == 0)
									l_inimigos.removeFirst();
								else if (i == l_inimigos.getSize()-1)
									l_inimigos.removeLast();
								else
									l_inimigos.remove(i);

								l_inimigos.add(k);
							}
						}

						for (int j = 0; j < l_inimigos.getSize(); ++j)
						{
							Circulo k = l_inimigos.get(i);

							if(l_inimigos.get(i).colidiu(l_inimigos.get(j)) && i != j)
							{
								if(l_inimigos.get(i).getDirecao() == UP)
								{ 
									k.MoveDown();
									if (!l_inimigos.get(i).colidiu(l_inimigos.get(j)))
										k.setDirecao(DOWN);
								}
								else if(l_inimigos.get(i).getDirecao() == DOWN)
								{
									k.MoveUp();
									if (!l_inimigos.get(i).colidiu(l_inimigos.get(j)))
										k.setDirecao(UP);
								}			
								else if(l_inimigos.get(i).getDirecao() == LEFT)
								{
									k.MoveRight();
									if (!l_inimigos.get(i).colidiu(l_inimigos.get(j)))
										k.setDirecao(RIGHT);
								}
								else if(l_inimigos.get(i).getDirecao() == RIGHT)
								{
									k.MoveLeft();
									if (!l_inimigos.get(i).colidiu(l_inimigos.get(j)))
										k.setDirecao(LEFT);
								}

								if (i == 0)
									l_inimigos.removeFirst();
								else if (i == l_inimigos.getSize()-1)
									l_inimigos.removeLast();
								else
									l_inimigos.remove(i);

								l_inimigos.add(k);
							}
						}
					}
					//==========================================FIM MOVIMENTOS INIMIGO=========================================


					//==========================================COLETAR ITEM===================================================
					for (int i = 0; i < a.getSize(); i++)
					{
							if((heroi->colidiu(a.get(i)) == true))
							{
								mochila.addLast(a.get(i));
								if((i) == 0)
									a.removeFirst();
								else if((i) == a.getSize() - 1)
									a.removeLast();
								else
									a.remove(i);

								al_play_sample(som_item, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
					}
					//==========================================FIM COLETAR ITEM================================================


					//==========================================COLETAR PODER===================================================
					for (int i = 0; i < poderes.getSize(); ++i)
					{
						if((heroi->colidiu(poderes.get(i)) == true))
						{
							poderes_coletados.addLast(poderes.get(i));
							if((i) == 0)
								poderes.removeFirst();
							else if((i) == poderes.getSize() - 1)
								poderes.removeLast();
							else
								poderes.remove(i);

							al_play_sample(som_item, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
						}

					}
					//========================================FIM COLETAR PODER===================================================
					

					//===========================================MOVIMENTO TIROS====================================================
					for (int i = 0; i < tiros.getSize(); ++i)
					{
						Circulo k = tiros.get(i);
						
						if(k.getDirecao() == DOWN)
							k.MoveDown();
						if(k.getDirecao() == UP)
							k.MoveUp();
						if(k.getDirecao() == LEFT)
							k.MoveLeft();
						if(k.getDirecao() == RIGHT)
							k.MoveRight();
			
						if (i == 0)
							tiros.removeFirst();
						else if (i == tiros.getSize()-1)
							tiros.removeLast();
						else
							tiros.remove(i);

						tiros.add(k);
					}
					//=======================================FIM MOVIMENTO TIROS====================================================


					//===========================================MOVIMENTO ROSCAS====================================================
					for (int i = 0; i < roscas.getSize(); ++i)
					{
						Circulo k;
						k = roscas.get(i);

						if(k.getDirecao() == DOWN)
							k.MoveDown();
						if(k.getDirecao() == UP)
							k.MoveUp();
						if(k.getDirecao() == LEFT)
							k.MoveLeft();
						if(k.getDirecao() == RIGHT)
							k.MoveRight();
						
						if (i == 0)
							roscas.removeFirst();
						else if (i == roscas.getSize()-1)
							roscas.removeLast();
						else
							roscas.remove(i);

						roscas.add(k);
					}
					//=======================================FIM MOVIMENTO ROSCAS=======================================


					//========================================COLISAO DO TIRO======================================================
					for (int i = 0; i < tiros.getSize(); ++i)
					{
						for (int j = 0; j < l_obstaculos.getSize(); ++j)
						{
							if(tiros.get(i).colidiu(l_obstaculos.get(j)) == true)
							{
								if((i) == 0)
									tiros.removeFirst();
								else if((i) == tiros.getSize() - 1)
									tiros.removeLast();
								else
									tiros.remove(i);
							}
						}
						for (int j = 0; j < l_inimigos.getSize(); ++j)
						{
							if(tiros.get(i).colidiu(l_inimigos.get(j)) == true)
							{
								if((j) == 0)
									l_inimigos.removeFirst();
								else if((j) == l_inimigos.getSize() - 1)
									l_inimigos.removeLast();
								else
									l_inimigos.remove(j);

								if((i) == 0)
									tiros.removeFirst();
								else if((i) == tiros.getSize() - 1)
									tiros.removeLast();
								else
									tiros.remove(i);

								al_play_sample(som_colidiu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
						}
					}
					//========================================FIM COLISÃO DO TIRO==================================================


					//========================================COLISAO DAS ROSCAS======================================================
					for (int i = 0; i < roscas.getSize(); ++i)
					{
						for (int j = 0; j < l_obstaculos.getSize(); ++j)
						{
							if(roscas.get(i).colidiu(l_obstaculos.get(j)) == true)
							{
								if((i) == 0)
									roscas.removeFirst();
								else if((i) == roscas.getSize() - 1)
									roscas.removeLast();
								else
									roscas.remove(i);
							}
						}
						for (int j = 0; j < l_inimigos.getSize(); ++j)
						{
							if(roscas.get(i).colidiu(l_inimigos.get(j)) == true)
							{
								if((j) == 0)
									l_inimigos.removeFirst();
								else if((j) == l_inimigos.getSize() - 1)
									l_inimigos.removeLast();
								else
									l_inimigos.remove(j);

								if((i) == 0)
									roscas.removeFirst();
								else if((i) == roscas.getSize() - 1)
									roscas.removeLast();
								else
									roscas.remove(i);

								al_play_sample(som_colidiu, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
							}
						}
					}
					//========================================FIM COLISÃO DAS ROSCAS==================================================
					
					
					//========================================MOVIMENTO HEROI======================================================

					//DIRECAO
					if (heroi->getDirecao() == DOWN)
					{
						heroi->MoveDown();
					}
					if (heroi->getDirecao() ==  UP)
					{
						heroi->MoveUp();
					}
					if (heroi->getDirecao() == LEFT)
					{
						heroi->MoveLeft();
					}
					if (heroi->getDirecao() == RIGHT)
					{
						heroi->MoveRight();
					}

					//TRATMENTO DE COLISOES
					if(heroi->colidiu(*castelo) == true && l_inimigos.isEmpty())
					{				
						al_play_sample(som_ganhou, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);	
						al_show_native_message_box(display, "O som_item", "Resultado:","GANHOU!!!", NULL, 0);
						

						sai = ganhou = true;	
					}

					for (int i = 0; i < l_obstaculos.getSize(); ++i)
					{
						if(heroi->colidiu(l_obstaculos.get(i)) || heroi->colidiu(*castelo))
						{
							if(heroi->getDirecao() == UP)
							{ 
								heroi->MoveDown();
								if (!heroi->colidiu(l_obstaculos.get(i)))
									heroi->setDirecao(DOWN);
							}
							else if(heroi->getDirecao() == DOWN)
							{
								heroi->MoveUp();
								if (!heroi->colidiu(l_obstaculos.get(i)))
									heroi->setDirecao(UP);
							}			
							else if(heroi->getDirecao() == LEFT)
							{
								heroi->MoveRight();
								if (!heroi->colidiu(l_obstaculos.get(i)))
									heroi->setDirecao(RIGHT);
							}
							else if(heroi->getDirecao() == RIGHT)
							{
								heroi->MoveLeft();
								if (!heroi->colidiu(l_obstaculos.get(i)))
									heroi->setDirecao(LEFT);
							}
						}
					}
					//=================================================FIM MOVIMENTO HEROI=====================================

					//=============================================FUNCIONAMENTO DOS GELOS=====================================
					if (gelos.isEmpty())
					{
						al_stop_timer(contador);
						seg = 0;

						for (int i = 0; i < l_inimigos.getSize(); ++i)
						{	
							Circulo k = l_inimigos.get(i);
							k.setVelocidade(v);

							if (i == 0)
								l_inimigos.removeFirst();
							else if (i == l_inimigos.getSize()-1)
								l_inimigos.removeLast();
							else
								l_inimigos.remove(i);

							l_inimigos.add(k);			
						}
					}
					else if (seg % 5 == 0 && seg > 0)
					{	
						seg++;
						gelos.removeLast();
					}
					//=============================================FIM FUNCIONAMENTO DOS GELOS==================================


					//================================================ULTILIZAÇÃO DE ITENS========================================
					if(item_usado != SEM_EVENTO)
					{
						if(item_usado == F1)
						{
							if(mochila.get(0).getNome() == "Tiro")
							{
								Circulo k = mochila.getFirst();
								k.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());

								k.setDirecao(heroi->getDirecao());

								tiros.addLast(k);
								mochila.removeFirst();
							}
							else if(mochila.get(0).getNome() == "Vida")
							{
								vidas.addLast(mochila.getFirst());
								mochila.removeFirst();
							}
						}
					
						if(item_usado == F2)
						{
							if(mochila.get(1).getNome() == "Tiro")
							{
								Circulo k = mochila.get(1);
								k.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());

								k.setDirecao(heroi->getDirecao());

								tiros.addLast(k);
								if(mochila.getSize() > 3)
										mochila.remove(1);
									else
										mochila.removeLast();
							}
							else if(mochila.get(1).getNome() == "Vida")
							{
								vidas.addLast(mochila.get(1));
									if(mochila.getSize() > 3)
										mochila.remove(1);
									else
										mochila.removeLast();
							}
						}

						if(item_usado == F3)
						{
							if(mochila.get(2).getNome() == "Tiro")
							{
								Circulo k = mochila.get(2);
								k.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());

								k.setDirecao(heroi->getDirecao());

								tiros.addLast(k);
								if(mochila.getSize() > 4)
										mochila.remove(2);
									else
										mochila.removeLast();
			
							}
							else if(mochila.get(2).getNome() == "Vida")
							{
								vidas.addLast(mochila.get(2));
								mochila.removeLast();
							}
						}

						if(item_usado == F4)
						{
							if(mochila.get(3).getNome() == "Tiro")
							{
								Circulo k = mochila.get(3);
								k.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());

								k.setDirecao(heroi->getDirecao());

								tiros.addLast(k);
								mochila.removeLast();
							}
							else if(mochila.get(3).getNome() == "Vida")
							{
								vidas.addLast(mochila.get(3));
								mochila.removeLast();
							}
						}

						//USANDO O PODER
						if(item_usado == ESPACO)
						{
							if(poderes_coletados.get(poderes_coletados.getSize() - 1).getNome() == "Rosca")
							{	
								Circulo k1 = poderes_coletados.get(poderes_coletados.getSize() - 1);
								Circulo k2 = poderes_coletados.get(poderes_coletados.getSize() - 1);
								Circulo k3 = poderes_coletados.get(poderes_coletados.getSize() - 1);
								Circulo k4 = poderes_coletados.get(poderes_coletados.getSize() - 1);

								k1.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());
								k2.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());
								k3.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());
								k4.setPosicao(heroi->getPosicao_x(), heroi->getPosicao_y());

								k1.setDirecao(DOWN);
								k2.setDirecao(UP);
								k3.setDirecao(LEFT);
								k4.setDirecao(RIGHT);

								roscas.addLast(k1);
								roscas.addLast(k2);
								roscas.addLast(k3);
								roscas.addLast(k4);

								poderes_coletados.removeLast();
							}
							else if(poderes_coletados.get(poderes_coletados.getSize() - 1).getNome() == "Gelo")
							{	
								al_start_timer(contador);
								for (int i = 0; i < l_inimigos.getSize(); ++i)
								{
									Circulo k = l_inimigos.get(i);
									k.setVelocidade(v_gelo);
									if (i == 0)
										l_inimigos.removeFirst();
									else if (i == l_inimigos.getSize()-1)
										l_inimigos.removeLast();
									else
									l_inimigos.remove(i);

									l_inimigos.add(k);

									al_play_sample(som_gelo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);

										
								}

								gelos.addLast(poderes_coletados.get(poderes_coletados.getSize() - 1));
								poderes_coletados.removeLast();
							}
						}

						item_usado = SEM_EVENTO;
					}
					//===============================================FIM DA UTILIZAÇÃO DE ITENS===========================================					
					
					draw = true;
				}
				//TIMER PARA GELO
				else if (events.timer.source == contador)
				{
					seg++;
				}
				//TIMER PARA MUDAR ITENS DE POSICAO
				else if (events.timer.source == item_timer)
				{
					srand(time(NULL));
					bool pode;
					for (int i = 0; i < a.getSize(); ++i)
					{	
						pode = false;
						Circulo k = a.get(i);
						while (!pode)
						{
							int y = rand() % HEIGHT;
							int x = rand() % WIDTH;
							k.setPosicao(x, y);
							pode = true;

							for (int j = 0; j < l_obstaculos.getSize(); ++j)
								if (k.colidiu(l_obstaculos.get(j)))
									pode = false;

							for (int j = 0; j < a.getSize(); ++j)
								if (k.colidiu(a.get(j)))
									pode = false;

							for (int j = 0; j < poderes.getSize(); ++j)
								if (k.colidiu(poderes.get(j)))
									pode = false;

							for (int j = 0; j < l_inimigos.getSize(); ++j)
								if (k.colidiu(l_inimigos.get(j)))
									pode = false;

							if (k.colidiu(*castelo))
								pode = false;

							if (k.colidiu(*heroi))
								pode = false;
						}

						if (i == 0)
							a.removeFirst();
						else if (i == a.getSize() - 1)
							a.removeLast();
						else
							a.remove(i);

						a.add(k);
					}

					for (int i = 0; i < poderes.getSize(); ++i)
					{	
						pode = false;
						Circulo k = poderes.get(i);
						while (!pode)
						{
							int y = rand() % HEIGHT;
							int x = rand() % WIDTH;
							k.setPosicao(x, y);
							pode = true;

							for (int j = 0; j < l_obstaculos.getSize(); ++j)
								if (k.colidiu(l_obstaculos.get(j)))
									pode = false;

							for (int j = 0; j < a.getSize(); ++j)
								if (k.colidiu(a.get(j)))
									pode = false;

							for (int j = 0; j < poderes.getSize(); ++j)
								if (k.colidiu(poderes.get(j)))
									pode = false;

							for (int j = 0; j < l_inimigos.getSize(); ++j)
								if (k.colidiu(l_inimigos.get(j)))
									pode = false;

							if (k.colidiu(*castelo))
								pode = false;

							if (k.colidiu(*heroi))
								pode = false;
						}

						if (i == 0)
							poderes.removeFirst();
						else if (i == poderes.getSize() - 1)
							poderes.removeLast();
						else
							poderes.remove(i);

						poderes.add(k);
					}
				}
				//TIMER PARA ROTACIONAR
				else if (events.timer.source == rot_timer) 
				{
					mile += 0.05;
				}
				
			}
			//===========================================FIM EVENTOS TIMER===========================================


			//===========================================REDERIZAÇÃO NA TELA=========================================
			if(draw)
			{
				draw = false;
				al_clear_to_color(al_map_rgb(0, 100, 0));
					al_start_timer(rot_timer);

				heroi->Render();

				for (int i = 0; i < a.getSize(); i++)
				{
					a.get(i).Render();
				}

				for (int i = 0; i < poderes.getSize(); i++)
				{
					poderes.get(i).Render(mile);
				}

				for (int i = 0; i < l_inimigos.getSize(); ++i)
				{
					l_inimigos.get(i).Render();
				}

				for (int i = 0; i < l_obstaculos.getSize(); i++)
				{
					l_obstaculos.get(i).Render();
				}

				al_draw_text(f_western, al_map_rgb(0,0,0), 25, 10, 0, "Status: ");
				al_draw_scaled_bitmap(imagem, 0, 0, al_get_bitmap_width(imagem), al_get_bitmap_height(imagem), 20, 40, 80, 80, 0);

				al_draw_scaled_bitmap(img_vida_black, 0, 0, al_get_bitmap_width(img_vida_black), al_get_bitmap_height(img_vida_black), 20, 125, 30, 30, 0);
				al_draw_scaled_bitmap(img_vida_black, 0, 0, al_get_bitmap_width(img_vida_black), al_get_bitmap_height(img_vida_black), 20 + 1 * 30, 125, 30, 30, 0);
				al_draw_scaled_bitmap(img_vida_black, 0, 0, al_get_bitmap_width(img_vida_black), al_get_bitmap_height(img_vida_black), 20 + 2 * 30, 125, 30, 30, 0);

				al_draw_text(f_western, al_map_rgb(0,0,0), 25, 170, 0, "Status: ");
				al_draw_scaled_bitmap(img_isaac, 0, 0, al_get_bitmap_width(img_isaac), al_get_bitmap_height(img_isaac), 20 , 190, 80, 80, 0);
				al_draw_scaled_bitmap(img_gelo_black, 0, 0, al_get_bitmap_width(img_gelo_black), al_get_bitmap_height(img_gelo_black), 15, 275, 30, 30, 0);
				al_draw_scaled_bitmap(img_gelo_black, 0, 0, al_get_bitmap_width(img_gelo_black), al_get_bitmap_height(img_gelo_black), 15 + 1 * 33, 275, 30, 30, 0);
				al_draw_scaled_bitmap(img_gelo_black, 0, 0, al_get_bitmap_width(img_gelo_black), al_get_bitmap_height(img_gelo_black), 15 + 2 * 33, 275, 30, 30, 0);

				al_draw_text(f_western, al_map_rgb(0,0,0), 15, 400, 0, "Poderes: ");

				if (!poderes_coletados.isEmpty())
					al_draw_text(f_western, al_map_rgb(0,0,0), 20, 460, 0, "Espaco");

				//DESENHANDO VIDAS NA TELA
				for (int i = 0; i < vidas.getSize(); ++i)
				{
					Circulo v = vidas.get(i);
					v.setPosicao(35 + i * 30, 140);

					if (i == 0)
						vidas.removeFirst();
					else if(i == vidas.getSize() - 1)
						vidas.removeLast();
					else
						vidas.remove(i);
						
					vidas.add(v, i);

					vidas.get(i).Render();

				}

				for (int i = 0; i < gelos.getSize(); ++i)
				{
						Circulo g = gelos.get(i);
						g.setPosicao(30 + i * 33, 290);

						if (i == 0)
							gelos.removeFirst();
						else if(i == gelos.getSize() - 1)
							gelos.removeLast();
						else
							gelos.remove(i);
							
						gelos.add(g, i);

						gelos.get(i).Render();
				}


				castelo->Render();
					
				al_draw_text(f_western, al_map_rgb(0,0,0), 15, 550, 0, "Mochila: ");

				//MUDANDO POSICAO DOS ITENS PARA COLOCAR NA MOCHILA
				for (int i = 0; i < mochila.getSize(); ++i)
				{
					Circulo k = mochila.get(i);
					k.setPosicao(20 + i * 30, 600);
					
					if (i == 0)
						mochila.removeFirst();
					else if(i == mochila.getSize() - 1)
						mochila.removeLast();
					else
						mochila.remove(i);
						
					mochila.add(k, i);

					if(i == 0)
					{
						al_draw_text(f_western, al_map_rgb(0,0,0), 10 + i * 33, 620, 0, "F1");
					}
					if(i == 1)
					{
						al_draw_text(f_western, al_map_rgb(0,0,0), 10 + i * 33, 620, 0, "F2");
					}
					if(i == 2)
					{
						al_draw_text(f_western, al_map_rgb(0,0,0), 10 + i * 33, 620, 0, "F3");
					}
					if(i == 3)
					{
						al_draw_text(f_western, al_map_rgb(0,0,0), 10 + i * 33, 620, 0, "F4");
					}
					mochila.get(i).Render();
				}

				//MUDANDO POSICAO DOS PODERES PARA COLOCAR NA BARRA DE PODERES
				for (int i = 0; i < poderes_coletados.getSize(); ++i)
				{
					Circulo k = poderes_coletados.get(i);
					k.setPosicao(20 + i * 32, 440);
					
					if (i == 0)
						poderes_coletados.removeFirst();
					else if(i == poderes_coletados.getSize() - 1)
						poderes_coletados.removeLast();
					else
						poderes_coletados.remove(i);
						
					poderes_coletados.add(k, i);
					poderes_coletados.get(i).Render();
				}

				for (int i = 0; i < tiros.getSize(); ++i)
				{
					tiros.get(i).Render(mile);
				}
				
				for (int i = 0; i < roscas.getSize(); ++i)
				{
					roscas.get(i).Render(mile);
				}
				
				al_flip_display();
			}
		}
		//=======================================================FIM GAME LOOP=============================================

		//EVITA ALGUNS VAZAMENTOS DE MEMÓRIA
		al_destroy_timer(timer);
		al_destroy_timer(item_timer);
		al_destroy_timer(contador);
		al_destroy_timer(rot_timer);
	}
	return 0;
}