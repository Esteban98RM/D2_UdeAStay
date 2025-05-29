#include "fecha.h"
#include "medicionrecursos.h"

#include <sstream>
#include <iomanip>
#include <ctime>

Fecha::Fecha() : dia(0), mes(0), anio(0) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
}

Fecha::Fecha(int d, int m, int a) : dia(d), mes(m), anio(a) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
}

Fecha::Fecha(const string& fechaStr) {
    incrementarIteracion();
    agregarMemoria(sizeof(Fecha));
    char sep1, sep2;
    stringstream ss(fechaStr);
    if (!(ss >> dia >> sep1 >> mes >> sep2 >> anio) || sep1 != '/' || sep2 != '/' || !esValida()) {
        dia = mes = anio = 0;
    }
}

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

string Fecha::toString() const {
    stringstream ss;
    ss << setw(2) << setfill('0') << dia << "/"
       << setw(2) << setfill('0') << mes << "/"
       << anio;
    return ss.str();
}

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

string Fecha::obtenerFechaActual() {
    time_t tiempoActual = time(0);
    tm* tiempoLocal = localtime(&tiempoActual);

    ostringstream oss;
    oss << setfill('0') << setw(2) << tiempoLocal->tm_mday << "/"
        << setfill('0') << setw(2) << (tiempoLocal->tm_mon + 1) << "/"
        << (tiempoLocal->tm_year + 1900);
    return oss.str();
}

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

Fecha Fecha::calcularFechaFinal(int noches) const {
    return this->sumarDias(noches - 1); // Reutiliza sumarDias
}

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

bool Fecha::operator==(const Fecha& otra) const {
    incrementarIteracion();
    return dia == otra.dia && mes == otra.mes && anio == otra.anio;
}

bool Fecha::operator<(const Fecha& otra) const {
    incrementarIteracion();
    return this->aDiasDesdeFechaBase() < otra.aDiasDesdeFechaBase();
}

bool Fecha::operator>(const Fecha& otra) const {
    return otra < *this;
}

bool Fecha::operator<=(const Fecha& otra) const {
    return !(*this > otra);
}

bool Fecha::operator>=(const Fecha& otra) const {
    return !(*this < otra);
}

ostream& operator<<(ostream& os, const Fecha& f) {
    os << f.toString();
    return os;
}

istream& operator>>(istream& is, Fecha& f) {
    string str;
    is >> str;
    f = Fecha(str);
    return is;
}

