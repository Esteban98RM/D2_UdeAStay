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

/**
 * @brief Cuenta la cantidad de líneas en un archivo de texto.
 *
 * Esta función abre el archivo especificado y cuenta cuántas líneas contiene,
 * incrementando el contador de iteraciones por cada línea leída.
 *
 * @param nombreArchivo Nombre del archivo a analizar.
 * @return int Número total de líneas en el archivo. Retorna 0 si no se puede abrir.
 */
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

/**
 * @brief Carga los datos de anfitriones desde un archivo y los almacena en un arreglo dinámico.
 *
 * Esta función abre el archivo "Anfitriones.txt", cuenta cuántos anfitriones hay,
 * reserva memoria dinámica para almacenarlos, y luego carga los datos parseando cada línea.
 * Además, imprime en consola los datos cargados de cada anfitrión.
 *
 * @param anfitriones Referencia al puntero donde se almacenará el arreglo dinámico de anfitriones.
 * @param cantidad Referencia al entero donde se guardará la cantidad total de anfitriones cargados.
 */
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

/**
 * @brief Carga los datos de los huéspedes desde el archivo "Huespedes.txt".
 *
 * Lee el archivo línea por línea, reserva memoria dinámica para almacenar cada huésped,
 * parsea los datos separados por el carácter '|' y almacena los objetos `Huesped` en un arreglo.
 * También imprime los datos cargados en consola para verificación.
 *
 * @param huespedes Referencia al puntero donde se almacenará el arreglo dinámico de huéspedes.
 * @param cantidad Referencia al entero donde se guardará la cantidad de huéspedes cargados.
 */
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

/**
 * @brief Carga los datos de los alojamientos desde el archivo "Alojamientos.txt".
 *
 * Procesa cada línea del archivo, reserva memoria dinámica para los alojamientos,
 * parsea los campos separados por '|', y construye objetos `Alojamiento` que se almacenan
 * en un arreglo. Los datos incluyen información de ubicación, precio, tipo y amenidades.
 * Imprime en consola los datos cargados para verificación.
 *
 * @param alojamientos Referencia al puntero donde se almacenará el arreglo dinámico de alojamientos.
 * @param cantidad Referencia al entero donde se guardará la cantidad de alojamientos cargados.
 */
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

/**
 * @brief Carga los datos de las reservaciones desde el archivo "Reservaciones.txt".
 *
 * Lee el archivo línea a línea, reserva memoria dinámica, parsea los datos separados por '|',
 * y construye objetos `Reservacion` que se almacenan en un arreglo. Incluye datos como
 * fechas, duración, alojamiento, método de pago y monto. El método de pago se interpreta
 * como carácter para uso interno, pero se imprime su valor textual original.
 *
 * @param reservaciones Referencia al puntero donde se almacenará el arreglo dinámico de reservaciones.
 * @param cantidad Referencia al entero donde se guardará la cantidad de reservaciones cargadas.
 */
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
        // fecha_pago, monto, anotacion);

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

/**
 * @brief Guarda una nueva reservación en el archivo "Reservaciones.txt".
 *
 * Este método abre el archivo en modo append y escribe la información de la reservación
 * en una línea utilizando el carácter '|' como delimitador de campos.
 *
 * @param reservacion Objeto de tipo Reservacion que contiene los datos a guardar.
 */
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

/**
 * @brief Sobrescribe el archivo "Reservaciones.txt" con todas las reservaciones actuales.
 *
 * Este método reescribe completamente el archivo, por lo tanto, se debe usar con cuidado
 * para evitar pérdida de datos. Se utiliza generalmente cuando se actualiza o elimina una reservación.
 *
 * @param reservaciones Arreglo de objetos Reservacion a escribir en el archivo.
 * @param cantidad Número de elementos presentes en el arreglo de reservaciones.
 */
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

/**
 * @brief Guarda un conjunto de reservaciones finalizadas en el archivo histórico.
 *
 * Este método agrega una sección al final del archivo "HistoricoReservaciones.txt"
 * con información sobre la fecha de corte y la cantidad de reservaciones almacenadas,
 * seguida por el detalle de cada reservación.
 *
 * @param reservaciones Arreglo de reservaciones que se desea guardar en el histórico.
 * @param cantidad Número de reservaciones a guardar.
 * @param fechaCorte Fecha de corte que indica hasta cuándo se consideran finalizadas las reservaciones.
 * @return true Si el archivo fue abierto y escrito correctamente.
 * @return false Si no se pudo abrir el archivo.
 */
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

/**
 * @brief Convierte un carácter que representa el método de pago a una cadena descriptiva.
 *
 * Convierte el carácter del método de pago a una versión corta usada para guardar en archivos.
 *
 * @param metodo Carácter que representa el método de pago ('T' para Tarjeta, 'P' para PSE).
 * @return string Cadena abreviada del método de pago ("TC" o "PSE").
 */
string GestionArchivos::formatearMetodoPago(char metodo) {
    switch (metodo) {
    incrementarIteracion();
    case 'T': return "TC";  // Tarjeta de Credito
    case 'P': return "PSE"; // PSE
    default: return "TC";   // Por defecto
    }
}

/**
 * @brief Carga el último ID de reservación almacenado en el archivo "ultimo_id.txt".
 *
 * Este ID se utiliza para asignar un nuevo código único a una nueva reservación.
 *
 * @return int El último ID de reservación leído del archivo. Si el archivo no existe, retorna 0.
 */
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

/**
 * @brief Guarda el último ID de reservación en el archivo "ultimo_id.txt".
 *
 * Esta función se utiliza para persistir el identificador más reciente y asegurar
 * la continuidad en la generación de nuevos códigos de reservación.
 *
 * @param id El ID de reservación que se desea guardar.
 */
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

/**
 * @brief Carga la última fecha de corte usada en el histórico de reservaciones.
 *
 * Lee desde el archivo "fecha_corte_historico.txt" la última fecha hasta la cual se procesaron
 * reservaciones finalizadas.
 *
 * @return string Fecha en formato string. Si el archivo no existe o está vacío, retorna cadena vacía.
 */
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

/**
 * @brief Guarda la fecha de corte más reciente en el archivo "fecha_corte_historico.txt".
 *
 * Esta fecha es utilizada como referencia para el próximo procesamiento del histórico
 * de reservaciones.
 *
 * @param fecha Fecha de corte que se desea guardar.
 */
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
