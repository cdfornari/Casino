#include <iostream>
#include <time.h>
#include "libraries/HandleFiles.h"

using namespace std;
    
int main (){
    Nodo *mazo = NULL;
    Nodo *cartasMesa = NULL;
    Jugador jugador;
    Jugador computadora;
    jugador.cartasMazo = NULL;
    jugador.clarezas = 0;
    jugador.idEmparejamiento = 0;
    computadora.cartasMazo = NULL;
    computadora.clarezas = 0;
    computadora.idEmparejamiento = 0;
    bool isMac = seleccionarSistemaOperativo();
    bool opcionNuevaPartida=false;
    bool ok=false;
    char opcionSeleccionada;
    short int reparte;
    short int contadorCartasJugador;
    short int contadorCartasComputadora;
    short int *contadorCartasMesa = new short int;
    Ultimo ultimoEnRecogerPorEmparejamiento = Null;
    Ultimo ultimoEnRealizarJugada = Null;
    do
    {
        limpiarConsola(isMac);
        imprimirMenuPrincipal();
        opcionSeleccionada = seleccionarOpcion();
        switch (opcionSeleccionada)
        {
            case '1':
                opcionNuevaPartida=true;
                srand(time(0));
                barajear(mazo);
                repartirAMesa(mazo,cartasMesa);
                *contadorCartasMesa = 4;
                reparte = rand()%(2);
                (reparte == 0)
                    ? cout << "Reparte el jugador\n"
                    : cout << "Reparte la computadora\n";
                    pausarConsola();
                    limpiarConsola(isMac);
                do
                {
                    repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                    contadorCartasJugador = 4;
                    contadorCartasComputadora = 4;
                    do
                    {
                        if(reparte == 0){
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,&computadora,&ultimoEnRecogerPorEmparejamiento);
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                        }else{
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,&computadora,&ultimoEnRecogerPorEmparejamiento);
                        }
                        contadorCartasJugador--;
                        contadorCartasComputadora--;
                        vaciarInformacion(mazo, cartasMesa, jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                } while (mazo != NULL);
                if(cartasMesa != NULL)
                    switch (ultimoEnRecogerPorEmparejamiento)
                    {
                        case Persona:
                            while (cartasMesa != NULL)
                            {
                                insertarCartaEnMazo(jugador.cartasRecogidas,cartasMesa->carta);
                                eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                            }
                        break;
                        case Computadora:
                            while (cartasMesa != NULL)
                            {
                                insertarCartaEnMazo(computadora.cartasRecogidas,cartasMesa->carta);
                                eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                            }
                        break;
                        default:
                            while (cartasMesa != NULL && contarCartas(cartasMesa) > 1)
                            {
                                insertarCartaEnMazo(jugador.cartasRecogidas,cartasMesa->carta);
                                eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                                insertarCartaEnMazo(computadora.cartasRecogidas,cartasMesa->carta);
                                eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                            }
                            if(contarCartas(cartasMesa) == 1)
                                if (rand()%2 == 0){
                                    insertarCartaEnMazo(jugador.cartasRecogidas,cartasMesa->carta);
                                    eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                                }else{
                                    insertarCartaEnMazo(computadora.cartasRecogidas,cartasMesa->carta);
                                    eliminarCartaDeMazo(cartasMesa,cartasMesa->carta);
                                }
                        break;
                    }
                contarPuntaje(jugador.cartasRecogidas,computadora.cartasRecogidas,jugador.clarezas,computadora.clarezas);
            break;
            case '2':
                if(opcionNuevaPartida==false){ //Si el jugador NO selecciono la opcion de nueva partida
                    if(existeArchivo()){
                        cargarInformacion(mazo, cartasMesa, jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada);
                        cout<<"Partida cargada\n";
                    }
                } else
                    ok=false;
                if(ok)
                   // jugarPartida(); //Continua con la partida
            break;
            case '3':
            break;
            case '4':
                srand(time(0));
                barajear(mazo);
                imprimirMazo(mazo);
                repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                cout << "Jugador" << endl;
                imprimirMazo(jugador.cartasMazo);
                cout << "Computadora" << endl;
                imprimirMazo(computadora.cartasMazo);
                cout << "mazo despues de repartir" << endl;
                imprimirMazo(mazo);
            break;
            default:
                cout << "Seleccione una opcion correcta\n";
            break;
        }
        pausarConsola();
    } while (opcionSeleccionada != '3');
    delete contadorCartasMesa;
    
    return 0;
}