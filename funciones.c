// Funciones.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funciones.h"

void descripcionPrograma() {
    printf("\n--- Sistema de Gestion de Calidad del Aire ---\n");
    printf("Este programa permite monitorear, predecir y gestionar los niveles de contaminacion del aire.\n");
    mostrarRangosContaminantes();
}

void mostrarRangosContaminantes() {
    printf("\n--- Rangos de Calidad del Aire ---\n");
    printf("CO2 (ppm): Muy Bajo: 0-200 | Bajo: 201-400 | Normal: 401-600 | Alto: 601-800 | Muy Alto: >800\n");
    printf("SO2 (ppb): Muy Bajo: 0-10 | Bajo: 11-20 | Normal: 21-50 | Alto: 51-100 | Muy Alto: >100\n");
    printf("NO2 (ppb): Muy Bajo: 0-20 | Bajo: 21-40 | Normal: 41-80 | Alto: 81-120 | Muy Alto: >120\n");
    printf("PM2.5 (µg/m³): Muy Bajo: 0-12 | Bajo: 13-25 | Normal: 26-50 | Alto: 51-100 | Muy Alto: >100\n\n");
}


float validarEntrada(const char *nombre, float maximo, const char *rango) {
    float valor;
    do {
        printf("Ingrese el valor de %s (%s, Maximo permitido: %.2f): ", nombre, rango, maximo);
        scanf("%f", &valor);
        if (valor < 0 || valor > maximo) {
            printf("Valor invalido. Debe ser entre 0 y %.2f\n", maximo);
        }
    } while (valor < 0 || valor > maximo);
    return valor;
}


void leerDatosZona(char *nombreZona, float co2[], float so2[], float no2[], float pm25[]) {
    char nombreArchivo[50];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "%s.txt", nombreZona);
    FILE *archivo = fopen(nombreArchivo, "r");

    if (archivo) {
        for (int j = 0; j < 30; j++) {
            if (fscanf(archivo, "%f %f %f %f", &co2[j], &so2[j], &no2[j], &pm25[j]) != 4) {
                // Si no se leen 4 valores, inicializa los valores en 0
                co2[j] = so2[j] = no2[j] = pm25[j] = 0;
            }
        }
        fclose(archivo);
    } else {
        printf("No se encontro el archivo para %s. Inicializando con ceros.\n", nombreZona);
        for (int j = 0; j < 30; j++) {
            co2[j] = so2[j] = no2[j] = pm25[j] = 0;
        }
    }
}


void guardarDatosZona(char *nombreZona, float co2[], float so2[], float no2[], float pm25[]) {
    char nombreArchivo[50];
    snprintf(nombreArchivo, sizeof(nombreArchivo), "%s.txt", nombreZona);
    FILE *archivo = fopen(nombreArchivo, "w");

    if (archivo) {
        for (int j = 0; j < 35; j++) {
            fprintf(archivo, "%.2f %.2f %.2f %.2f\n", co2[j], so2[j], no2[j], pm25[j]);
        }
        fclose(archivo);
    } else {
        printf("Error al guardar los datos para %s.\n", nombreZona);
    }
}

void cargarDatos(Zona zonas[], int cantidadZonas) {
    const char *nombresZonas[] = {"QuitoNorte", "QuitoSur", "QuitoCentro", "Valle", "Tumbaco"};
    cantidadZonas = (cantidadZonas > ZONAS) ? ZONAS : cantidadZonas;

    for (int i = 0; i < cantidadZonas; i++) {
        strcpy(zonas[i].nombreZona, nombresZonas[i]);

        // Leer datos historicos desde archivo
        leerDatosZona(zonas[i].nombreZona, zonas[i].co2, zonas[i].so2, zonas[i].no2, zonas[i].pm25);

        printf("\nZona: %s\n", zonas[i].nombreZona);
        for (int j = 30; j < 35; j++) {
            printf("\n--- Datos para el Dia %d en %s ---\n", j + 1, zonas[i].nombreZona);
            zonas[i].co2[j] = validarEntrada("CO2 (ppm)", MAX_CO2, "Muy Bajo: 0-200 | Bajo: 200-400 | Normal: 400-600 | Alto: 600-800 | Muy Alto: >800");
            zonas[i].so2[j] = validarEntrada("SO2 (ppb)", MAX_SO2, "Muy Bajo: 0-10 | Bajo: 10-20 | Normal: 20-50 | Alto: 50-100 | Muy Alto: >100");
            zonas[i].no2[j] = validarEntrada("NO2 (ppb)", MAX_NO2, "Muy Bajo: 0-20 | Bajo: 20-40 | Normal: 40-80 | Alto: 80-120 | Muy Alto: >120");
            zonas[i].pm25[j] = validarEntrada("PM2.5 (µg/m³)", MAX_PM25, "Muy Bajo: 0-12 | Bajo: 12-25 | Normal: 25-50 | Alto: 50-100 | Muy Alto: >100");
        }

        // Guardar datos actualizados en el archivo
        guardarDatosZona(zonas[i].nombreZona, zonas[i].co2, zonas[i].so2, zonas[i].no2, zonas[i].pm25);
    }
}

