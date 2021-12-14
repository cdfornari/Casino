#include <iostream>
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include "ConsoleUI.h"
using namespace std;

/******************* TIPOS ***********************/
enum Ultimo {Persona,Computadora,Null};

enum Figuras {espada,trebol,corazon,diamante};
typedef struct {
    Figuras figura;
    short int valor;
    char representacion;
    short int puntaje;
    int idEmparejamiento;
    short int sumaEmparejadas;
    bool doblada;
}Carta;

struct Nodo{
    Carta carta;
    Nodo *siguiente;
};

typedef struct {
    Nodo *cartasMazo;
    Nodo *cartasRecogidas;
    int clarezas;
    int idEmparejamiento;
} Jugador;

/******************* HELPERS/FUNCIONES BASICAS ***********************/

Figuras generarFigura(){
    Figuras figura;
    int numero = 1 + rand()%(4);
    switch (numero)
    {
        case 1:
            figura = espada;
        break;
        case 2:
            figura = trebol;
        break;
        case 3:
            figura = corazon;
        break;
        case 4:
            figura = diamante;
        break;
    }
    return figura;
}

Carta generarCarta(){
    Carta carta;
    int numero;
    numero = 1 + rand()%(13);
    carta.valor = numero;
    switch (carta.valor)
    {
        case 1:
            carta.representacion = 'A';
        break;
        case 11:
            carta.representacion = 'J';
        break;
        case 12:
            carta.representacion = 'Q';
        break;
        case 13:
            carta.representacion = 'K';
        break;
        default:
            carta.representacion = '-';
        break;
    }
    carta.figura = generarFigura();
    if(carta.valor == 10 && carta.figura == diamante){
        carta.puntaje = 2;
    }else if((carta.valor == 2 && carta.figura == espada) || (carta.valor == 1)){
        carta.puntaje = 1;
    }else{
        carta.puntaje = 0;
    }
    carta.idEmparejamiento = 0;
    carta.sumaEmparejadas = 0;
    carta.doblada = false;
    return carta;
}

void insertarCartaEnMazo(Nodo *&mazo, Carta carta){
    Nodo *newCarta = new Nodo();
    newCarta -> carta = carta;
    Nodo *auxiliar = mazo;
    mazo = newCarta;
    newCarta -> siguiente = auxiliar;
}

int generarIdEmparejamiento(){
    return rand()%(1000) + 1;
}

int contarCartas(Nodo *&mazo){
    int contador = 0;
    Nodo *auxiliar=mazo;
    while (auxiliar != NULL)
    {
        contador++;
        auxiliar = auxiliar->siguiente;
    }
    return contador;
}

int contarCartasPorValor(Nodo *mazo, int valor){
    int contador = 0;
    while (mazo != NULL)
    {
        if(mazo->carta.valor == valor)
            contador++;
        mazo = mazo->siguiente;
    }
    return contador;
}

void eliminarCartaDeMazo(Nodo *&mazo, Carta carta){
    if(mazo != NULL){
        Nodo *auxiliar = mazo;
        Nodo *anterior = NULL;
        while (auxiliar != NULL && (auxiliar->carta.valor != carta.valor || auxiliar->carta.figura != carta.figura))
        {
            anterior = auxiliar;
            auxiliar = auxiliar -> siguiente;
        }
        if (auxiliar == NULL){
            //no encontro la carta
        } else if (anterior == NULL){ //la carta es la primera del mazo
            mazo = mazo -> siguiente;
            delete auxiliar;
        }else{
            anterior->siguiente = auxiliar->siguiente;
            delete auxiliar;
        }
    }
}

bool cartaExisteEnMazo(Nodo *mazo, Carta carta){
    bool cartaEnMazo = false;
    while(mazo != NULL && cartaEnMazo == false){
        if(mazo->carta.valor == carta.valor && mazo->carta.figura == carta.figura)
            cartaEnMazo = true;
        mazo=mazo->siguiente;
    }
    return cartaEnMazo;    
}

Carta buscarCartaEnMazoPorIdEmparejamiento(Nodo *mazo, int id){
    while(mazo != NULL){
        if(mazo->carta.idEmparejamiento == id)
            return mazo->carta;
        mazo=mazo->siguiente;
    }
    Carta noEncontrada;
    noEncontrada.valor = NULL;
    return noEncontrada;
}

//Busca una carta con mismo valor en un mazo
Carta buscarCarta(Nodo *mazo, Carta cartaABuscar){
    Carta carta;
    while (mazo!=NULL){   
        carta=mazo->carta;
        if(mazo->carta.valor==cartaABuscar.valor)
            return carta;
        mazo=mazo->siguiente;
    }
    carta.valor=-1; //Si no la encuentra, retorna una carta con valor -1.
    return carta;
}

Carta buscarCartaPorPosicion(Nodo *cartasJugador, short int posicionABuscar){
    short int posicion = 1;
    if (posicionABuscar == 1)
        return cartasJugador->carta;
    else
        while (posicion < posicionABuscar && cartasJugador != NULL){
            cartasJugador = cartasJugador->siguiente;
            posicion++;
        }
    return cartasJugador->carta;
}

Carta buscarCartaExacta(Nodo *mazo, Carta cartaAbuscar){
    while(mazo!=NULL){
        if(mazo->carta.valor==cartaAbuscar.valor && mazo->carta.figura==cartaAbuscar.figura)
            return mazo->carta;
        mazo=mazo->siguiente;
    }
}

/******************* FUNCIONES DEL JUEGO ***********************/

void barajear(Nodo *&mazo){
    Carta carta;
    for (int i = 1; i <= 52; i++)
    {
        do
        {
            carta = generarCarta();
        } while (cartaExisteEnMazo(mazo,carta));
        insertarCartaEnMazo(mazo,carta);
    }
}

void repartirCartas(Nodo *&mazo, Nodo *&cartasJugador, Nodo *&cartasComputadora){
    short int *contadorCartas = new short int(0);
    Nodo *auxiliar = mazo;
    while(auxiliar != NULL && *contadorCartas < 4){
        insertarCartaEnMazo(cartasJugador,auxiliar->carta);
        eliminarCartaDeMazo(mazo,auxiliar->carta);
        auxiliar = auxiliar->siguiente;
        insertarCartaEnMazo(cartasComputadora,auxiliar->carta);
        eliminarCartaDeMazo(mazo,auxiliar->carta);
        auxiliar = auxiliar->siguiente;
        (*contadorCartas)++;
    }
    delete contadorCartas;
}

void repartirAMesa(Nodo *&mazo, Nodo *&mesa){
    short int *contadorCartas = new short int(0);
    Nodo *auxiliar = mazo;
    while(auxiliar != NULL && *contadorCartas < 4){
        insertarCartaEnMazo(mesa,auxiliar->carta);
        eliminarCartaDeMazo(mazo,auxiliar->carta);
        auxiliar = auxiliar->siguiente;
        (*contadorCartas)++;
    }
    delete contadorCartas;
}

/******************* PUNTAJE ***********************/

int contarPuntaje(Nodo *cartasRecogidasJugador, Nodo *cartasRecogidasComputadora, int clarezasJugador, int clarezasComputadora){
    int puntajeJugador = clarezasJugador;
    int contadorCartasJugador = 0;
    int contadorEspadasJugador = 0;
    while (cartasRecogidasJugador != NULL){
        contadorCartasJugador++;
        puntajeJugador = puntajeJugador + cartasRecogidasJugador->carta.puntaje;
        if (cartasRecogidasJugador->carta.figura == espada)
            contadorEspadasJugador++;
        cartasRecogidasJugador = cartasRecogidasJugador->siguiente;
    }
    int puntajeComputadora = clarezasComputadora;
    int contadorCartasComputadora = 0;
    int contadorEspadasComputadora = 0;
    while (cartasRecogidasComputadora != NULL){
        contadorCartasComputadora++;
        puntajeComputadora = puntajeComputadora + cartasRecogidasComputadora->carta.puntaje;
        if (cartasRecogidasComputadora->carta.figura == espada)
            contadorEspadasComputadora++;
        cartasRecogidasComputadora = cartasRecogidasComputadora->siguiente;
    }
    if(contadorCartasJugador == contadorCartasComputadora){
        if(contadorEspadasJugador > contadorEspadasComputadora)
            puntajeJugador = puntajeJugador + 3;
        else
            puntajeComputadora = puntajeComputadora + 3;
    }else{
        if (contadorCartasJugador > contadorCartasComputadora)
            puntajeJugador = puntajeJugador + 3;
        else
            puntajeComputadora = puntajeComputadora + 3;
        if(contadorEspadasJugador > contadorEspadasComputadora)
            puntajeJugador++;
        else
            puntajeComputadora++;
    }
    if(puntajeJugador == puntajeComputadora)
        cout << "EMPATE\n";
    else
        if(puntajeJugador > puntajeComputadora)
            cout << "GANO JUGADOR\n";
        else
            cout << "GANO COMPUTADORA\n";
}

