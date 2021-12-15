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
    jugador.cartasRecogidas = NULL;
    jugador.clarezas = 0;
    jugador.idEmparejamiento = 0;
    computadora.cartasMazo = NULL;
    computadora.cartasRecogidas = NULL;
    computadora.clarezas = 0;
    computadora.idEmparejamiento = 0;
    bool isMac = seleccionarSistemaOperativo();
    bool opcionNuevaPartida = false;
    bool ok = false;
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
                    ultimoEnRealizarJugada = Null;
                    repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                    contadorCartasJugador = 4;
                    contadorCartasComputadora = 4;
                    do
                    {
                        if(reparte == 0){
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Computadora;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Persona;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);

                        }else{
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Persona;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Computadora;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                        }
                        contadorCartasJugador--;
                        contadorCartasComputadora--;
                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                } while (mazo != NULL);
                asignarCartasSobrantes(cartasMesa,jugador,computadora,ultimoEnRecogerPorEmparejamiento);
                contarPuntaje(jugador.cartasRecogidas,computadora.cartasRecogidas,jugador.clarezas,computadora.clarezas);
                reiniciarValores(mazo, cartasMesa, jugador, computadora, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada);
            break;
            case '2':
                    if(existeArchivo()){
                        cargarInformacion(mazo, cartasMesa, jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada);
                        cout<<"\nPartida cargada\n";
                        ok=true;
                        *contadorCartasMesa = contarCartas(cartasMesa);
                        if(contarCartas(jugador.cartasMazo) == 0 && contarCartas(computadora.cartasMazo) == 0){
                            repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                            contadorCartasJugador = 4;
                            contadorCartasComputadora = 4;
                        }else{
                            contadorCartasJugador = contarCartas(jugador.cartasMazo);
                            contadorCartasComputadora = contarCartas(computadora.cartasMazo);
                        }
                        if(ultimoEnRealizarJugada != Null){
                            if (reparte == 0)
                            {
                                if(ultimoEnRealizarJugada == Persona){
                                    do
                                    {
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }else{
                                    do
                                    {                                        
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;
                                        if(computadora.cartasMazo != NULL){
                                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                            ultimoEnRealizarJugada = Computadora;
                                        }
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }
                            }
                            else
                            {
                                if(ultimoEnRealizarJugada == Persona){
                                    do
                                    {
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                        if(jugador.cartasMazo != NULL){
                                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                            ultimoEnRealizarJugada = Persona;
                                        }
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }else{
                                    do
                                    {   
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;                                     
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }
                            }
                        }
                        do
                        {
                            ultimoEnRealizarJugada = Null;
                         //   repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                            contadorCartasJugador = 4;
                            contadorCartasComputadora = 4;
                            do
                            {
                                if(reparte == 0){
                                    movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Computadora;
                                    seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Persona;
                                }else{
                                    seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Persona;
                                    movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Computadora;
                                }
                                contadorCartasJugador--;
                                contadorCartasComputadora--;
                            vaciarInformacion(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                            } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                        } while (mazo != NULL);
                        asignarCartasSobrantes(cartasMesa,jugador,computadora,ultimoEnRecogerPorEmparejamiento);
                        contarPuntaje(jugador.cartasRecogidas,computadora.cartasRecogidas,jugador.clarezas,computadora.clarezas);
                        reiniciarValores(mazo, cartasMesa, jugador, computadora, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada);
                    }else
                        cout<<"\nNo pudo cargarse la partida. No existe el archivo\n";
            break;
            case '3':
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