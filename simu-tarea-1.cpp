#include <iostream>
#include <string>
#include <cstring>
#include <limits>
#include <conio.h>
#include <stdlib.h>
#include "header/doublelist.h"

using namespace std;

// Función para asignar una letra a cada columna
// con el fin de guiar más intituivamente al usuario
void generarEncabezados(int* nColumnas){
	string guiones = "__________";
	string espacios = "    ";
	
	cout << "  ";
	for(int i = 0; i < *nColumnas; i++){
			// Se imprimen las letras correspondientes según el número de columnas
			cout << "| " << espacios << char(65 + i) << espacios;
		}
	cout << "|\n  ";

	// Aquí se imprimen guiones y espacios para complementar la casilla
	// (decoración)
	for(int i = 0; i < *nColumnas; i++){
			cout << "|"<< guiones;
		}
	cout << "|\n";
}

// Función principal para la parte visual
// Se encarga de imprimir tanto la forma de la casilla como
// el contenido correspondiente
void generarHoja(int* nFilas, int* nColumnas, int* filaActual, int* columnaActual, ListaDobleEnlazada* lista){
	int espaciosTotales = *nColumnas * 11, pos;
	string contenido;
	
	// Los encabezados se imprimen desde su propia función
	// ya que es un elemento extra
	generarEncabezados(nColumnas);
	
	/*
	La impresion de la hoja consiste de un for principal
	y otros 3 for (no anidados) internos
	El exterior itera las instrucciones por cada fila
	Este for depende del número de filas (no menor a 5)
	*/
	for(int j = 0; j < *nFilas; j++){
		cout << "  ";

		/*
		Cada fila contendrá 3 líneas (1 arriba y abajo llena de espacios vacíos
		y 1 en medio donde se visualizara el contenido) de diez espacios cada una
		Cada casilla puede contener palabras de 8 caracteres como maximo,
		más un espacio en blanco a cada lado para que la palabra se muestre centrada en la misma

	  	 ---------- ---------- ---------- ---------- ----------
		|          |          |          |          |          |
	 	|          |          |          |          |          |
	 	|          |          |          |          |          |
	  	 ---------- ---------- ---------- ---------- ----------
		*/

		// El primer for interno se encarga de imprimir espacios vacios " "
		// y lineas verticales | para delimitar las casillas
		// Esto conforma la primera línea de la casilla (superior)
		for(int i = 0; i < espaciosTotales; i++){
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			else cout << " ";
		}

		/*
		Ademas de las letras para las columnas, se imprimen los números de las filas
		en la linea central de la casilla (por simetria)

		Las filas se comienzan a contar desde 0, pero se presentan
		desde el 1 para comodidad del usuario

		Hasta la fila 9, se agrega un espacio en blanco (" ") extra
		para evitar que se deforme la tabla cuando se comience a contar
		las filas a partir de 10
		*/
		if(j < 9) cout << "|\n " << j + 1;
		else cout << "|\n" << j + 1;

		/*
		El segundo for se encarga de imprimir la linea central,
		la cual muestra el contenido de la celda si existe,
		de lo contrario muestra espacios en blanco
		*/
		for(int i = 0; i < *nColumnas + 1; i++){

			// La variable pos almacena la posicion de la celda que se esta imprimiendo
			// Si el valor corresponde con el de un valor almacenado, entonces lo imprime
			pos = (j) * 100 + i -1;
			contenido = lista->devolverContenido(&j, &pos);

			// El if hace que se imprima una barra vertical | para  delimitar la celda
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			//else if(*filaActual == j && *columnaActual/10 + 1 == i) cout << ">";
			else{
				/*
				El if anidado evalua si se encuentra en el espacio en blanco a la izquierda de la casilla
				Si el usuario se posicionó anteriormente en la casilla,
				se imprime una flecha ">" para indicar la posicion dentro de la hoja
				con el fin de orientar al usuario
				*/
				if(*filaActual == j && *columnaActual/10 + 1 == i) cout << ">" << contenido << " |";
				else cout << " " << contenido << " |";
			}
			
		}
		cout << "\n  ";

		// El tercer for imprime la línea inferior de la casilla
		// Es similar a la primera línea, excepto que aquí imprime
		// barras verticales | y guines bajos _ para cerar la parte inferior de la casilla
		for(int i = 0; i < espaciosTotales; i++){
			if((i % 11 == 0) || (i % espaciosTotales == 0)) cout << "|";
			else cout << "_";
		}
		cout << "|\n";
	}
}

// La función solamente manda a llamar la función de la clase
// para crear el archivo JSON
void guardar(ListaDobleEnlazada* lista){
	lista->exportarJson();
}

