#include <iostream>
#include <time.h>
#include "libraries/Casino.h"

using namespace std;

Nodo *mazo = NULL;
Nodo *cartasMesa = NULL;
Jugador jugador;
Jugador computadora;

int main (){ //NO HACER TESTING EN LA MISMA EJECUCIÓN QUE JUGAR
    jugador.cartasMazo = NULL;
    jugador.clarezas = 0;
    jugador.idEmparejamiento = 0;
    computadora.cartasMazo = NULL;
    computadora.clarezas = 0;
    computadora.idEmparejamiento = 0;
    bool isMac = seleccionarSistemaOperativo();
    char opcionSeleccionada;
    short int reparte;
    short int contadorCartasJugador;
    short int *contadorCartasMesa = new short int;
    do
    {
        limpiarConsola(isMac);
        imprimirMenuPrincipal();
        opcionSeleccionada = seleccionarOpcion();
        switch (opcionSeleccionada)
        {
            case '1':
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
                    do
                    {
                        if(reparte == 0){
                            //mueve comp
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac);
                        }else{
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac);
                            //mueve comp
                        }
                        contadorCartasJugador--;
                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                } while (mazo != NULL);
                contarPuntaje(jugador.cartasRecogidas,computadora.cartasRecogidas,jugador.clarezas,computadora.clarezas);
            break;
            case '2':
                cout << "COMING SOON\n";
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