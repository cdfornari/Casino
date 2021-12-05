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
        insertarCartaEnMazo(recogidas,cartaRecogida);
        eliminarCartaDeMazo(mesa,cartaRecogida);
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
    }else{
        insertarCartaEnMazo(recogidas,cartaRecogida);
        eliminarCartaDeMazo(mesa,cartaRecogida);
        insertarCartaEnMazo(recogidas,cartaRecogiendo);
        eliminarCartaDeMazo(mazoJugador,cartaRecogiendo);
        int *contador = new int(0);
        Carta carta; 
        while (buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaRecogida.idEmparejamiento).valor != NULL)
        {
            carta = buscarCartaEnMazoPorIdEmparejamiento(mesa,cartaRecogida.idEmparejamiento);
            insertarCartaEnMazo(recogidas,carta);
            eliminarCartaDeMazo(mesa,carta);
            (*contador)++;
        }
        delete contador;
    }  
}

bool cartaPuedeRecogerse(Carta cartaConLaQueSeRecoge, Carta cartaQueSeRecoge, Nodo *mesa){
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

bool valorExisteEnEmparejamiento(Nodo *mesa, int valor){
    while(mesa != NULL){
        if (mesa->carta.valor == valor)
            return true;
        mesa = mesa->siguiente;
    }
    return false;
}

bool cartaPuedeEmparejarse(Jugador jugador, Carta cartaAEmparejar, Carta cartaConLaQueSeEmpareja, Nodo *mesa){
    if(valorExisteEnEmparejamiento(mesa,cartaAEmparejar.valor)){
        cout << "No puedes emparejar, ya existe una carta con este valor en el emparejamiento" << endl << "Consejo: Intenta doblar" << endl;
        return false;
    }
    if (cartaAEmparejar.valor > 10 || cartaConLaQueSeEmpareja.valor > 10){
        cout << "No puedes emparejar cartas J, Q o K. Solo recoger 2 iguales" << endl;
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
    if(cartaAEmparejar.valor + cartaConLaQueSeEmpareja.sumaEmparejadas > 10){
        cout << "No puedes emparejar, la suma del emparejamiento pasaría de 10" << endl;
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
    short int posicionCartaSeleccionada;
    Carta cartaSeleccionada;
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
        cout << "2. Emparejar carta" << endl;
        cout << "3. Recoger carta o emparejamiento" << endl;
        cout << "4. Doblar o proteger emparejamiento" << endl;
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
                cout << "Con que carta desea emparejar?" << endl;
                *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                if (cartaPuedeEmparejarse(*jugador,cartaSeleccionada,*cartaMesaSeleccionada,mesa))
                {
                    if((*cartaMesaSeleccionada).idEmparejamiento != 0 && (*cartaMesaSeleccionada).idEmparejamiento == (*computadora).idEmparejamiento)
                        (*computadora).idEmparejamiento = 0;
                    jugador->idEmparejamiento = cartaMesaSeleccionada->idEmparejamiento;
                    emparejarCarta(mesa,cartasJugador,cartaSeleccionada,*cartaMesaSeleccionada);
                    *contadorCartasMesa = contarCartas(mesa);
                    *movimientoValido = true;
                }
            break;
            case '3':
                cout << "Que carta desea usar para recoger?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                cout << "Que carta desea recoger?" << endl;
                *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                imprimirCarta(cartaSeleccionada);
                imprimirCarta(*cartaMesaSeleccionada);
                if (cartaPuedeRecogerse(cartaSeleccionada,*cartaMesaSeleccionada,mesa))
                {
                    if((*cartaMesaSeleccionada).idEmparejamiento != 0)
                        if((*cartaMesaSeleccionada).idEmparejamiento == (*jugador).idEmparejamiento)
                            (*jugador).idEmparejamiento = 0;
                        else
                            (*computadora).idEmparejamiento = 0;
                    recogerCartaDeMesa(mesa,recogidasJugador,cartasJugador,cartaSeleccionada,*cartaMesaSeleccionada);
                    *contadorCartasMesa = contarCartas(mesa);
                    if(*contadorCartasMesa == 0)
                        (*jugador).clarezas++;
                    *movimientoValido = true;
                }
            break;
            case '4':
                cout << "Que carta desea usar para doblar?" << endl;
                posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasJugador,contadorCartasJugador);
                cartaSeleccionada = buscarCartaPorPosicion(cartasJugador, posicionCartaSeleccionada);
                cout << "Que carta desea doblar?" << endl;
                *posicionCartaDeMesa = seleccionarCartaPorPosicion(mesa,*contadorCartasMesa);
                *cartaMesaSeleccionada = buscarCartaPorPosicion(mesa,*posicionCartaDeMesa);
                if (true)
                {
                    //doblar
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