#include "fecha.h"
#include "medicionrecursos.h"

#include <sstream>
#include <iomanip>
#include <ctime>

/**
 * @brief Constructor por defecto de la clase Fecha.
 *
 * Inicializa la fecha con valores inválidos (0/0/0).
 * También actualiza la contabilidad de memoria e iteraciones.
 */
Fecha::Fecha() : dia(0), mes(0), anio(0) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
}

/**
 * @brief Constructor con parámetros de día, mes y año.
 *
 * @param d Día del mes.
 * @param m Mes del año.
 * @param a Año completo.
 *
 * Asume que los valores dados son válidos.
 * La validación puede hacerse por separado mediante el método `esValida()`.
 */
Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
}

/**
 * @brief Constructor que inicializa una fecha a partir de un string en formato "DD/MM/AAAA".
 *
 * @param fechaStr Cadena de texto que representa una fecha.
 *
 * Si el formato es incorrecto o la fecha es inválida, la fecha se inicializa a 0/0/0.
 */
Fecha::Fecha(const string& fechaStr) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
    char sep1, sep2;
    stringstream ss(fechaStr);
    if (!(ss >> dia >> sep1 >> mes >> sep2 >> anio) || sep1 != '/' || sep2 != '/' || !esValida()) {
        dia = mes = anio = 0;
    }
}

/**
 * @brief Verifica si la fecha actual es válida.
 *
 * @return true si la fecha es válida, false si no lo es.
 *
 * Se considera la cantidad de días por mes y los años bisiestos.
 */
bool Fecha::esValida() const {
    incrementarIteracion();
    if (dia <= 0 || mes <= 0 || anio <= 0) return false;
    if (mes > 12 || dia > 31) return false;

    int diasMes[] = {31,28,31,30,31,30,31,31,30,31,30,31};
    if (mes == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0)) {
        return dia <= 29;
    }
    return dia <= diasMes[mes - 1];
}

/**
 * @brief Verifica si la fecha representada ya ha pasado en relación con la fecha actual del sistema.
 *
 * @return true si la fecha actual del objeto es anterior a la fecha del sistema.
 * @return false si la fecha es hoy o una fecha futura.
 *
 * Se basa en la hora local del sistema para comparar año, mes y día.
 */
bool Fecha::yaPaso() const {
    incrementarIteracion();
    time_t t = time(nullptr);
    tm* now = localtime(&t);

    int hoyDia = now->tm_mday;
    int hoyMes = now->tm_mon + 1;
    int hoyAnio = now->tm_year + 1900;

    if (anio < hoyAnio) return true;
    if (anio == hoyAnio && mes < hoyMes) return true;
    if (anio == hoyAnio && mes == hoyMes && dia < hoyDia) return true;

    return false;
}

/**
 * @brief Convierte la fecha a una cadena de texto en formato "DD/MM/AAAA".
 *
 * @return Cadena con la fecha formateada.
 *
 * Utiliza ceros a la izquierda para los valores de día y mes si son menores a 10.
 */
string Fecha::toString() const {
    stringstream ss;
    ss << setw(2) << setfill('0') << dia << "/"
       << setw(2) << setfill('0') << mes << "/"
       << anio;
    return ss.str();
}

/**
 * @brief Devuelve la fecha en un formato textual más completo y amigable.
 *
 * @return Cadena en formato "[día de la semana], [D] de [mes] del [AAAA]".
 *
 * Por ejemplo: "martes, 28 de mayo del 2024".
 * Calcula el día de la semana usando la fórmula de Zeller.
 */
string Fecha::toStringCompleto() const {
    string nombresDias[] = {"domingo", "lunes", "martes", "miercoles", "jueves", "viernes", "sabado"};
    string nombresMeses[] = {"enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto",
                             "septiembre", "octubre", "noviembre", "diciembre"};

    int d = dia;
    int m = mes;
    int a = anio;

    if (m < 3) {
        m += 12;
        a--;
    }

    int diaSemana = (d + (13 * (m + 1)) / 5 + a + a/4 - a/100 + a/400) % 7;
    diaSemana = (diaSemana + 5) % 7; // Ajuste para que domingo = 0

    ostringstream oss;
    oss << nombresDias[diaSemana] << ", " << dia << " de "
        << nombresMeses[mes - 1] << " del " << anio;

    return oss.str();
}

/**
 * @brief Obtiene la fecha actual del sistema en formato "DD/MM/AAAA".
 *
 * @return Cadena con la fecha actual según la hora local del sistema.
 */
string Fecha::obtenerFechaActual() {
    time_t tiempoActual = time(0);
    tm* tiempoLocal = localtime(&tiempoActual);

    ostringstream oss;
    oss << setfill('0') << setw(2) << tiempoLocal->tm_mday << "/"
        << setfill('0') << setw(2) << (tiempoLocal->tm_mon + 1) << "/"
        << (tiempoLocal->tm_year + 1900);
    return oss.str();
}

