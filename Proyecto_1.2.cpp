#include <iostream>
#include <fstream>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#include <windows.h>
#include <queue>
#include <stack>
#include <ctime>
#include <cstdlib>
#include <cstring>
using namespace std;

//	ESTRUCTURA DE LOS PRODUCTOS
struct Productos
{
    int id;
    char descripcion[100];
    double precio;
    int stock;
    int ventas = 0;
}productos[19];

//	ESTRUCTURA DE CLIENTES
struct Clientes
{
    char nombre[40];
    int cv;
    char Nmr[100];
    float dinero_disponible;
    double PagoT = 0;
    stack<string> Carrito;
    int tiempo_de_compra = 0;
    int tiempo_de_llegada = 0;
}clientes[14];


// DECLARACION DE LA COLA DE CLIENTES
queue<Clientes> colaClientes;

// DEFINICION GLOBAL DE VARIABLES
int clientes_presentes= 0;
int compradores = 0;											//Clientes que excedieron el tiempo y no les alcanso el dinero
int tiempo_excedido = 0;										//Agrega a los clientes que excedieron el tiempo
int espaciado;
float totalVenta = 0;

//	PROTOTIPOS DE FUNCIONES
void cargar_productos();										//Cargar productos al programa
void cargar_clientes();											//Cargar clientes al programa
void mostrar_Productos();
void mostrar_Clientes();
void mostrar_nombre_supermercado();								//Muestra el Nombre del Supermercado
void encolar(const Clientes& cliente);							//Agrega a los clientes en un cola
void Creditos();												//Creditos de todos los participantes
void menu(int& eleccion);										//Menu del usuario
void iniciarSimulacion();										//Inicializar la simulacion de compra
void Generar_Tiempos();											//Generar Tiempos Aleatorios
void esperando_compra(int tiempo);								//Funcion para pausar el programa mientras se realiza  una compra
void Otros();

//		FUNCION PRINCIPAL
int main()
{
	int eleccion;
	
	//	ENCABEZADO
    mostrar_nombre_supermercado();
    
	// SELECCION DEL MENU
    do
    {
        menu(eleccion);
        switch (eleccion)
		{
            case 1: iniciarSimulacion(); 	break;	//OPCION 1 LLAMA A LA FUNCION INICIARSIMULACION
            case 2:	Creditos();				break;	//OPCION 2 MUESTRA LOS PARTICIPANTES DEL PROYECTO
            case 3: Otros();				break;	//MUESTRA OPCION DE BASES DE DATOS
            case 4: exit;					break;	//CIERRA EL PROGRAMA
            default: cout << "\nOpcion invalida";
        }
        system("cls");
    }while(eleccion != 4);
    
	for (int j = 0; j < 35; j++) //Centrando el texto
		{
            cout << " ";
        }
    cout << "Saliendo del programa, muchas gracias por visitarnos";
    getch();
    return 0;
}


//			TITULO HIPERMOD
void mostrar_nombre_supermercado()
{
    char nombre[8][70] =
	{
        {"   _   _  _                    ___  ___            _ "},
        {"  | | | |(_)                   |  \\/  |           | |"},
        {"  | |_| | _  _ __    ___  _ __ | .  . |  ___    __| |"},
        {"  |  _  || || '_ \\  / _ \\| '__|| |\\/| | / _ \\  / _` |"},
        {"  | | | || || |_) ||  __/| |   | |  | || (_) || (_| |"},
        {"  \\_| |_/|_|| .__/  \\___||_|   \\_|  |_/ \\___/  \\\\___|"},
        {"            | |                                      "},
        {"            |_|                                      "},
    };
	
	// CAMBIO DE COLOR DEL LOGO
    
    
    // CONSEGUIR TAMAÑO DE LA CONSOLA
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleWidth = consoleInfo.dwSize.X;

    // CALCULO DEL TAMAÑO NECESARIO PARA EL LOGO
    int espaciosEnBlanco = (consoleWidth - 70) / 2;
	int espaciado = espaciosEnBlanco;
    for (int i = 0; i < 8; i++)
	{
        // Imprimir espacios en blanco para centrar el texto
        for (int j = 0; j < espaciosEnBlanco; j++)
		{
            cout << " ";
        }

        for (int j = 0; j < 70; j++)
		{
            cout << nombre[i][j];
        }
        cout << "\n";
    }
    
    //	RETRASO EN EL SISTEMA PARA APRECIAR EL LOGO :D
    Sleep(1500);
    system("cls");
}

/*		FUNCION PARA GENERAL TIEMPO DE COMPRA ALEATORIO	*/
void Generar_Tiempos()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 14; i++)  
	{
        clientes[i].tiempo_de_compra = rand() % 15 + 1;
        if(clientes[i].tiempo_de_compra >= 10)
    	{
    		tiempo_excedido++;
		}
		fflush(stdin);
    }
    
    for (int i = 0; i < 14; i++)
    {
    	clientes[i].tiempo_de_llegada = rand() % 6 + 1;
	}
}

//				FUNCION PARA ENCOLAR A LOS CLIENTES
void encolar(const Clientes& cliente)
{
    colaClientes.push(cliente);
}