void asignarCartasSobrantes(Nodo *&mesa, Jugador &jugador, Jugador &computadora, Ultimo ultimoEnRecogerPorEmparejamiento){
    if(mesa != NULL)
        switch (ultimoEnRecogerPorEmparejamiento)
        {
            case Persona:
                while (mesa != NULL)
                {
                    insertarCartaEnMazo(jugador.cartasRecogidas,mesa->carta);
                    eliminarCartaDeMazo(mesa,mesa->carta);
                }
            break;
            case Computadora:
                while (mesa != NULL)
                {
                    insertarCartaEnMazo(computadora.cartasRecogidas,mesa->carta);
                    eliminarCartaDeMazo(mesa,mesa->carta);
                }
            break;
            default:
                while (mesa != NULL && contarCartas(mesa) > 1)
                {
                    insertarCartaEnMazo(jugador.cartasRecogidas,mesa->carta);
                    eliminarCartaDeMazo(mesa,mesa->carta);
                    insertarCartaEnMazo(computadora.cartasRecogidas,mesa->carta);
                    eliminarCartaDeMazo(mesa,mesa->carta);
                }
                if(contarCartas(mesa) == 1)
                    if (rand()%2 == 0){
                        insertarCartaEnMazo(jugador.cartasRecogidas,mesa->carta);
                        eliminarCartaDeMazo(mesa,mesa->carta);
                    }else{
                        insertarCartaEnMazo(computadora.cartasRecogidas,mesa->carta);
                        eliminarCartaDeMazo(mesa,mesa->carta);
                    }
            break;
        }
}

/******************* IMPRIMIR ***********************/

void imprimirFigura (Carta carta){
    switch (carta.figura)
    {
        case espada:
            printf("%c",6);
        break;
        case trebol:
            printf("%c",5);
        break;
        case corazon:
            printf("%c",3);
        break;
        case diamante:
            printf("%c",4);
        break;
    }
}

//Funcion que imprime una sola carta
void imprimirCarta(Carta carta){
    if(carta.representacion == '-'){
        
        cout << carta.valor << setw(4);
        imprimirFigura(carta);
    }else{
        
        cout << carta.representacion << setw(4);
        imprimirFigura(carta);
    }
}

void imprimirMazo(Nodo *mazo){
    while(mazo != NULL){
        imprimirCarta(mazo->carta);
        mazo = mazo->siguiente; 
    }
    cout << endl;
}

void imprimirEmparejamientos(Nodo *mesa){
    Nodo *emparejamiento1 = NULL;
    int id1 = 0;
    Nodo *emparejamiento2 = NULL;
    int id2 = 0;
    while (mesa != NULL)
    {
        if(mesa->carta.idEmparejamiento != 0){
            if(id1 == 0){
                id1 = mesa->carta.idEmparejamiento;
                insertarCartaEnMazo(emparejamiento1,mesa->carta);
            }else if (mesa->carta.idEmparejamiento == id1){
                insertarCartaEnMazo(emparejamiento1,mesa->carta);
            }else if(id2 == 0){
                id2 = mesa->carta.idEmparejamiento;
                insertarCartaEnMazo(emparejamiento2,mesa->carta);
            }else if (mesa->carta.idEmparejamiento == id2){
                insertarCartaEnMazo(emparejamiento2,mesa->carta);
            }
        }
        mesa = mesa->siguiente;
    }
    if (id1 == 0 && id2 == 0)
    {
        cout << "No hay emparejamientos\n";
    }else if (id1 != 0)
    {
        cout << "Emparejamiento 1:\n";
        imprimirMazo(emparejamiento1);
    }else if (id1 != 0 && id2 != 0)
    {
        cout << "Emparejamiento 1:\n";
        imprimirMazo(emparejamiento1);
        cout << "Emparejamiento 2:\n";
        imprimirMazo(emparejamiento2);
    }
}

void imprimirCartasPorPosicion(Nodo *cartasJugador){
    int *contador = new int(1);
    while(cartasJugador != NULL){
        cout << *contador << "." << setw(1);
        imprimirCarta(cartasJugador->carta);
        cartasJugador = cartasJugador->siguiente;
        (*contador)++; 
    }
    delete contador;
}

/******************* ACCIONES Y VALIDACIONES JUGADOR ***********************/

void lanzarCartaEnMesa(Nodo *&mesa, Nodo *&mazoJugador, Carta carta){
    insertarCartaEnMazo(mesa,carta);
    eliminarCartaDeMazo(mazoJugador,carta);
}

void recogerCartaDeMesa(Nodo *&mesa, Nodo *&recogidas,Nodo *&mazoJugador, Carta cartaRecogiendo, Carta cartaQueSeRecoge){
    if(cartaQueSeRecoge.idEmparejamiento == 0){
        insertarCartaEnMazo(recogidas,cartaQueSeRecoge);
        eliminarCartaDeMazo(mesa,cartaQueSeRecoge);
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
    }else{
        insertarCartaEnMazo(recogidas,cartaQueSeRecoge);
        eliminarCartaDeMazo(mesa,cartaQueSeRecoge);
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
        int *contador = new int(0);
        Carta carta; 
        while (buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaQueSeRecoge.idEmparejamiento).valor != NULL)
        {
            carta = buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaQueSeRecoge.idEmparejamiento);
            insertarCartaEnMazo(recogidas,carta);
            eliminarCartaDeMazo(mesa,carta);
            (*contador)++;
        }
        delete contador;
    }  
}

bool cartaPuedeRecogerse(Carta cartaConLaQueSeRecoge, Carta cartaQueSeRecoge, Nodo *mesa, Jugador jugador, bool mostrarMensajeDeError){
    if(cartaQueSeRecoge.idEmparejamiento != 0){
        if (cartaConLaQueSeRecoge.valor == cartaQueSeRecoge.sumaEmparejadas)
            return true;
        else{
            if(mostrarMensajeDeError)
                cout << "No se puede recoger. El valor de la carta no es igual a la suma del emparejamiento" << endl;
            return false;
        }
    }else{
        if (cartaConLaQueSeRecoge.valor == cartaQueSeRecoge.valor)
            return true;
        else{
            if(mostrarMensajeDeError)
                cout << "No se puede recoger. El valor de las cartas no es igual" << endl;
            return false;
        }
    }
    if (jugador.idEmparejamiento != 0 && cartaQueSeRecoge.idEmparejamiento != jugador.idEmparejamiento)
    {
        Carta cartaABuscar;
        cartaABuscar.valor = buscarCartaEnMazoPorIdEmparejamiento(mesa,jugador.idEmparejamiento).sumaEmparejadas;
        if(cartaConLaQueSeRecoge.valor == cartaABuscar.valor){
            if(contarCartasPorValor(jugador.cartasMazo,cartaABuscar.valor) < 2){
                if(mostrarMensajeDeError)
                    cout << "No puedes recoger porque no tienes otra carta con la que recoger tu emparejamiento activo" << endl;
                return false;
            }
        }
    }  
    return true;
}