/**
 * @brief Suma una cantidad de días a la fecha actual y devuelve el resultado.
 *
 * @param diasSumar Número de días a sumar (debe ser >= 0).
 * @return Nueva fecha resultado de la suma.
 *
 * Tiene en cuenta los años bisiestos y cambios de mes y año.
 */
Fecha Fecha::sumarDias(int diasSumar) const {
    incrementarIteracion();
    int d = dia, m = mes, a = anio;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    while (diasSumar > 0) {
        if (m == 2 && ((a % 4 == 0 && a % 100 != 0) || a % 400 == 0))
            diasMes[1] = 29;
        else
            diasMes[1] = 28;

        int diasRestantesMes = diasMes[m - 1] - d + 1;
        if (diasSumar < diasRestantesMes) {
            d += diasSumar;
            diasSumar = 0;
        } else {
            diasSumar -= diasRestantesMes;
            d = 1;
            m++;
            if (m > 12) {
                m = 1;
                a++;
            }
        }
    }
    return Fecha(d, m, a);
}

/**
 * @brief Calcula la fecha final considerando una duración en noches.
 *
 * @param noches Número de noches (debe ser >= 1).
 * @return Fecha de salida (fecha actual + noches - 1 días).
 *
 * Por ejemplo, si se entra el 1 y se reservan 3 noches, la fecha final será el 3.
 */
Fecha Fecha::calcularFechaFinal(int noches) const {
    return this->sumarDias(noches - 1); // Reutiliza sumarDias
}

/**
 * @brief Convierte la fecha actual en el número de días transcurridos desde el 01/01/1900.
 *
 * @return Número entero que representa los días desde el 01/01/1900.
 *
 * Este método se puede usar para comparar fechas o calcular diferencias.
 * Considera años bisiestos.
 */
// Usaremos algoritmo simple para contar días desde 01/01/1900
int Fecha::aDiasDesdeFechaBase() const {
    incrementarIteracion();
    int totalDias = 0;
    int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    // Contar días completos por años
    for (int y = 1900; y < anio; y++) {
        totalDias += (y % 4 == 0 && (y % 100 != 0 || y % 400 == 0)) ? 366 : 365;
    }

    // Contar días completos por meses del año actual
    for (int m = 1; m < mes; m++) {
        if (m == 2 && ((anio % 4 == 0 && anio % 100 != 0) || anio % 400 == 0))
            totalDias += 29;
        else
            totalDias += diasMes[m - 1];
    }

    // Sumar días restantes del mes
    totalDias += dia - 1; // -1 porque 01 cuenta como día 0

    return totalDias;
}

/**
 * @brief Compara si dos fechas son iguales.
 *
 * @param otra Fecha con la que se compara.
 * @return true si ambas fechas tienen el mismo día, mes y año.
 * @return false en caso contrario.
 */
bool Fecha::operator==(const Fecha& otra) const {
    incrementarIteracion();
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

/**
 * @brief Compara si la fecha actual es menor que otra.
 *
 * La comparación se hace con base en la cantidad de días desde una fecha base.
 *
 * @param otra Fecha con la que se compara.
 * @return true si la fecha actual es anterior a la fecha dada.
 * @return false en caso contrario.
 */
bool Fecha::operator<(const Fecha& otra) const {
    incrementarIteracion();
    return this->aDiasDesdeFechaBase() < otra.aDiasDesdeFechaBase();
}

/**
 * @brief Compara si la fecha actual es mayor que otra.
 *
 * @param otra Fecha con la que se compara.
 * @return true si la fecha actual es posterior a la fecha dada.
 * @return false en caso contrario.
 */
bool Fecha::operator>(const Fecha& otra) const {
    return otra < *this;
}

/**
 * @brief Compara si la fecha actual es menor o igual que otra.
 *
 * @param otra Fecha con la que se compara.
 * @return true si la fecha actual es anterior o igual a la fecha dada.
 * @return false en caso contrario.
 */
bool Fecha::operator<=(const Fecha& otra) const {
    return !(*this > otra);
}

/**
 * @brief Compara si la fecha actual es mayor o igual que otra.
 *
 * @param otra Fecha con la que se compara.
 * @return true si la fecha actual es posterior o igual a la fecha dada.
 * @return false en caso contrario.
 */
bool Fecha::operator>=(const Fecha& otra) const {
    return !(*this < otra);
}

/**
 * @brief Sobrecarga del operador de salida para mostrar una fecha como cadena.
 *
 * @param os Flujo de salida.
 * @param f Fecha a mostrar.
 * @return Referencia al flujo de salida con la fecha formateada.
 */
ostream& operator<<(ostream& os, const Fecha& f) {
    os << f.toString();
    return os;
}

/**
 * @brief Sobrecarga del operador de entrada para leer una fecha desde una cadena.
 *
 * El formato de entrada debe ser compatible con el constructor de Fecha que acepta un string.
 *
 * @param is Flujo de entrada.
 * @param f Fecha donde se almacenará el valor leído.
 * @return Referencia al flujo de entrada.
 */
istream& operator>>(istream& is, Fecha& f) {
    string str;
    is >> str;
    f = Fecha(str);
    return is;
}