/*
Esta función se encarga de crear la lista "principal",
que es la que contendrá una lista dentro de cada nodo

Se puede visualizar como una lista en forma de columna

Por cada nodo de la lista principal que se cree,
es equivalente a agregar una fila a la hoja
*/
void crearListaPrincipal(int* filas, ListaDobleEnlazada* lista, int* opcMenuPrincipal, string* nombreArchivo){

	/*
	El for agregara un nodo por cada iteracion,
	que depende del número de filas del archivo

	Esto solo crea la lista "principal",
	sin agregar nodos a la lista que se asigna como su contenido
	*/
	for(int i = 0; i < *filas; i++)
		/*
		Se pasa i como parametro para ser asignado como posicion del nodo dentro de la lista
		Como la funcion require un valor para restar,
		se envía 0 porque aqui no es necesaria la resta
		*/
		(*lista).listaPrincipalInsertarFinal(&i, 0);

	/*
	Cuando se abra un archivo (opcion 2 del menu principal), se recibe como parametro
	para agregar los nodos correspondientes de las sublista de cada nodo de
	la lista principal
	*/
	if(*opcMenuPrincipal == 2) lista->llenarTabla(nombreArchivo);
}

//Funcion para la adicion de texto dentro de las casillas
//Requiere la posicion de la casilla, es decir, la fila y la columna
void escribirTexto(int* filaActual, int* columnaActual, ListaDobleEnlazada* lista){
	string contenido = "", comparacion = "";
	
	cout << "\nEscriba una palabra para almacenar en la celda " << char(65 + (*columnaActual)/10) << *filaActual + 1;
	cout << "\n (Maximo 8 caracteres): ";
	cin >> contenido;

	// Se verifica que el contenido de la casilla no exceda los 8 caracteres
	if(contenido.length() > 8) cout << "\n\nLa palabra contiene mas de 8 caracteres\n\n";
	else{

		/*
		Para mantener el aspecto uniforme de la casilla,
		se agregan espacios en blanco al contenido hasta alcanzar los 8 caracteres
		en caso de tener menos
		*/		
		for(int i = contenido.length(); i < 8; i++)
		contenido.push_back(' ');
		
		/*
		El texto se almacena en un nodo junto con una posición dentro de la lista
		Para evitar confusion entre algunas casillas, la posicion aumenta de valor
		+100 por cada fila
		*/
		int pos = (*filaActual) * 100 + (*columnaActual)/10;
		
		/*
		Se compara el contenido de la celda actual (posicion del usuario)

		Si la casilla no contiene texto, es decir, su cuerpo son espacios vacios,
		entonces no hay nodo existente, por lo que crea uno con el texto
		Si la casilla contiene texto, es decir, su cuerpo NO son espacios vacios,
		entonces ya existe un nodo, por lo que solo es necesario cambiar el texto de dicho nodo
		*/
		comparacion = lista->devolverContenido(filaActual, &pos);
		comparacion.compare("        ") == 0 ? lista->llenarSubLista(filaActual, &pos, &contenido) : lista->modificarContenido(&contenido, filaActual, &pos);
		//lista->mostrarListaPrincipal();
	}
}


// Funcion utilizada para poder moverse a una celda arbitraria
// en lugar de desplazarse una fila/columna a la vez
void ingresarCelda(int* filas, int* columnas, int* filaActual, int* columnaActual){
	char col = ' ';
	int fila = 0;
	
	/*
	Se solicita la celda para desplazarse en formato ColumnaFila
	Por ejemplo, A1, B3, D5

	Se almacena la letra de la columna como mayuscula,
	en case que el usuario la ingrese como minuscula
	*/
	cout << "Ingrese la celda destino (ColumnaFila): ";
	cin >> col;
	col = toupper(col);
	cin.clear();
	cin >> fila;
	
	/*
	Se verifica que la fila ingresada esté dentro del rango de filas en la hoja
	Se realiza una verificación similar para las columnas
	
	Si tanto la fila como la columna se encuentran dentro de la hoja,
	entonces los valores de la fila y columna actual se cambian al ingresado por el usuario
	*/
	if(fila < 0 || fila > *filas ) cout << "\nNo es posible moverse a la fila ingresada (" << fila << ")";
	else if((int(col) - 65) < 0 || (int(col) - 65) > *columnas - 1) cout << "\nNo es posible moverse a la columna ingresada (" << col << ")";
	else{
		*filaActual = fila - 1;
		*columnaActual = (int(col) - 65) * 11;
	}
}

