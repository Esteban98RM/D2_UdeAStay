#include "gestionarchivos.h"
#include "anfitrion.h"
#include "huesped.h"
#include "alojamiento.h"
#include "reservacion.h"
#include "medicionrecursos.h"

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
        incrementarIteracion();
        contador++;
    }

    return contador;
}

void GestionArchivos::cargarAnfitriones(Anfitrion *&anfitriones, int &cantidad){
    const string nombreArchivo = "Anfitriones.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    anfitriones = new Anfitrion[total];
    agregarMemoria(sizeof(Anfitrion) * total);
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE ANFITRIONES CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        incrementarIteracion();
        istringstream ss(linea);
        string codigo, documento, clave, antiguedadStr, puntuacionStr;

        getline(ss, codigo, '|');
        getline(ss, documento, '|');
        getline(ss, clave, '|');
        getline(ss, antiguedadStr, '|');
        getline(ss, puntuacionStr);


        int antiguedad = stoi(antiguedadStr);
        float puntuacion = stof(puntuacionStr);

        anfitriones[i++] = Anfitrion(codigo, documento, clave, antiguedad, puntuacion);

        // anfitriones[i] = Anfitrion(codigo, documento, clave, antiguedad, puntuacion);

        // cout << "Anfitrion #" << (i + 1) << ":" << endl;
        // cout << "  - Codigo: " << codigo << endl;
        // cout << "  - Documento: " << documento << endl;
        // cout << "  - Clave: " << clave << endl;
        // cout << "  - Antiguedad: " << antiguedad << endl;
        // cout << "  - Puntuacion: " << puntuacion << endl;
        // cout << "----------------------------------" << endl;

        // i++;
    }
    cantidad = i;
}

void GestionArchivos::cargarHuespedes(Huesped*& huespedes, int& cantidad) {
    const string nombreArchivo = "Huespedes.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    huespedes = new Huesped[total];
    agregarMemoria(sizeof(Huesped) * total);
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE HUESPEDES CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        incrementarIteracion();
        istringstream ss(linea);
        string nombre, documento, clave, antiguedadStr, puntuacionStr;

        getline(ss, nombre, '|');
        getline(ss, documento, '|');
        getline(ss, clave, '|');
        getline(ss, antiguedadStr, '|');
        getline(ss, puntuacionStr);

        int antiguedad = stoi(antiguedadStr);
        float puntuacion = stof(puntuacionStr);

        huespedes[i++] = Huesped(nombre, documento, clave, antiguedad, puntuacion);

        // huespedes[i] = Huesped(nombre, documento, clave, antiguedad, puntuacion);

        // // Impresion de verificacion
        // cout << "Huesped #" << (i + 1) << ":" << endl;
        // cout << "  - Nombre: " << nombre << endl;
        // cout << "  - Documento: " << documento << endl;
        // cout << "  - Clave: " << clave << endl;
        // cout << "  - Antiguedad: " << antiguedad << endl;
        // cout << "  - Puntuacion: " << puntuacion << endl;
        // cout << "----------------------------------" << endl;

        // i++;
    }

    cantidad = i;
}

void GestionArchivos::cargarAlojamientos(Alojamiento*& alojamientos, int& cantidad) {
    const string nombreArchivo = "Alojamientos.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    alojamientos = new Alojamiento[total];
    agregarMemoria(sizeof(Alojamiento) * total);
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE ALOJAMIENTOS CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        incrementarIteracion();
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

        alojamientos[i++] = Alojamiento(codigo, nombre, documento, departamento, municipio,
        tipo, direccion, precio, amenidades);

        // alojamientos[i] = Alojamiento(codigo, nombre, documento, departamento, municipio,
        //                               tipo, direccion, precio, amenidades);

        // // Impresion de verificacion
        // cout << "Alojamiento #" << (i + 1) << ":" << endl;
        // cout << "  - Codigo: " << codigo << endl;
        // cout << "  - Nombre: " << nombre << endl;
        // cout << "  - Documento anfitrion: " << documento << endl;
        // cout << "  - Departamento: " << departamento << endl;
        // cout << "  - Municipio: " << municipio << endl;
        // cout << "  - Tipo: " << tipo << endl;
        // cout << "  - Direccion: " << direccion << endl;
        // cout << "  - Precio: " << precio << endl;
        // cout << "  - Amenidades: " << amenidades << endl;
        // cout << "----------------------------------" << endl;

        // i++;
    }

    cantidad = i;
}

