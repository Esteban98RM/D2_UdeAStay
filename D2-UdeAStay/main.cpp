#include "sistema.h"

#include <iostream>

using namespace std;

int main() {
    Sistema sistema;
    sistema.cargarDatos();

    string documento, clave;

    cout << "Sistema de Autenticacion\n";
    cout << "Usuario: ";
    cin >> documento;
    cout << "Clave: ";
    cin >> clave;

    sistema.iniciarSesion(documento, clave);

    return 0;
}