/*		FUNCION PARA LA INIALIZACION DEL PROGRAMA DE COMPRAS			*/
void iniciarSimulacion()
{	
	// LIMPIEZA DE PANTALLA
	system("cls");
	
	//	LLAMO A LA FUNCION PARA AGREGAR LOS TIEMPOS A CADA CLIENTE
	Generar_Tiempos();
	fflush(stdin);
	
	// LLAMO A LA FUNCION PARA AGREGAR A TODOS LOS USUARIOS
    cargar_clientes();
    fflush(stdin);
	
	// LLAMO A LA FUNCION PARA AGREGAR TODOS LOS PRODUCTOS
	cargar_productos();
    fflush(stdin);
	
	// AGREGAR PRODUCTOS A LOS CLIENTES
	int j,k; 										//	J = PRODUCTO RANDOM, K = NUMERO DE ARTICULOS A AGREGAR AL CARRITO
	srand(static_cast<unsigned int>(time(nullptr)));	//	DEFINO SRAND PARA CREAR UN NUMERO RANDOM
	
	for(int m = 0;m < 14;m++)
	{
		for(int i = 0; i < k; i++)	//	i = VARIABLE DE ITERACION HASTA LLEGAR AL NUMERO K
		{
			k = rand() % 30 + 1;	//	GENERA UN NUMERO RANDOM PARA LA CANTIDAD DE PRODUCTOS A ALMACENAR
		    j = rand() % 9 + 0;		//	GENERA UN NUMERO RANDOM PARA EL PRODUCTO A ESCOGER
		    productos[j].stock --;	//	ELIMINAMOS -1 AL STOCK DE DICHO ARTICULO SELECCIONADO
		    clientes[m].Carrito.push(productos[j].descripcion);
			clientes[m].PagoT += productos[j].precio;
			fflush(stdin);
		}
	}
	fflush(stdin);
	
	//	MOSTRAMOS LOS DATOS DEL CLIENTE
	system("cls");
    for (int i=0; i<14; i++)
	{
		if(clientes[i].tiempo_de_compra < 11 && clientes[i].dinero_disponible > clientes[i].PagoT)
		{
			cout << "\n";																				//ESPACIO ENTRE DATOS DE CADA CLIENTE
	        cout << "Nombre: " 					<< clientes[i].nombre 				<< endl;			//NOMBRE DEL CLIENTE
	        cout << "Cedula de identidad: V-" 	<< clientes[i].cv 					<< endl;			//CEDULA DEL CLIENTE
	        cout << "Numero de Telefono: +58-" 	<< clientes[i].Nmr 					<< endl;			//CEDULA DEL CLIENTE
	        cout << "Saldo disponible: " 		<< clientes[i].dinero_disponible 	<<"$"	  << endl;	//DINERO DEL CLIENTE
	        cout << "Total a pagar: " 			<< clientes[i].PagoT 				<<"$"	  << endl;	//PAGO TOTAL A REALIZAR
	        cout << "Tiempo de compra: " 		<< clientes[i].tiempo_de_compra 	<< " min" << endl;	//MUESTRA EL TIEMPO TOTAL DE COMPRA
	        cout << "Tiempo de llegada: " 		<< clientes[i].tiempo_de_llegada 	<< " min" << endl;	//MUESTRA EL TIEMPO TOTAL DE LLEGADA
	        
	        for(int l = 1;!clientes[i].Carrito.empty();l++)
			{
			    cout <<".- "<< clientes[i].Carrito.top() << endl;
			    clientes[i].Carrito.pop();
		    }
		    totalVenta += clientes[i].PagoT;
	        compradores++;
	        clientes_presentes++;		    
		    esperando_compra(clientes[i].tiempo_de_compra);
	        fflush(stdin);
	    }
	    else
	    {
	        clientes_presentes++;	    	
	    	cout << "\n";
	    	cout << "El cliente "<<clientes[i].nombre 	<< endl;
	    	cout << "\nHa excedido el tiempo de compra" << endl;
	    	cout << "No tiene fondos suficientes" 		<< endl;
	    	cout << "Ha salido de la cola" 				<< endl;
	    	Sleep(3000);
	    	system("cls");
			fflush(stdin);
		}	
	}
	
	//	MUESTRA LA CANTIDAD QUE EXCEDIO EL TIEMPO DE COMPRA
	cout << "\n\tCantidad de Clientes que superaron el tiempo limite: " << tiempo_excedido << endl;
	cout << "\n\tCantidad de Clientes que compraron exitosamente: " 	<< compradores << endl;
	cout << "\n\tCantidad de Clientes que se presentaron a comprar: " 	<< clientes_presentes << endl;
	cout << "\n\tTotal de ventas: $"									<< totalVenta<<endl;
	
    while (!colaClientes.empty())
	{
        Clientes cliente = colaClientes.front();
        colaClientes.pop();
    }
    system("pause");
}

