#include<iostream>
#include<cmath>
#include <iomanip>
#include <stdlib.h>
#include "Casino.h"

using namespace std;

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
//Al encontrar una cadena de emparejamientos, se salta a la ultima carta de ese emparejamiento
Nodo *saltarEmparejamiento(Nodo *lista){
    int id;
    id=lista->carta.idEmparejamiento;
    Nodo *anterior=NULL;
    while (lista != NULL && lista->carta.idEmparejamiento==id){
        anterior=lista;
        lista=lista->siguiente;
    }
    return anterior; 
}
//Retorna la carta buscada dentro de una lista por indice. Similar a buscar un elemento en un arreglo.
Carta buscarPorIndice(Nodo *lista, int indice){
    Carta carta;
    int contador=0;
    Nodo *auxiliar=nullptr;
    while(contador<indice && lista!=NULL){
        carta=lista->carta;;
        if(carta.idEmparejamiento!=0)
            lista=saltarEmparejamiento(lista);
        lista=lista->siguiente;
        ++contador;
    }
    if(lista==NULL || contador==0)
        carta.valor=-1;
    return carta;
}
//Retorna carta que cumpla que haya una carta de valor igual a la suma en el mazo del jugador
Carta sumaEstaEnMazo(Nodo *lista, int suma, bool &existe){ 
    Carta carta;
    while (lista!=NULL){
        if(lista->carta.valor==suma){
            existe=true;
            carta=lista->carta;
        }
        lista=lista->siguiente;
    }
    return carta;
}
//Funcion que agrega los emparejamientos a la lista de recogidas
void recogerEmparejamiento(Nodo *mesa, Nodo *&recogidas, Carta cartaRecogida){
        Carta carta;
        while(mesa!=NULL){
            carta=mesa->carta;
            if(carta.idEmparejamiento==cartaRecogida.idEmparejamiento)
                insertarCartaEnMazo(recogidas, carta);
            mesa=mesa->siguiente;   
        }
}
//Funcion que vacia todos los elementos que contenga la lista
void vaciarLista(Nodo *&lista){
    Nodo *auxiliar=nullptr;
    while (lista!=NULL)
    {   auxiliar=lista;
        lista=lista->siguiente;
        delete auxiliar;
    }
}

//Funcion que calcula todas las sumas posibles dado un mazo y que la suma este en el mazo del jugador.
void sumasPosibles(Nodo *&mesa, Nodo *&mazoJugador, Nodo *&recogidas, int combinacion, bool &eliminar){
    int suma=0;
    bool ok=false;
    Carta carta, cartaDelMazo;
    Nodo *listaAuxiliar=nullptr;
    int  posiblesCasos = pow(2,sumaMazo(mesa));
    for (int i = 0; i < posiblesCasos; i++){
        suma=0;
        ok=false;
        for (int j = 0; j < sumaMazo(mesa); j++){
            if ((i & (1 << j)) > 0){
        
                carta=buscarPorIndice(mesa,j+1);
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
        cartaDelMazo=sumaEstaEnMazo(mazoJugador, suma, ok);
        if(ok==true){
            if(combinacion==-1){
                cout<<"\nCombinacion: "<<i<<endl;
                cout<<"\nCarta del mazo: ";
                imprimirCarta(cartaDelMazo);
                cout<<"\n";
                cout<<"Cartas de la mesa: "<<endl;
                imprimirMazo(listaAuxiliar);
            } else if(i==combinacion){
                eliminarCartaDeMazo(mazoJugador, cartaDelMazo); //Se elimina carta del mazo jugador
                eliminarListaUsandoOtra(mesa, listaAuxiliar, recogidas); //Se inserta en las recogidas y elimina de la mesa
                insertarCartaEnMazo(recogidas,cartaDelMazo); //Se inserta la carta en mesa recogidas
                eliminar=true;
            }
        }
        vaciarLista(listaAuxiliar);    
    }
}
//Funcion que presenta la opcion de recoger la suma de cartas
void recogerSumaDeCartas(Nodo *&mesa, Nodo *&mazoJugador, Nodo *&recogidas){
    int opcion, combinacion=-1;
    bool elimino=false;
    do{
        sumasPosibles(mesa, mazoJugador, recogidas, combinacion, elimino);
        cout<<"\n1. Escoger cartas a recoger\n";
        cout<<"2. Salir\n";
        cout<<"\nIntroduce la opcion: ";
        cin>>opcion;
        switch (opcion)
        {
        case 1:
                do{   
                    cout<<"\nIntroduce el numero de la combinacion de cartas a recoger. Escribe un numero negativo para salir\n";
                    cin>>combinacion;
                    sumasPosibles(mesa,mazoJugador,recogidas,combinacion,elimino);
                        if(elimino==false)
                        cout<<"\nLa combinacion escogida no es valida\n";
                        if(elimino==true){
                            cout<<"\nJugador\n";
                            imprimirMazo(mazoJugador);
                            cout<<"Mesa\n";
                            imprimirMazo(mesa);
                            cout<<"Recogidas\n";
                            imprimirMazo(recogidas);
                        }
                        
                } while (elimino!=true && combinacion>=0);
            break;
        case 2:
            cout<<"\nSaliste\n";
            break; 
        default:
            cout<<"\nNo introduciste una opcion valida. Intenta de nuevo\n";
            break;
        }
    } while (opcion!=2 && elimino!=true);
}