void recogerVariasCartasDeMesa(Nodo *&mesa, Nodo *&recogidas,Nodo *&mazoJugador, Carta cartaRecogiendo, Nodo *&cartasQueSeRecogen){
    insertarCartaEnMazo(recogidas,cartaRecogiendo);
    eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
    while (cartasQueSeRecogen != NULL)
    {
        insertarCartaEnMazo(recogidas,cartasQueSeRecogen->carta);
        eliminarCartaDeMazo(mesa,cartasQueSeRecogen->carta);
    }
}

bool multiplesCartasPuedenRecogerse(Carta cartaConLaQueSeRecoge, Nodo *cartasQueSeRecogen, Nodo *mesa, Jugador jugador, bool mostrarMensajeDeError){
    Nodo *auxiliar = cartasQueSeRecogen;
    bool figuraEnSeleccion = false;
    while(auxiliar != NULL){
        if(auxiliar->carta.valor > 10){
            figuraEnSeleccion = true;
            break;
        }
        auxiliar=auxiliar->siguiente;
    }
    if(figuraEnSeleccion)
    {
        if(cartaConLaQueSeRecoge.valor <= 10){
            if(mostrarMensajeDeError)
                cout << "No puedes recoger figuras con cartas numericas" << endl;
            return false;
        }
        //Verificar que solo se hayan seleccionado figuras
        auxiliar = cartasQueSeRecogen;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor <= 10){
                if(mostrarMensajeDeError)
                    cout << "No puedes recoger cartas numericas con figuras" << endl;
                return false;
            }
        }
        //Verificar que sean la misma figura
        auxiliar = cartasQueSeRecogen;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor != cartaConLaQueSeRecoge.valor){
                if(mostrarMensajeDeError)
                    cout << "Solo se pueden recoger figuras iguales" << endl;
                return false;
            }
        }
        if(contarCartas(cartasQueSeRecogen) == 2){ //2 + la del mazo del jugador
            if(mostrarMensajeDeError)
                cout << "No se puede recoger 3 cartas de una figura" << endl;
            return false;
        }
        return true;
    }
    else
    {
        if(cartaConLaQueSeRecoge.valor > 10){
            if(mostrarMensajeDeError)
                cout << "No puedes recoger cartas numericas con figuras" << endl;
            return false;
        }
        //Verificar si se seleccionaron emparejamientos
        auxiliar = cartasQueSeRecogen;
        int id1 = 0;
        int id2 = 0;
        while (auxiliar != NULL)
        {
            if (auxiliar->carta.idEmparejamiento != 0)
            {
                if (id1 == 0){
                    id1 = auxiliar->carta.idEmparejamiento;
                }else if (id1 != auxiliar->carta.idEmparejamiento && id2 == 0){
                    id2 = auxiliar->carta.idEmparejamiento;
                }
            }
            auxiliar = auxiliar->siguiente;
        }
        if (id1 == 0 && id2 == 0){ //Niguna carta seleccionada esta emparejada
            auxiliar = cartasQueSeRecogen;
            int sumaQueSeRecoge = 0;
            while (auxiliar != NULL)
            {
                sumaQueSeRecoge = sumaQueSeRecoge + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if(sumaQueSeRecoge > 10){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            if (sumaQueSeRecoge != cartaConLaQueSeRecoge.valor){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas no coincide con el valor de la carta usada para recoger" << endl;
                return false;
            }
            if (jugador.idEmparejamiento != 0)
            {
                Carta cartaABuscar;
                cartaABuscar.valor = buscarCartaEnMazoPorIdEmparejamiento(mesa,jugador.idEmparejamiento).sumaEmparejadas;
                if(cartaConLaQueSeRecoge.valor == cartaABuscar.valor){
                    if(contarCartasPorValor(jugador.cartasMazo,cartaABuscar.valor) < 2){
                        if(mostrarMensajeDeError)
                            cout << "No puedes recoger porque no tienes otra carta con la que recoger tu emparejamiento activo" << endl;
                        return false;
                    }
                }
            } 
            return true;
        }else if(id1 != 0 && id2 != 0 && id1 != id2){
            if(mostrarMensajeDeError)
                cout << "No puedes recoger. Hay cartas de emparejamientos distintos" << endl;
            return false;
        }else{
            //determinar la suma que se espera del emparejamiento que pertenece a la seleccion
            int sumaEsperada;
            auxiliar = cartasQueSeRecogen;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento != 0){
                    sumaEsperada = auxiliar->carta.sumaEmparejadas;
                    if (auxiliar->carta.doblada) //si esta doblado multiplicar con 2 para ver si la carta que se uso para doblar esta seleccionada
                        sumaEsperada = sumaEsperada * 2;  
                    break;
                }
                auxiliar = auxiliar->siguiente;
            }
            //Determinar suma real del emparejamiento que pertenece a la seleccion
            auxiliar = cartasQueSeRecogen;
            int sumaEmparejadas = 0;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento != 0)
                    sumaEmparejadas = sumaEmparejadas + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if (sumaEmparejadas != sumaEsperada){
                if(mostrarMensajeDeError)
                    cout << "No estan seleccionadas todas las cartas de un emparejamiento" << endl;
                return false;
            }
            //determinar suma total de la seleccion
            int sumaTotal = sumaEmparejadas;
            auxiliar = cartasQueSeRecogen;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento != 0){
                    if (auxiliar->carta.doblada) //si esta doblado dividir 2 para ver tener valor real del emparejamiento
                        sumaTotal = sumaTotal / 2;
                    break;
                }
                auxiliar = auxiliar->siguiente;
            }
            //Al valor acumulado del emparejamiento sumarle el valor de las cartas seleccionadas que no estan emparejadas
            auxiliar = cartasQueSeRecogen;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento == 0)
                    sumaTotal = sumaTotal + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if(sumaTotal > 10){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            if (sumaTotal != cartaConLaQueSeRecoge.valor){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas no coincide con el valor de la carta usada para recoger" << endl;
                return false;
            }
            if (jugador.idEmparejamiento != 0 && id1 != jugador.idEmparejamiento)
            {
                Carta cartaABuscar;
                cartaABuscar.valor = buscarCartaEnMazoPorIdEmparejamiento(mesa,jugador.idEmparejamiento).sumaEmparejadas;
                if(cartaConLaQueSeRecoge.valor == cartaABuscar.valor){
                    if(contarCartasPorValor(jugador.cartasMazo,cartaABuscar.valor) < 2){
                        if(mostrarMensajeDeError)
                            cout << "No puedes recoger porque no tienes otra carta con la que recoger tu emparejamiento activo" << endl;
                        return false;
                    }
                }
            } 
            return true;
        }
    }
    
}

void emparejarCarta(Nodo *&mesa, Nodo *&mazoJugador, Carta cartaAEmparejar, Carta cartaConLaQueSeEmpareja){
    insertarCartaEnMazo(mesa,cartaAEmparejar);
    eliminarCartaDeMazo(mazoJugador,cartaAEmparejar);
    Nodo *auxiliar = mesa;
    if(cartaConLaQueSeEmpareja.idEmparejamiento == 0){
        int id = generarIdEmparejamiento();
        while (auxiliar != NULL){
            if ((auxiliar->carta.valor == cartaAEmparejar.valor && auxiliar->carta.figura == cartaAEmparejar.figura) || (auxiliar->carta.valor == cartaConLaQueSeEmpareja.valor && auxiliar->carta.figura == cartaConLaQueSeEmpareja.figura)){
                auxiliar->carta.idEmparejamiento = id;
                auxiliar->carta.sumaEmparejadas = cartaConLaQueSeEmpareja.valor + auxiliar->carta.valor;  
            }    
            auxiliar = auxiliar->siguiente;
        }
    }else{
        while (auxiliar != NULL){
            if (auxiliar->carta.valor == cartaAEmparejar.valor && auxiliar->carta.figura == cartaAEmparejar.figura){ //si es la carta a emparejar
                auxiliar->carta.idEmparejamiento = cartaConLaQueSeEmpareja.idEmparejamiento;
                auxiliar->carta.sumaEmparejadas = cartaConLaQueSeEmpareja.sumaEmparejadas + auxiliar->carta.valor;    //suma = su valor + suma emparejamiento
            }else if (auxiliar->carta.idEmparejamiento == cartaConLaQueSeEmpareja.idEmparejamiento){  //si forma parte del emparejamiento
                auxiliar->carta.sumaEmparejadas = cartaAEmparejar.valor + auxiliar->carta.sumaEmparejadas;  //suma = su suma emparejada + valor carta nueva
            }
            auxiliar = auxiliar->siguiente;
        }
    }
}

