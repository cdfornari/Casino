#include <iostream>
#include <time.h>
#include "libraries/Casino.h"

using namespace std;

typedef struct {
    Nodo *cartasMazo;
    Nodo *cartasRecogidas;
    int clarezas;
} Jugador;

Nodo *mazo = NULL;
Nodo *cartasMesa = NULL;
Jugador jugador;
Jugador computadora;

int main (){ //NO HACER TESTING EN LA MISMA EJECUCIÓN QUE JUGAR
    jugador.cartasMazo = NULL;
    jugador.clarezas = 0;
    computadora.cartasMazo = NULL;
    computadora.clarezas = 0;
    bool isMac = seleccionarSistemaOperativo();
    char opcionSeleccionada;
    short int reparte;
    short int contadorCartasJugador;
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
                            //seleccionarMovimiento(cartasMesa,computadora.cartasMazo);
                            seleccionarMovimiento(cartasMesa,jugador.cartasMazo,contadorCartasJugador,isMac);
                        }else{
                            seleccionarMovimiento(cartasMesa,jugador.cartasMazo,contadorCartasJugador,isMac);
                            //seleccionarMovimiento(cartasMesa,computadora.cartasMazo);
                        }
                        contadorCartasJugador--;
                        pausarConsola();
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
    
    
    return 0;
}