#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include "doublelist.h"

using namespace std;

void generarEncabezados(int nColumnas){
	string guiones = "__________";
	string espacios = "    ";
	string encabezado = "unNombre";
	
	cout << "  ";
	for(int i = 0; i < nColumnas; i++){
			cout << "| " << espacios << char(65 + i) << espacios;
		}
	cout << "|\n  ";
	for(int i = 0; i < nColumnas; i++){
			cout << "|"<< guiones;
		}
	cout << "|\n  ";
	for(int i = 0; i < nColumnas; i++){
			cout << "| " << encabezado << " ";
		}
	cout << "|\n  ";
	for(int i = 0; i < nColumnas; i++){
			cout << "|"<< guiones;
		}
	cout << "|\n";
}

void generarHoja(int* nFilas, int* nColumnas, int* filaActual, int* columnaActual, ListaDobleEnlazada* lista){
	int espaciosTotales = *nColumnas * 11, pos;
	string contenido;
	
	generarEncabezados(*nColumnas);
	
	for(int j = 0; j < *nFilas; j++){
		cout << "  ";
		for(int i = 0; i < espaciosTotales; i++){
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			else cout << " ";
		}
		if(j < 9) cout << "|\n " << j + 1;
		else cout << "|\n" << j + 1;
		for(int i = 0; i < *nColumnas + 1; i++){
			pos = (j) * 100 + i -1;
			contenido = lista->devolverContenido(&j, &pos);
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			//else if(*filaActual == j && *columnaActual/10 + 1 == i) cout << ">";
			else{
				if(*filaActual == j && *columnaActual/10 + 1 == i) cout << ">" << contenido << " |";
				else cout << " " << contenido << " |";
			}
			
		}
		cout << "|\n  ";
		for(int i = 0; i < espaciosTotales; i++){
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			else cout << "_";
		}
		cout << "|\n";
	}
}

void crearListaPrincipal(int* filas, int* columnas, ListaDobleEnlazada* lista){
	for(int i = 0; i < *filas; i++){
		(*lista).listaPrincipalInsertarFinal(&i, 0);
		
	}
}

void escribirTexto(int* filas, int* columnas, int* filaActual, int* columnaActual, ListaDobleEnlazada* lista){
	string contenido = "", comparacion = "";
	
	cout << "\nEscriba una palabra para almacenar en la celda " << char(65 + (*columnaActual)/10) << *filaActual + 1;
	cout << "\n (Maximo 8 caracteres): ";
	cin >> contenido;
	if(contenido.length() > 8) cout << "\n\nLa palabra contiene mas de 8 caracteres\n\n";
	else{
		for(int i = contenido.length(); i < 8; i++)
		contenido.push_back(' ');
		
		int pos = (*filaActual) * 100 + (*columnaActual)/10;
		cout << pos;
		
		comparacion = lista->devolverContenido(filaActual, &pos);
		comparacion.compare("        ") == 0 ? lista->llenarSubLista(filaActual, &pos, &contenido) : lista->modificarContenido(&contenido, filaActual, &pos);
		lista->mostrarListaPrincipal();
	}
}

void ingresarCelda(int* filas, int* columnas, int* filaActual, int* columnaActual){
	char col = ' ';
	int fila = 0;
	
	cout << "Ingrese la celda destino (ColumnaFila): ";
	cin >> col;
	col = toupper(col);
	cin.clear();
	cin >> fila;
	
	if(fila < 0 || fila > *filas ) cout << "\nNo es posible moverse a la fila ingresada (" << fila << ")";
	else if((int(col) - 65) < 0 || (int(col) - 65) > *columnas - 1) cout << "\nNo es posible moverse a la columna ingresada (" << col << ")";
	else{
		*filaActual = fila - 1;
		*columnaActual = (int(col) - 65) * 11;
	}
}

void moverCelda(int* filas, int* columnas, int* filaActual, int* columnaActual, int* opc){
	switch(*opc){
		case 2:
			ingresarCelda(filas, columnas, filaActual, columnaActual);
			break;
		case 6:
			if(*columnaActual == 0) cout << "\n\nNo es posible moverse a la izquierda\n\n";
			else *columnaActual -= 11;
			break;
		case 7:
			if(*columnaActual > (*columnas - 2) * 11) cout << "\n\nNo es posible moverse a la derecha\n\n";
			else *columnaActual += 11;
			break;
		case 8:
			if(*filaActual == 0) cout << "\n\nNo es posible moverse arriba\n\n";
			else *filaActual -= 1;
			break;
		case 9:
			
			if(*filaActual == *filas - 1) cout << "\n\nNo es posible moverse abajo\n\n";
			else *filaActual += 1;
			break;
	}
}

string copiarTexto(int* filaActual, int* columnaActual, ListaDobleEnlazada* lista, int* opc){
	int pos = (*filaActual) * 100 + *columnaActual/10;
	cout << "pos: " << pos << "\n";
	string contenido = lista->devolverContenido(filaActual, &pos);
	if(contenido.compare("        ") == 0) cout << "\nLa casilla esta vacia";
	if(*opc == 4) lista->eliminarNodo(filaActual, &pos);
	cout << "Se copio la palabra: " << contenido << ".";
	
	return contenido;
}

