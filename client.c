#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include "coap.h"

#define BUF_SIZE 256
#define OPTIONVALUE_BUF_SIZE 20
#define PAYLOAD_BUF_SIZE 100
#define VERSION 1

int main(int argc, char *argv[]){
	/* socket */
	int clnt_sock;
	struct sockaddr_in serv_adr;
	int adr_size;

	/* packet */
	int str_len;
	char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];
	
	int recvToken, sendToken;
	char* recvPayload = NULL, sendPayload[PAYLOAD_BUF_SIZE];
	int recvPay_len, sendPay_len;
	int curOptionOffset;	/* used with option delta */
	int piggy;			/* Can server send piggyback? TRUE or False */
	int send_pkt_size;

	/* coap Packet */
	Header* recvHeader = NULL, *sendHeader = NULL;
	OptionNode* recvOptionNode =NULL, *sendOptionNode= NULL, *curOptionNode = NULL;

	/* Header */
	unsigned char ver_t_tkl = 0;
	unsigned char code = 0;
	unsigned short message_id;

	/* Option */
	unsigned char delta_length = 0;
	char optionvalue[OPTIONVALUE_BUF_SIZE];

	if (argc != 3)
	{
		printf("Input error! < %s > < IP ADDRESS > < PORT NUMBER >  \n", argv[0]);
		exit(1);
	}

	if ((clnt_sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(atoi(argv[2]));
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]);

	

	setVer_T_Tkl(&ver_t_tkl, 1, CON, 0);
	setCode(&code, 0, 1);
	message_id = 10;
	sendHeader = makeHeader(ver_t_tkl, code, message_id);

	setOption(&delta_length, 11, 7);
	makeOptionNode(sendOptionNode, delta_length, "battery");

	send_pkt_size = makePacket(sendBuf, sendHeader, 0, sendOptionNode, NULL, 0);

	adr_size = sizeof(struct sockaddr);
	
	
	
	sendto(clnt_sock, sendBuf, send_pkt_size, 0, (struct sockaddr *)&serv_adr, adr_size);

	splitPacket(sendBuf, str_len, recvHeader, &recvToken, recvOptionNode, recvPayload, &recvPay_len);
	printf("[Ver: %d] [T: %d] [Tkl: %d] [Code: %d.%02d] [MessageID: %d]\n"
		, getVer(getVer_T_Tkl(recvHeader))
		, getT(getVer_T_Tkl(recvHeader))
		, getTkl(getVer_T_Tkl(recvHeader))
		, getCodeClass(getCode(recvHeader))
		, getCodeDetail(getCode(recvHeader))
		, getMessageID(recvHeader));




	
	str_len = recvfrom(clnt_sock, recvBuf, BUF_SIZE, 0, (struct sockaddr *)&serv_adr, &adr_size);

	splitPacket(recvBuf, str_len, recvHeader, &recvToken, recvOptionNode, recvPayload, &recvPay_len);
	printf("[Ver: %d] [T: %d] [Tkl: %d] [Code: %d.%02d] [MessageID: %d]\n"
		, getVer(getVer_T_Tkl(recvHeader))
		, getT(getVer_T_Tkl(recvHeader))
		, getTkl(getVer_T_Tkl(recvHeader))
		, getCodeClass(getCode(recvHeader))
		, getCodeDetail(getCode(recvHeader))
		, getMessageID(recvHeader));

	if (getTkl(getVer_T_Tkl(recvHeader)) == 0)
		puts("[Token length is zero]");
	else
		printf("Token value is %d\n", recvToken);

}
