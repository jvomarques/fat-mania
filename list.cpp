#include "list.h"

using namespace std;

template<class T>
List<T>::List(){
	this->head = new Node();
	this->head->prox = NULL;
	this->head->ant = NULL;
}

template<class T>
List<T>::~List(){
	
}

template<class T>
bool List<T>::isEmpty(){
	return this->head->prox == NULL;
}

template<class T>
int List<T>::getSize(){
	int x = 0;
	Node *aux = this->head->prox;
	while (aux != NULL)
	{
		x++;
		aux = aux->prox;
	}
	return x;
}

template <class T>
void List<T>::addFirst(T x){
	Node *n = new Node();
	n->valor = x;
	
	if(this->isEmpty())
		this->head->ant = n;
	else
		this->head->prox->ant = n;
			
	n->ant = NULL;
	n->prox = this->head->prox;
	this->head->prox = n;

}

template <class T>
void List<T>::addLast(T x){
	if (isEmpty())
	{
		this->addFirst(x);
	}
	else
	{
		Node *n = new Node();
		n->valor = x;
		n->prox = NULL;
		n->ant = this->head->ant;

		this->head->ant->prox = n;
		this->head->ant = n;
	}
}

template <class T>
void List<T>::add(T x, int pos){
	if (this->isEmpty() || pos == 0)
		this->addFirst(x);
	else if (pos == this->getSize())
		this->addLast(x);
	else
	{
		Node * n = new Node();
		Node * aux = this->head->prox;
		
		int cont = 0;
		while (pos > cont++)
			aux = aux->prox;
			
		n->valor = x;
		n->prox = aux;
		n->ant = aux->ant;
		aux->ant->prox = n;
		aux->ant = n;
	}
}

template<class T>
void List<T>::removeFirst(){
	if (this->isEmpty())
		return;	
	T r = this->head->prox->valor;
	if(this->getSize() == 1)
	{
		Node *aux = this->head->prox;
		this->head->prox = NULL;
		this->head->ant = NULL;
		delete aux;
	}
	else
	{
		Node *aux = this->head->prox;
		this->head->prox = this->head->prox->prox;
		this->head->prox->ant = NULL;
		delete aux;		
	}
}

template<class T>
void List<T>::removeLast(){
	if (this->isEmpty())
		return;
	T r = this->head->ant->valor;
	if (this->getSize() == 1)
		this->removeFirst();
	else
	{
		Node *aux = this->head->ant;
		this->head->ant = this->head->ant->ant;
		this->head->ant->prox = NULL;
		delete aux;
	}
}

template <class T>
void List<T>::remove(int pos){
	if (this->isEmpty() || pos >= this->getSize() || pos < 0)
		return;
	if (pos == 0)
		this->removeFirst();
	if (pos == this->getSize()-1)
		this->removeLast();
	
	int cont = 0;
	Node *aux = this->head->prox;
	while (pos > cont++)
		aux = aux->prox;
	
	aux->ant->prox = aux->prox;
	aux->prox->ant = aux->ant;
	delete aux;
}

template <class T>
T List<T>::getFirst(){
	T *a = new T();
	return this->isEmpty() ? *a:this->head->prox->valor;
}

template <class T>
T List<T>::getLast(){
	T *a = new T();
	return this->isEmpty() ? *a:this->head->ant->valor;
}

template <class T>
T List<T>::get(int pos){
	T *a = new T();
	if (this->isEmpty() || pos >= this->getSize() || pos < 0)
		return *a;
	int cont = 0;
	Node * aux = this->head->prox;
	while (pos > cont++)
		aux = aux->prox;
	return aux->valor;
}

template class List<Circulo>;
template class List<Retangulo>;