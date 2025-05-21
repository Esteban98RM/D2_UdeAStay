#ifndef RESERVACION_H
#define RESERVACION_H

#include <string>

using namespace std;

class Reservacion {
private:

    string codigo;
    string fecha_entrada;
    int duracion_estadia;
    string codigo_alojamiento;
    string documento;
    char metodo_pago;
    string fecha_pago;
    float monto;

    char* anotacion;
    size_t capacidad_anotacion;

    void redimensionarAnotaciones(size_t nueva_capacidad);
    void copiarAnotaciones(const char* nuevas_anotaciones, size_t longitud);

public:

    //Constructor
    Reservacion();
    Reservacion(const string& cod, const string& f_entrada, int duracion,
                const string& cod_aloja, const string& doc_huesped, char metodo,
                const string& f_pago, float monto_reserva, const string& notas);

    // Constructor de copia
    Reservacion(const Reservacion& otro);

    // Sobrecarga del operador de asignación
    Reservacion& operator=(const Reservacion& otro);

    //Destructor
    ~Reservacion();

    // Metodos de acceso
    const string& getCodigo() const { return codigo; }
    const string& getFechaEntrada() const { return fecha_entrada; }
    int getDuracion() const { return duracion_estadia; }
    const string& getCodigoAlojamiento() const { return codigo_alojamiento; }
    const string& getDocumento() const { return documento; }
    char getMetodoPago() const { return metodo_pago; }
    const string& getFechaPago() const { return fecha_pago; }
    float getMonto() const { return monto; }
    string getAnotacion() const;

    // Metodos para modificar
    void setAnotacion(const string& nueva_anotacion);

    void reservarEspacio(size_t nueva_capacidad);
    void copiarAnotacion(const char* nueva_anotacion);
};

#endif // RESERVACION_H