float promedioPonderado(float datos[], int dias) {
    float suma = 0, pesoTotal = 0;
    for (int i = 0; i < dias; i++) {
        suma += datos[i] * (i + 1);
        pesoTotal += (i + 1);
    }
    return suma / pesoTotal;
}



void predecirContaminacion(Zona zonas[], int cantidadZonas) {
    printf("\n--- Prediccion de Contaminacion ---\n");

    for (int i = 0; i < cantidadZonas; i++) {
        // Filtrar valores mayores a 1000 antes de calcular la prediccion
        for (int j = 0; j < 35; j++) {
            if (zonas[i].co2[j] > 1000) zonas[i].co2[j] = 0;
            if (zonas[i].so2[j] > 1000) zonas[i].so2[j] = 0;
            if (zonas[i].no2[j] > 1000) zonas[i].no2[j] = 0;
            if (zonas[i].pm25[j] > 1000) zonas[i].pm25[j] = 0;
        }

        // Calcular predicciones ponderadas
        float prediccionCO2 = promedioPonderado(zonas[i].co2, 35);
        float prediccionSO2 = promedioPonderado(zonas[i].so2, 35);
        float prediccionNO2 = promedioPonderado(zonas[i].no2, 35);
        float prediccionPM25 = promedioPonderado(zonas[i].pm25, 35);

        // Imprimir predicciones
        printf("Zona: %s\n", zonas[i].nombreZona);
        printf("Prediccion CO2: %.2f ppm\n", prediccionCO2);
        printf("Prediccion SO2: %.2f ppb\n", prediccionSO2);
        printf("Prediccion NO2: %.2f ppb\n", prediccionNO2);
        printf("Prediccion PM2.5: %.2f µg/m³\n", prediccionPM25);
    }
}