bool cartaPuedeEmparejarse(Jugador jugador, Carta cartaAEmparejar, Carta cartaConLaQueSeEmpareja, Nodo *mesa, bool mostrarMensajeDeError){
    if (cartaAEmparejar.valor > 10 || cartaConLaQueSeEmpareja.valor > 10){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar 2 cartas J, Q o K" << endl;
        return false;
    }
    if(jugador.idEmparejamiento != 0){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar, ya tienes un emparejamiento activo" << endl;
        return false;
    }
    if(cartaConLaQueSeEmpareja.doblada){
        if(mostrarMensajeDeError)
            cout << "No puedes sumar al emparejamiento. Esta protegido" << endl;
        return false;
    }
    if(cartaAEmparejar.valor + cartaConLaQueSeEmpareja.sumaEmparejadas > 10 || cartaAEmparejar.valor + cartaConLaQueSeEmpareja.valor > 10){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar, la suma del emparejamiento pasaria de 10" << endl;
        return false;
    }
    Carta cartaABuscar;
    if(cartaConLaQueSeEmpareja.idEmparejamiento == 0)
        cartaABuscar.valor = cartaAEmparejar.valor + cartaConLaQueSeEmpareja.valor;
    else
        cartaABuscar.valor = cartaAEmparejar.valor + cartaConLaQueSeEmpareja.sumaEmparejadas;
    if(buscarCarta(jugador.cartasMazo,cartaABuscar).valor == -1){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar porque no tienes una carta con la que recoger el emparejamiento" << endl;
        return false;
    }
    return true;
}

void emparejarMultiplesCartas(Nodo *&mesa, Nodo *&mazoJugador, Carta cartaAEmparejar, Nodo *&cartasQueSeEmparejan){
    insertarCartaEnMazo(mesa,cartaAEmparejar);
    eliminarCartaDeMazo(mazoJugador,cartaAEmparejar);
    Nodo *auxiliar = mesa;
    int idExistente = 0;
    while(auxiliar != NULL){
        if(auxiliar->carta.idEmparejamiento != 0){
            idExistente = auxiliar->carta.idEmparejamiento;
            break;
        }
    }
    int sumaEmparejadas = cartaAEmparejar.valor;
    auxiliar = cartasQueSeEmparejan;
    while(auxiliar != NULL)
        sumaEmparejadas = sumaEmparejadas + auxiliar->carta.valor;
    int id;
    if (idExistente == 0)
        id = generarIdEmparejamiento();
    else
        id = idExistente;
    auxiliar = mesa;
    while (auxiliar != NULL){
        if ((auxiliar->carta.valor == cartaAEmparejar.valor && auxiliar->carta.figura == cartaAEmparejar.figura)){
            auxiliar->carta.idEmparejamiento = id;
            auxiliar->carta.sumaEmparejadas = sumaEmparejadas;  
        }    
        auxiliar = auxiliar->siguiente;
    }
    auxiliar = mesa;
    while (auxiliar != NULL)
    {
        if(cartaExisteEnMazo(cartasQueSeEmparejan,auxiliar->carta) || (auxiliar->carta.valor == cartaAEmparejar.valor && auxiliar->carta.figura == cartaAEmparejar.figura)){
            auxiliar->carta.idEmparejamiento = id;
            auxiliar->carta.sumaEmparejadas = sumaEmparejadas;
        }
        auxiliar = auxiliar->siguiente;
    }
}

bool multiplesCartasPuedenEmparejarse(Carta cartaAEmparejar, Nodo *cartasQueSeEmparejan, Jugador jugador, bool mostrarMensajeDeError){
    if(jugador.idEmparejamiento != 0){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar, ya tienes un emparejamiento activo" << endl;
        return false;
    }
    Nodo *auxiliar = cartasQueSeEmparejan;
    bool figuraEnSeleccion = false;
    while(auxiliar != NULL){
        if(auxiliar->carta.valor > 10){
            figuraEnSeleccion = true;
            break;
        }
        auxiliar=auxiliar->siguiente;
    }
    if(figuraEnSeleccion)
    {
        if(cartaAEmparejar.valor <= 10){
            if(mostrarMensajeDeError)
                cout << "No puedes emparejar cartas numericas con figuras" << endl;
            return false;
        }
        //Verificar que solo se hayan seleccionado figuras
        auxiliar = cartasQueSeEmparejan;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor <= 10){
                if(mostrarMensajeDeError)
                    cout << "No puedes emparejar figuras con cartas numericas" << endl;
                return false;
            }
        }
        //Verificar que sean la misma figura
        auxiliar = cartasQueSeEmparejan;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor != cartaAEmparejar.valor){
                if(mostrarMensajeDeError)
                    cout << "Solo se pueden recoger figuras iguales" << endl;
                return false;
            }
        }
        if(contarCartas(cartasQueSeEmparejan) == 3){ //3 + la del mazo del jugador
            if(mostrarMensajeDeError)
                cout << "No se puede emparejar 4 cartas de una figura." << endl << "Consejo: prueba recogiendolas" << endl;
            return false;
        }
        if(contarCartasPorValor(jugador.cartasMazo,cartaAEmparejar.valor) < 2){
            if(mostrarMensajeDeError)
                cout << "No puedes emparejar porque no tienes otra carta de la misma figura para recoger el emparejamiento" << endl;
            return false;
        }
        return true;
    }
    else
    {
        if(cartaAEmparejar.valor > 10){
            if(mostrarMensajeDeError)
                cout << "No puedes emparejar cartas numericas con figuras" << endl;
            return false;
        }
        //Verificar si se seleccionaron emparejamientos
        auxiliar = cartasQueSeEmparejan;
        int id1 = 0;
        int id2 = 0;
        while (auxiliar != NULL)
        {
            if (auxiliar->carta.idEmparejamiento != 0)
            {
                if(auxiliar->carta.doblada){
                    if(mostrarMensajeDeError)
                        cout << "Seleccionaste un emparejamiento protegido, no puedes sumar" << endl;
                    return false;
                }
                if (id1 == 0){
                    id1 = auxiliar->carta.idEmparejamiento;
                }else if (id1 != auxiliar->carta.idEmparejamiento && id2 == 0){
                    id2 = auxiliar->carta.idEmparejamiento;
                }
            }
            auxiliar = auxiliar->siguiente;
        }
        if (id1 == 0 && id2 == 0){ //Niguna carta seleccionada esta emparejada
            auxiliar = cartasQueSeEmparejan;
            int sumaQueSeEmpareja = cartaAEmparejar.valor;
            while (auxiliar != NULL)
            {
                sumaQueSeEmpareja = sumaQueSeEmpareja + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if(sumaQueSeEmpareja > 10){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            return true;
        }else if(id1 != 0 && id2 != 0 && id1 != id2){
            if(mostrarMensajeDeError)
                cout << "No puedes emparejar. Hay cartas de emparejamientos distintos" << endl;
            return false;
        }else{
            //determinar la suma que se espera del emparejamiento que pertenece a la seleccion
            int sumaEsperada;
            auxiliar = cartasQueSeEmparejan;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento != 0){
                    sumaEsperada = auxiliar->carta.sumaEmparejadas;
                    break;
                }
                auxiliar = auxiliar->siguiente;
            }
            //Determinar suma real del emparejamiento que pertenece a la seleccion
            auxiliar = cartasQueSeEmparejan;
            int sumaEmparejadas = 0;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento != 0)
                    sumaEmparejadas = sumaEmparejadas + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if (sumaEmparejadas != sumaEsperada){
                if(mostrarMensajeDeError)
                    cout << "No estan seleccionadas todas las cartas de un emparejamiento" << endl;
                return false;
            }
            //determinar suma total de la seleccion
            int sumaTotal = sumaEmparejadas;
            auxiliar = cartasQueSeEmparejan;
            //Al valor acumulado del emparejamiento sumarle el valor de las cartas seleccionadas que no estan emparejadas
            auxiliar = cartasQueSeEmparejan;
            while (auxiliar != NULL)
            {
                if(auxiliar->carta.idEmparejamiento == 0)
                    sumaTotal = sumaTotal + auxiliar->carta.valor;
                auxiliar = auxiliar->siguiente;
            }
            if(sumaTotal > 10){
                if(mostrarMensajeDeError)
                    cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            Carta cartaABuscar;
            cartaABuscar.valor = sumaTotal;
            if(buscarCarta(jugador.cartasMazo,cartaABuscar).valor == -1){
                if(mostrarMensajeDeError)
                    cout << "No puedes emparejar porque no tienes una carta con la que recoger el emparejamiento" << endl;
                return false;
            }
            return true;
        }
    }
}

