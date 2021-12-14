#include<iostream>
#include"Casino.h"
#include<string.h>
#include<fstream>

using namespace std;

const string nombreArchivo = "PartidaAnterior.txt"; //Para que no cambie el nombre del archivo

////////////////////////////////////////////////Vaciar en Archivo////////////////////////////////////////////////////////////////////////

//Escribe en el archivo la informacion de un jugador 
void vaciarJugador(Jugador *jugador, ofstream &archivo){
    archivo<<jugador->clarezas<<endl;
    archivo<<jugador->idEmparejamiento<<endl;
}
//Escribe en el archivo todo el contenido de una sola carta
void vaciarCarta(Carta carta, ofstream &archivo){ 
    archivo<<carta.valor<<endl;
    archivo<<carta.figura<<endl;
    archivo<<carta.representacion<<endl;
    archivo<<carta.puntaje<<endl;
    archivo<<carta.idEmparejamiento<<endl;
    archivo<<carta.sumaEmparejadas<<endl;
    archivo<<carta.doblada<<endl;
}
//Vacia en el archivo un jugador
void vaciarJugadoresArchivo(Jugador *jugador, bool &ok){
    ofstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::app);
    if(archivo.fail()){
        ok=false;
    } else {
        vaciarJugador(jugador, archivo);
        archivo<<"//"<<endl;                 //Separa cada jugador con un //
        archivo.close();
    }
}
//Vacia un mazo de cartas en el archivo
void vaciarMazo(Nodo *mazo, bool &ok){  
    ofstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::app); //Si no hay archivo, lo crea. 
    if(archivo.fail()){
       ok=false;
    } else {
         while (mazo!=NULL){   
             vaciarCarta(mazo->carta, archivo);
             mazo=mazo->siguiente;
         }
        archivo<<"/"<<endl;                 //Separa cada mazo con un /
        archivo.close();
    }   
}
//Si el archivo ya tenia contenido, lo borra
void borrarContenidoArchivo(bool &ok){ 
    ofstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::trunc);
    if(archivo.fail())
        ok=false;
    archivo.close();
}

//////////////////////////////Funciones a usar para vaciar la informacion/////////////////////////////////////////////////////////////////

//Vacian todas las cartas en el archivo
void vaciarLosMazos(Nodo *&mazo1, Nodo *&mazo2, Nodo *&mazo3, Nodo *&mazo4, Nodo *&mazo5, Nodo *&mazo6, bool &ok){ //Vaciar todo de una vez
    borrarContenidoArchivo(ok);
    vaciarMazo(mazo1, ok);
    vaciarMazo(mazo2, ok);
    vaciarMazo(mazo3, ok);
    vaciarMazo(mazo4, ok);
    vaciarMazo(mazo5, ok);
    vaciarMazo(mazo6, ok);
}
//Vacia toda la informacion de los jugadores en el archivo
void vaciarLosJugadores(Jugador *jugador, Jugador *computadora, bool &ok){
    vaciarJugadoresArchivo(jugador, ok);
    vaciarJugadoresArchivo(computadora, ok);
}

//Vacia las variable global booleana
void vaciarReparte(int reparte, bool &ok){
    ofstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::app); //Si no hay archivo, lo crea. 
    if(archivo.fail()){
       ok=false;
    } else {
        archivo<<reparte<<endl; 
        archivo<<"///"<<endl;                 //Separa cada variable con un ///
        archivo.close();
    }  
}

void vaciarUltimo(Ultimo ultimoRecogerOJugar, bool &ok){
    ofstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::app); //Si no hay archivo, lo crea. 
    if(archivo.fail()){
       ok=false;
    } else {
        archivo<<ultimoRecogerOJugar<<endl; 
        archivo<<"///"<<endl;                 //Separa cada variable con un ///
        archivo.close();
    }  
}

////////////////////////////////////////////////Cargar archivo////////////////////////////////////////////////////////////////////////

//Permite insertar cartas al final de la lista
void insertarAlFinal(Nodo *&lista, Carta carta){
    Nodo *newCarta=new Nodo();
    newCarta->carta=carta;
    Nodo *auxiliar=lista;
    if(lista==NULL)
        insertarCartaEnMazo(lista,carta);
    else {
        while (auxiliar->siguiente!=NULL){
            auxiliar=auxiliar->siguiente;
        }
        newCarta->siguiente=auxiliar->siguiente;
        auxiliar->siguiente=newCarta;
    }
}
//Funcion que permite convertir un texto a su correspondiente enum Ultimo
Ultimo convertirUltimo(Ultimo &ultimoRecogerOJugar, string linea){
    char figura=linea[0];
    switch (figura){
    case '0':
            return Persona;
        break;
    case '1':
            return Computadora;
        break;
    default:
            return Null;
        break;
    }
}

