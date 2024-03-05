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
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
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
void mostrar_Productos();										//Muestra los productos en pantalla
void mostrar_Clientes();										//Muestra los clientes en pantalla
void leerFacturas();											//Genera una Factura con las compras
void Producto_Mas_vendido();									//Genera el Producto ams vendido
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
	
	// GENERAR TIEMPOS DE LLEGADA Y COMPRA
	Generar_Tiempos();
	
	// LLAMO A LA FUNCION PARA AGREGAR A LOS CLIENTES
	cargar_clientes();
	fflush(stdin);
	
	//	ORDENAR CLIENTES POR TIEMPO DE LLEGADA
	sort(clientes, clientes + 14, [](const Clientes& a, const Clientes& b)
	{
        return a.tiempo_de_llegada < b.tiempo_de_llegada;
    });
	
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
	ofstream file; 	// CREAMOS UN OBJETO DE TIPO ofstreamtd::ofstream file; // CREAMOS UN OBJETO DE TIPO ofstream
	file.open("ultimas_facturas.txt");
    for (int i=0; i<14; i++)
	{
		if(clientes[i].tiempo_de_compra < 10 && clientes[i].dinero_disponible > clientes[i].PagoT)
		{
			time_t now = time(0);
		    char* dt = ctime(&now);
		    
 			cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
		    cout << "FACTURA DE COMPRA\n"; // Título de la factura
		    cout << "Fecha y hora: " << dt; // Fecha y hora actual
		    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
		    cout << "Nombre del cliente: " 		<< clientes[i].nombre 	<< "\n";
		    cout << "Cedula de identidad: V-" 	<< clientes[i].cv 		<< "\n";
		    cout << "Numero de Telefono: +58-" 	<< clientes[i].Nmr 		<< "\n";
		    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
		    cout << "PRODUCTOS COMPRADOS:\n"; // Título de los productos comprados
		    
		    file << "----------------------------------------------------------------------------------------------------\n" // Línea de separación
		    	 << "FACTURA DE COMPRA\n" // Título de la factura
		    	 << "Fecha y hora: \n" << dt // Fecha y hora actual
		    	 << "----------------------------------------------------------------------------------------------------\n" // Línea de separación
		    	 << "Nombre del cliente: " 		<< clientes[i].nombre 	<< "\n"
		    	 << "Cedula de identidad: V-" 	<< clientes[i].cv 		<< "\n"
		    	 << "Numero de Telefono: +58-" 	<< clientes[i].Nmr 		<< "\n"
		    	 << "----------------------------------------------------------------------------------------------------\n" // Línea de separación
		    	 << "PRODUCTOS COMPRADOS:\n"; // Título de los productos comprados
		    
	        for(int l = 1;!clientes[i].Carrito.empty();l++)
			{
			    cout <<".- "<< clientes[i].Carrito.top() << endl;
			    file <<".- "<< clientes[i].Carrito.top() << endl;
			    clientes[i].Carrito.pop();
		    }
		    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
		    cout << "Total a pagar: $" << clientes[i].PagoT << "\n";
		    cout << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min\n";
		    cout << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << " min\n";
		    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
			
			file << "----------------------------------------------------------------------------------------------------\n" // Línea de separación
		    	 << "Total a pagar: $" << clientes[i].PagoT << "\n"
		    	 << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min\n"
		    	 << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << " min\n"
		    	 << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
			
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
		    cout << "===========================================" 	<< endl;
		    cout << "Cliente: "<<clientes[i].nombre 				<< endl;
		    cout << "-------------------------------------------" 	<< endl;
		    cout << "Estado: - Ha excedido el tiempo de compra" 	<< endl;
		    cout << "        - No tiene fondos suficientes" 		<< endl;
		    cout << "        - Ha salido de la cola." 				<< endl;
		    cout << "-------------------------------------------" 	<< endl;
		    cout <<          "Gracias por su visita!" 				<< endl;
		    cout << "===========================================" 	<< endl;
		    	    	
		    file << "\n"
		    	 << "===========================================\n" 	<< endl
		    	 << "Cliente: "<<clientes[i].nombre<<"\n" 				<< endl
		    	 << "-------------------------------------------\n" 	<< endl
		    	 << "Estado: - Ha excedido el tiempo de compra\n" 		<< endl
		    	 << "        - No tiene fondos suficientes\n" 			<< endl
		    	 << "        - Ha salido de la cola.\n" 				<< endl
		    	 << "-------------------------------------------\n" 	<< endl
		    	 <<          "Gracias por su visita!\n" 				<< endl
		    	 << "===========================================\n" 	<< endl;
		    Sleep(3000);
		    system("cls");
		    fflush(stdin);
		}	
	}
	
	
	//	MUESTRA LA CANTIDAD QUE EXCEDIO EL TIEMPO DE COMPRA
	cout << "\n" << endl;
	cout << "\n\tCantidad de Clientes que superaron el tiempo limite: " 						<< tiempo_excedido << endl;
	cout << "\n\tCantidad de Clientes que compraron exitosamente: " 							<< compradores << endl;
	cout << "\n\tCantidad de Clientes que se presentaron a comprar: " 							<< clientes_presentes << endl;
	cout << "\n\tTotal de ventas: $"															<< totalVenta<<endl;
	cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
	
	file << "\n" << endl;
	file << "\n\tCantidad de Clientes que superaron el tiempo limite: " << tiempo_excedido << endl;
	file << "\n\tCantidad de Clientes que compraron exitosamente: " 	<< compradores << endl;
	file << "\n\tCantidad de Clientes que se presentaron a comprar: " 	<< clientes_presentes << endl;
	file << "\n\tTotal de ventas: $"									<< totalVenta<<endl;
	file << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
	
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
	for(int i= 0; i<50; i ++)
	{
		cout<<" ";
	}
	cout<<"Comprando..."<<endl;
	
	for(int i= 0; i<30; i ++)
	{
		cout<<" ";
	}	
	for(int i=0; i<tiempo; i++)
	{
		if (_kbhit())
		{
			char ch = _getch();
			if (ch == 'r' || ch == 'R')
			{
				system("cls");
				cout << "\nReiniciando el programa...\n";
				//	MUESTRA REPORTE DE LA COMPRA 
				cout << "=========================================================================" << endl;
				cout << "\n\t-Cantidad de Clientes que superaron el tiempo limite: " 				<< tiempo_excedido 		<< endl;
				cout << "\n\t-Cantidad de Clientes que compraron exitosamente: " 					<< compradores 			<< endl;
				cout << "\n\t-Cantidad de Clientes que se presentaron a comprar: " 					<< clientes_presentes 	<< endl;
				cout << "\n\t-Total de ventas: $"													<< totalVenta			<<endl;
				cout << "\n";
				cout << "=========================================================================" << endl;

				Sleep(6000);
				system("cls");
				main();
			}
		}
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
	cout << "---------------------------\n";
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
	cout << "3.-\tMostrar Facturas Generadas." << endl;
	cout << "\n\tDigite una opcion: "; cin >> opcion;
	
	switch(opcion)
	{
		//	MOSTRAR PRODUCTOS CARGADOS
		case 1:mostrar_Productos(); break;
		
		//	MOSTRAR LOS CLIENTES EN LA BASE DE DATOS
		case 2:mostrar_Clientes(); break;
		
		// MOSTRAMOS FACTURAS GENERADAS
		case 3:leerFacturas(); break;
		
		// OPCION INVALIDA
		default: cout << "\nOpcion invalida";
	}
	system("pause");
}


/*					BASES DE DATOS									*/	
	
// FUNCION PARA MOSTRAR PRODUCTOS EN OPCION (OTROS)
void mostrar_Productos()
{
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
	cargar_clientes();	
	for (int i = 0; i<14; i++)
	{
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

// FUNCION PARA MOSTRAR LAS ULTIMAS FACTURAS GENERADAS
void leerFacturas()
{
	system("cls");
    string linea;
    ifstream archivo("ultimas_facturas.txt");

    if (archivo.is_open())
	{
        while (getline(archivo, linea))
		{
            cout << linea << '\n';
        }
        archivo.close();
    } else {
    	cout << "\n";
        cout << "No se ha facturado ninguna compra" << endl;
    }
}