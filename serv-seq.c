#include <unistd.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h> // --- DIFERE DA DOCUMENTACAO !!
#include <netdb.h>
#include <stdlib.h>

#define EMPTYLINE "\n"
#define PORTA     3456

//------------------------------------------------------------
int sPassivo,sAtivo;
struct sockaddr_in enderServ1;
struct sockaddr_in enderConex1;
int tamEnderConex;
char linha[80];
int i;
char Nome[64];
struct hostent *ptrDadosDoHost;
int result;

void wait(void) {
    sleep(2);

    printf("[Cliente] Processo cliente comecou a rodar.\n");
}

int main(void) {
    pthread_t thread;
    int err;

    err = pthread_create(&thread, NULL, wait, NULL);

    if(bind(sPassivo,(struct sockaddr *)&enderServ1,sizeof(enderServ1))){
       perror("[Servidor] Erro no bind: ");
       exit(0);
    }

    if(listen(sPassivo,10)){
       perror("[Servidor] Erro no listen: ");
       exit(0);
    }

    while(1){
         printf("[Servidor] (Vai esperar pedido de servico...)\n");
         sAtivo = accept(sPassivo,&enderConex1,&tamEnderConex);
         if(sAtivo<0){
            perror("[Servidor] Erro no Accept: ");
            exit(0);
         }
         if(read(sAtivo,linha,80)==-1){
            perror("[Servidor] Erro no read: ");
         }else{
            printf("[Servidor] Recebeu: %s\n",linha);
            sleep(2);
            strcpy(linha,"Pois nao. O servico foi prestado.");
            write(sAtivo,linha,strlen(linha));
         }
         shutdown(sAtivo,2);
    }

    return 0;
}
