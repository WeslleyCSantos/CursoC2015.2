#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define INVALID_SOCKET -1

int create_socket(void);
int connect_socket(int sockfd, char* ip, int port, int timeout_recv);
void disconnect_socket(int sockfd);
void destroy_socket(int sockfd);
int send_socket(int sockfd, char* buffer, int to_send);
int recv_socket(int sockfd, char* buffer, int to_read);
int keyboard_read(char* message, char* buffer, int to_read);

int main(int argc, char *argv[])
{
	initscr();
	clear();
	scrollok(stdscr, TRUE);
	char IP_ADDR[11] = {0}, buffer[256] = {0}, REC_MEN[5000] = {0};
	int PORT_NO;
	move(0, 0);
	printw("Informe o IP do servidor: ");
	refresh();
	scanw("%s", IP_ADDR);
	move(1, 0);
	printw("Informe o numero da Porta: ");
	refresh();
	scanw("%d", &PORT_NO);
	move(2, 0);
	printw("Informe seu Nickname: ");
	refresh();
	scanw("%s", buffer);
	
    int sockfd = create_socket();

    if (sockfd != INVALID_SOCKET)
    {
        if (connect_socket(sockfd, IP_ADDR, PORT_NO, 10))
        {
        	clear();
        	move(0, 0);
        	printw("Conectado ao servidor!\n\n");
           	printw("Nick: %s\n\n", buffer);
       		refresh();
            int n;
			n = send_socket(sockfd, buffer, strlen(buffer));
            if(n >= 0)
            {
           		char op;
            	while(1)
            	{
            		printw("1 - Para enviar Mensagem!\n");
            		printw("2 - Visualizar Mensagens!\n");
            		printw("3 - Sair do Chat!\n");
            		scanw("%c", &op);
            		refresh();
            		clear();
            		if(op=='1')
            		{
   			            printw("Envio de mensagem: ");
       		      		refresh();
				        keyboard_read("", buffer, 255);
				        strcat(REC_MEN, "Eu ~ ");
				        strcat(REC_MEN, buffer);
   				        strcat(REC_MEN, "\n");
				        n = send_socket(sockfd, buffer, strlen(buffer));            		
		        	    if (n >= 0)
				        {
				            printw("Mensagem enviada!\n\n");
				        }
				        else
				        {
				            printw("ERRO no envio da mensagem!\n");
				        }
				        refresh();
				    }
				    else if(op=='2')
				    {
				        memset((void*) buffer, 0, 256);
			            n = recv_socket(sockfd, buffer, 256);
			            if(strlen(buffer)>0)
			            {
				        	strcat(REC_MEN, buffer);
   	   				        strcat(REC_MEN, "\n");
						    if (n >= 0)
						    {
						        printw("Mensagens Recebidas: %s", REC_MEN);
						    }
						    else
						    {
						        printw("ERRO na leitura da mensagem!\n");
						    }
							refresh();
						}
						else
						{
							printw("Mensagens Recebidas: %s\n", REC_MEN);
						}
			        }
			        else if(op=='3')
			        {
			        	break;
			        }
			        if(strlen(REC_MEN)>=4999)
			        {
			        	strcpy(REC_MEN, "\0");
			        }
            	}
            }
            else
            {
               printw("ERRO no envio do Nickname!\n");
            }
            refresh();
        }
        else
        {
            printw("ERRO na conexao do Chat!\n");
        }
        disconnect_socket(sockfd);
        printw("Desconectando...\n");
        refresh();
    }
    else
    {
        printw("ERRO na inicializacao do Chat!\n");
    }
	refresh();
    destroy_socket(sockfd);
    printw("Pressione qualquer tecla para continuar...\n");
    getch();
	endwin();
    return 0;
}

int create_socket(void)
{
    return socket(AF_INET, SOCK_STREAM, 0);
}

int connect_socket(int sockfd, char* ip, int port, int timeout_recv)
{
    int result = 0;

    if ((sockfd != INVALID_SOCKET) && ip)
    {
        struct sockaddr_in serv_addr = {0};
        struct timeval tv = {0};

        tv.tv_sec = 1;

        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (char*) &tv, sizeof(struct timeval));

        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = inet_addr(ip);
        serv_addr.sin_port = htons(port);

        result = connect(sockfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) >= 0;
    }
    return result;
}

void disconnect_socket(int sockfd)
{
    if (sockfd != INVALID_SOCKET)
    {
        close(sockfd);
    }
}

void destroy_socket(int sockfd)
{

}

int send_socket(int sockfd, char* buffer, int to_send)
{
    int sent = 0;
    if ((sockfd != INVALID_SOCKET) && buffer && (to_send > 0))
    {
        sent = send(sockfd, buffer, to_send, 0);
    }
    return sent;
}

int recv_socket(int sockfd, char* buffer, int to_read)
{
    int read = 0;	
    if ((sockfd != INVALID_SOCKET) && buffer && (to_read > 0))
    {
        read = recv(sockfd, buffer, to_read, 0); 
    }
    return read;
}

int keyboard_read(char* message, char* buffer, int to_read)
{
    int read = 0;
    if (message && buffer && (to_read > 0))
    {
        printw("%s", message);
   		refresh();
        scanw("%[^\n]s", buffer);
   		refresh();
        read = strlen(buffer);
    }
    return read;
}
