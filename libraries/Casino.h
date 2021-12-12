#include "ConsoleUI.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

using namespace std;

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

int contarCartas(Nodo *mazo){
    int contador = 0;
    while (mazo != NULL)
    {
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

bool cartaPuedeRecogerse(Carta cartaConLaQueSeRecoge, Carta cartaQueSeRecoge){
    if(cartaQueSeRecoge.idEmparejamiento != 0){
        if (cartaConLaQueSeRecoge.valor == cartaQueSeRecoge.sumaEmparejadas)
            return true;
        else{
            cout << "No se puede recoger. El valor de la carta no es igual a la suma del emparejamiento" << endl;
            return false;
        }
    }else{
        if (cartaConLaQueSeRecoge.valor == cartaQueSeRecoge.valor)
            return true;
        else{
            cout << "No se puede recoger. El valor de las cartas no es igual" << endl;
            return false;
        }
    }
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

bool multiplesCartasPuedenRecogerse(Carta cartaConLaQueSeRecoge, Nodo *cartasQueSeRecogen){
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
            cout << "No puedes recoger figuras con cartas numericas" << endl;
            return false;
        }
        //Verificar que solo se hayan seleccionado figuras
        auxiliar = cartasQueSeRecogen;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor <= 10){
                cout << "No puedes recoger cartas numericas con figuras" << endl;
                return false;
            }
        }
        //Verificar que sean la misma figura
        auxiliar = cartasQueSeRecogen;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor != cartaConLaQueSeRecoge.valor){
                cout << "Solo se pueden recoger figuras iguales" << endl;
                return false;
            }
        }
        if(contarCartas(cartasQueSeRecogen) == 2){ //2 + la del mazo del jugador
            cout << "No se puede recoger 3 cartas de una figura" << endl;
            return false;
        }
        return true;
    }
    else
    {
        if(cartaConLaQueSeRecoge.valor > 10){
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
                cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            if (sumaQueSeRecoge != cartaConLaQueSeRecoge.valor){
                cout << "La suma de las cartas seleccionadas no coincide con el valor de la carta usada para recoger" << endl;
                return false;
            }
            return true;
        }else if(id1 != 0 && id2 != 0 && id1 != id2){
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
                cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            if (sumaTotal != cartaConLaQueSeRecoge.valor){
                cout << "La suma de las cartas seleccionadas no coincide con el valor de la carta usada para recoger" << endl;
                return false;
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

bool cartaPuedeEmparejarse(Jugador jugador, Carta cartaAEmparejar, Carta cartaConLaQueSeEmpareja, Nodo *mesa){
    if (cartaAEmparejar.valor > 10 || cartaConLaQueSeEmpareja.valor > 10){
        cout << "No puedes emparejar 2 cartas J, Q o K" << endl;
        return false;
    }
    if(jugador.idEmparejamiento != 0){
        cout << "No puedes emparejar, ya tienes un emparejamiento activo" << endl;
        return false;
    }
    if(cartaConLaQueSeEmpareja.doblada){
        cout << "No puedes sumar al emparejamiento. Esta protegido" << endl;
        return false;
    }
    if(cartaAEmparejar.valor + cartaConLaQueSeEmpareja.sumaEmparejadas > 10 || cartaAEmparejar.valor + cartaConLaQueSeEmpareja.valor > 10){
        cout << "No puedes emparejar, la suma del emparejamiento pasaria de 10" << endl;
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

bool multiplesCartasPuedenEmparejarse(Carta cartaAEmparejar, Nodo *cartasQueSeEmparejan, Jugador jugador){
    if(jugador.idEmparejamiento != 0){
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
            cout << "No puedes emparejar cartas numericas con figuras" << endl;
            return false;
        }
        //Verificar que solo se hayan seleccionado figuras
        auxiliar = cartasQueSeEmparejan;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor <= 10){
                cout << "No puedes emparejar figuras con cartas numericas" << endl;
                return false;
            }
        }
        //Verificar que sean la misma figura
        auxiliar = cartasQueSeEmparejan;
        while (auxiliar != NULL)
        {
            if(auxiliar->carta.valor != cartaAEmparejar.valor){
                cout << "Solo se pueden recoger figuras iguales" << endl;
                return false;
            }
        }
        if(contarCartas(cartasQueSeEmparejan) == 3){ //3 + la del mazo del jugador
            cout << "No se puede emparejar 4 cartas de una figura." << endl;
            cout << "Consejo: prueba recogiendolas" << endl;
            return false;
        }
        return true;
    }
    else
    {
        if(cartaAEmparejar.valor > 10){
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
                cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
                return false;
            }
            return true;
        }else if(id1 != 0 && id2 != 0 && id1 != id2){
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
                cout << "La suma de las cartas seleccionadas superan el valor de 10" << endl;
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

bool cartaPuedeDoblarse(Jugador jugador, Carta cartaADoblar, Carta cartaConLaQueSeDobla, Nodo *mesa){
    if(cartaConLaQueSeDobla.idEmparejamiento == 0 && cartaADoblar.valor != cartaConLaQueSeDobla.valor){
        cout << "Para doblar con otra carta que no esta emparejada estas deben tener el mismo valor" << endl;
        return false;
    }
    if(cartaADoblar.valor != cartaConLaQueSeDobla.sumaEmparejadas){
        cout << "No puedes doblar. La carta no vale igual que la suma del emparejamiento" << endl;
        return false;
    }
    return true;
}

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

void seleccionarMovimiento(Jugador *jugador, Jugador *computadora, Nodo *&mesa, Nodo *&cartasJugador, Nodo *&recogidasJugador, short int contadorCartasJugador, short int *contadorCartasMesa, bool isMac){
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
        limpiarConsola(isMac);
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
                cout << "Que carta desea lanzar?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                lanzarCartaEnMesa(mesa, cartasJugador, cartaSeleccionada);
                (*contadorCartasMesa)++;
                *movimientoValido = true;
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
                    if (cartaPuedeEmparejarse(*jugador,cartaSeleccionada,*cartaMesaSeleccionada,mesa))
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
                    if (multiplesCartasPuedenEmparejarse(cartaSeleccionada,cartasParaEmparejar,*jugador))
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
                    if (cartaPuedeRecogerse(cartaSeleccionada,*cartaMesaSeleccionada))
                    {
                        if((*cartaMesaSeleccionada).idEmparejamiento != 0)
                            if((*cartaMesaSeleccionada).idEmparejamiento == (*jugador).idEmparejamiento)
                                (*jugador).idEmparejamiento = 0;
                            else
                                (*computadora).idEmparejamiento = 0;
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
                    if (multiplesCartasPuedenRecogerse(cartaSeleccionada,cartasParaRecoger))
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
                if (cartaPuedeDoblarse(*jugador,cartaSeleccionada,*cartaMesaSeleccionada,mesa))
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

//Funcion que retorna un jugador dependiendo del turno. Para usarlo en parametros, se puede dereferenciar la funcion con *
// Jugador* retornaJugador(int jugado, Jugador &usuario, Jugador &computador){
//     if(jugado==1)
//         return &usuario;
//     else
//         return &computador;
// }
// //Funcion que retorna una lista dependiendo del turno
// Nodo* retornaLista(int jugado, Nodo *&lista1, Nodo *&lista2){
//     if(jugado==1)
//         return lista1;
//     else
//         return lista2;
// }

//Comprueba si la carta a emparejar esta protegida. La carta debe tener un campo .doblado
// bool cartasDoblado(Carta cartaARecoger){ 
//     bool doblado;
//     doblado = (cartaARecoger.doblado==true) ? true : false;
//     return doblado; 
// }
//Comprueba que no se puedan recoger figuras 3 figuras. Una + dos en la mesa
// bool figurasEmparejadas(Carta cartaARecoger){
//     bool recoger=true;
//     if((cartaARecoger.valor=='-') && (cartaARecoger.contadorEmparejadas==2));
//         recoger=false;
//     return recoger;
// }
// //Comprueba si el jugador ya emparejó antes. No puede agregar otro emparejamiento
// bool jugadorEmparejo(Jugador jugador){
//     bool emparejo=false;
//     emparejo=(jugador.emparejo==true) ? true : false;
//     return emparejo;
// }

// //Funcion que agrega punto de clareza si recoge ultima carta. Jugador debe tener campo .clareza
// void puntoDeClareza(bool cartaRecogida, Nodo *mesa, Jugador &jugador){ 
//     if(mesa->siguiente==NULL)
//     (cartaRecogida==true) ? ++jugador.clareza : jugador.clareza;
// }

// bool valorExisteEnEmparejamiento(Nodo *mesa, int valor, int idEmparejamiento){
//     if(idEmparejamiento == 0)
//         return false;
//     while(mesa != NULL){
//         if (mesa->carta.valor == valor && mesa->carta.idEmparejamiento == idEmparejamiento)
//             return true;
//         mesa = mesa->siguiente;
//     }
//     return false;
// }