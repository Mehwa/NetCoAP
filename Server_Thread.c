#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

//network
/*		³×Æ®¿öÅ© ½Ç½À
		¸ÖÆ¼ ¾²·¹µå¸¦ ÀÌ¿ëÇÑ Ã¤ÆÃ ÇÁ·Î±×·¥
		ÀÛ¼ºÀÚ : À±¼º¿ì
		¼öÁ¤ÀÚ : ¹Ú°­±Ô
		20900260 ¹Ú°­±Ô
		2015.06.08
*/

#define BUF_SIZE 100
#define MAX_CLNT 256	//Áß°£¿¡ client°¡ ºüÁ®³ª°¥ °æ¿ì client°¡ °¡µæÂ÷´Â ¹®Á¦°¡ »ý±ä´Ù.
						//client Á¾·á ½Ã ³ª¸ÓÁö clientµéÀ» ¾ÕÀ¸·Î ´ç°ÜÁà¾ßÇÑ´Ù.

void * handle_clnt(void * arg);
void send_msg(int clnt_sock, char * msg, int len);
void error_handling(char * msg);

int clnt_cnt=0;
int clnt_socks[MAX_CLNT];	//common resource
pthread_mutex_t mutx;

int main(int argc, char *argv[])
{
	int serv_sock, clnt_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int clnt_adr_sz;
	pthread_t t_id;
	if(argc!=2) {
		printf("Usage : %s <port>\n", argv[0]);
		exit(1);
	}
  
	pthread_mutex_init(&mutx, NULL);	//init
	serv_sock=socket(PF_INET, SOCK_STREAM, 0);

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family=AF_INET; 
	serv_adr.sin_addr.s_addr=htonl(INADDR_ANY);
	serv_adr.sin_port=htons(atoi(argv[1]));
	
	if(bind(serv_sock, (struct sockaddr*) &serv_adr, sizeof(serv_adr))==-1)
		error_handling("bind() error");
	if(listen(serv_sock, 5)==-1)
		error_handling("listen() error");
	
	while(1)
	{
		clnt_adr_sz=sizeof(clnt_adr);
		clnt_sock=accept(serv_sock, (struct sockaddr*)&clnt_adr,&clnt_adr_sz);
		
		pthread_mutex_lock(&mutx);	//lock
		clnt_socks[clnt_cnt++]=clnt_sock;	//access resource
		pthread_mutex_unlock(&mutx);	//unlock
//		int i;
//		for (i = 0; i < clnt_cnt; i++)
//			printf("clnt_socks[%d] : %d\n", i, clnt_socks[i]);
	
		pthread_create(&t_id, NULL, handle_clnt, (void*)&clnt_sock); //
		pthread_detach(t_id);	//joinÀº »ý¼ºµÈ thread°¡ Á¾·á½Ã±îÁö ±â´Ù¸®°Ô µÇ´Âµ¥ detach¸¦ »ç¿ëÇÏ¸é thread ½ÇÇàÀÌ ¿Ï·áµÇ¸é OS¿¡¼­ ¾Ë¾Æ¼­ Á¾·áÇØÁÖ¶ó´Â °Í.
		printf("Connected client IP: %s , client num : %d\n", inet_ntoa(clnt_adr.sin_addr), clnt_sock);
	}
	close(serv_sock);
	return 0;
}

//ÀÌ ºÎºÐ¿¡ ¹ö±×°¡ ÀÖÀ½.
void * handle_clnt(void * arg)
{
	int clnt_sock=*((int*)arg);
	int str_len=0, i;
	char msg[BUF_SIZE];
	
	while((str_len=read(clnt_sock, msg, sizeof(msg)))!=0)	//0ÀÌ ¾Æ´Ï¸é ¸Þ¼¼Áö ¹ÞÀº°Í, 0ÀÌ¸é Å¬¶óÀÌ¾ðÆ®¿¡¼­ Á¾·á ¿äÃ»
		send_msg(clnt_sock, msg, str_len);
	
	pthread_mutex_lock(&mutx);
	for(i=0; i<clnt_cnt; i++)   // remove disconnected client
	{
		if (clnt_sock == clnt_socks[i])
		{
			while (i < clnt_cnt - 1){		//¿ø·¡ ÄÚµå°¡ ÀÌ ºÎºÐ¿¡ i++°¡ µÇ¾îÀÖ¾ú´Âµ¥ ±×·² °æ¿ì ¾Æ·¡ ÄÚµå ½ÇÇà Àü¿¡ i°¡ Áõ°¡µÇ´Â ¹®Á¦°¡ »ý±ä´Ù.
											// => i++¸¦ ½ÇÇà ÄÚµå ´ÙÀ½À¸·Î ³»·ÁÁØ´Ù.
			clnt_socks[i] = clnt_socks[i + 1];	//Á¾·áµÈ Å¬¶óÀÌ¾ðÆ® ÀÚ¸®¸¦ ¸Þ¿ì±â À§ÇØ ³ª¸ÓÁö Å¬¶óÀÌ¾ðÆ®µéÀ» ´ç±ä´Ù.
			i++;
			}
//			for (i = 0; i < clnt_cnt; i++)
//			printf("clnt_socks[%d] : %d\n", i, clnt_socks[i]);
//			printf("closed socket is %d\n", clnt_sock);
			break;
		}
	}
	clnt_cnt--;
	pthread_mutex_unlock(&mutx);
	close(clnt_sock);
	return NULL;
}
void send_msg(int clnt_sock, char * msg, int len)   // send to all except sender client.
{
	int i;
	pthread_mutex_lock(&mutx);
	for (i = 0; i < clnt_cnt; i++){	//clnt_cnt °ªÀÌ Áß°£¿¡ º¯ÇÒ ¼ö°¡ ÀÖ´Ù.
		if (clnt_socks[i] == clnt_sock) continue;	//ÇØ´ç ¸Þ¼¼Áö¸¦ º¸³½ Å¬¶óÀÌ¾ðÆ®¿¡°Ô´Â ¸Þ¼¼Áö¸¦ ´Ù½Ã º¸³»Áö ¾ÊÀ½.
		write(clnt_socks[i], msg, len);
	}
	pthread_mutex_unlock(&mutx);
}
void error_handling(char * msg)
{
	fputs(msg, stderr);
	fputc('\n', stderr);
	exit(1);
}
