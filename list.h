#ifndef __LIST_H__
#define __LIST_H__

#include "circulo.h"
#include "retangulo.h"

using namespace std;

//Lista duplamente encadeada implementada com uso de templates	
template <class T>
class List{
	struct Node{
		T valor;
		Node * prox;
		Node * ant;
	};
	Node *head;
	
	public:
	//CONSTRUTOR E DESTRUTOR
	List();
	~List();
	
	//VERIFICA SE LISTA É VAZIA
	bool isEmpty();

	//RETORNA TAMANHO DA LISTA
	int getSize();
	
	//ADICIONA NA PRIMEIRA, NA ULTIMA OU EM POSIÇÃO VALIDA DEFINIDA
	void addFirst(T x);
	void addLast(T x);
	void add(T x, int pos = 0);
	
	//REMOVE NA PRIMEIRA, NA ULTIMA OU EM POSIÇÃO VALIDA DEFINIDA
	void removeFirst();
	void removeLast();
	void remove(int pos = 0);
	
	//RETORNA PRIMEIRO, ULTIMO OU ELEMENTO DEFINIDO
	T getFirst();
	T getLast();
	T get(int pos = 0);
};

#endif