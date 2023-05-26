#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <typeinfo>
#include <fstream>
#include "nlohmann/json.hpp"

using namespace std;
using json = nlohmann::json;

class ListaDobleEnlazada
{
private:

	// Estructura para los nodos de la lista que contendra el texto
	// Consiste del contenido (texto) y una posicion dentro de la lista
	struct Nodo
	{
		string contenido;
		int pos;
		Nodo *siguiente;
		Nodo *anterior;
	};
	Nodo nodo;

	// Estructura para la lista "principal"
	// Su contenido sera un puntero al primer nodo de la lista para el texto
	// Cuenta con una posicion para ubicarlo dentro de la lista
	// además de nodos que contengan los valores de inicio y final de la lista "secundaria"
	struct NodoDeNodo
	{
		Nodo *contenido;
		int pos;
		NodoDeNodo *siguiente;
		NodoDeNodo *anterior;
		Nodo *inicio = NULL;
		Nodo *final = NULL;
	};

	NodoDeNodo *primero = NULL;
	NodoDeNodo *ultimo = NULL;

public:

	// Funcion para insertar nodos a la lista principal
	// Solamente se agregan nodos al final de la lista
	void listaPrincipalInsertarFinal(int *pos, int r)
	{
		NodoDeNodo *nuevoNodo = new NodoDeNodo;
		nuevoNodo->contenido = NULL;
		nuevoNodo->pos = (*pos) - r;
		nuevoNodo->siguiente = NULL;
		if (primero == NULL)
		{
			primero = nuevoNodo;
		}
		else
		{
			NodoDeNodo *p = primero;
			NodoDeNodo *q = NULL;
			while (p != NULL)
			{
				q = p;
				p = p->siguiente;
			}
			q->siguiente = nuevoNodo;
		}
		ultimo = nuevoNodo;
	}

	// Funcion no utilizada para el programa principal
	// Imprime el contenido de las listas sin la parte visual de las casillas
	void mostrarListaPrincipal()
	{
		NodoDeNodo *actual = primero;

		while (actual != NULL)
		{
			Nodo *b = actual->inicio;
			Nodo *c = actual->final;
			cout << "\nPosicion de nodo: " << actual->pos;
			if (b)
				cout << "\n\tactual->inicio: " << b->pos << " actual->final: " << c->pos;

			Nodo *a = actual->inicio;
			while (a != NULL)
			{
				cout << "\n\tTexto del nodo: " << a->pos << ": " << a->contenido;
				a = a->siguiente;
			}

			actual = actual->siguiente;
		}
		cout << "\nInicio: " << primero->pos << "\nFinal: " << ultimo->pos << "\n";
	}

	/*
	Funcion para remover cualquier nodo
	Se debe buscar primero dentro de la lista principal,
	y luego se busca dentro de la lista del nodo actual

	Se utiliza para la opcion de Cortar,
	ya que al cortar se retira el contenido de la casilla
	*/
	void eliminarNodo(int *pos, int *subPos)
	{
		NodoDeNodo *actual = primero;
		while (actual != NULL)
		{
			if (actual->pos == *pos)
			{
				Nodo *a = actual->inicio, *b = NULL;

				// Se recorre la lista mientras hayan nodos
				// y la posicion no corresponda al del nodo a eliminar
				while (a != NULL && a->pos != *subPos)
				{
					b = a;
					a = a->siguiente;
				}

				/*
				Diferentes condiciones para evaluar los diferentes escenarios
				de la eliminación del nodo:
				- Al incio
				- En medio
				- Al final

				Y realizar el intercambio de punteros correspondiente
				*/
				if (b == NULL && a->siguiente == NULL)
				{
					a->anterior = NULL;
					actual->final = NULL;
					actual->inicio = NULL;
				}

				else if (b == NULL)
				{
					actual->inicio = a->siguiente;
					actual->inicio->anterior = NULL;
				}
				else if (a->siguiente != NULL)
				{
					b->siguiente = a->siguiente;
					a->siguiente->anterior = a->anterior;
				}
				else if (a->siguiente == NULL)
				{
					b->siguiente = NULL;
					actual->final = a->anterior;
				}

				else if (a == actual->final)
				{
					a->anterior->siguiente = NULL;
					a->anterior = NULL;
				}

				// Al final se libera el espacio ocupado por el nodo
				delete (a);
			}
			actual = actual->siguiente;
		}
		//mostrarListaPrincipal();
	}

	// Funcion que devolvera el contenido del nodo en la lista "secundaria"
	// Primero busca la posicion en la lista principal
	// y luego dentro de la lista secundaria
	string devolverContenido(int *pos, int *subPos)
	{
		NodoDeNodo *actual = primero;
		while (actual != NULL)
		{
			if (actual->pos == *pos)
			{
				Nodo *a = actual->inicio;
				while (a != NULL)
				{
					// Si existe un nodo con la posicion especificada
					// se devolvera el contenido (texto)
					if (a->pos == *subPos)
					{
						return a->contenido;
					}
					a = a->siguiente;
				}
			}
			actual = actual->siguiente;
		}
		// Si no existe un nodo con la posicion especificada,
		// entonces se devuelven espacios vacios
		return "        ";
	}

