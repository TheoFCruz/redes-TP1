#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> 
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#define SERVER_PORT 54321
#define MAX_LINE 256

int main(int argc, char * argv[]) {
    struct sockaddr_in sin;
    char *ip_address;
    char buf[MAX_LINE];
    int s;
    int len;

    if (argc == 2) {
        ip_address = argv[1];
    } else {
        fprintf(stderr, "No host address provided. Usage: exp*_client xxx.xxx.xxx.xxx\n");
        exit(1);
    }

    /* constrói a estrutura de dados do endereço */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;

    /* Correção 4: Lê direto o IP em dotted notation e converte para binário */
    sin.sin_addr.s_addr = inet_addr(ip_address);
    if (sin.sin_addr.s_addr == INADDR_NONE) {
        fprintf(stderr, "Erro: Endereço IP inválido.\n");
        exit(1);
    }

    sin.sin_port = htons(SERVER_PORT);

    /* abertura ativa */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }
    if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) {
        perror("simplex-talk: connect");
        close(s);
        exit(1);
    }

    printf("Connected to %s:%d\n", ip_address, SERVER_PORT);

    /* loop principal: obtém e envia linhas de texto */
    while (fgets(buf, sizeof(buf), stdin)) {
        struct timespec start, end;
        buf[MAX_LINE-1] = '\0';
        len = strlen(buf) + 1;
        clock_gettime(CLOCK_MONOTONIC, &start);

        /* send message to the server */
        send(s, buf, len, 0);

        /* EXERCICIO 3: ESPERA A RESPOSTA DO SERVIDOR*/
        int received = recv(s, buf, sizeof(buf), 0);
        if (received>0){
            double elapsed_ms;
            clock_gettime(CLOCK_MONOTONIC, &end);

            /* prints answer and elapsed time */
            elapsed_ms = (end.tv_sec - start.tv_sec) * 1000.0;
            elapsed_ms += (end.tv_nsec - start.tv_nsec) / 1000000.0;
            printf("Server Answer: %s", buf);
            printf("Elapsed Time: %.3f ms\n", elapsed_ms);
        } else if (received == 0) {
            printf("Server error.\n");
            break;
        } else {
            perror("recv error.\n");
            break;
        }
    }
    close(s);
    return 0;
}