void GestionArchivos::cargarReservaciones(Reservacion*& reservaciones, int& cantidad) {
    const string nombreArchivo = "Reservaciones.txt";
    int total = contarLineas(nombreArchivo);

    if (total == 0) return;

    reservaciones = new Reservacion[total];
    agregarMemoria(sizeof(Reservacion) * total);
    ifstream archivo(nombreArchivo);
    string linea;
    int i = 0;

    cout << "=== DATOS DE RESERVACION CARGADOS ===" << endl;

    while (getline(archivo, linea)) {
        incrementarIteracion();
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

        reservaciones[i++] = Reservacion(codigo, fecha_entrada, duracion_estadia, codigo_alojamiento, documento,
        metodo_pago, fecha_pago, monto, anotacion);

        // reservaciones[i] = Reservacion(codigo, fecha_entrada, duracion_estadia,codigo_alojamiento, documento, metodo_pago,
        //                                fecha_pago, monto, anotacion);

        // // Formatear el monto para mostrar sin notacion científica
        // ostringstream montoFormateado;
        // montoFormateado << fixed << setprecision(0) << monto;

        // // Impresion de verificacion mejorada
        // cout << "Reservacion #" << (i + 1) << ":" << endl;
        // cout << "  - Codigo: " << codigo << endl;
        // cout << "  - Fecha entrada: " << fecha_entrada << endl;
        // cout << "  - Duracion estadia: " << duracion_estadia << endl;
        // cout << "  - Codigo alojamiento: " << codigo_alojamiento << endl;
        // cout << "  - Documento huesped: " << documento << endl;
        // cout << "  - Metodo: " << metodo_mostrar << endl; // Mostramos el string completo
        // cout << "  - Fecha pago: " << fecha_pago << endl;
        // cout << "  - Monto: " << montoFormateado.str() << endl; // Monto formateado
        // cout << "  - Anotacion: " << anotacion << endl;
        // cout << "----------------------------------" << endl;

        // i++;
    }

    cantidad = i;
}

void GestionArchivos::guardarReservacion(const Reservacion& reservacion) {
    ofstream archivo("Reservaciones.txt", ios::app);
    if (!archivo.is_open()) {
        incrementarIteracion();
        cout << "Error: No se pudo abrir el archivo para guardar la reservacion.\n";
        return;
    }

    // Formatear método de pago para archivo
    string metodoPagoStr = formatearMetodoPago(reservacion.getMetodoPago());

    // Escribir en formato específico
    archivo << reservacion.getCodigo() << "|"
            << reservacion.getFechaEntrada() << "|"
            << reservacion.getDuracion() << "|"
            << reservacion.getCodigoAlojamiento() << "|"
            << reservacion.getDocumento() << "|"
            << metodoPagoStr << "|"
            << reservacion.getFechaPago() << "|"
            << fixed << setprecision(0) << reservacion.getMonto() << "|"
            << reservacion.getAnotacion() << "\n";

    archivo.close();
    cout << "Reservacion guardada exitosamente en el archivo.\n";
}