//Funcion que permite convertir un texto a su correspondiente enum Figuras
Figuras convertirFiguras(string linea){
    char figura=linea[0];
    switch (figura){
    case '0':
            return espada;
        break;
    case '1':
            return trebol;
        break;
    case '2':
            return corazon;
        break;
    default:
            return diamante;
        break;
    }
}
//Funcion que construye la carta leyendo linea por linea el archivo
void creandoCarta(string linea, int contador, Nodo *&mazo, Carta &carta){ 
    switch (contador)
    {
    case 1: 
            carta.valor=stoi(linea);
        break;
    case 2:
            carta.figura=convertirFiguras(linea);
        break;
    case 3: 
            carta.representacion=linea[0];
        break;
    case 4: 
            carta.puntaje=stoi(linea);
        break;
    case 5: 
            carta.idEmparejamiento=stoi(linea);
        break;
    case 6: 
            carta.sumaEmparejadas=stoi(linea);
        break;
    case 7: 
        (linea=="0") ? carta.doblada=false : carta.doblada=true; 
        insertarAlFinal(mazo, carta);
        break;
    default :
        break;    
    }
}
//Funcion que carga en el programa todo el contenido de un mazo de cartas
void cargarMazo(Nodo *&mazo, ifstream &archivo){
   int contador=0;
   Carta carta;
   string linea="k"; //Necesario para que lea la linea siguiente en el archivo
    while(!archivo.eof() && linea!="/"){ // El "/" es el simbolo de parada de llenar cartas
        contador++;
        getline(archivo, linea);
        (linea!="/") ? creandoCarta(linea,contador, mazo, carta) : creandoCarta(linea, 10, mazo, carta); 
        if(contador>=7)
            contador=0;
    }
}
//Funcion que crea al jugador leyendo cada linea del archivo
void creandoJugador(Jugador *&jugador, string linea, int contador){
    if(linea=="//") contador=10;
    switch (contador)
    {
    case 1:
        jugador->clarezas=stoi(linea);
        break;
    case 2:
        jugador->idEmparejamiento=stoi(linea);
    default:
        break;
    }
}
//Funcion que carga en el programa todo el contenido de un jugador
void cargarJugador(Jugador *&jugador, ifstream &archivo){
    int contador=0;
    string linea="l";
    jugador=new Jugador();
    while(!archivo.eof() && linea!="//"){
        ++contador;
        getline(archivo, linea);
        if(linea!="//")
            creandoJugador(jugador,linea,contador);
        if(contador>=2)
            contador=0;
    }
}
//Cargar variable reparte
void cargarReparte(int &reparte, ifstream &archivo){
    string linea="l";
    while(!archivo.eof() && linea!="///"){
        getline(archivo, linea);
        (linea=="0") ? reparte=0 : reparte=1;
    }
}

//Cargar enum Ultimo
void cargarUltimo(Ultimo &ultimoRecogerOJugar, ifstream &archivo){
    string linea="l";
    while(!archivo.eof() && linea!="///"){
        getline(archivo, linea);
        convertirUltimo(ultimoRecogerOJugar, linea);
    }
}

//Funcion que carga los jugadores del juego
void cargarVariosJugadores(Jugador *&jugador, Jugador *&computadora, ifstream &archivo){
        cargarJugador(jugador, archivo);
        cargarJugador(computadora, archivo);
}

//Funcion que carga al programa principal todos los mazos necesarios y jugadores para reconstruir la partida anterior
void cargarInformacion(Nodo *&mazo1, Nodo *&mazo2, Nodo *&mazo3, Nodo *&mazo4, Nodo *&mazo5, Nodo *&mazo6, Jugador *&jugador, Jugador *&computadora, int &reparte, Ultimo &ultimoEnRecogerPorEmparejamiento, Ultimo &ultimoEnRealizarJugada){ //Cargar todo de una vez
    ifstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::in);
    if(archivo.fail()){
        cout<<"No existe archivo";
    } else{
        cargarMazo(mazo1, archivo);
        cargarMazo(mazo2, archivo);
        cargarMazo(mazo3, archivo);
        cargarMazo(mazo4, archivo);
        cargarMazo(mazo5, archivo);
        cargarMazo(mazo6, archivo);
        cargarVariosJugadores(jugador,computadora,archivo);
        cargarReparte(reparte, archivo);
        cargarUltimo(ultimoEnRecogerPorEmparejamiento, archivo);
        cargarUltimo(ultimoEnRealizarJugada, archivo);
    }
    archivo.close();
}

