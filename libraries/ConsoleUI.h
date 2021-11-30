#include <iostream>
using namespace std;

//limpiar consola según el sistema operativo
void limpiarConsola(bool isMac){
    (isMac)
        ? system("clear")
        : system("cls");  
};
//El usuario proporciona su sistema operativo para la correcta limpieza de pantalla en el programa
bool seleccionarSistemaOperativo(){
    string sistemaOperativo;
    bool isMac;
    cout << "Si estas usando MacOS ingrese 'mac', si no, ingrese cualquier cosa, para el buen funcionamiento del programa\n";
    cin >> sistemaOperativo;
    (sistemaOperativo.compare("mac") == 0)
        ? isMac = true
        : isMac = false;
    return isMac;
};
//imprimir menu 
void imprimirMenuPrincipal(){
    cout << "1. Jugar partida nueva\n";
    cout << "2. Jugar partida anterior\n"; 
    cout << "3. Salir\n";
    cout << "4. TESTING\n";
    cout << "Escoja una opcion\n";
};
//seleccionar una opcion del menu
char seleccionarOpcion(){
    char opcionSeleccionada;
    cin >> opcionSeleccionada;
    return opcionSeleccionada;
};
//pausar programa
void pausarConsola(){
    string *pause = new string;
    cout << "Escriba una letra para continuar\n";
    cin >> *pause;
    delete pause;
}