	/*
	Funcion utilizada cuando no es necesario crear un nuevo nodo
	para almacenar texto, puesto que ya existía uno

	Por ejemplo, cuando se escribe dentro de una casilla
	que ya contiene texto, o se pega dentro de la misma
	texto copiado de otra casilla

	Neceista la posicion de la lista principal y luego de la lista secundaria
	para localizar el nodo indicado e insertar el contenido (texto)
	*/
	void modificarContenido(string *contenido, int *pos, int *subPos)
	{
		// Se recorre la lista principal mientras existan nodos
		NodoDeNodo *actual = primero;
		while (actual != NULL)
		{
			// Si se encuentra la posicion de la lista principal (fila),
			// comienza a buscar dentro de la lista secundaria
			if (actual->pos == *pos)
			{
				Nodo *a = actual->inicio;
				while (a != NULL)
				{
					// Se recorre la lista secundaria hasta encontrar al nodo indicado
					if (a->pos == *subPos)
					{
						// Si se encuentra el nodo indicado
						// solamente se le asigna el nuevo valor a la variable
						a->contenido = *contenido;
					}
					a = a->siguiente;
				}
			}
			actual = actual->siguiente;
		}
	}

	// Las siguientes 3 funciones se utilizan para la insercion de nodos,
	// contemplando los diferentes escenarios posibles: inicio, final o en medio de la lista

	// Esta funcion inserta un nodo al inicio de la lista secundaria
	void insertarInicioSubLista(string* contenido, int* pos, Nodo* nuevo, NodoDeNodo* nodo)
	{
		
		nuevo->contenido = *contenido;
		nuevo->pos = *pos;

		/*
		 El nuevo nodo no apunta a un nodo anterior,
		 puesto que se encuentra al inicio de la lista

		 El nuevo nodo debe apuntar a un valor siguiente,
		 ya sea un nodo existen o a nulo
		*/
		nuevo->anterior = NULL;
		nuevo->siguiente = nodo->inicio;

		// Se establece que el inicio (local) de la lista tome el valor del nuevo nodo
		nodo->inicio = nuevo;
	}

	// Esta funcion inserta un nodo al final de la lista secundaria
	void insertarFinalSubLista(string* contenido, int* pos, Nodo* nuevo, NodoDeNodo* nodo)
	{
		nuevo->contenido = *contenido;
		nuevo->pos = *pos;
		
		/*
		 El nuevo nodo no apunta a un nodo siguiente,
		 puesto que se encuentra al final de la lista

		 El nuevo nodo debe apuntar a un valor anterior,
		 por lo que se debe recorrer la lista en busca de dicho nodo
		*/
		nuevo->siguiente = NULL;

		Nodo *p = nodo->inicio;
		Nodo *q = NULL;

		while (p != NULL)
		{
			q = p;
			p = p->siguiente;
		}
			
		nuevo->anterior = q;
		q->siguiente = nuevo;

		// Se establece que el final (local) de la lista tome el valor del nuevo nodo
		nodo->final = nuevo;
	}

	// Esta funcion inserta un nodo en cualquier posicion de la lista que no sea el inicio o el final
	void insertarMedioSubLista(string* contenido, int* pos, Nodo* nuevo, NodoDeNodo* nodo){
		bool flag = true;
		nuevo->contenido = *contenido;
		nuevo->pos = *pos;

		Nodo* p = nodo->inicio;

		// Se recorre la lista en busca de la ubicacion adecuada
		while(p != NULL && flag){

			/*
			Se evalua el valor de la posicion del nodo a insertar
			con el de las posiciones existentes

			Se debe posicionar después de una posicion menor
			y antes de una posicion mayor
			Por ejemplo: de las posiciones existentes {1, 3, 5},
			la posicion 4 es mayor que 1, pero no que 3,
			por lo que avanza hasta que 4 es mayor que 3 pero no que 5
			*/
			if(p->pos < *pos && p->siguiente->pos > *pos){
				nuevo->anterior = p;
				nuevo->siguiente = p->siguiente;
				p->siguiente->anterior = nuevo;
				p->siguiente = nuevo;

				// El valor ayuda para detener el while
				flag = false;
			}
			p = p->siguiente;
		}
	}

