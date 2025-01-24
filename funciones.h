#ifndef FUNCIONES_H
#define FUNCIONES_H

#define ZONAS 5
#define DIAS 5  

#define LIMITE_CO2 400   
#define LIMITE_SO2 20    
#define LIMITE_NO2 40    
#define LIMITE_PM25 25   

#define MAX_CO2 1000
#define MAX_SO2 500
#define MAX_NO2 500
#define MAX_PM25 500

typedef struct {
    char nombreZona[50];
    float co2[DIAS];
    float so2[DIAS];
    float no2[DIAS];
    float pm25[DIAS];
} Zona;

void descripcionPrograma();
void mostrarRangosContaminantes();
void leerDatosZona(char *nombreZona, float co2[], float so2[], float no2[], float pm25[]);
void guardarDatosZona(char *nombreZona, float co2[], float so2[], float no2[], float pm25[]);
void cargarDatos(Zona zonas[], int cantidadZonas);
void monitorearContaminacion(Zona zonas[], int cantidadZonas);
void mostrarRangosContaminantes();
void predecirContaminacion(Zona zonas[], int cantidadZonas);
void emitirAlertas(Zona zonas[], int cantidadZonas);
void generarRecomendaciones(Zona zonas[], int cantidadZonas);
void exportarDatos(Zona zonas[], int cantidadZonas);
float promedioPonderado(float datos[], int dias);
float calcularPromedio(float datos[], int dias);
void calcularPromediosHistoricos(Zona zonas[], int cantidadZonas);


float validarEntrada(const char *nombre, float maximo, const char *rango);
#endif