void doblarCarta(Nodo *&mesa, Nodo *&mazoJugador, Carta cartaADoblar, Carta cartaConLaQueSeDobla){
    insertarCartaEnMazo(mesa,cartaADoblar);
    eliminarCartaDeMazo(mazoJugador,cartaADoblar);
    Nodo *auxiliar = mesa;
    if(cartaConLaQueSeDobla.idEmparejamiento == 0)
    {
        int id = generarIdEmparejamiento();
        while (auxiliar != NULL){
            if (auxiliar->carta.valor == cartaADoblar.valor && auxiliar->carta.figura == cartaADoblar.figura){ //si es la carta que se usa para doblar
                auxiliar->carta.doblada = true;
                auxiliar->carta.idEmparejamiento = id;
                auxiliar->carta.sumaEmparejadas = cartaConLaQueSeDobla.valor;
            }else if (auxiliar->carta.valor == cartaConLaQueSeDobla.valor && auxiliar->carta.figura == cartaConLaQueSeDobla.figura){  //si es la carta que se va a doblar
                auxiliar->carta.doblada = true; 
                auxiliar->carta.idEmparejamiento = id;
                auxiliar->carta.sumaEmparejadas = cartaConLaQueSeDobla.valor;
            }
            auxiliar = auxiliar->siguiente;
        }
    }
    else
    {
        while (auxiliar != NULL){
            if (auxiliar->carta.valor == cartaADoblar.valor && auxiliar->carta.figura == cartaADoblar.figura){ //si es la carta a doblar
                auxiliar->carta.doblada = true;
                auxiliar->carta.idEmparejamiento = cartaConLaQueSeDobla.idEmparejamiento;
                auxiliar->carta.sumaEmparejadas = cartaConLaQueSeDobla.sumaEmparejadas;
            }else if (auxiliar->carta.idEmparejamiento == cartaConLaQueSeDobla.idEmparejamiento){  //si forma parte del emparejamiento
                auxiliar->carta.doblada = true; 
            }
            auxiliar = auxiliar->siguiente;
        }
    }
}

bool cartaPuedeDoblarse(Jugador jugador, Carta cartaADoblar, Carta cartaConLaQueSeDobla, Nodo *mesa, bool mostrarMensajeDeError){
    Carta cartaABuscar;
    if(cartaConLaQueSeDobla.idEmparejamiento == 0){
        if(cartaADoblar.valor != cartaConLaQueSeDobla.valor){
            if(mostrarMensajeDeError)
                cout << "Para doblar con otra carta que no esta emparejada estas deben tener el mismo valor" << endl;
            return false;
        }else
            cartaABuscar.valor = cartaConLaQueSeDobla.valor;
    }else
        if(cartaADoblar.valor != cartaConLaQueSeDobla.sumaEmparejadas){
            if(mostrarMensajeDeError)
                cout << "No puedes doblar. La carta no vale igual que la suma del emparejamiento" << endl;
            return false;
        }else
            cartaABuscar.valor = cartaConLaQueSeDobla.sumaEmparejadas;
    if(contarCartasPorValor(jugador.cartasMazo,cartaABuscar.valor) < 2){
        if(mostrarMensajeDeError)
            cout << "No puedes emparejar porque no tienes otra carta con la que recoger el emparejamiento doblado" << endl;
        return false;
    }
    return true;
}

/******************* SELECCIONAR MOVIMIENTO JUGADOR ***********************/

short int seleccionarCartaPorPosicion(Nodo *cartas, short int contadorCartas){
    short int posicionSeleccionada;
    do
    {
        imprimirCartasPorPosicion(cartas);
        cout << endl;
        cin >> posicionSeleccionada;
        while(cin.fail()){
            cout << "Error - Vuelva a ingresar posicion (numero)" << endl;
            cin.clear();
            cin.ignore(256,'\n');
            cin >> posicionSeleccionada;
        }
        if (posicionSeleccionada < 1 || posicionSeleccionada > contadorCartas){
            cout << "Selecciona una posicion correcta" << endl;
            pausarConsola();
        }
    } while (posicionSeleccionada < 1 || posicionSeleccionada > contadorCartas);
    return posicionSeleccionada;
}

