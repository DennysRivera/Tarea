#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class ListaDobleEnlazada {
	private:
	    struct Nodo{
	    	string contenido;
	    	int pos;
	    	Nodo* siguiente;
	    	Nodo* anterior;
		};
		Nodo nodo;
		
		
		
		struct NodoDeNodo{
			Nodo* contenido;
			int pos;
			NodoDeNodo* siguiente;
			NodoDeNodo* anterior;
			Nodo* inicio = NULL;
			Nodo* final = NULL;
		};
		
		NodoDeNodo* primero = NULL;
		NodoDeNodo* ultimo = NULL;

	public:
	    void insertarInicio(string contenido, int* pos, Nodo* inicio, Nodo* final){
	    	Nodo* nuevo = new Nodo;
	    	nuevo->contenido = contenido;
	    	nuevo->pos = *pos;
	    	nuevo->anterior = NULL;
	    	nuevo->siguiente = inicio;
	    	inicio = nuevo;
	    	if(final == NULL){
	    		final = nuevo;
			}
		}
		
		void insertarFinal(string contenido, int* pos, Nodo* inicio, Nodo* final){
			Nodo* nuevo = new Nodo;
			nuevo->contenido = contenido;
			nuevo->pos = *pos;
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
	
		void mostrarTodo(Nodo* inicio, Nodo* final){
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
		
		void listaPrincipalInsertarFinal(int* pos, int r){
			NodoDeNodo* nuevoNodo = new NodoDeNodo;
			nuevoNodo->contenido = NULL;
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
				Nodo* b = actual->inicio;
				Nodo* c = actual->final;
				cout << "\nPosicion de nodo: " << actual->pos;
				if(b) cout << "\n\tactual->inicio: " << b->pos << " actual->final: " << c->pos;
				
				Nodo* a = actual->inicio;
				while(a != NULL){
					cout << "\n\tTexto del nodo: " << a->pos << ": " << a->contenido;
					a = a->siguiente;
				}

				actual = actual->siguiente;
			}
			cout << "\nInicio: " << primero->pos << "\nFinal: " << ultimo->pos << "\n";
		}

		void buscarListaPrincipal(int* pos){
			NodoDeNodo* n = primero;
			Nodo* nuevo = new Nodo;
			while(n != NULL){
				if(n->pos == *pos){
					//n->contenido = new Nodo;
					n->contenido = nuevo;
				}
				n = n->siguiente;
			}
		}
		
		void eliminarNodo(int* pos, int* subPos){
			NodoDeNodo* actual = primero;
			while(actual != NULL){
				if(actual->pos == *pos){
					Nodo* a = actual->inicio, *b = NULL;
					while(a != NULL && a->pos != *subPos){
						b = a;
						a = a->siguiente;
					}
					cout << "Se detuvo en nodo subPos: " << a->pos << "\n";
					
					if(b == NULL && a->siguiente == NULL){
						a->anterior = NULL;
						actual->final = NULL;
						actual->inicio = NULL;
					}
					
					else if(b == NULL){
						actual->inicio = a->siguiente;
						actual->inicio->anterior = NULL;
					}
					else if(a->siguiente != NULL){
						b->siguiente = a->siguiente;
						a->siguiente->anterior = a->anterior;
					}
					else if(a->siguiente = NULL){
						b->siguiente = NULL;
						actual->final = a->anterior;
					}
					
					else if(a == actual->final){
						a->anterior->siguiente = NULL;
						a->anterior = NULL;
					}
					
					
					delete(a);
					
				}
				actual = actual->siguiente;
			}
			mostrarListaPrincipal();
		}

		string devolverContenido(int* pos, int* subPos){
			NodoDeNodo* actual = primero;
			while(actual != NULL){
				if(actual->pos == *pos){
					Nodo* a = actual->inicio;
					while(a != NULL){
						if(a->pos == *subPos){
							return a->contenido;
						}
						a = a->siguiente;
					}
				}
				actual = actual->siguiente;
			}
			return "        ";
		}
		
		void modificarContenido(string* contenido, int* pos, int* subPos){
			NodoDeNodo* actual = primero;
			while(actual != NULL){
				if(actual->pos == *pos){
					Nodo* a = actual->inicio;
					while(a != NULL){
						if(a->pos == *subPos){
							a->contenido = *contenido;
						}
						a = a->siguiente;
					}
				}
				actual = actual->siguiente;
			}
		}

		void insertarFinalSubLista(string* contenido, int* pos, Nodo* nuevo, NodoDeNodo* nodo){
			
			nuevo->contenido = *contenido;
			nuevo->pos = *pos;
			nuevo->siguiente = NULL;
			if(nodo->inicio == NULL){
				nodo->inicio = nuevo;
			}
			else{
				Nodo* p = nodo->inicio;
				Nodo* q = NULL;
				while(p != NULL){
					q = p;
					p = p->siguiente;
				}
				nuevo->anterior = q;
				q->siguiente = nuevo;
			}
			nodo->final = nuevo;
		}

		void llenarSubLista(int* pos, int* subPos, string* contenido){
			NodoDeNodo* n = primero;
			Nodo* nuevo = new Nodo;
			while(n != NULL){
				if(n->pos == *pos){
					if(n->contenido == NULL){
						n->contenido = nuevo;

						insertarFinalSubLista(contenido, subPos, nuevo, n);
						n->inicio = nuevo;
						n->final = nuevo;
					}
					//Posiblemente se puede retirar este else
					else{
						insertarFinalSubLista(contenido, subPos, nuevo, n);
						n->final = nuevo;
					}
					//cout << nuevo->contenido;

					

				cout << "n inicio: " << n->inicio->contenido << "\nn final: " << n->final->contenido;
				}
				n = n->siguiente;
			}
			cout << "\n\n";
			//cout << n->contenido->contenido;
		}

		void exportarJson(){
			json j;
			string jsonString = "", nombreArchivo = "";
			NodoDeNodo* actual = primero;

			while(actual != NULL){
				Nodo* a = actual->inicio;
				j["filas"] = actual->pos + 1;
				while(a != NULL){
					j["Nodos"].push_back({
						actual->pos,
						a->pos,
						a->contenido
					});
					a = a->siguiente;
				}
				actual = actual->siguiente;
			}
			cout << "Nombre para el archivo: "; cin >> nombreArchivo;
			nombreArchivo += ".json";
			jsonString = j.dump(2);

			ofstream archivo;
			archivo.open(nombreArchivo);
			archivo << jsonString;
			archivo.close();
			cout << "Se creo el archivo";
		}

		int devolverTotalFilas(string* nombreArchivo){
			json j;

			if(!((*nombreArchivo).find(".json") != string::npos))
				*nombreArchivo += ".json";

			cout << *nombreArchivo << "\n";
			
			ifstream archivo;
			archivo.open(*nombreArchivo);
			j << archivo;
			archivo.close();

			return j["filas"];
		}

		int devolverTotalColumnas(string* nombreArchivo){
			json j;
			int columnas = 0, mayor = 0;

			ifstream archivo;
			cout << *nombreArchivo << "\n";
			archivo.open(*nombreArchivo);
			j << archivo;
			archivo.close();

			for(int i = 0; i < j["Nodos"].size(); i++){
				columnas = j["Nodos"][i][1];
				columnas = columnas % 100;
				if(columnas > mayor) mayor = columnas;
			}
			
			return (mayor < 5) ? 5 : mayor;
		}

		void llenarTabla(string* nombreArchivo){
			json j;
			int pos = 0, subPos = 0;
			string contenido = "";

			ifstream archivo;
			cout << *nombreArchivo << "\n";
			archivo.open(*nombreArchivo);
			j << archivo;
			archivo.close();

			for(int i = 0; i < j["Nodos"].size(); i++){
				pos = j["Nodos"][i][0];
				subPos = j["Nodos"][i][1];
				contenido = j["Nodos"][i][2];
				cout << "Pos: " << pos << " subPos: " << subPos << " contenido: " << contenido;

				llenarSubLista(&pos, &subPos, &contenido);
			}
		}
};
