#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

/* Correção 2: Porta alterada de 5432 para 54321 */
#define SERVER_PORT 54321 
#define MAX_PENDING 5
#define MAX_LINE 256

int main() {
    struct sockaddr_in sin;
    char buf[MAX_LINE];
    int buf_len;
    socklen_t addr_len;
    int s, new_s;

    /* construindo a estrutura de dados do endereço */
    bzero((char *)&sin, sizeof(sin));
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(SERVER_PORT);
    
    /* CORREÇÃO: Criando o socket antes de usar o bind */
    if ((s = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("simplex-talk: socket");
        exit(1);
    }

    /* configura a abertura passiva */
    if ((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0) {
        perror("simplex-talk: bind");
        exit(1);
    }

    /* CORREÇÃO: Colocando o socket em modo de escuta */
    if (listen(s, MAX_PENDING) < 0) {
        perror("simplex-talk: listen");
        exit(1);
    }

    /* espera pela conexão, então recebe e imprime o texto */
    while(1){
        /* Correção 3: Inicializando addr_len antes do accept */
        addr_len = sizeof(sin); 
        
        if((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0) {
            perror("simplex-talk: accept");
            exit(1);
        }
        
        while((buf_len = recv(new_s, buf, sizeof(buf), 0))) {
            fputs(buf, stdout);
           /* ALTERAÇÕES EXERCICIO 33: Envia a mesma linha
           de volta para o cliente*/
            if (send(new_s, buf, buf_len,0) <0){
            perror("Erro ao tentar enviar de volta");
            break;
        }
      }        
        close(new_s);
    }
    
    return 0;
}