void seleccionarMovimiento(Jugador *jugador, Jugador *computadora, Nodo *&mesa, Nodo *&cartasJugador, Nodo *&recogidasJugador, short int contadorCartasJugador, short int *contadorCartasMesa, bool isMac, Ultimo *ultimoEnRecogerPorEmparejamiento){
    char *opcionSeleccionada = new char;
    bool *movimientoValido= new bool(false);
    short int *posicionCartaDeMesa = new short int;
    Carta *cartaMesaSeleccionada = new Carta;
    int cantidadCartasRecoger;
    int cantidadCartasEmparejar;
    short int posicionCartaSeleccionada;
    Carta cartaSeleccionada;
    Nodo *cartasParaRecoger;
    Nodo *cartasParaEmparejar;
    do
    {
        //limpiarConsola(isMac);
        cout << "Tus cartas:\n";
        imprimirMazo(cartasJugador);
        cout << "Cartas en la mesa:\n";
        imprimirMazo(mesa);
        imprimirEmparejamientos(mesa);
        cout << "Que jugada quiere realizar?" << endl;
        cout << "1. Lanzar carta en mesa" << endl;
        cout << "2. Emparejar una o varias cartas" << endl;
        cout << "3. Recoger una carta o varias cartas o un emparejamiento" << endl;
        cout << "4. Doblar o proteger carta o emparejamiento" << endl;
        *opcionSeleccionada = seleccionarOpcion();
        switch (*opcionSeleccionada)
        {
            case '1':
                if(jugador->idEmparejamiento != 0)
                    cout << "Debes recoger tu emparejamiento antes de lanzar una carta" << endl;
                else{
                    cout << "Que carta desea lanzar?" << endl;
                    posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                    cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                    lanzarCartaEnMesa(mesa, cartasJugador, cartaSeleccionada);
                    (*contadorCartasMesa)++;
                    *movimientoValido = true;
                }
            break;
            case '2':
                cout << "Que carta desea usar para emparejar?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                cout << "Cuantas cartas desea emparejar?" << endl;
                do
                {
                    cin >> cantidadCartasEmparejar;
                    while(cin.fail()) {
                        cout << "Error - Vuelva a ingresar numero" << endl;
                        cin.clear();
                        cin.ignore(256,'\n');
                        cin >> cantidadCartasEmparejar;
                    }
                } while (cantidadCartasEmparejar < 1 || cantidadCartasEmparejar > *contadorCartasMesa);
                if(cantidadCartasEmparejar == 1)
                {
                    cout << "Con que carta desea emparejar?" << endl;
                    *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                    *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                    if (cartaPuedeEmparejarse(*jugador,cartaSeleccionada,*cartaMesaSeleccionada,mesa,true))
                    {
                        if((*cartaMesaSeleccionada).idEmparejamiento != 0 && (*cartaMesaSeleccionada).idEmparejamiento == (*computadora).idEmparejamiento)
                            computadora->idEmparejamiento = 0;
                        jugador->idEmparejamiento = cartaMesaSeleccionada->idEmparejamiento;
                        emparejarCarta(mesa,cartasJugador,cartaSeleccionada,*cartaMesaSeleccionada);
                        *contadorCartasMesa = contarCartas(mesa);
                        *movimientoValido = true;
                    }
                }
                else
                {
                    cout << "Con que cartas desea emparejar?" << endl;
                    for (int i = 0; i < cantidadCartasEmparejar; i++)
                    {
                        cout << "Seleccionadas: " << endl;
                        imprimirMazo(cartasParaEmparejar);
                        do
                        {
                            *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                            *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                            if(cartaExisteEnMazo(cartasParaEmparejar,*cartaMesaSeleccionada))
                                cout << "Ya seleccionaste esa carta, selecciona otra" << endl;
                        } while (cartaExisteEnMazo(cartasParaEmparejar,*cartaMesaSeleccionada));
                        insertarCartaEnMazo(cartasParaEmparejar,*cartaMesaSeleccionada);
                    }
                    if (multiplesCartasPuedenEmparejarse(cartaSeleccionada,cartasParaEmparejar,*jugador,true))
                    {
                        Nodo *auxiliar = cartasParaEmparejar;
                        int id;
                        while (auxiliar != NULL)
                        {
                            if(auxiliar->carta.idEmparejamiento != 0 ){
                                jugador->idEmparejamiento = auxiliar->carta.idEmparejamiento;
                                if (auxiliar->carta.idEmparejamiento == (*computadora).idEmparejamiento)
                                    computadora->idEmparejamiento = 0;
                            }           
                        }
                        emparejarMultiplesCartas(mesa,cartasJugador,cartaSeleccionada,cartasParaEmparejar);
                        *contadorCartasMesa = contarCartas(mesa);
                        *movimientoValido = true;
                    }
                }
            break;
            case '3':
                cout << "Que carta desea usar para recoger?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                cout << "Cuantas cartas desea recoger?" << endl;
                do
                {
                    cin >> cantidadCartasRecoger;
                    while(cin.fail()) {
                        cout << "Error - Vuelva a ingresar numero" << endl;
                        cin.clear();
                        cin.ignore(256,'\n');
                        cin >> cantidadCartasRecoger;
                    }
                } while (cantidadCartasRecoger < 1 || cantidadCartasRecoger > *contadorCartasMesa);
                if (cantidadCartasRecoger == 1)
                {
                    cout << "Que carta desea recoger?" << endl;
                    *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                    *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                    if (cartaPuedeRecogerse(cartaSeleccionada,*cartaMesaSeleccionada,mesa,*jugador,true))
                    {
                        if((*cartaMesaSeleccionada).idEmparejamiento != 0){
                            *ultimoEnRecogerPorEmparejamiento = Persona;
                            if((*cartaMesaSeleccionada).idEmparejamiento == (*jugador).idEmparejamiento)
                                (*jugador).idEmparejamiento = 0;
                            else
                                (*computadora).idEmparejamiento = 0;
                        }
                        recogerCartaDeMesa(mesa,recogidasJugador,cartasJugador,cartaSeleccionada,*cartaMesaSeleccionada);
                        *contadorCartasMesa = contarCartas(mesa);
                        if(*contadorCartasMesa == 0)
                            jugador->clarezas++;
                        *movimientoValido = true;
                    }
                }
                else
                {
                    cout << "Que cartas desea recoger?" << endl;
                    for (int i = 0; i < cantidadCartasRecoger; i++)
                    {
                        cout << "Seleccionadas: " << endl;
                        imprimirMazo(cartasParaRecoger);
                        do
                        {
                            *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                            *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                            if(cartaExisteEnMazo(cartasParaRecoger,*cartaMesaSeleccionada))
                                cout << "Ya seleccionaste esa carta, selecciona otra" << endl;
                        } while (cartaExisteEnMazo(cartasParaRecoger,*cartaMesaSeleccionada));
                        insertarCartaEnMazo(cartasParaRecoger,*cartaMesaSeleccionada);
                    }
                    if (multiplesCartasPuedenRecogerse(cartaSeleccionada,cartasParaRecoger,mesa,*jugador,true))
                    {
                        Nodo *auxiliar = cartasParaRecoger;
                        while(auxiliar != NULL){
                            if(auxiliar->carta.idEmparejamiento != 0){
                                if(auxiliar->carta.idEmparejamiento == (*jugador).idEmparejamiento)
                                    (*jugador).idEmparejamiento = 0;
                                else
                                    (*computadora).idEmparejamiento = 0;
                                break;
                            }
                        }
                        recogerVariasCartasDeMesa(mesa,recogidasJugador,cartasJugador,cartaSeleccionada,cartasParaRecoger);
                        *contadorCartasMesa = contarCartas(mesa);
                        if(*contadorCartasMesa == 0)
                            jugador->clarezas++;
                        *movimientoValido = true;
                    }        
                }
            break;
            case '4':
                cout << "Que carta desea usar para doblar?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                cout << "Que carta desea doblar?" << endl;
                *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                if (cartaPuedeDoblarse(*jugador,cartaSeleccionada,*cartaMesaSeleccionada,mesa,true))
                {
                    doblarCarta(mesa,jugador->cartasMazo,cartaSeleccionada,*cartaMesaSeleccionada);
                    *contadorCartasMesa = contarCartas(mesa);
                    *movimientoValido = true;
                }
            break;
            default:
                cout << "Seleccione una opcion correcta\n";
            break;
        }
        pausarConsola();
    } while (!*movimientoValido);
    delete movimientoValido;
    delete opcionSeleccionada;
    delete posicionCartaDeMesa;
    delete cartaMesaSeleccionada;
}

/******************* FUNCIONES DE JUEGO PARA LA COMPUTADORA ***********************/

//Funcion que retorna la cantidad de cartas que hay en un mazo. Considera un emparejamiento como una carta.
int sumaMazo(Nodo *lista){ 
    int contador=0;
    int suma=0;
    int id1=-1;
    int id2=-1;
    while (lista!=NULL)
    {
        suma=1;
        if(lista->carta.idEmparejamiento!=0){
           suma=0;
            if(lista->carta.idEmparejamiento != id1 && lista->carta.idEmparejamiento != id2){
                if (lista->carta.idEmparejamiento != id1 && id1 != -1){
                    ++suma;
                    id2=lista->carta.idEmparejamiento;
                } else{
                    if(id1==-1)
                    ++suma;
                    id1=lista->carta.idEmparejamiento;
                }
            }  
        }     
        lista=lista->siguiente;
        contador+=suma;
    }
    return contador;
}
//Funcion que usando una lista como auxiliar, elimina todos los elementos de la otra lista.
void eliminarListaUsandoOtra(Nodo *&mesa, Nodo *auxiliar, Nodo *&recogidas){
    Carta carta;
    while (auxiliar!=NULL){
        carta=auxiliar->carta;
        insertarCartaEnMazo(recogidas, carta);
        eliminarCartaDeMazo(mesa, carta);
        auxiliar=auxiliar->siguiente;
    }
}
//Funcion que vacia todos los elementos que contenga la lista
void vaciarLista(Nodo *&lista){
    Nodo *auxiliar = lista;
    lista = lista->siguiente;
    while (auxiliar != NULL)
    {
        delete auxiliar;  
        auxiliar=lista;
        if(lista != NULL)
            lista = lista->siguiente;
    }
    lista = NULL;
}
//Funcion que vacia todos los elementos que contenga la lista
void reiniciarValores(Nodo *mazo, Nodo *cartasMesa, Jugador &jugador, Jugador &computadora, Ultimo &ultimoEnRecogerPorEmparejamiento, Ultimo &ultimoEnRealizarJugada){
    vaciarLista(mazo);
    vaciarLista(cartasMesa);
    jugador.clarezas = 0;
    jugador.idEmparejamiento = 0;
    vaciarLista(jugador.cartasMazo);
    vaciarLista(jugador.cartasRecogidas);
    computadora.clarezas = 0;
    computadora.idEmparejamiento = 0;
    vaciarLista(computadora.cartasMazo);
    vaciarLista(computadora.cartasRecogidas);
    ultimoEnRecogerPorEmparejamiento=Null;
    ultimoEnRealizarJugada=Null;
}
//Recoger en la mesa las espadas, dandole prioridad al sumar el puntaje
void recogerPorFigura(Figuras figuraARecoger, Nodo *&mesa, Nodo *&recogidas, Nodo *&mazoComputadora, Jugador &computadora, bool &movimientoRealizado){ //
    Carta cartaDeMesa, cartaDelMazo;
    Nodo *auxiliar=mesa;
    while(auxiliar!=NULL){
        if(auxiliar->carta.figura == figuraARecoger){
            cartaDeMesa=auxiliar->carta;
            cartaDelMazo=buscarCarta(mazoComputadora, auxiliar->carta);
        }
        auxiliar=auxiliar->siguiente;
        if(cartaDelMazo.valor!=-1){
            if (cartaPuedeRecogerse(cartaDelMazo,cartaDeMesa,mesa,computadora,false)){
                recogerCartaDeMesa(mesa,recogidas,mazoComputadora,cartaDelMazo,cartaDeMesa);
                 cout<<"\nComputadora: ";
                 cout<<"\nRecogio con su carta ";
                 imprimirCarta(cartaDelMazo);
                 cout<< " la carta de la mesa ";
                 imprimirCarta(cartaDeMesa);
                 cout<<"\n";
                if(contarCartas(mesa)==0)
                    computadora.clarezas++;
                movimientoRealizado=true;
                break;
            }
        }      
    }
}