void monitorearContaminacion(Zona zonas[], int cantidadZonas) {
    printf("\n--- Monitoreo de Contaminacion ---\n");

    for (int i = 0; i < cantidadZonas; i++) {
        printf("\nZona: %s\n", zonas[i].nombreZona);
        printf("%-10s %-10s %-10s %-10s %-10s\n", "Dia", "CO2", "SO2", "NO2", "PM2.5");

        for (int j = 0; j < 35; j++) {
            float co2 = (zonas[i].co2[j] > 1000) ? 0 : zonas[i].co2[j];
            float so2 = (zonas[i].so2[j] > 1000) ? 0 : zonas[i].so2[j];
            float no2 = (zonas[i].no2[j] > 1000) ? 0 : zonas[i].no2[j];
            float pm25 = (zonas[i].pm25[j] > 1000) ? 0 : zonas[i].pm25[j];

            printf("%-10d %-10.2f %-10.2f %-10.2f %-10.2f\n", j + 1, co2, so2, no2, pm25);
        }

        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        printf("\nPromedios de la Zona %s:\n", zonas[i].nombreZona);
        printf("CO2: %.2f ppm\n", promedioCO2);
        printf("SO2: %.2f ppb\n", promedioSO2);
        printf("NO2: %.2f ppb\n", promedioNO2);
        printf("PM2.5: %.2f µg/m³\n", promedioPM25);
    }

    // Determinar las zonas con mayor y menor contaminacion por componente
    float maxCO2 = -1, minCO2 = 1e6;
    float maxSO2 = -1, minSO2 = 1e6;
    float maxNO2 = -1, minNO2 = 1e6;
    float maxPM25 = -1, minPM25 = 1e6;

    char maxCO2Zona[50], minCO2Zona[50];
    char maxSO2Zona[50], minSO2Zona[50];
    char maxNO2Zona[50], minNO2Zona[50];
    char maxPM25Zona[50], minPM25Zona[50];

    for (int i = 0; i < cantidadZonas; i++) {
        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        if (promedioCO2 > maxCO2) {
            maxCO2 = promedioCO2;
            strcpy(maxCO2Zona, zonas[i].nombreZona);
        }
        if (promedioCO2 < minCO2) {
            minCO2 = promedioCO2;
            strcpy(minCO2Zona, zonas[i].nombreZona);
        }

        if (promedioSO2 > maxSO2) {
            maxSO2 = promedioSO2;
            strcpy(maxSO2Zona, zonas[i].nombreZona);
        }
        if (promedioSO2 < minSO2) {
            minSO2 = promedioSO2;
            strcpy(minSO2Zona, zonas[i].nombreZona);
        }

        if (promedioNO2 > maxNO2) {
            maxNO2 = promedioNO2;
            strcpy(maxNO2Zona, zonas[i].nombreZona);
        }
        if (promedioNO2 < minNO2) {
            minNO2 = promedioNO2;
            strcpy(minNO2Zona, zonas[i].nombreZona);
        }

        if (promedioPM25 > maxPM25) {
            maxPM25 = promedioPM25;
            strcpy(maxPM25Zona, zonas[i].nombreZona);
        }
        if (promedioPM25 < minPM25) {
            minPM25 = promedioPM25;
            strcpy(minPM25Zona, zonas[i].nombreZona);
        }
    }

    printf("\n--- Zonas con Mayor y Menor Contaminacion ---\n");
    printf("CO2: Mayor: %s (%.2f ppm), Menor: %s (%.2f ppm)\n", maxCO2Zona, maxCO2, minCO2Zona, minCO2);
    printf("SO2: Mayor: %s (%.2f ppb), Menor: %s (%.2f ppb)\n", maxSO2Zona, maxSO2, minSO2Zona, minSO2);
    printf("NO2: Mayor: %s (%.2f ppb), Menor: %s (%.2f ppb)\n", maxNO2Zona, maxNO2, minNO2Zona, minNO2);
    printf("PM2.5: Mayor: %s (%.2f µg/m³), Menor: %s (%.2f µg/m³)\n", maxPM25Zona, maxPM25, minPM25Zona, minPM25);
}


float calcularPromedio(float datos[], int dias) {
    float suma = 0;
    int count = 0;

    for (int i = 0; i < dias; i++) {
        if (datos[i] <= 1000) { // Solo sumar valores válidos
            suma += datos[i];
            count++;
        }
    }
    return (count > 0) ? suma / count : 0; // Evitar division por 0
}