//		FUNCION PARA TIEMPOS ENTRE COMPRAS
void esperando_compra(int tiempo)
{	
	for(int i= 0; i<50; i ++){
		cout<<" ";
	}
	cout<<"Comprando..."<<endl;
	
	for(int i= 0; i<30; i ++){
		cout<<" ";
	}	
	for(int i=0; i<tiempo; i++)
	{
		cout<<"------";
		Sleep(1000);
	}
	cout<<"\n\n";
	system("cls");
}

/*				MENUS						*/

/*			MENU PRINCIPAL					*/
void menu(int& eleccion)
{
    cout << "\tMENU PRINCIPAL: \n";
    cout << "1.-\tIniciar simulacion." 	<< endl;
    cout << "2.-\tCreditos." 			<< endl;
    cout << "3.-\tOtros." 				<< endl;
    cout << "4.-\tSalir." 				<< endl;
    cout << "\n-\tDigite su opcion: ";
    cin >> eleccion;
}

/*		MENU OPCION (CREDITOS)				*/
void Creditos()
{
	system("cls");
    cout << "\n\nCreadores: "<<endl;
    cout << "Francisco Fonseca  CI: V-26.359.537" << endl;
    cout << "Manuel Pastrano    CI: V-28.534.779" << endl;
    cout << "Carlos Hernandez   CI: V-27.975.753" << endl;
    cout << "Andres Gomez       CI: V-31.085.717" << endl;
    cout << "Ernesto Balbas     CI: V-30.932.082" << endl;

    //cout << "\nPresiona cualquier tecla para salir. ";
    system("pause");
}

/*		MENU OPCION (OTROS)					*/
void Otros()
{
	system("cls");
	int opcion;
	cout << "\n";
	cout << "1.-\tMostrar Productos cargados." << endl;
	cout << "2.-\tMostrar Clientes en el mercado." << endl;
	cout << "\n\tDigite una opcion: "; cin >> opcion;
	
	switch(opcion)
	{
		//	MOSTRAR PRODUCTOS CARGADOS
		case 1:mostrar_Productos(); break;
		
		//	MOSTRAR LOS CLIENTES EN LA BASE DE DATOS
		case 2:mostrar_Clientes(); break;
		
		// OPCION INVALIDA
		default: cout << "\nOpcion invalida";
	}
	system("pause");
}


/*					BASES DE DATOS									*/	
	
// FUNCION PARA MOSTRAR PRODUCTOS EN OPCION (OTROS)
void mostrar_Productos()
{
	system("cls");
	cargar_productos();
	
    cout << "\n";
	for(int i=0; i<10; i++)
	{
		cout<<"ID "		<<productos[i].id			<<endl;
	    cout<<".- "		<<productos[i].descripcion	<<endl;
	    cout<<" $"		<<productos[i].precio		<<endl;
	    cout<<" Stock: "<<productos[i].stock		<<endl;
	    cout<<"\n";
	};
    fflush(stdin);
}

// FUNCION PARA MOSTRAR CLIENTES EN OPCION (OTROS)
void mostrar_Clientes()
{
	system("cls");
	cargar_clientes();	
	for (int i = 0; i<14; i++){
			cout << "\n";																				//ESPACIO ENTRE DATOS DE CADA CLIENTE
	        cout << "Nombre: " 					<< clientes[i].nombre << endl;							//NOMBRE DEL CLIENTE
	        cout << "Cedula de identidad: V-" 	<< clientes[i].cv << endl;								//CEDULA DEL CLIENTE
	        cout << "Numero de Telefono: +58-" 	<< clientes[i].Nmr 					<< endl;			//CEDULA DEL CLIENTE
	        cout << "Dinero disponible: $"		<< clientes[i].dinero_disponible << endl;				//DINERO DEL CLIENTE
	}
}

// FUNCION PARA CARGAR CLIENTES
void cargar_clientes()
{
	/*		CARGAR CLIENTES		*/
	ifstream file("clientes.txt");
    if (file.is_open())
	{
		//	ITERADOR PARA AGREGAR CLIENTES DESDE EL ARCHIVO A LA ESTRUCTURA CLIENTES
		int i = 0;
        while (i < 14 && file >> clientes[i].nombre >> clientes[i].cv >> clientes[i].Nmr >> clientes[i].dinero_disponible)
		{
            i++;
            fflush(stdin);
        }
        file.close();
	}
		//	EN CASO DE QUE EL ARCHIVO NO SE ECUENTRE EN LA MISMA CARPETA DEL PROGRAMA
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
	}	
}

// FUNCION PARA CARGAR PRODUCTOS
void cargar_productos()
{
	/*		CARGAR PRODUCTOS			*/
	ifstream archivo("productos.txt");
	
    if (archivo.is_open())
	{
        //	ITERADOR PARA AGREGAR PRODUCTOS DESDE EL ARCHIVO A LA ESTRUCTURA PRODUCTOS
        int i = 0;
        while (i < 19 && archivo >> productos[i].id >> productos[i].descripcion >> productos[i].precio >> productos[i].stock)
		{
            i++;
            fflush(stdin);
        }
        archivo.close();
    }
    	//	EN CASO DE QUE EL ARCHIVO NO SE ECUENTRE EN LA MISMA CARPETA DEL PROGRAMA
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
    }
}
