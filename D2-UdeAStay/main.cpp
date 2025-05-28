#include "sistema.h"

#include <iostream>

using namespace std;

int main() {
    Sistema sistema;
    sistema.cargarDatos();

    bool salirPrograma = false;

    while (!salirPrograma) {
        string documento, clave;
        bool sesionIniciada = false;

        cout << "\n=========================================\n";
        cout << "        SISTEMA DE AUTENTICACION\n";
        cout << "=========================================\n";

        while (!sesionIniciada) {
            cout << "\nIngrese sus credenciales\n";
            cout << "Usuario (documento): ";
            cin >> documento;
            cout << "Clave: ";
            cin >> clave;

            sesionIniciada = sistema.iniciarSesion(documento, clave);

            if (!sesionIniciada) {
                cout << "\nCredenciales incorrectas. Intentelo nuevamente.\n";
            }
        }
    }

    return 0;
}