void recogerPorValor(int valorARecoger, Nodo *&mesa, Nodo *&recogidas, Nodo *&mazoComputadora, Jugador &computadora, bool &movimientoRealizado){ //
    Carta cartaDeMesa, cartaDelMazo;
    Nodo *auxiliar=mesa;
    while(auxiliar!=NULL){
        if(auxiliar->carta.valor == valorARecoger){
            cartaDeMesa=auxiliar->carta;
            cartaDelMazo=buscarCarta(mazoComputadora, auxiliar->carta);
        }
        auxiliar=auxiliar->siguiente;
        if(cartaDelMazo.valor!=-1){
            if (cartaPuedeRecogerse(cartaDelMazo,cartaDeMesa,mesa,computadora,false)){
                cout<<"\nComputadora: ";
                 cout<<"\nRecogio con su carta ";
                 imprimirCarta(cartaDelMazo);
                 cout<< " la carta de la mesa ";
                 imprimirCarta(cartaDeMesa);
                 cout<<"\n";
                recogerCartaDeMesa(mesa,recogidas,mazoComputadora,cartaDelMazo,cartaDeMesa);
                if(contarCartas(mesa)==0)
                    computadora.clarezas++;
                movimientoRealizado=true;
                break;
            }
        }      
    }
}

void recogerPorCartaEspecifica(Figuras figuraARecoger, int valorARecoger, Nodo *&mesa, Nodo *&recogidas, Nodo *&mazoComputadora, Jugador &computadora, bool &movimientoRealizado){ //
    Carta cartaDeMesa, cartaDelMazo;
    Nodo *auxiliar=mesa;
    while(auxiliar!=NULL){
        if(auxiliar->carta.valor == valorARecoger && auxiliar->carta.figura == figuraARecoger){
            cartaDeMesa=auxiliar->carta;
            cartaDelMazo=buscarCarta(mazoComputadora, auxiliar->carta);
        }
        auxiliar=auxiliar->siguiente;
        if(cartaDelMazo.valor!=-1){
            if (cartaPuedeRecogerse(cartaDelMazo,cartaDeMesa,mesa,computadora,false)){
                recogerCartaDeMesa(mesa,recogidas,mazoComputadora,cartaDelMazo,cartaDeMesa);
                cout<<"\nComputadora: ";
                 cout<<"\nRecogio con su carta ";
                 imprimirCarta(cartaDelMazo);
                 cout<< " la carta de la mesa ";
                 imprimirCarta(cartaDeMesa);
                 cout<<"\n";
                if(contarCartas(mesa)==0)
                    ++computadora.clarezas;
                movimientoRealizado=true;
                break;
            }
        }      
    }
}
//Funcion que realiza todas las sumas posibles que puede recoger la computadora
void recogerSumasComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado){
    int suma=0, cantidadCartas=0;
    bool ok=false;
    Carta carta, cartaDelMazo, cartaABuscar;
    Nodo *listaAuxiliar=nullptr;
    int  posiblesCasos = pow(2,sumaMazo(mesa));
    for (int i = 0; i < posiblesCasos; i++){
        suma=0; cantidadCartas=0;
        ok=false;
        for (int j = 0; j < sumaMazo(mesa); j++){
            if ((i & (1 << j)) > 0){
                carta=buscarCartaPorPosicion(mesa,j+1);
                suma+=carta.valor;
                insertarCartaEnMazo(listaAuxiliar,carta);
                ++cantidadCartas;
                if(carta.representacion!='-' || carta.idEmparejamiento!=0 || suma>10){
                    suma=0;
                    break;
                }
            }   
        }
        cartaABuscar.valor=suma;
        cartaDelMazo=buscarCarta(mazoComputadora, cartaABuscar);
        if(cartaDelMazo.valor!=-1 && cantidadCartas>=2){
            eliminarCartaDeMazo(mazoComputadora, cartaDelMazo); //Se elimina carta del mazo jugador
            eliminarListaUsandoOtra(mesa, listaAuxiliar, recogidas); //Se inserta en las recogidas y elimina de la mesa
            insertarCartaEnMazo(recogidas,cartaDelMazo); //Se inserta la carta en mesa recogidas
            cout<<"\nComputadora: ";
            cout<<"\nRecogio con su carta ";
            imprimirCarta(cartaDelMazo);
            cout<< " la suma de las siguientes cartas: ";
            imprimirMazo(listaAuxiliar);
            cout<<"\n";
            movimientoRealizado=true;
            break;
        }
        vaciarLista(listaAuxiliar);   
    }
}
//Eliminar cartas de un mismo valor repetidas de un mazo. (Usado para recoger varias cartas en la mesa iguales con la que tienes)
void eliminarCartasRepetidasEnUnMazo(Nodo *&mesa, Nodo *&recogidas, Carta carta, bool &movimientoRealizado){
    Carta cartaEliminada;
    Nodo *auxiliar=mesa;
    Nodo *anterior=NULL;
    do{
        if(mesa->carta.valor==carta.valor && mesa->carta.idEmparejamiento==0){
            insertarCartaEnMazo(recogidas, mesa->carta);
            mesa=mesa->siguiente;
            delete auxiliar;
            movimientoRealizado=true;
            auxiliar=mesa;
        }
    } while (mesa->carta.valor==carta.valor);

    while(auxiliar!=NULL){
        if(auxiliar->carta.valor == carta.valor && mesa->carta.idEmparejamiento==0){
            insertarCartaEnMazo(recogidas, auxiliar->carta);
            anterior->siguiente=auxiliar->siguiente;
            delete auxiliar;
            movimientoRealizado=true;
            auxiliar=anterior;
        }
        anterior=auxiliar;
        auxiliar=auxiliar->siguiente;
    }
}
//Funcion que recorre el mazo de la computadora si encuentra la oportunidad de recoger cartas iguales
void utilizarCadaCarta(Nodo *&mesa, Nodo *&mazoJugador, Nodo *&recogidas, bool &movimientoRealizado){
    Carta carta;
    Nodo *auxiliar=mazoJugador;
    while (auxiliar!=NULL)
    {
        carta=auxiliar->carta;
        eliminarCartasRepetidasEnUnMazo(mesa, recogidas, carta, movimientoRealizado);
        if(movimientoRealizado)  break;
        auxiliar=auxiliar->siguiente;
    }

    if(movimientoRealizado){
        cout<<"\nComputadora: ";
        cout<<"\nRecogio con su carta ";
        imprimirCarta(carta);
        cout<< " todas las cartas de la mesa iguales a ";
        imprimirCarta(carta);
        cout<<"\n";
        insertarCartaEnMazo(recogidas, carta);
        eliminarCartaDeMazo(mazoJugador, carta);
    }
}
//Funcion que recoge una carta igual
void recogerUnaCarta(Nodo *&mesa, Nodo *&mazoJugador, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    Nodo *auxiliar=mazoJugador;
    Carta cartaRecogiendo;
    Carta cartaQueSeRecoge;
    while (auxiliar!=NULL){
        cartaRecogiendo=auxiliar->carta;
        Carta cartaQueSeRecoge=buscarCarta(mesa, cartaRecogiendo);
        auxiliar=auxiliar->siguiente;
        if(cartaQueSeRecoge.valor!=-1){
            cout<<"\nComputadora: ";
            cout<<"\nRecogio con su carta ";
            imprimirCarta(cartaRecogiendo);
            cout<< " la carta de la mesa ";
            imprimirCarta(cartaQueSeRecoge);
            cout<<"\n";
            recogerCartaDeMesa(mesa, recogidas, mazoJugador, cartaRecogiendo, cartaQueSeRecoge);
            movimientoRealizado=true;
            if(contarCartas(mesa)==0)
                ++computadora.clarezas;
        }
        if(movimientoRealizado) break;
    }
}
//Recoger el emparejamiento de la computadora
void recogerEmparejamientoComputadora(Nodo *&mesa, Nodo *&recogidas, Nodo *&mazoComputadora, bool &movimientoRealizado, Jugador &computadora){
    Carta cartaQueSeRecoge=buscarCartaEnMazoPorIdEmparejamiento(mesa, computadora.idEmparejamiento);
    Carta cartaABuscar;
    cartaABuscar.valor=cartaQueSeRecoge.sumaEmparejadas;
    Carta cartaRecogiendo=buscarCarta(mazoComputadora, cartaABuscar); //Busca la carta en el mazo de la compu el que tenga un valor igual a la suma 
    recogerCartaDeMesa(mesa, recogidas, mazoComputadora, cartaRecogiendo, cartaQueSeRecoge);
    cout<<"\nComputadora: ";
    cout<<"\nRecogio con su carta ";
    imprimirCarta(cartaRecogiendo);
    cout<< " el emparejamiento ";
    cout<<"\n";
    movimientoRealizado=true;
    computadora.idEmparejamiento=0;
}
//Todas las formas de recoger de la computadora
void recogerCartas(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora, Ultimo *ultimoEnRecogerPorEmparejamiento){
    for (int i = 1; i <=8; i++)
    {
        switch (i){
            case 1:
                if(computadora.idEmparejamiento!=0){
                    recogerEmparejamientoComputadora(mesa, recogidas, mazoComputadora, movimientoRealizado, computadora);
                    *ultimoEnRecogerPorEmparejamiento = Computadora;
                }
            break;
            case 2:
                recogerPorCartaEspecifica(diamante,10,mesa, recogidas,mazoComputadora, computadora, movimientoRealizado);
            break;
            case 3:
                recogerPorCartaEspecifica(espada,2,mesa, recogidas,mazoComputadora, computadora, movimientoRealizado);
            break;
            case 4:
                recogerPorValor(1,mesa, recogidas,mazoComputadora, computadora, movimientoRealizado);
            break;
            case 5:
                recogerPorFigura(espada,mesa, recogidas,mazoComputadora, computadora, movimientoRealizado);
            break;
            case 6:
                utilizarCadaCarta(mesa, mazoComputadora,recogidas, movimientoRealizado);
            break;
            case 7:
                recogerSumasComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado);
            break;      
            default:
                recogerUnaCarta(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
        }    
        if(movimientoRealizado) break;
    }
}
//Emparejar
void emparejarComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    Carta cartaMesaSeleccionada, cartaSeleccionada, cartaConId;
    for (int i = 1; i <= contarCartas(mazoComputadora); i++){
        cartaSeleccionada=buscarCartaPorPosicion(mazoComputadora,i);
        for(int j=1; j<=contarCartas(mesa); j++){
            cartaMesaSeleccionada=buscarCartaPorPosicion(mesa, j);
            if (cartaPuedeEmparejarse(computadora,cartaSeleccionada,cartaMesaSeleccionada,mesa,false)){
                if(cartaMesaSeleccionada.idEmparejamiento != 0 && cartaMesaSeleccionada.idEmparejamiento == computadora.idEmparejamiento) //Condicion para quitarle el emparejado al jugador
                    computadora.idEmparejamiento = 0;
                emparejarCarta(mesa, mazoComputadora, cartaSeleccionada, cartaMesaSeleccionada);
                cartaConId=buscarCartaExacta(mesa, cartaSeleccionada);
                computadora.idEmparejamiento=cartaConId.idEmparejamiento;
                 cout<<"\nComputadora: ";
                 cout<<"\nEmparejo la carta ";
                 imprimirCarta(cartaSeleccionada);
                 cout<< " Con la carta de la mesa ";
                 imprimirCarta(cartaMesaSeleccionada);
                 cout<<"\n";
                movimientoRealizado=true;
                break;
            }
        }
        if(movimientoRealizado)  break;
    }
}
//Doblar
void doblarComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    Carta cartaMesaSeleccionada, cartaSeleccionada;
    for (int i = 1; i <= contarCartas(mazoComputadora); i++){
        cartaSeleccionada=buscarCartaPorPosicion(mazoComputadora,i);
        for(int j=1; j<=contarCartas(mesa); j++){
            cartaMesaSeleccionada=buscarCartaPorPosicion(mesa, j);
                 if (cartaPuedeDoblarse(computadora,cartaSeleccionada,cartaMesaSeleccionada,mesa,false)){
                    doblarCarta(mesa,mazoComputadora,cartaSeleccionada,cartaMesaSeleccionada);
                    cout<<"\nComputadora: ";
                    cout<<"\nDoblo la carta ";
                    imprimirCarta(cartaSeleccionada);
                    cout<< " Con la carta de la mesa ";
                    imprimirCarta(cartaMesaSeleccionada);
                    cout<<"\n";
                    movimientoRealizado = true;
                    break;
                 } 
        }
        if(movimientoRealizado) break;
    }
}
//Lanzar una carta
void lanzarCarta(Nodo *&mesa, Nodo *&cartasComputadora, short int &contadorCartasComputadora, short int &contadorCartasMesa, bool &movimientoRealizado){
    int posicionCartaSeleccionada;
    Carta cartaSeleccionada;
        do{   
          posicionCartaSeleccionada=rand()%10;
        } while (posicionCartaSeleccionada < 1 || posicionCartaSeleccionada > contadorCartasComputadora);
        cartaSeleccionada = buscarCartaPorPosicion(cartasComputadora, posicionCartaSeleccionada);
                            //Guardando la figura para la ultima jugada
    lanzarCartaEnMesa(mesa, cartasComputadora, cartaSeleccionada);
    movimientoRealizado=true;
    cout<<"\nComputadora: ";
    cout<<"\nLanzo la carta: ";
    imprimirCarta(cartaSeleccionada);
    cout<<"\n";
}
void movimientosComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, short int &contadorCartasComputadora, short int &contadorCartasMesa, Jugador &computadora, Ultimo *ultimoEnRecogerPorEmparejamiento){
    bool movimientoRealizado = false;
    cout<<"\nCartas de la mesa: \n";
    imprimirMazo(mesa);
    for (int i = 1; i <=4 ; i++){
        switch (i){
            case 1:
                recogerCartas(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora, ultimoEnRecogerPorEmparejamiento);
            break;
            case 2:
                if(computadora.idEmparejamiento==0)
                    emparejarComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
            case 3:
                doblarComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
            default:
                lanzarCarta(mesa,mazoComputadora,contadorCartasComputadora, contadorCartasMesa,movimientoRealizado);
            break;
        }
        if(movimientoRealizado)
            pausarConsola();
        contadorCartasMesa=contarCartas(mesa);
        if(movimientoRealizado) break;
    }
}