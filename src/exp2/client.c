#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> /* Necessário para o inet_addr */
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Correção 2: A porta deve bater exatamente com a do servidor */
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
       fprintf(stderr, "Uso: ./cliente <IP_DO_SERVIDOR>\n");
       exit(1);
    }

    /* constrói a estrutura de dados do endereço */
    bzero((char *)&sin, sizeof(sin));
    /* Correção 1: Usando AF_INET */
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
    
     printf("Conectado ao IP %s na porta %d!\nDigite suas mensagens:\n", ip_address, SERVER_PORT);

    /* loop principal: obtém e envia linhas de texto */
    while (fgets(buf, sizeof(buf), stdin)) {
         buf[MAX_LINE-1] = '\0';
         len = strlen(buf) + 1;
        /* Aqui envia para o servidor */
         send(s, buf, len, 0);

        /* ALTERAÇÕES EXERCICIO 33: ESPERA A RESPOSTA DO SERVIDOR*/
        int received = recv(s, buf, sizeof(buf), 0);
        if (received>0){
 	   printf("Resposta do Servidor: %s", buf);
        } else if (received == 0) {
	    printf("Servidor sem conexão.\n");
	    break;
         } else {
	     perror("Erro no recv");
             break;
    }
  
    }
    close(s);
    return 0;
}
