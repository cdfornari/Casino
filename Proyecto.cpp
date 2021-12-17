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
    bool ok = false;
    char opcionSeleccionada;
    short int reparte;
    short int contadorCartasJugador;
    short int contadorCartasComputadora;
    short int *contadorCartasMesa = new short int;
    Ultimo ultimoEnRecogerPorEmparejamiento = Null;
    Ultimo ultimoEnRealizarJugada = Null;
    bool isMac = seleccionarSistemaOperativo();
    limpiarConsola(isMac);
    do
    {   
        imprimirMenuPrincipal();
        opcionSeleccionada = seleccionarOpcion();
        limpiarConsola(isMac);
        switch (opcionSeleccionada)
        {
            case '1': //Jugar
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
                            cout << "Cartas en la mesa:\n";
                            imprimirMazo(cartasMesa);
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Computadora;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Persona;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                        }else{
                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                            ultimoEnRealizarJugada = Persona;
                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                            cout << "Cartas en la mesa:\n";
                            imprimirMazo(cartasMesa);
                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
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
                vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok); break;
            case '2': //partida anterior
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
                        if(ultimoEnRealizarJugada != Null){ //si se quedo en medio de una ronda, completarla (turno 2, 3 o 4)
                            if (reparte == 0) //reparte jugador
                            {
                                if(ultimoEnRealizarJugada == Persona){ //el orden depende del ultimo que haya hecho un movimiento
                                    do
                                    {   //quedan 2 movimientos
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }else{
                                    do
                                    {                                        
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        if(computadora.cartasMazo != NULL){  //pudo haber quedado en el ultimo movimiento de la ronda o faltan 3 turnos
                                            movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                            ultimoEnRealizarJugada = Computadora;
                                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        }
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }
                            }
                            else //reparte computadora
                            {
                                if(ultimoEnRealizarJugada == Persona){  //el orden depende del ultimo que haya hecho un movimiento
                                    do
                                    {   
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        if(jugador.cartasMazo != NULL){  //pudo haber quedado en el ultimo movimiento de la ronda o faltan 3 turnos
                                            seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                            ultimoEnRealizarJugada = Persona;
                                            vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        }
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }else{
                                    do
                                    {   //quedan 2 movimientos
                                        seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Persona;                                     
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                        movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                        ultimoEnRealizarJugada = Computadora;
                                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                    } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                                }
                            }
                        }
                        while (mazo != NULL) //Continuar normalmente con las otras rondas
                        {
                            ultimoEnRealizarJugada = Null;
                            repartirCartas(mazo,jugador.cartasMazo,computadora.cartasMazo);
                            contadorCartasJugador = 4;
                            contadorCartasComputadora = 4;
                            do
                            {
                                if(reparte == 0){
                                    cout << "Cartas en la mesa:\n";
                                    imprimirMazo(cartasMesa);
                                    movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Computadora;
                                    vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                    seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Persona;
                                    vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                }else{
                                    seleccionarMovimiento(&jugador,&computadora,cartasMesa,jugador.cartasMazo,jugador.cartasRecogidas,contadorCartasJugador,contadorCartasMesa,isMac,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Persona;
                                    vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                    cout << "Cartas en la mesa:\n";
                                    imprimirMazo(cartasMesa);
                                    movimientosComputadora(cartasMesa,computadora.cartasMazo,computadora.cartasRecogidas,contadorCartasComputadora,*contadorCartasMesa,computadora,jugador,&ultimoEnRecogerPorEmparejamiento);
                                    ultimoEnRealizarJugada = Computadora;
                                    vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
                                }
                                contadorCartasJugador--;
                                contadorCartasComputadora--;
                            } while (jugador.cartasMazo != NULL || computadora.cartasMazo!= NULL);
                        }
                        asignarCartasSobrantes(cartasMesa,jugador,computadora,ultimoEnRecogerPorEmparejamiento);
                        contarPuntaje(jugador.cartasRecogidas,computadora.cartasRecogidas,jugador.clarezas,computadora.clarezas);
                        reiniciarValores(mazo, cartasMesa, jugador, computadora, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada);
                        vaciarEnArchivo(mazo, cartasMesa,  jugador, computadora, reparte, ultimoEnRecogerPorEmparejamiento, ultimoEnRealizarJugada, ok);
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
        limpiarConsola(isMac);
    } while (opcionSeleccionada != '3');
    delete contadorCartasMesa;
    
    return 0;
}