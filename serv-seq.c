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

//------------------------------------------------------------
void wait(void) {
    sleep(2);

    printf("[Cliente] Processo cliente comecou a rodar.\n");
}

//------------------------------------------------------------
int main(void) {
    pthread_t thread;
    int err;

    err = pthread_create(&thread, NULL, wait, NULL);

    for(i=1;i<5;i++)printf(EMPTYLINE);
    printf("[Servidor] Servidor SEQ-SEQ entrando em funcionamento.\n    Usando a porta %d.\n",PORTA);

    result = gethostname(Nome,sizeof(Nome));
    printf("    gethostname:\n    result = %d , Nome : %s.\n",result,Nome);
    ptrDadosDoHost = (struct hostent *)gethostbyname(Nome);
    printf("Tamanho do nome: %d\n",ptrDadosDoHost->h_length);

    sPassivo = socket(PF_INET,SOCK_STREAM,0);
    if(sPassivo == -1){
       perror("[Servidor] Erro na criacao do sock 1 do servidor: ");
       exit(0);
    }

    enderServ1.sin_family = AF_INET;
    enderServ1.sin_port   = htons(PORTA);
    bcopy(ptrDadosDoHost->h_addr,
          &enderServ1.sin_addr.s_addr, 
          ptrDadosDoHost->h_length);

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
         if (read(sAtivo,linha,80)==-1){
            perror("[Servidor] Erro no read: ");
         if (err){
            printf("[Servidor] Erro no listen: %d", err);
            return 1;
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
