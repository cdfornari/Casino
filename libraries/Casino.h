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
    short int contadorEmparejadas;
    short int sumaEmparejadas;
}Carta;

struct Nodo {
    Carta carta;
    Nodo *siguiente;
};

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
    carta.contadorEmparejadas = 0;
    carta.sumaEmparejadas = 0;
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

void eliminarCartaDeMazo(Nodo *&mazo, Carta carta){
    if(mazo != NULL){
        Nodo *auxiliar = mazo;
        Nodo *anterior = NULL;
        while (auxiliar != NULL && auxiliar->carta.valor != carta.valor && auxiliar->carta.figura != carta.figura)
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

void recogerCartaDeMesa(Nodo *&mesa, Nodo *&recogidas,Nodo *&mazoJugador, Carta cartaRecogiendo, Carta cartaRecogida){
    if(cartaRecogida.idEmparejamiento == 0){
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        insertarCartaEnMazo(recogidas,cartaRecogida);
        eliminarCartaDeMazo(mesa,cartaRecogida);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
    }else{
        insertarCartaEnMazo(recogidas,cartaRecogida);
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        eliminarCartaDeMazo(mesa,cartaRecogida);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
        int *contador = new int(0);
        Carta carta; 
        while (*contador < cartaRecogida.contadorEmparejadas && buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaRecogida.idEmparejamiento).valor != NULL)
        {
            carta = buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaRecogida.idEmparejamiento);
            insertarCartaEnMazo(recogidas,carta);
            eliminarCartaDeMazo(mesa,carta);
            (*contador)++;
        }
        delete contador;
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
        auxiliar = mesa;
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

void seleccionarMovimiento(Nodo *mesa, Nodo *cartasJugador){

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

void imprimirMazo(Nodo *mazo){
    while(mazo != NULL){
        if(mazo->carta.representacion == '-'){
            cout << mazo->carta.valor;
            imprimirFigura(mazo->carta);
            cout << setw(4);
        }else{
            cout << mazo->carta.representacion;
            imprimirFigura(mazo->carta);
            cout << setw(4);
        }
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
            }else if(id2 == 0){
                id2 = mesa->carta.idEmparejamiento;
                insertarCartaEnMazo(emparejamiento2,mesa->carta);
            }else if (mesa->carta.idEmparejamiento == id1){
                insertarCartaEnMazo(emparejamiento1,mesa->carta);
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