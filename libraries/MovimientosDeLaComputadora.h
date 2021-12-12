#include<iostream>

#include"RecogerSumas.h"

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
//Recoger en la mesa las espadas, dandole prioridad al sumar el puntaje
void recogerEspadas(Nodo *&mesa, Nodo *&recogidas, Nodo *&mazoComputadora, Jugador &computadora, bool &movimientoRealizado){ //
    Carta cartaDeMesa, cartaDelMazo;
    Nodo *auxiliar=mesa;
    while(auxiliar!=NULL){
        if(auxiliar->carta.figura==espada){
            cartaDeMesa=auxiliar->carta;
            cartaDelMazo=buscarCarta(mazoComputadora, auxiliar->carta);
        }
        auxiliar=auxiliar->siguiente;
        if(cartaDelMazo.valor!=-1){
            if (cartaPuedeRecogerse(cartaDelMazo,cartaDeMesa)){
                recogerCartaDeMesa(mesa,recogidas,mazoComputadora,cartaDelMazo,cartaDeMesa);
                //eliminarCartasRepetidasEnUnMazo
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
    Carta carta, cartaDelMazo;
    Nodo *listaAuxiliar=nullptr;
    int  posiblesCasos = pow(2,sumaMazo(mesa));
    for (int i = 0; i < posiblesCasos; i++){
        suma=0; cantidadCartas=0;
        ok=false;
        for (int j = 0; j < sumaMazo(mesa); j++){
            if ((i & (1 << j)) > 0){
        
                carta=buscarCartaPorPosicion(mesa,j+1);
                ++cantidadCartas;
                imprimirCarta(carta);

                if(carta.representacion!='-'){
                    suma=0;
                    break;
                }
                if(carta.idEmparejamiento!=0){
                    recogerEmparejamiento(mesa, listaAuxiliar, carta);
                    suma+=carta.sumaEmparejadas;
                }else{
                    insertarCartaEnMazo(listaAuxiliar, carta);
                    suma+=carta.valor;
                }
            }   
        }
        cartaDelMazo=sumaEstaEnMazo(mazoComputadora, suma, ok);
        
        if(ok==true && cantidadCartas>=2){
            eliminarCartaDeMazo(mazoComputadora, cartaDelMazo); //Se elimina carta del mazo jugador
            eliminarListaUsandoOtra(mesa, listaAuxiliar, recogidas); //Se inserta en las recogidas y elimina de la mesa
            insertarCartaEnMazo(recogidas,cartaDelMazo); //Se inserta la carta en mesa recogidas
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
            mesa=mesa->siguiente;
            delete auxiliar;
            movimientoRealizado=true;
            auxiliar=mesa;
        }
    } while (mesa->carta.valor==carta.valor);

    while(auxiliar!=NULL){
        if(auxiliar->carta.valor == carta.valor && mesa->carta.idEmparejamiento==0){
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
void utilizarCadaCarta(Nodo *&mesa, Nodo *mazoJugador, Nodo *&recogidas, bool &movimientoRealizado){
    Carta carta;
    while (mazoJugador!=NULL)
    {
        carta=mazoJugador->carta;
        eliminarCartasRepetidasEnUnMazo(mesa, recogidas, carta, movimientoRealizado);
        if(movimientoRealizado)  break;
        mazoJugador=mazoJugador->siguiente;
    }
    if(movimientoRealizado){
        insertarCartaEnMazo(recogidas, mazoJugador->carta);
        eliminarCartaDeMazo(mesa, mazoJugador->carta);
    }
}
//Todas las formas de recoger de la computadora
void recogerCartas(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    for (int i = 1; i <=4; i++)
    {
        switch (i){
        case 1:
              //recogerEmparejamiento();
            break;
        case 2:
            recogerEspadas(mesa, recogidas,mazoComputadora, computadora, movimientoRealizado);
            break;
        case 3:
            utilizarCadaCarta(mesa, mazoComputadora,recogidas, movimientoRealizado );
            break;
        case 4:
            recogerSumasComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado);
            break;      
        default:
            break;
        }    

        if(movimientoRealizado) break;

    }
}
//Emparejar
void emparejarComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    Carta cartaMesaSeleccionada, cartaSeleccionada;
    for (int i = 1; i <= contarCartas(mazoComputadora); i++){
        cartaSeleccionada=buscarCartaPorPosicion(mazoComputadora,i);
        for(int j=1; i<=contarCartas(mesa); j++){
            cartaMesaSeleccionada=buscarCartaPorPosicion(mesa, j);
            if (cartaPuedeEmparejarse(computadora,cartaSeleccionada,cartaMesaSeleccionada,mesa)){
                emparejarCarta(mesa, mazoComputadora, cartaSeleccionada, cartaMesaSeleccionada);
                computadora.idEmparejamiento==cartaMesaSeleccionada.idEmparejamiento;
                movimientoRealizado=true;
                break;
            }
        }
        if(movimientoRealizado)
            break;
    }
}
//Doblar
void doblarComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, bool &movimientoRealizado, Jugador &computadora){
    Carta cartaMesaSeleccionada, cartaSeleccionada;
    for (int i = 1; i <= contarCartas(mazoComputadora); i++){
        cartaSeleccionada=buscarCartaPorPosicion(mazoComputadora,i);
        for(int j=1; i<=contarCartas(mesa); j++){
            cartaMesaSeleccionada=buscarCartaPorPosicion(mesa, j);
                 if (cartaPuedeDoblarse(computadora,cartaSeleccionada,cartaMesaSeleccionada,mesa)){
                     doblarCarta(mesa,mazoComputadora,cartaSeleccionada,cartaMesaSeleccionada);
                    movimientoRealizado=true;
                    break;
                 } 
        }
        if(movimientoRealizado)
            break;
    }
}
//Lanzar una carta
void lanzarCarta(Nodo *&mesa, Nodo *&cartasComputadora, short int contadorCartasComputadora, short int contadorCartasMesa){
    int posicionCartaSeleccionada;
    Carta cartaSeleccionada;
    do
    {
            do{   
          posicionCartaSeleccionada=rand() % 10;
        } while (posicionCartaSeleccionada < 1 || posicionCartaSeleccionada > contadorCartasComputadora);
        posicionCartaSeleccionada = seleccionarCartaPorPosicion(cartasComputadora,contadorCartasComputadora);
        cartaSeleccionada = buscarCartaPorPosicion(cartasComputadora, posicionCartaSeleccionada);
    } while (cartaSeleccionada.valor>10 && contarCartas(cartasComputadora)>1 && contadorCartasMesa==0); //Guardando la figura para la ultima jugada
    lanzarCartaEnMesa(mesa, cartasComputadora, cartaSeleccionada);
}
void movimientosComputadora(Nodo *&mesa, Nodo *&mazoComputadora, Nodo *&recogidas, short int contadorCartasComputadora, short int &contadorCartasMesa, Jugador &computadora){
    bool movimientoRealizado=false;
    for (int i = 1; i <=4 ; i++)
    {
        switch (i)
        {
            case 1:
                if(computadora.idEmparejamiento==0)
                    emparejarComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
            case 2:
                recogerCartas(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
            case 3:
                doblarComputadora(mesa, mazoComputadora, recogidas, movimientoRealizado, computadora);
            break;
            default:
                lanzarCarta(mesa,mazoComputadora,contadorCartasComputadora, contadorCartasMesa);
            break;
        }
        contadorCartasMesa=contarCartas(mesa);
        if(movimientoRealizado) break;
    }
}