void calcularMejorPeorZona(Zona zonas[], int cantidadZonas) {
    float mejorCO2 = 1e6, peorCO2 = -1;
    float mejorSO2 = 1e6, peorSO2 = -1;
    float mejorNO2 = 1e6, peorNO2 = -1;
    float mejorPM25 = 1e6, peorPM25 = -1;

    char mejorZonaCO2[50], peorZonaCO2[50];
    char mejorZonaSO2[50], peorZonaSO2[50];
    char mejorZonaNO2[50], peorZonaNO2[50];
    char mejorZonaPM25[50], peorZonaPM25[50];

    for (int i = 0; i < cantidadZonas; i++) {
        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        if (promedioCO2 < mejorCO2) {
            mejorCO2 = promedioCO2;
            strcpy(mejorZonaCO2, zonas[i].nombreZona);
        }
        if (promedioCO2 > peorCO2) {
            peorCO2 = promedioCO2;
            strcpy(peorZonaCO2, zonas[i].nombreZona);
        }

        if (promedioSO2 < mejorSO2) {
            mejorSO2 = promedioSO2;
            strcpy(mejorZonaSO2, zonas[i].nombreZona);
        }
        if (promedioSO2 > peorSO2) {
            peorSO2 = promedioSO2;
            strcpy(peorZonaSO2, zonas[i].nombreZona);
        }

        if (promedioNO2 < mejorNO2) {
            mejorNO2 = promedioNO2;
            strcpy(mejorZonaNO2, zonas[i].nombreZona);
        }
        if (promedioNO2 > peorNO2) {
            peorNO2 = promedioNO2;
            strcpy(peorZonaNO2, zonas[i].nombreZona);
        }

        if (promedioPM25 < mejorPM25) {
            mejorPM25 = promedioPM25;
            strcpy(mejorZonaPM25, zonas[i].nombreZona);
        }
        if (promedioPM25 > peorPM25) {
            peorPM25 = promedioPM25;
            strcpy(peorZonaPM25, zonas[i].nombreZona);
        }
    }

    printf("\n--- Mejor y Peor Zona por Contaminante ---\n");
    printf("CO2: Mejor Zona: %s (%.2f ppm), Peor Zona: %s (%.2f ppm)\n", mejorZonaCO2, mejorCO2, peorZonaCO2, peorCO2);
    printf("SO2: Mejor Zona: %s (%.2f ppb), Peor Zona: %s (%.2f ppb)\n", mejorZonaSO2, mejorSO2, peorZonaSO2, peorSO2);
    printf("NO2: Mejor Zona: %s (%.2f ppb), Peor Zona: %s (%.2f ppb)\n", mejorZonaNO2, mejorNO2, peorZonaNO2, peorNO2);
    printf("PM2.5: Mejor Zona: %s (%.2f µg/m³), Peor Zona: %s (%.2f µg/m³)\n", mejorZonaPM25, mejorPM25, peorZonaPM25, peorPM25);
}

void exportarDatos(Zona zonas[], int cantidadZonas) {
    FILE *archivo = fopen("reporte_contaminacion.txt", "w");
    if (!archivo) {
        printf("Error al crear el archivo.\n");
        return;
    }
    fprintf(archivo, "Reporte de Contaminacion\n");
    for (int i = 0; i < cantidadZonas; i++) {
        fprintf(archivo, "Zona: %s\n", zonas[i].nombreZona);
        fprintf(archivo, "Promedio CO2: %.2f ppm\n", calcularPromedio(zonas[i].co2, 35));
        fprintf(archivo, "Promedio SO2: %.2f ppb\n", calcularPromedio(zonas[i].so2, 35));
        fprintf(archivo, "Promedio NO2: %.2f ppb\n", calcularPromedio(zonas[i].no2, 35));
        fprintf(archivo, "Promedio PM2.5: %.2f µg/m³\n", calcularPromedio(zonas[i].pm25, 35));
    }
    fclose(archivo);
    printf("Datos exportados a reporte_contaminacion.txt\n");
}

void calcularPromediosHistoricos(Zona zonas[], int cantidadZonas) {
    printf("\n--- Promedios Historicos ---\n");
    for (int i = 0; i < cantidadZonas; i++) {
        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        printf("Zona: %s\n", zonas[i].nombreZona);
        printf("Promedio CO2: %.2f ppm\n", promedioCO2);
        printf("Promedio SO2: %.2f ppb\n", promedioSO2);
        printf("Promedio NO2: %.2f ppb\n", promedioNO2);
        printf("Promedio PM2.5: %.2f µg/m³\n", promedioPM25);
    }
}