//Verifica si el archivo de texto para cargar la partida fue creado
bool existeArchivo(){
    bool existe=false;
    ifstream archivo;
    archivo.open(nombreArchivo.c_str(), ios::in);
    if(archivo.fail())
	    existe=false;
    else
        existe=true;
    return existe;
}

/////////////////////////////////////////////////////////TESTING////////////////////////////////////////////////////////////////////

/*

Carta crearCarta(int valor, Figuras figura, char representacion, int puntaje, int id, int suma, bool doblada){
    Carta carta;
    carta.valor=valor;
    carta.figura=figura;
    carta.representacion=representacion;
    carta.puntaje=puntaje;
    carta.idEmparejamiento=id;
    carta.sumaEmparejadas=suma;
    carta.doblada=doblada;
    return carta;
}

int main(){
    Nodo *mazo=NULL;
    Nodo *mesa=NULL;
    Nodo *mazoJ=NULL;
    Nodo *mazoC=NULL;
    Nodo *mazoRC=NULL;
    Nodo *mazoRJ=NULL;
    Jugador *computadora=NULL;
    Jugador *jugador=NULL;
    bool ok=false;
    bool recogio=false, reparte=false;
    cargarInformacion(mazo,mesa,mazoJ,mazoRJ,mazoC,mazoRC,jugador,computadora,recogio,reparte);
    
    cout<<"Mazo"<<endl;
    imprimirMazo(mazo);
    cout<<"MESA\n"<<endl;
    imprimirMazo(mesa);
    cout<<"Mazo J\n"<<endl;
    imprimirMazo(mazoJ);
    cout<<"Mazo RJ\n"<<endl;
    imprimirMazo(mazoRJ);
    cout<<"Mazo C\n"<<endl;
    imprimirMazo(mazoC);
    cout<<"Mazo RC\n"<<endl;
    imprimirMazo(mazoRC);
    cout<<"Jugador Clarezas"<<jugador->clarezas<<endl;
    cout<<"Jugador id"<<jugador->idEmparejamiento<<endl;
   cout<<"Compu Clarezas"<<computadora->clarezas<<endl;
    cout<<"Compu id"<<computadora->idEmparejamiento<<endl;
    cout<<"Recogio"<<recogio<<endl;
    cout<<"Reparte"<<reparte<<endl;

    return 0;
}


*/


/*

int main(){
    Nodo *mazo=NULL;
    Nodo *mesa=NULL;
    Nodo *mazoJ=NULL;
    Nodo *mazoC=NULL;
    Nodo *mazoRC=NULL;
    Nodo *mazoRJ=NULL;
    Jugador *computadora=new Jugador();
    Jugador *jugador=new Jugador();

    bool ok=false, reparte=true, recogio=false;
    borrarContenidoArchivo(ok);
    barajear(mazo);
    repartirAMesa(mazo, mesa);
    repartirAMesa(mazo, mesa);
    repartirAMesa(mazo, mazoC);
    repartirAMesa(mazo, mazoC);
    repartirAMesa(mazo, mazoJ);
    repartirAMesa(mazo, mazoJ);
    repartirAMesa(mazo, mazoRC);
    repartirAMesa(mazo, mazoRJ);
    computadora->idEmparejamiento=777;
    computadora->clarezas=2;
    jugador->idEmparejamiento=0;
    jugador->clarezas=1;

    cout<<"Mazo"<<endl;
    imprimirMazo(mazo);
    cout<<"MESA\n"<<endl;
    imprimirMazo(mesa);
    cout<<"Mazo J\n"<<endl;
    imprimirMazo(mazoJ);
    cout<<"Mazo RJ\n"<<endl;
    imprimirMazo(mazoRJ);
    cout<<"Mazo C\n"<<endl;
    imprimirMazo(mazoC);
    cout<<"Mazo RC\n"<<endl;
    imprimirMazo(mazoRC);
    vaciarLosMazos(mazo,mesa,mazoJ,mazoRJ,mazoC, mazoRC,ok);
    vaciarLosJugadores(jugador,computadora,ok);
    vaciarTodasVariablesGlobales(reparte, recogio, ok);
    return 0;
} 
*/
