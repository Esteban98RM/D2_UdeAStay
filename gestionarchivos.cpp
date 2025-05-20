#include "gestionArchivos.h"
#include "anfitrion.h"
#include "huesped.h"
#include "alojamiento.h"
#include "reservacion.h"

#include <iostream>
#include <fstream>
#include <sstream>

GestionArchivos::GestionArchivos() {}

int GestionArchivos::contarLineas(const string& nombreArchivo) {
    ifstream archivo(nombreArchivo);
    if (!archivo.is_open()) return 0;

    int contador = 0;
    string linea;

    while (getline(archivo, linea)) {
        contador++;
    }

    return contador;
}

void GestionArchivos::cargarAnfitriones(Anfitrion *&anfitriones, int &cantidad){
    const string nombreArchivo = "Anfitriones.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    anfitriones = new Anfitrion[total];
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE ANFITRIONES CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string codigo, documento, clave, antiguedadStr, puntuacionStr;

        getline(ss, codigo, '|');
        getline(ss, documento, '|');
        getline(ss, clave, '|');
        getline(ss, antiguedadStr, '|');
        getline(ss, puntuacionStr);


        int antiguedad = stoi(antiguedadStr);
        float puntuacion = stof(puntuacionStr);

        //anfitriones[i++] = Anfitrion(codigo, documento, clave, antiguedad, puntuacion);

        anfitriones[i] = Anfitrion(codigo, documento, clave, antiguedad, puntuacion);

        cout << "Anfitrion #" << (i + 1) << ":" << endl;
        cout << "  - Codigo: " << codigo << endl;
        cout << "  - Documento: " << documento << endl;
        cout << "  - Clave: " << clave << endl;
        cout << "  - Antiguedad: " << antiguedad << endl;
        cout << "  - Puntuacion: " << puntuacion << endl;
        cout << "----------------------------------" << endl;

        i++;
    }
    cantidad = i;
}

void GestionArchivos::cargarHuespedes(Huesped*& huespedes, int& cantidad) {
    const string nombreArchivo = "Huespedes.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    huespedes = new Huesped[total];
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE HUESPEDES CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string documento, clave, antiguedadStr, puntuacionStr;

        getline(ss, documento, '|');
        getline(ss, clave, '|');
        getline(ss, antiguedadStr, '|');
        getline(ss, puntuacionStr);

        int antiguedad = stoi(antiguedadStr);
        float puntuacion = stof(puntuacionStr);

        //huespedes[i++] = Huesped(documento, clave, antiguedad, puntuacion);

        huespedes[i] = Huesped(documento, clave, antiguedad, puntuacion);

        // Impresión de verificación
        cout << "Huesped #" << (i + 1) << ":" << endl;
        cout << "  - Documento: " << documento << endl;
        cout << "  - Clave: " << clave << endl;
        cout << "  - Antiguedad: " << antiguedad << endl;
        cout << "  - Puntuacion: " << puntuacion << endl;
        cout << "----------------------------------" << endl;

        i++;
    }

    cantidad = i;
}

void GestionArchivos::cargarAlojamientos(Alojamiento*& alojamientos, int& cantidad) {
    const string nombreArchivo = "Alojamientos.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    alojamientos = new Alojamiento[total];
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE ALOJAMIENTOS CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string codigo, nombre, documento, departamento, municipio, tipoStr,
            direccion, precioStr, amenidades;

        getline(ss, codigo, '|');
        getline(ss, nombre, '|');
        getline(ss, documento, '|');
        getline(ss, departamento, '|');
        getline(ss, municipio, '|');
        getline(ss, tipoStr, '|');
        getline(ss, direccion, '|');
        getline(ss, precioStr, '|');
        getline(ss, amenidades);

        char tipo = '\0';
        if (tipoStr == "A" || tipoStr == "C") {
            tipo = tipoStr[0];
        }
        float precio = stof(precioStr);

        //alojamientos[i++] = Alojamiento(codigo, nombre, documento, departamento, municipio,
        //tipo, direccion, precio, amenidades);

        alojamientos[i] = Alojamiento(codigo, nombre, documento, departamento, municipio,
                                      tipo, direccion, precio, amenidades);

        // Impresión de verificacion
        cout << "Alojamiento #" << (i + 1) << ":" << endl;
        cout << "  - Codigo: " << codigo << endl;
        cout << "  - Nombre: " << nombre << endl;
        cout << "  - Documento anfitrion: " << documento << endl;
        cout << "  - Departamento: " << departamento << endl;
        cout << "  - Municipio: " << municipio << endl;
        cout << "  - Tipo: " << tipo << endl;
        cout << "  - Direccion: " << direccion << endl;
        cout << "  - Precio: " << precio << endl;
        cout << "  - Amenidades: " << amenidades << endl;
        cout << "----------------------------------" << endl;

        i++;
    }

    cantidad = i;
}

void GestionArchivos::cargarReservaciones(Reservacion*& reservaciones, int& cantidad) {
    const string nombreArchivo = "Reservaciones.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    reservaciones = new Reservacion[total];
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE RESERVACION CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        istringstream ss(linea);
        string codigo, fecha_entrada, duracion_estadiaStr, codigo_alojamiento,
            documento, metodo_pagoStr, fecha_pago, montoStr, anotacion;

        getline(ss, codigo, '|');
        getline(ss, fecha_entrada, '|');
        getline(ss, duracion_estadiaStr, '|');
        getline(ss, codigo_alojamiento, '|');
        getline(ss, documento, '|');
        getline(ss, metodo_pagoStr, '|');
        getline(ss, fecha_pago, '|');
        getline(ss, montoStr, '|');
        getline(ss, anotacion);

        char metodo_pago = '\0';
        string metodo_mostrar = metodo_pagoStr; // Para mostrar el texto completo

        if (metodo_pagoStr == "TC" || metodo_pagoStr == "PSE") {
            metodo_pago = metodo_pagoStr[0];
        }

        int duracion_estadia = stoi(duracion_estadiaStr);
        float monto = stof(montoStr);

        //reservaciones[i++] = Reservacion(codigo, fecha_entrada, duracion_estadia, codigo_alojamiento, docuemento,
        //metodo_pago, fecha_pago, monto, anotacion);

        reservaciones[i] = Reservacion(codigo, fecha_entrada, duracion_estadia,codigo_alojamiento, documento, metodo_pago,
                                       fecha_pago, monto, anotacion);

        // Formatear el monto para mostrar sin notación científica
        ostringstream montoFormateado;
        montoFormateado << fixed << setprecision(0) << monto;

        // Impresión de verificación mejorada
        cout << "Reservacion #" << (i + 1) << ":" << endl;
        cout << "  - Codigo: " << codigo << endl;
        cout << "  - Fecha entrada: " << fecha_entrada << endl;
        cout << "  - Duracion estadia: " << duracion_estadia << endl;
        cout << "  - Codigo alojamiento: " << codigo_alojamiento << endl;
        cout << "  - Documento huesped: " << documento << endl;
        cout << "  - Metodo: " << metodo_mostrar << endl; // Mostramos el string completo
        cout << "  - Fecha pago: " << fecha_pago << endl;
        cout << "  - Monto: " << montoFormateado.str() << endl; // Monto formateado
        cout << "  - Anotacion: " << anotacion << endl;
        cout << "----------------------------------" << endl;

        i++;
    }

    cantidad = i;
}