void GestionArchivos::actualizarArchivoReservaciones(Reservacion* reservaciones, int cantidad) {
    ofstream archivo("Reservaciones.txt");
    if (!archivo.is_open()) {
        incrementarIteracion();
        cout << "Error: No se pudo actualizar el archivo de reservaciones.\n";
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        incrementarIteracion();
        string metodoPagoStr = formatearMetodoPago(reservaciones[i].getMetodoPago());

        archivo << reservaciones[i].getCodigo() << "|"
                << reservaciones[i].getFechaEntrada() << "|"
                << reservaciones[i].getDuracion() << "|"
                << reservaciones[i].getCodigoAlojamiento() << "|"
                << reservaciones[i].getDocumento() << "|"
                << metodoPagoStr << "|"
                << reservaciones[i].getFechaPago() << "|"
                << fixed << setprecision(0) << reservaciones[i].getMonto() << "|"
                << reservaciones[i].getAnotacion();

        if (i < cantidad - 1) archivo << "\n";
    }

    archivo.close();
}

bool GestionArchivos::guardarReservacionesHistorico(Reservacion* reservaciones, int cantidad, const string& fechaCorte) {
    // Abrir archivo historico en modo append
    ofstream archivo("HistoricoReservaciones.txt", ios::app);
    if (!archivo.is_open()) {
        incrementarIteracion();
        cout << "Error: No se pudo abrir el archivo historico.\n";
        return false;
    }

    // Agregar marca de tiempo y fecha de corte de la actualizacion
    archivo << "# === ACTUALIZACION HISTORICO ===\n";
    archivo << "# Fecha de procesamiento: " << Fecha::obtenerFechaActual() << "\n";
    archivo << "# Fecha de corte: " << fechaCorte << "\n";  // Usar string directamente
    archivo << "# Reservaciones finalizadas: " << cantidad << "\n";
    archivo << "# ================================\n";

    // Guardar reservaciones finalizadas
    for (int i = 0; i < cantidad; i++) {
        incrementarIteracion();
        string metodoPagoStr = formatearMetodoPago(reservaciones[i].getMetodoPago());
        archivo << reservaciones[i].getCodigo() << "|"
                << reservaciones[i].getFechaEntrada() << "|"
                << reservaciones[i].getDuracion() << "|"
                << reservaciones[i].getCodigoAlojamiento() << "|"
                << reservaciones[i].getDocumento() << "|"
                << metodoPagoStr << "|"
                << reservaciones[i].getFechaPago() << "|"
                << fixed << setprecision(0) << reservaciones[i].getMonto() << "|"
                << reservaciones[i].getAnotacion() << "\n";
    }

    archivo << "# === FIN ACTUALIZACION ===\n\n";
    archivo.close();
    return true;
}

string GestionArchivos::formatearMetodoPago(char metodo) {
    switch (metodo) {
    incrementarIteracion();
    case 'T': return "TC";  // Tarjeta de Credito
    case 'P': return "PSE"; // PSE
    default: return "TC";   // Por defecto
    }
}

int GestionArchivos::cargarUltimoIdReservacion() {
    incrementarIteracion();
    ifstream archivo("ultimo_id.txt");
    int id = 0;
    if (archivo) {
        archivo >> id;
        agregarMemoria(sizeof(id));
    }  // Si no existe, retorna 0
    return id;
}

void GestionArchivos::guardarUltimoIdReservacion(int id) {
    incrementarIteracion();
    ofstream archivo("ultimo_id.txt");
    if (archivo) {
        archivo << id;
        agregarMemoria(sizeof(id));
    } else {
        cerr << "Error al guardar el ultimo ID de reservacion.\n";
    }
}

string GestionArchivos::cargarUltimaFechaCorteHistorico() {
    incrementarIteracion();
    ifstream archivo("fecha_corte_historico.txt");
    string fecha = "";
    if (archivo) {
        archivo >> fecha;
        agregarMemoria(fecha.capacity());
    }
    // Si no existe archivo o esta vacio, retorna cadena vacia
    archivo.close();
    return fecha;
}

void GestionArchivos::guardarUltimaFechaCorteHistorico(const string& fecha) {
    incrementarIteracion();
    ofstream archivo("fecha_corte_historico.txt");
    if (archivo) {
        archivo << fecha;
        agregarMemoria(fecha.size());
        archivo.close();
    } else {
        cerr << "Error al guardar la fecha de corte del historico.\n";
    }
}
