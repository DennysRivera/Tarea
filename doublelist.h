#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>

using namespace std;

class ListaDobleEnlazada {
	private:
	    struct Nodo{
	    	string contenido;
	    	int pos;
	    	Nodo* siguiente;
	    	Nodo* anterior;
		};
		Nodo nodo;
		
		Nodo* inicio = NULL;
		Nodo* final = NULL;
		
		struct NodoDeNodo{
			Nodo contenido;
			int pos;
			NodoDeNodo* siguiente;
			NodoDeNodo* anterior;
		};
		
		NodoDeNodo* primero = NULL;
		NodoDeNodo* ultimo = NULL;

	public:
	    void insertarInicio(string contenido, int pos){
	    	Nodo* nuevo = new Nodo;
	    	nuevo->contenido = contenido;
	    	nuevo->pos = pos;
	    	nuevo->anterior = NULL;
	    	nuevo->siguiente = inicio;
	    	inicio = nuevo;
	    	if(final == NULL){
	    		final = nuevo;
			}
		}
		
		void insertarFinal(string contenido, int pos){
			Nodo* nuevo = new Nodo;
			nuevo->contenido = contenido;
			nuevo->pos = pos;
			nuevo->siguiente = NULL;
			if(inicio == NULL){
				inicio = nuevo;
			}
			else{
				Nodo* p = inicio;
				Nodo* q = NULL;
				while(p != NULL){
					q = p;
					p = p->siguiente;
				}
				q->siguiente = nuevo;
			}
			final = nuevo;
		}
	
		void mostrarTodo(){
			Nodo* actual = inicio;
			while(actual != NULL){
				cout << "Texto actual: " << actual->contenido;
				actual = actual->siguiente;
			}
			cout << "\nInicio: " << inicio->contenido << "\nFinal: " << final->contenido;
		}
		
		//M�todos para la lista que contandr� la sublista
		
		void insertarInicio(int pos){
	    	NodoDeNodo* nuevoNodo = new NodoDeNodo;
	    	//nuevoNodo->contenido = NULL;
	    	nuevoNodo->pos = pos;
	    	nuevoNodo->anterior = NULL;
	    	nuevoNodo->siguiente = primero;
	    	primero = nuevoNodo;
	    	if(ultimo == NULL){
	    		ultimo = nuevoNodo;
			}
		}
		
		void insertarFinal(int* pos, int r){
			NodoDeNodo* nuevoNodo = new NodoDeNodo;
			//nuevoNodo->contenido = NULL;
			nuevoNodo->pos = (*pos) - r;
			nuevoNodo->siguiente = NULL;
			if(primero == NULL){
				primero = nuevoNodo;
			}
			else{
				NodoDeNodo* p = primero;
				NodoDeNodo* q = NULL;
				while(p != NULL){
					q = p;
					p = p->siguiente;
				}
				q->siguiente = nuevoNodo;
			}
			ultimo = nuevoNodo;
		}
		
		void mostrarListaPrincipal(){
			NodoDeNodo* actual = primero;
			while(actual != NULL){
				cout << "Posicion de nodo: " << actual->pos;
				actual = actual->siguiente;
			}
			cout << "\nInicio: " << primero->pos << "\nFinal: " << ultimo->pos << "\n";
		}
};
