/*----------------------------------------------------------------
|  Autor: Eduardo Ovalles Almonte     Matricula: 1-18-0795        |
|  Fecha: 20/12/2024                       Version:  C++   		  |
|-----------------------------------------------------------------|
|  Descripcion del Programa:Un programa  de gestion para una
 empresa de prestamos. Permite registrar clientes con su 
 informacion personal, administrar prestamos, procesar pagos y 
 generar un balance general de los clientes. Tambien incluye
  funciones para autenticación de usuario con clave, cambio de
   clave y manejo de datos.                                       |
| ---------------------------------------------------------------*/

//Bibliotecas
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Definicion de la estructura del cliente
typedef struct {
    int id;
    char nombre[50];
    char ciudad[50];
    char direccion[50];
    char cedula[20];
    char telefono[15];
    float monto_prestado;  
    float interes;         
    char fecha_prestamo[15];
} Cliente;

FILE *fClientes; // Archivo para almacenar la informacion de los clientes
char usuario[20] = "eduardo20", clave[20]; // Usuario y clave por defecto

// Declaracion de funciones
int login(); // Funcion para manejar el inicio de sesion del usuario
void menu_principal(); // Funcion para mostrar el menu principal 
void registrar_cliente(); // Funcion para registrar un nuevo cliente
void consultar_cliente(); // Funcion para consultar los datos de un cliente
void registrar_prestamo(); // Funcion para registrar un prestamo
void realizar_pago(); // Funcion para registrar  pagos 
void cambiar_clave(); // Funcion para cambiar la clave
void balance_general(); // Funcion para mostrar el balance general de los clientes
void guardar_cliente(Cliente c); // Funcion para guardar un cliente en el archivo
void actualizar_cliente(int id, void (*accion)(Cliente*)); // Funcion  para actualizar datos de un cliente
void cargar_clave(); // Funcion para cargar la clave desde un archivo
void guardar_clave(const char *nuevaClave); // Funcion para guardar una nueva clave

int main() {
    cargar_clave(); // Cargar la clave desde el archivo 
    if (login()) // Verificar  del usuario
        menu_principal(); // Mostrar menu principal si el login es exitoso
    else
        printf("Login incorrecto.\n");
    return 0;
}

// Funcion para cargar la clave desde un archivo 
void cargar_clave() {
    FILE *fClave = fopen("clave.dat", "r");// fopen para abrir archivo clave.dat y fclose para cerrarlo
    if (fClave) {
        fscanf(fClave, "%s", clave); // Leer clave almacenada en el archivo
        fclose(fClave);
    } else {
        strcpy(clave, "2028"); // Clave por defecto si el archivo no existe
        guardar_clave(clave);
    }
}

// Funcion para guardar una nueva clave en un archivo
void guardar_clave(const char *nuevaClave) {
    FILE *fClave = fopen("clave.dat", "w");
    if (fClave) {
        fprintf(fClave, "%s", nuevaClave); // Escribir la nueva clave en el archivo
        fclose(fClave);
    }
}

// Funcion de login para validar el usuario y la clave
int login() {
    char inputUsuario[20], inputClave[20];
    for (int intentos = 3; intentos > 0; --intentos) {
        printf("\nBienvenido a casa de empeno Ovalles\n");
        printf("\nUsuario: ");
        scanf("%s", inputUsuario);
        printf("Clave: ");
        scanf("%s", inputClave);
        if (!strcmp(inputUsuario, usuario) && !strcmp(inputClave, clave))
            return 1; // Login exitoso
        printf("Usuario o clave incorrecto. Intentos restantes: %d\n", intentos - 1);
    }
    return 0; // Login fallido
}

// Menu principal del sistema
void menu_principal() {
    int opcion;
    do {
        printf("\n1. Registrar Cliente\n2. Consultar Cliente\n3. Registrar Prestamo\n4. Realizar Pago\n5. Cambiar Clave\n6. Balance General\n7. Salir\nOpcion: ");
        scanf("%d", &opcion);
        switch (opcion) {
            case 1: registrar_cliente(); break; // Registrar cliente
            case 2: consultar_cliente(); break; // Consultar informacion del cliente
            case 3: registrar_prestamo(); break; // Registrar un prestamo 
            case 4: realizar_pago(); break; // Registrar pago 
            case 5: cambiar_clave(); break; // Cambiar la clave 
            case 6: balance_general(); break; // Mostrar balance general
            case 7: break; // Salir del sistema
            default: printf("Opcion incorrecta.\n");
        }
    } while (opcion != 7);
}