// Funcion principal para el desplazamiento de celdas
// por lo que necesita el total de filas y columnas, asi como la posicion actual
void moverCelda(int* filas, int* columnas, int* filaActual, int* columnaActual, int* opc){

	/*
	Se evalua la opcion seleccionada en el menu de acciones:
	2 - Moverse a una celda arbitraria
	6 - Moverse a la izquierda
	7 - Moverse a la derecha
	8 - Moverse arriba
	9 - Moverse abajo
	*/
	switch(*opc){
		case 2:
			// El desplazamiento a una celda arbitraria se realiza en su propia función
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

// Funcion para copiar el texto de una celda
string copiarTexto(int* filaActual, int* columnaActual, ListaDobleEnlazada* lista, int* opc){

	// Se almacena la posicion de la celda actual
	// y se llama la funcion para retornar el texto de la celda
	int pos = (*filaActual) * 100 + *columnaActual/10;
	string contenido = lista->devolverContenido(filaActual, &pos);

	// Si la celda esta vacía, entonces se le indicara al usuario con un mensaje
	if(contenido.compare("        ") == 0) cout << "\nLa casilla esta vacia";

	/*
	Se recicla la funcion para la opcion de cortar
	debido a que debe realizar la misma accion,
	con la diferencia de eliminar el contenido de la celda de donde se recorta
	Para esto, se evalua la opcion del menu de acciones,
	donde la opcion 4 es Recortar Texto*/
	if(*opc == 4) lista->eliminarNodo(filaActual, &pos);
	
	// Al final se retorna un string, ya sea espacios en blanco o un texto
	return contenido;
}

// Funcion para pegar texto en una casilla
void pegarTexto(int* filaActual, int* columnaActual, ListaDobleEnlazada* lista, string* textoCopiado){
	
	// Se almacena la posicion de la casilla actual
	// y se llama la funcion para retornar el texto de la casilla
	int pos = (*filaActual) * 100 + (*columnaActual)/10;
	string contenido = lista->devolverContenido(filaActual, &pos);
	
	/*
	Se evalua el contenido de la casilla actual
	Si la casilla esta vacia, quiere decir que no existe un nodo en la casilla,
	y el texto copiado no esta vacio, es decir, contiene texto,
	entonces se crea un nuevo nodo y se le asigna el texto
	*/
	if(contenido.compare("        ") == 0 && *textoCopiado != "        "){
		lista->llenarSubLista(filaActual, &pos, textoCopiado);
	}

	/*
	Se evalua el contenido de la casilla actual
	Si la casilla ya contiene un texto, quiere decir que ya existe un nodo,
	y el texto copiado no está vacio,
	entonces se debe modificar el texto del nodo
	*/
	else if(contenido != "        " && *textoCopiado != "        "){
		lista->modificarContenido(textoCopiado, filaActual, &pos);
	}
}

// Funcion simple para la adicion de columnas, con un maximo de 25 columnas
void agregarColumna(int* columna){
	// Se le indica al usuario si ya ha alcanzado el maximo
	if(*columna > 24) cout << "\n\nHa alcanzado el maximo de columnas permitidas\n\n";
	else *columna += 1;
}

// Funcion simple para la adicion de filas, con un maximo de 25 filas
void agregarFila(int* fila, ListaDobleEnlazada* lista){
	// Se le indica al usuario si ya ha alcanzado el maximo
	if(*fila > 24) cout << "\n\nHa alcanzado el maximo de filas permitidas\n\n";
	else{
		// Si se puede agregar otra fila,
		// entonces también se debe agregar otro nodo a la lista "principal"
		*fila += 1;

		// Se le resta 1 para que su posicion concuerde con la sucesion
		(*lista).listaPrincipalInsertarFinal(fila, 1);
	}
}

// Funcion para cuando haya cambios sin guardar dentro de la hoja
void confirmarGuardar(bool* cambio, ListaDobleEnlazada* lista){
	int confirmacion = 0;

	if(*cambio){
		do{
			cout << "Tiene cambios sin guardar\n\nLe gustaria guardar antes de salir?\n"
			<< "1. Guardar antes de salir\n2. Salir sin guardar\nOpcion: ";
			cin >> confirmacion;

			if(confirmacion == 1) guardar(lista);
			else if(confirmacion == 2) return;
			else cout << "\nOpcion invalida\n\n";

			if(cin.fail()){
				confirmacion = 3;
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(),'\n');
			}
		}while(confirmacion < 1 || confirmacion > 2);
	}
}

// Funcion donde se seleccionan las acciones posibles dentro de la hoja
int menuAcciones(int* nFilas, int* nColumnas, ListaDobleEnlazada* lista, int* opcMenuPrincipal, string* nombreArchivo){

	//Se asignan los valores iniciales para las filas y columnas
	// es decir, la fila y columna actual
	int opc = 0, f = 0, c = 0;
	string textoCopiado = "";
	bool cambio = false;

	// Al entrar al menu se genera la lista "principal"
	crearListaPrincipal(nFilas, lista, opcMenuPrincipal, nombreArchivo);
	
	// Este menu se mostrara despues de realizar cualquier acción
	// hasta que la opcion sea regresar al menu principal
	do{
		// Mensaje para indicar al usuario su posicion actual
		cout << "\n\nSe encuentra en la celda: " << char(65 + (c/10)) << f + 1<< "\n\n";

		// Se llama la funcion que se encarga la parte visual de la hoja
		generarHoja(nFilas, nColumnas, &f, &c, lista);

		cout << " \nSeleccione una opcion:\n\n";
		cout << "1. Escribir\n2. Moverse de celda\n3. Copiar\n4. Cortar\n5. Pegar\n6. Moverse a la izquierda\n7. Moverse a la derecha\n8. Moverse arriba\n9. Moverse abajo\n10. Agregar columna\n11. Agregar fila\n12. Guardar\n13. Menu principal\nOpcion:";
		cin >> opc;
		
		// Se realiza una comprobacion del input del usuario
		// Si el usuario ingresa algo que no sea un int,
		// entonces falla y se asigna un valor que caiga dentro del case default del switch
		if(cin.fail()){
			opc = 14;
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(),'\n');
		}
		
		switch(opc){
			case 1: escribirTexto(&f, &c, lista); cambio = true; break;
			case 2: moverCelda(nFilas, nColumnas, &f, &c, &opc); break;
			case 3: textoCopiado = copiarTexto(&f, &c, lista, &opc); break;
			case 4: textoCopiado = copiarTexto(&f, &c, lista, &opc); cambio = true; break;
			case 5: pegarTexto(&f, &c, lista, &textoCopiado); cambio = true; break;
			case 6: moverCelda(nFilas, nColumnas, &f, &c, &opc); break;
			case 7: moverCelda(nFilas, nColumnas, &f, &c, &opc); break;
			case 8: moverCelda(nFilas, nColumnas, &f, &c, &opc); break;
			case 9: moverCelda(nFilas, nColumnas, &f, &c, &opc); break;
			case 10: agregarColumna(nColumnas); cambio = true; break;
			case 11: agregarFila(nFilas, lista); cambio = true; break;
			case 12: guardar(lista); cambio = false; break;
			case 13: confirmarGuardar(&cambio, lista); cout << "Regresando al menu principal...\n\n"; break;
			default: cout << "Opcion invalida!"; break;
		}
	}while(opc != 13);
	
	return opc;
}

