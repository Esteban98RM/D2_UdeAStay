#include "sistema.h"

#include <iostream>

using namespace std;

int main() {
    Sistema sistema;

    sistema.cargarDatos();

    string documento, clave, tipoRol;

    cout << "Sistema de Autenticacion\n";
    cout << "Usuario: ";
    cin >> documento;
    cout << "Clave: ";
    cin >> clave;

    if (sistema.autenticarUsuario(documento, clave, tipoRol)) {
        cout << "\nBienvenido " << tipoRol << "!\n";
    } else {
        cout << "\nCredenciales incorrectas\n";
    }

    return 0;
}