void emitirAlertas(Zona zonas[], int cantidadZonas) {
    printf("\n--- Alertas de Contaminacion ---\n");

    for (int i = 0; i < cantidadZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombreZona);

        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        // Alertas según los niveles de contaminacion
        if (promedioCO2 > 600) {
            printf("  - CO2: %.2f ppm -> ", promedioCO2);
            if (promedioCO2 <= 800) {
                printf("ALERTA: Niveles altos. Limitar tráfico vehicular y promover transporte público.\n");
            } else {
                printf("ALERTA URGENTE: Niveles muy altos. Prohibir actividades industriales temporalmente.\n");
            }
        }

        if (promedioSO2 > 50) {
            printf("  - SO2: %.2f ppb -> ", promedioSO2);
            if (promedioSO2 <= 100) {
                printf("ALERTA: Niveles altos. Restringir emisiones de fábricas y refinerias.\n");
            } else {
                printf("ALERTA URGENTE: Niveles muy altos. Parar operaciones de plantas industriales.\n");
            }
        }

        if (promedioNO2 > 80) {
            printf("  - NO2: %.2f ppb -> ", promedioNO2);
            if (promedioNO2 <= 120) {
                printf("ALERTA: Niveles altos. Reducir emisiones vehiculares con restricciones de circulacion.\n");
            } else {
                printf("ALERTA URGENTE: Niveles muy altos. Prohibir actividades al aire libre.\n");
            }
        }

        if (promedioPM25 > 50) {
            printf("  - PM2.5: %.2f µg/m³ -> ", promedioPM25);
            if (promedioPM25 <= 100) {
                printf("ALERTA: Niveles altos. Restringir actividades al aire libre y quemas agricolas.\n");
            } else {
                printf("ALERTA URGENTE: Niveles muy altos. Recomendacion de uso de mascarillas y cierre de escuelas.\n");
            }
        }

        if (promedioCO2 <= 600 && promedioSO2 <= 50 && promedioNO2 <= 80 && promedioPM25 <= 50) {
            printf("Sin alertas para esta zona. Calidad del aire dentro de limites aceptables.\n");
        }

        printf("\n");
    }
}


void generarRecomendaciones(Zona zonas[], int cantidadZonas) {
    printf("\n--- Recomendaciones para Mejorar la Calidad del Aire ---\n");

    for (int i = 0; i < cantidadZonas; i++) {
        printf("Zona: %s\n", zonas[i].nombreZona);

        float promedioCO2 = calcularPromedio(zonas[i].co2, 35);
        float promedioSO2 = calcularPromedio(zonas[i].so2, 35);
        float promedioNO2 = calcularPromedio(zonas[i].no2, 35);
        float promedioPM25 = calcularPromedio(zonas[i].pm25, 35);

        // Recomendaciones especificas según niveles de contaminacion
        if (promedioCO2 > 600) {
            printf("  - CO2: %.2f ppm -> ", promedioCO2);
            if (promedioCO2 <= 800) {
                printf("Recomendacion: Incentivar uso de transporte público y bicicletas.\n");
            } else {
                printf("Recomendacion URGENTE: Implementar cierre temporal de industrias y restriccion vehicular total.\n");
            }
        }

        if (promedioSO2 > 50) {
            printf("  - SO2: %.2f ppb -> ", promedioSO2);
            if (promedioSO2 <= 100) {
                printf("Recomendacion: Monitorear emisiones de fábricas y mejorar tecnologia de filtrado.\n");
            } else {
                printf("Recomendacion URGENTE: Suspender temporalmente operaciones industriales de alto impacto.\n");
            }
        }

        if (promedioNO2 > 80) {
            printf("  - NO2: %.2f ppb -> ", promedioNO2);
            if (promedioNO2 <= 120) {
                printf("Recomendacion: Reducir horarios pico vehiculares y promover energias limpias.\n");
            } else {
                printf("Recomendacion URGENTE: Suspender actividades al aire libre y aplicar restricciones estrictas al tráfico.\n");
            }
        }

        if (promedioPM25 > 50) {
            printf("  - PM2.5: %.2f µg/m³ -> ", promedioPM25);
            if (promedioPM25 <= 100) {
                printf("Recomendacion: Evitar quemas agricolas y promover campañas de forestacion urbana.\n");
            } else {
                printf("Recomendacion URGENTE: Distribuir mascarillas y restringir actividades al aire libre por completo.\n");
            }
        }

        if (promedioCO2 <= 600 && promedioSO2 <= 50 && promedioNO2 <= 80 && promedioPM25 <= 50) {
            printf("- Excelente calidad del aire. Continuar con politicas actuales de mitigacion.\n");
        }

        printf("\n");
    }
}