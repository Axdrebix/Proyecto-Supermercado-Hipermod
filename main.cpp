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

struct Productos
{
    int id;
    char descripcion[100];
    double precio;
    int stock;
    int ventas = 0;
}productos[11];

struct Clientes
{
    char nombre[40];
    int cv;
    float dinero_disponible;
    double PagoT = 0;
    char cosmeticos[100];
    int tiempo_de_compra;
    int tiempo_de_llegada;
}clientes[11];

        
queue<Clientes> colaClientes;  // Declaración de la cola de clientes

//MACROS
const int MAX_TOTAL_ARTICULOS = 30;
const int MAX_ARTICULO_POR_PRODUCTO = 5;
const int MAX_CLIENTES = 10;
const int MAX_TIEMPO_COMPRA = 10;
const int TAM_MAX_PILA = 10; // Tamaño máximo de la pila
const int TAM_MAX_COLA= 10; // Tamaño máximo de la cola

int compradores = 0;
int clientes_no_compradores = 0;
int tiempo_excedido = 0;

void cambiarColor(int colorFondo, int colorTexto);
void mostrar_nombre_supermercado();
void encolar(const Clientes& cliente);
void mostrar_clientes();
void menu(int& eleccion);
void iniciarSimulacion();
void CrearNumeroAleatorio();
void AgregarString(char* cosmetico, const char* nuevoString);

//		FUNCION PRINCIPAL
int main()
{
	int eleccion;
	
	//	ENCABEZADO
    cambiarColor(1, 9);
    mostrar_nombre_supermercado();
	
	// SELECCION DEL MENU
    do
    {
        menu(eleccion);
        switch (eleccion)
		{
            case 1: iniciarSimulacion(); 	break;
        }
        system("cls");
    }while(eleccion != 3);
    
    //	MOSTRAR CLIENTES CARGADOS
    mostrar_clientes();
    
    cout << "\n";
    //	MOSTRAR PRODUCTOS CARGADOS
    for(int i=0; i<11; i++)
	{
    	cout<<".- "<<productos[i].descripcion;
    	cout<<" "<<productos[i].precio;
    	cout<<" Stock: "<<productos[i].stock<<endl;
    	cout<<"\n";
	}

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
        {"  |  _  || || '_ \\  / _ \\| '|| |\\/| | / _ \\  / _` |"},
        {"  | | | || || |) ||  _/| |   | |  | || () || (| |"},
        {"  \\| |/||| ./  \\|||   \\|  |/ \\/  \\\\_|"},
        {"            | |                                      "},
        {"            |_|                                      "},
    };
	
	// CAMBIO DE COLOR DEL LOGO
    cambiarColor(0, 13);
    
    
    // CONSEGUIR TAMAÑO DE LA CONSOLA
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    int consoleWidth = consoleInfo.dwSize.X;

    // CALCULO DEL TAMAÑO NECESARIO PARA EL LOGO
    int espaciosEnBlanco = (consoleWidth - 70) / 2;

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

    Sleep(1500);
    system("cls");
}

void cambiarColor(int colorFondo, int colorTexto)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    int colorFinal = colorTexto + colorFondo * 16;
    SetConsoleTextAttribute(hConsole, colorFinal);
}

void AgregarString(char* cosmetico, const char* nuevoString)
{
    strcat(cosmetico, nuevoString);
}

//				GENERAR NUMERO ALEATORIO	PARA EL TIEMPO DE COMPRA
void CrearNumeroAleatorio()
{
    srand(static_cast<unsigned int>(time(nullptr)));
    for (int i = 0; i < 11; i++)
	{
        clientes[i].tiempo_de_compra = rand() % 15 + 1;
        if(clientes[i].tiempo_de_compra > 10)
    	{
    		tiempo_excedido++;
		}
    }
}

//				COLAS
void encolar(const Clientes& cliente)
{
    colaClientes.push(cliente);
}

//				MOSTRAR CLIENTES
void mostrar_clientes()
{ 
	int j,k;
	srand(static_cast<unsigned int>(time(nullptr)));
	for(int m = 0; m < 11; m++)
	{
		for(int n = 0; n < k; n++)
		{
			k = rand() % 30 + 1;
		    j = rand() % 11 + 0;
		    AgregarString(clientes[m].cosmeticos,productos[j].descripcion);
		    AgregarString(clientes[m].cosmeticos,", ");
		    productos[j].stock -1;
			clientes[m].PagoT += productos[j].precio;
			fflush(stdin);
		}
	}
    cout << "Base de datos de los clientes:\n"<<endl;
        // Mostramos los datos del cliente
        for (int i=0; i<11; i++)
		{
		cout << "\n";
        cout << "Nombre: " << clientes[i].nombre << endl;
        cout << "Cedula de identidad: V-" << clientes[i].cv << endl;
        cout << "Dinero disponible: $" << clientes[i].dinero_disponible << endl;
        cout << "Compra: " << clientes[i].cosmeticos << endl;
        cout << "Total a pagar: " << clientes[i].PagoT << endl;
        cout << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min" << endl;
        cout << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << endl;
        cout << "------------------------" << endl;
		}
		cout << "\nCantidad de Clientes que superaron el timepo limite: " << tiempo_excedido << endl;
}


void iniciarSimulacion()
{
	/*		CARGAR PRODUCTOS			*/
	ifstream archivo("productos.txt");
	
    if (archivo.is_open())
	{
        int i = 0;

        while (i < 11 && archivo >> productos[i].id >> productos[i].descripcion >> productos[i].precio >> productos[i].stock)
		{
            i++;
        }
        archivo.close();
    }
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
    }
    fflush(stdin);
    
	ifstream file("clientes.txt");
	
	/*		CARGAR CLIENTES		*/
    if (file.is_open())
	{
        int i = 1;
		CrearNumeroAleatorio();
		
		cout << "\nIngrese su nombre: ";	cin.getline(clientes[0].nombre,40,'\n');
		cout << "\nIngrese su CV: .-";		cin>>clientes[0].cv;
		cout << "\nDigite su Dinero Disponible: $"; cin>>clientes[0].dinero_disponible;
		
        while (i < 11 && file >> clientes[i].nombre >> clientes[i].cv >> clientes[i].dinero_disponible)
		{
            i++;
        }
        file.close();
	}
	else
	{
        cout << "No se pudo abrir el archivo." << endl;
	}
	fflush(stdin);
    
    while (!colaClientes.empty())
	{
        Clientes cliente = colaClientes.front();
        colaClientes.pop();
    }
}

void menu(int& eleccion)
{
    cout << "\tMENU PRINCIPAL: \n";
    cout << "1.-\tIniciar simulacion." << endl;
    cout << "2.-\tAyuda." << endl;
    cout << "3.-\tSalir." << endl;
    cout << "\n-\tDigite su opcion: ";
    cin >> eleccion;
}
