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
    {"   _    _  _                      ___  ___              _ "},
    {"  | |  | || |                    |  \\/  |             | |"},
    {"  | || || |  _ __     ___  _ __| .  . |  ___    __ _| |"},
    {"  |  __  || | | '_ \\   / _ \\| '| |\\/| | / _ \\  / _` | |"},
    {"  | |  | || | | |) | |  _/| |  | |  | || () || (| | |"},
    {"  ||  |||| | ./   \\|||  \\|  |/ \\/  \\,||"},
    {"              | |                                        "},
    {"              |_|                                        "},
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
        clientes[i].tiempo_de_compra = rand() % 13 +1 ;
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
 std::ofstream file; // CREAMOS UN OBJETO DE TIPO ofstreamtd::ofstream file; // CREAMOS UN OBJETO DE TIPO ofstream
for (int i=0; i<14; i++)
{
if(clientes[i].tiempo_de_compra < 10)
{
	time_t now = time(0);
    char* dt = ctime(&now);
    
    cout << "\n--------------------------------------------------------------------------------------------------\n"; // Línea de separación
    cout << "FACTURA DE COMPRA\n"; // Título de la factura
    cout << "Fecha y hora: " << dt; // Fecha y hora actual
    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    cout << "Nombre del cliente: " << clientes[i].nombre << "\n";
    cout << "Cedula de identidad: V-" << clientes[i].cv << "\n";
    cout << "Numero de Telefono: +58-" << clientes[i].Nmr << "\n";
    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    cout << "PRODUCTOS COMPRADOS:\n"; // Título de los productos comprados

    file << "\n--------------------------------------------------------------------------------------------------\n"; // Línea de separación
    file << "FACTURA DE COMPRA\n"; // Título de la factura
    file << "Fecha y hora: " << dt; // Fecha y hora actual
    file << "-----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    file << "Nombre del cliente: " << clientes[i].nombre << "\n";
    file << "Cedula de identidad: V-" << clientes[i].cv << "\n";
    file << "Numero de Telefono: +58-" << clientes[i].Nmr << "\n";
    file << "-----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    file << "PRODUCTOS COMPRADOS:\n"; // Título de los productos comprados

    std::set<std::string> productosUnicos; // Para evitar la repetición de productos
    std::stack<std::string> pilaDeProductos; // Pila de productos
    double totalAPagar = 0; // Inicializamos el total a pagar
    while(!clientes[i].Carrito.empty())
    {
        std::string producto = clientes[i].Carrito.top();
        if(productosUnicos.insert(producto).second) // Si el producto es único
        {
            pilaDeProductos.push(producto); // Agregamos el producto a la pila
        }
        clientes[i].Carrito.pop();
    }
    for(int l = 1;!pilaDeProductos.empty();l++)
    {
        int cantidad = rand() % 5 + 1; // Genera un número aleatorio entre 1 y 10
       //Busca el precio en el documento presio 
       double precio = 0.0;
std::ifstream archivo("precios.txt"); // Abre el archivo de precios
std::string linea;

while (std::getline(archivo, linea)) // Lee cada línea del archivo
{
    std::stringstream ss(linea);
    std::string nombreProducto;
    std::getline(ss, nombreProducto, '$'); // Obtiene el nombre del producto de la línea

    if (nombreProducto.find(nombreDelProducto) != std::string::npos) // Si el nombre del producto coincide
    {
        std::string precioStr;
        std::getline(ss, precioStr); // Obtiene el precio del producto
        try {
            precio = std::stod(precioStr); // Intenta convertir el precio a double
        } catch (const std::invalid_argument& ia) {
            std::cerr << "Error: precio invalido para el producto " << nombreProducto << ": " << precioStr << std::endl;
        }
    }
}
        totalAPagar += cantidad * precio; // Calculamos el total a pagar
        cout << l << ". " << pilaDeProductos.top() << " - Cantidad: " << cantidad << "  ----> Precio: $" << precio << "\n";
        file << l << ". " << pilaDeProductos.top() << " - Cantidad: " << cantidad << "  ----> Precio: $" << precio << "\n"; // ESCRIBIMOS EN EL ARCHIVO
        pilaDeProductos.pop();
    }
    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    cout << "Total a pagar: $" << totalAPagar << "\n";
    cout << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min\n";
    cout << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << " min\n";
    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación

    file << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    file << "Total a pagar: $" << totalAPagar << "\n";
    file << "Tiempo de compra: " << clientes[i].tiempo_de_compra << " min\n";
    file << "Tiempo de llegada: " << clientes[i].tiempo_de_llegada << " min\n";
    file << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación

    // Generamos un método de pago aleatorio
    int metodoDePago = rand() % 4;
    switch(metodoDePago)
    {
        case 0:
            cout << "Metodo de pago: Divisas $\n";
            file << "Metodo de pago: Divisas $\n";
            break;
        case 1:
            cout << "Metodo de pago: Pago por Punto de venta\n";
            file << "Metodo de pago: Pago Por Punto de venta\n";
            break;
        case 2:
            cout << "Metodo de pago: Efectivo\n";
            file << "Metodo de pago: Efectivo\n";
            break;
        case 3:
            cout << "Metodo de pago: Pago Movil\n";
            file << "Metodo de pago: Pago Movil\n";
            break;    
            
    }
    cout << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación
    file << "----------------------------------------------------------------------------------------------------\n"; // Línea de separación

    totalVenta += totalAPagar;
    compradores++;
    clientes_presentes++;		    
    esperando_compra(clientes[i].tiempo_de_compra);
    fflush(stdin);
}
	
else
{
    clientes_presentes++;	    	
    cout << "\n";
    cout << "==========================================" << endl;
    cout << "Cliente: "<<clientes[i].nombre 	<< endl;
    cout << "---------------------------------------" << endl;
    cout << "Estado: - Ha excedido el tiempo de compra" << endl;
    cout << "        - No tiene fondos suficientes" 		<< endl;
    cout << "        - Ha salido de la cola." 				<< endl;
    cout << "-------------------------------------------" << endl;
    cout <<          "Gracias por su visita!" << endl;
    cout << "===========================================" << endl;
    Sleep(3000);
    system("cls");
    fflush(stdin);
}
}