void pegarTexto(int* filaActual, int* columnaActual, ListaDobleEnlazada* lista, string* textoCopiado){
	cout << "El texto copiado es: " << *textoCopiado << ".";
	int pos = (*filaActual) * 100 + (*columnaActual)/10;
	string contenido = lista->devolverContenido(filaActual, &pos);
	//cout << pos;
	if(contenido.compare("        ") == 0 && *textoCopiado != "        "){
		lista->llenarSubLista(filaActual, &pos, textoCopiado);
		cout << "Se pego el texto: " << *textoCopiado;
	}
	else if(contenido != "        " && *textoCopiado != "        "){
		lista->modificarContenido(textoCopiado, filaActual, &pos);
		cout << "Entro en el else if con el texto: " << *textoCopiado;
	}
	
	lista->mostrarListaPrincipal();
}

void agregarColumna(int* fila, int* columna){
	if(*columna > 15) cout << "\n\nHa alcanzado el maximo de columnas permitidas\n\n";
	else *columna += 1;
}

void agregarFila(int* fila, int* columna, ListaDobleEnlazada* lista){
	if(*fila > 15) cout << "\n\nHa alcanzado el maximo de filas permitidas\n\n";
	else{
		*fila += 1;
		(*lista).listaPrincipalInsertarFinal(fila, 1);
	}
}

int menuAcciones(){
	int opc = 0, f = 0, c = 0, nFilas = 5, nColumnas = 5;
	string textoCopiado = "";
	ListaDobleEnlazada lista;
	crearListaPrincipal(&nFilas, &nColumnas, &lista);
	
	do{
		cout << "\n\nSe encuentra en la celda: " << char(65 + (c/10)) << f + 1<< "\n\n";
		generarHoja(&nFilas, &nColumnas, &f, &c, &lista);
		cout << " \nSeleccione una opcion:\n\n";
		cout << "1. Escribir\n2. Moverse de celda\n3. Copiar\n4. Cortar\n5. Pegar\n6. Moverse a la izquierda\n7. Moverse a la derecha\n8. Moverse arriba\n9. Moverse abajo\n10. Agregar columna\n11. Agregar fila\n12. Guardar\n13. Menu principal\nOpcion:";
		cin >> opc;
		
		if(cin.fail()){
			opc = 14;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
		
		switch(opc){
			case 1: escribirTexto(&nFilas, &nColumnas, &f, &c, &lista); break;
			case 2: moverCelda(&nFilas, &nColumnas, &f, &c, &opc); break;
			case 3: textoCopiado = copiarTexto(&f, &c, &lista, &opc); break;
			case 4: textoCopiado = copiarTexto(&f, &c, &lista, &opc); break;
			case 5: pegarTexto(&f, &c, &lista, &textoCopiado); break;
			case 6: moverCelda(&nFilas, &nColumnas, &f, &c, &opc); break;
			case 7: moverCelda(&nFilas, &nColumnas, &f, &c, &opc); break;
			case 8: moverCelda(&nFilas, &nColumnas, &f, &c, &opc); break;
			case 9: moverCelda(&nFilas, &nColumnas, &f, &c, &opc); break;
			case 10: agregarColumna(&nFilas, &nColumnas); break;
			case 11: agregarFila(&nFilas, &nColumnas, &lista); break;
			case 12: break;
			case 13: break;
			default: cout << "Opcion invalida!"; break;
		}
	}while(opc != 13);
	
	return opc;
}

int menuPrincipal(){
	int opcMenuPrincipal;
	
	do{
		cout << "Bienvenido\n\n";
		cout << "1. Nuevo\n2. Abrir\n3. Cerrar\nOpcion: ";
		cin >> opcMenuPrincipal;
		if(cin.fail()){
			opcMenuPrincipal = 4;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
	
		switch(opcMenuPrincipal){
			case 1: return 1; break;
			case 2: return 2; break;
			case 3: return 3; break;
			default: cout << "Opcion invalida!\n\n";
		}
	}while(opcMenuPrincipal < 1 || opcMenuPrincipal > 3);
	
	return opcMenuPrincipal;
}

int main(){
	
	int opcMenuPrincipal, opcMenuAcciones;
	// int filas = 5, col = 5, pos = 0;
	// string contenido = "HOLA";
	
	do{
		opcMenuPrincipal = menuPrincipal();
		switch(opcMenuPrincipal){
			case 1: menuAcciones();
			case 3: cout << "\n\nSaliendo del programa..."; break;
			default: cout << "\nOpcion invalida!";
		}
	}while(opcMenuPrincipal != 3);

	// ListaDobleEnlazada lista;
	// crearListaPrincipal(&filas, &col, &lista);
	// lista.mostrarListaPrincipal();
	// lista.llenarSubLista(&pos, &pos, &contenido);
	
	
	return 0;
}