// Funcion para registrar un nuevo cliente
void registrar_cliente() {
    Cliente c = {0}; // Inicializando estructura de cliente
    printf("ID Cliente: ");
    scanf("%d", &c.id);
    printf("Nombre: ");
    scanf("%s", c.nombre);
    printf("Ciudad: ");
    scanf("%s", c.ciudad);
    printf("Direccion: ");
    scanf("%s", c.direccion);
    printf("Cedula: ");
    scanf("%s", c.cedula);
    printf("Telefono: ");
    scanf("%s", c.telefono);
    guardar_cliente(c); // Guardar informacion del cliente en el archivo
    printf("Cliente registrado.\n");
}

// Funcion para guardar la informacion de un cliente en el archivo
void guardar_cliente(Cliente c) {
    fClientes = fopen("clientes.dat", "ab");
    if (fClientes) {
        fwrite(&c, sizeof(Cliente), 1, fClientes); // Escribir el cliente en el archivo
        fclose(fClientes);
    } else {
        printf("Error al abrir el archivo.\n");
    }
}

// Funcion para consultar informacion de un cliente 
void consultar_cliente() {
    Cliente c;
    int id;
    printf("ID del cliente: ");
    scanf("%d", &id);
    fClientes = fopen("clientes.dat", "rb");
    if (fClientes) {
        while (fread(&c, sizeof(Cliente), 1, fClientes)) {
            if (c.id == id) {
                printf("ID: %d, Nombre: %s, Monto: %.2f\n", c.id, c.nombre, c.monto_prestado);
                fclose(fClientes);
                return;
            }
        }
        fclose(fClientes);
    }
    printf("Cliente no encontrado.\n");
}

// Funcion para actualizar la informacion de un cliente
void actualizar_cliente(int id, void (*accion)(Cliente *)) {
    Cliente c;
    int encontrado = 0;
    FILE *temp = fopen("temp.dat", "wb"); // Archivo temporal para almacenar actualizaciones
    fClientes = fopen("clientes.dat", "rb");
    if (fClientes && temp) {
        while (fread(&c, sizeof(Cliente), 1, fClientes)) {// fread lee datos binarios
            if (c.id == id) {
                accion(&c); // Realizar la accion especifica sobre el cliente
                encontrado = 1;
            }
            fwrite(&c, sizeof(Cliente), 1, temp);
        }
        fclose(fClientes);
        fclose(temp);
        remove("clientes.dat"); // Reemplazar el archivo original
        rename("temp.dat", "clientes.dat");
    }
    if (!encontrado) printf("Cliente no encontrado.\n");
}

// Funcion para registrar un prestamo para un cliente
void registrar_prestamo() {
    int id;
    float monto, tasa;
    printf("ID del cliente: ");
    scanf("%d", &id);
    printf("Monto: ");
    scanf("%f", &monto);
    printf("Tasa de interes (%%): ");
    scanf("%f", &tasa);

    void actualizar_monto_prestamo(Cliente *c) {
        c->monto_prestado += monto; // Incrementar el monto prestado
        c->interes += monto * tasa / 100; // Calcular e incrementar el interes
    }

    actualizar_cliente(id, actualizar_monto_prestamo);
    printf("Prestamo registrado.\n");
}

// Funcion para realizar un pago de un cliente
void realizar_pago() {
    int id;
    float pago;
    printf("ID del cliente: ");
    scanf("%d", &id);
    printf("Monto a pagar: ");
    scanf("%f", &pago);

    void procesar_pago(Cliente *c) {
        float deuda = c->monto_prestado + c->interes; // Calcular la deuda total
        if (pago >= deuda) {
            c->monto_prestado = 0; // Si el pago cubre toda la deuda, eliminarla
            c->interes = 0;
        } else {
            c->monto_prestado -= pago; // Reducir el monto prestado segun el pago
        }
    }

    actualizar_cliente(id, procesar_pago);
    printf("Pago realizado.\n");
}

// Funcion para cambiar la clave del sistema
void cambiar_clave() {
    char nuevaClave[20];
    printf("Nueva clave: ");
    scanf("%s", nuevaClave);
    guardar_clave(nuevaClave); // Guardar la nueva clave en el archivo
    printf("Clave cambiada.\n");
}

// Funcion para mostrar el balance general 
void balance_general() {
    Cliente c;
    fClientes = fopen("clientes.dat", "rb");
    if (fClientes) {
        while (fread(&c, sizeof(Cliente), 1, fClientes)) {
            printf("%d\t%s\t%.2f\t%.2f\n", c.id, c.nombre, c.monto_prestado, c.interes);
        }
        fclose(fClientes);
    } else {
        printf("Error al abrir el archivo.\n");
    }
}
