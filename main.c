#include <stdio.h>
#include "Funciones.h"

int main() {
    Zona zonas[ZONAS];
    int opcion;
    int datosIngresados = 0; // Bandera para controlar si los datos han sido ingresados

    descripcionPrograma();

    do {
        printf("\n--- Menu Principal ---\n");
        printf("1. Ingresar datos\n");
        printf("2. Monitorear contaminacion\n");
        printf("3. Predecir contaminacion\n");
        printf("4. Emitir alertas\n");
        printf("5. Calcular promedios historicos\n");
        printf("6. Generar recomendaciones\n");
        printf("7. Exportar datos\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);

        // Verificar si el usuario ha ingresado datos antes de usar otras opciones
        if (opcion != 1 && opcion != 0 && !datosIngresados) {
            printf("\nDebe ingresar datos primero seleccionando la opcion 1.\n");
            continue;
        }

        switch (opcion) {
            case 1:
                cargarDatos(zonas, ZONAS);
                datosIngresados = 1; // Activar la bandera
                break;
            case 2:
                monitorearContaminacion(zonas, ZONAS);
                break;
            case 3:
                predecirContaminacion(zonas, ZONAS);
                break;
            case 4:
                emitirAlertas(zonas, ZONAS);
                break;
            case 5:
                calcularPromediosHistoricos(zonas, ZONAS);
                break;
            case 6:
                generarRecomendaciones(zonas, ZONAS);
                break;
            case 7:
                exportarDatos(zonas, ZONAS);
                break;
            case 0:
                printf("Saliendo...\n");
                break;
            default:
                printf("Opcion invalida\n");
        }
    } while (opcion != 0);

    return 0;
}