// Funcion para imprimir el menu principal
// y retornar la opcion seleccionada
int menuPrincipal(){
	int opcMenuPrincipal;
	
	// Este menu se mostrara hasta que la opcion sea cerrar el programa
	do{
		cout << "Bienvenido al Excel Chiquito\n\n";
		cout << "1. Nuevo\n2. Abrir\n3. Cerrar\nOpcion: ";
		cin >> opcMenuPrincipal;

		// Se realiza una comprobacion del input del usuario
		// Si el usuario ingresa algo que no sea un int,
		// entonces falla y se asigna un valor que caiga dentro del case default del switch
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
		// Las instrucciones se repetiran si no estan dentro del rango 1 a 3
	}while(opcMenuPrincipal < 1 || opcMenuPrincipal > 3);
	
	// Se retorna la opcion seleccionada
	return opcMenuPrincipal;
}

// Funcion main del programa
// Desde aqui se abre una nueva hoja o una ya existente, y se cierra el programa
int main(){
	int opcMenuPrincipal, opcMenuAcciones, nFilas, nColumnas;
	string nombreArchivo;
	bool flag;
	
	do{
		ListaDobleEnlazada lista;
		
		// Se asigna el valor de las filas y columnas por defecto para una nueva hoja
		// Si se abre una hoja existente, entonces se asignan los valores correspondientes
		nFilas = 5;
		nColumnas = 5;
		opcMenuPrincipal = menuPrincipal();

		switch(opcMenuPrincipal){
			/*
			Tanto la opcion 1 como la 2 abren el menu de acciones
			Sin embargo, la opcion 1 asigna valores por defecto para las filas y columnas,
			mientras que la 2 abre una hoja existente,
			además, se hace una comprobación de un valor booleano para ir al menu de acciones
			*/
			case 1: menuAcciones(&nFilas, &nColumnas, &lista, &opcMenuAcciones, &nombreArchivo); break;
			case 2:
				flag = true;
				cout << "Ingrese el nombre del archivo para abrir: "; cin >> nombreArchivo;
				nFilas = lista.devolverTotalFilas(&nombreArchivo, &flag);
				nColumnas = lista.devolverTotalColumnas(&nombreArchivo, &flag);
				if(flag) menuAcciones(&nFilas, &nColumnas, &lista, &opcMenuPrincipal, &nombreArchivo);
				break;
			case 3: cout << "\n\nSaliendo del programa..."; break;
			default: cout << "\nOpcion invalida!";
		}
	}while(opcMenuPrincipal != 3);
	
	return 0;
}