	// Funcion principal para evaluar donde se debe insertar un nodo
	void llenarSubLista(int *pos, int *subPos, string *contenido)
	{
		NodoDeNodo *n = primero;
		Nodo *nuevo = new Nodo;

		// Se recorre la lista principal hasta llegar al nodo indicado (si existe)
		while (n != NULL)
		{
			if (n->pos == *pos)
			{
				// Al llegar a la posicion de la lista principal,
				// se evalua el contenido (puntero a un nodo de la lista secundaria)
				if (n->contenido == NULL)
				{
					// Si el contenido es nulo, quiere decir que no hay una lista secundaria,
					// por lo que se debe insertar un nodo al inicio de la lista
					n->contenido = nuevo;

					insertarInicioSubLista(contenido, subPos, nuevo, n);
					n->final = nuevo;
				}
				
				// Se realizan otras comprobaciones cuando ya existe una lista
				else
				{
					/*
					Si ya existe una lista, pero se quiere insertar un nodo con una posicion previa,
					por ejemplo, si ya existe la casilla 3, pero se quiere insertar un nodo en la casilla 1,
					se debe insertar al inicio de la lista existente

					Se aplica la misma lógica para insertar un nodo con una posicion posterior,
					por ejemplo, si ya existe la casilla 3, pero se quiere insertar un nodo en la casilla 4,
					se debe insertar al final de la lista existente

					Por último, se evalua si el valor de la posicion actual se encuentra
					entre las casillas del inicio y del final.
					En ese caso se debe insertar en medio
					*/
					if(*subPos < n->inicio->pos) insertarInicioSubLista(contenido, subPos, nuevo, n);
					else if(*subPos > n->final->pos) insertarFinalSubLista(contenido, subPos, nuevo, n);
					else if(*subPos > n->inicio->pos && *subPos < n->final->pos) insertarMedioSubLista(contenido, subPos, nuevo, n);
				}
			}
			n = n->siguiente;
		}
	}

	// Funcion para guardar el contenido de la hoja en un documento de tipo JSON
	void exportarJson()
	{
		json j;
		string jsonString = "", nombreArchivo = "";
		NodoDeNodo *actual = primero;

		// Se recorre toda la lista principal
		while (actual != NULL)
		{
			Nodo *a = actual->inicio;

			// Se almacena el número de filas en la hoja
			j["filas"] = actual->pos + 1;

			// Si el nodo de la lista principal contiene una lista secundaria,
			// se recorre esa lista
			while (a != NULL)
			{
				/*
				Se almacenan los siguientes valores del nodo:
				- posicion de la fila
				- posicion de la casilla
				- contenido de la casilla
				*/
				j["Nodos"].push_back({actual->pos,
									  a->pos,
									  a->contenido});
				a = a->siguiente;
			}
			actual = actual->siguiente;
		}
		
		// Se solicita un nombre para el archivo sin extension, ya que se le agrega después
		cout << "\nNombre para el archivo (sin extension): ";
		cin >> nombreArchivo;
		nombreArchivo += ".json";
		jsonString = j.dump(2);

		ofstream archivo;
		archivo.open(nombreArchivo);
		archivo << jsonString;
		archivo.close();
		cout << "\n\nSe creo el archivo\n\n";
	}

	// Estas 2 funciones se utilizan para calcular las dimensiones que debe tener
	// la hoja al momento de abrirla

	//Funcion para retornar el número de filas de la hoja
	int devolverTotalFilas(string *nombreArchivo, bool* flag)
	{
		json j;

		// Se realiza una verificación del nombre del archivo
		// Si es escribe sin extension ".json", se le agrega
		if(!((*nombreArchivo).find(".json") != string::npos))
			*nombreArchivo += ".json";

		ifstream archivo;
		archivo.open(*nombreArchivo);

		// Se devuelve el número de filas de la hoja
		// Si no se logra abrir, se retorna el número por defecto de 5 (minimo)
		if(archivo.is_open()){
			j << archivo;
			archivo.close();
			return j["filas"];
		}
		else{
			cout << "No se pudo abrir el archivo\n\n";
			*flag = false;
			return 5;
		}
		
	}

	// Funcion para retornar el número de columnas de la hoja
	int devolverTotalColumnas(string *nombreArchivo, bool* flag)
	{
		json j;
		int columnas = 0, mayor = 0;

		ifstream archivo;
		archivo.open(*nombreArchivo);

		if(archivo.is_open()){
			j << archivo;
			archivo.close();

			// Se evaluan las posiciones de las casillas almacenadas
			// en busca de la que se posicionó en la columna de mayor valor
			for (int i = 0; i < j["Nodos"].size(); i++)
			{
				columnas = j["Nodos"][i][1];
				columnas = columnas % 100;
				if (columnas > mayor)
					mayor = columnas;
			}

			// En caso de que la hoja solo esté llenada hasta una posicion
			// menor a 5, por ejemplo, hasta la columna 3 (C),
			// se devolverá el mínimo por defecto (5)
			return (mayor < 5) ? 5 : mayor + 1;
		}
		else{
			*flag = false;
			return 5;
		}
	}

	// Esta funcion se utiliza para rellenar la hoja abierta
	// con los valores almacenados
	void llenarTabla(string *nombreArchivo)
	{
		json j;
		int pos = 0, subPos = 0;
		string contenido = "";

		ifstream archivo;
		cout << *nombreArchivo << "\n";
		archivo.open(*nombreArchivo);
		j << archivo;
		archivo.close();

		// Por cada valor almacenado, se crea un nodo a partir de
		// la posicion de la fila, posicion de la casilla y el contenido
		for (int i = 0; i < j["Nodos"].size(); i++)
		{
			pos = j["Nodos"][i][0];
			subPos = j["Nodos"][i][1];
			contenido = j["Nodos"][i][2];

			llenarSubLista(&pos, &subPos, &contenido);
		}
	}
};
