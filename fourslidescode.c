#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <stdio.h>

void imprimeDePipe(int leePipe); // Funcionalidad para el hijo
void enviaAPipe(int escribePipe); // Funcionalidad para el padre

int main() {
    pid_t procHijo;
    int pipeFileDescriptors[2]; // Descriptores de ambos extremos

    if (pipe(pipeFileDescriptors) == -1) { // Genera pipe
        printf("Error al crear pipe\n");
        exit(1);
    }

    procHijo = fork(); // Genera proceso hijo
    if (procHijo < 0) {
        int errnum = errno; // Preservamos código de error
        printf("Error %d al generar proceso hijo con fork\n", errnum);
        exit(1);
    }

    if (procHijo == 0) {
        // Es el hijo, cierra pipe de envío y procede
        close(pipeFileDescriptors[1]); // Cierra el extremo de escritura
        imprimeDePipe(pipeFileDescriptors[0]);
    } else {
        // Es el padre
        close(pipeFileDescriptors[0]); // Cierra el extremo de lectura
        enviaAPipe(pipeFileDescriptors[1]);
    }

    return 0; // Finaliza main correctamente
}

void imprimeDePipe(int leePipe) {
    char buf;
    printf("Proceso hijo, esperando cadena...\n");
    while (read(leePipe, &buf, 1) > 0) {
        write(STDOUT_FILENO, &buf, 1);
    }
    close(leePipe);
    printf("\nProceso hijo, finalizado\n");
    exit(0);
}

void enviaAPipe(int escribePipe) {
    char buf[10];
    printf("Proceso padre, ingresa una cadena de 10 caracteres y enter:\n");
    scanf("%10c", buf); // Lee hasta 10 caracteres
    printf("\n\n");
    write(escribePipe, buf, strlen(buf)); // Envía la cadena al hijo
    close(escribePipe);
    wait(NULL); // Espera a que el hijo termine
    printf("Hijo terminado, terminando proceso padre\n");
    exit(0);
}