file.close(); // CERRAMOS EL ARCHIVO QUE GUARDO DATOS DE FACTURA
	
	//	MUESTRA LA CANTIDAD QUE EXCEDIO EL TIEMPO DE COMPRA
	cout << "=========================================================================" << endl;
	cout << "\n\t-Cantidad de Clientes que superaron el tiempo limite: " << tiempo_excedido << endl;
	cout << "\n\t-Cantidad de Clientes que compraron exitosamente: " 	<< compradores << endl;
	cout << "\n\t-Cantidad de Clientes que se presentaron a comprar: " 	<< clientes_presentes << endl;
	cout << "\n\t-Total de ventas: $"									<< totalVenta<<endl;
	cout<<"\n";
	cout << "=========================================================================" << endl;
	
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
	cout << "\n\t-Cantidad de Clientes que superaron el tiempo limite: " << tiempo_excedido << endl;
	cout << "\n\t-Cantidad de Clientes que compraron exitosamente: " 	<< compradores << endl;
	cout << "\n\t-Cantidad de Clientes que se presentaron a comprar: " 	<< clientes_presentes << endl;
	cout << "\n\t-Total de ventas: $"									<< totalVenta<<endl;
	cout<<"\n";
	cout << "=========================================================================" << endl;

					Sleep(5000);
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
  cout << "\n\tMENU PRINCIPAL:\n"
     << "---------------------------\n"
     << "1.-\tIniciar simulacion.\n"
     << "2.-\tContribuyente.\n"
     << "3.-\tOtros.\n"
     << "4.-\tSalir.\n"
     << "\n-\tDigite su opcion: ";
cin >> eleccion;
}

/*		MENU OPCION (CREDITOS)				*/
void Creditos()
{
system("cls");

    cout << "\nDesarrollado por:\n"
         << "----------------------------------------\n"
         << "-Francisco Fonseca  CI: V-26.359.537\n"
         << "-Manuel Pastrano    CI: V-28.534.779\n"
         << "-Carlos Hernandez   CI: V-27.975.753\n"
         << "-Andres Gomez       CI: V-31.085.717\n"
         << "-Ernesto Balbas     CI: V-30.932.082\n\n";

    //cout << "\nPresiona cualquier tecla para salir. ";
    system("pause");
}

// FUNSION PARA MOSTRAR LAS ULTIMAS FACTURAS GENERADAS
void leerFacturas() {
	system("cls");
    std::string linea;
    std::ifstream archivo("ultimas_facturas.txt");

    if (archivo.is_open()) {
        while (getline(archivo, linea)) {
            std::cout << linea << '\n';
        }
        archivo.close();
    } else {
    	cout << "\n";
        cout << "No se ha facturado ninguna compra" << endl;
    }
}
  

/*		MENU OPCION (OTROS)					*/
void Otros()
{
	system("cls");
	int opcion;
	cout << "\n";
	cout << "1.-\tMostrar Productos cargados." << endl;
	cout << "2.-\tMostrar Clientes en el mercado." << endl;
	cout << "3.-\tMostrar Ultimas facturas generadas." << endl;
	cout << "\n\tDigite una opcion: "; cin >> opcion;
	
	switch(opcion)
	{
		//	MOSTRAR PRODUCTOS CARGADOS
		case 1:mostrar_Productos(); break;
		
		//	MOSTRAR LOS CLIENTES EN LA BASE DE DATOS
		case 2:mostrar_Clientes(); break;
		
		// LEER ULTIMAS FACTURAS
        case 3: leerFacturas(); break;
		
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
