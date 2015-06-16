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


int main(int argc, char* argv[]){
	/* socket */
	int serv_sock;
	struct sockaddr_in serv_adr, clnt_adr;
	int adr_size;

	/* packet */
	int str_len;
	char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];


	/* coap Packet*/
	Header* recvHeader, *sendHeader;
	OptionNode* recvOptionNode, *sendOptionNode, *curOptionNode;
	int recvToken, sendToken;
	char* recvPayload, sendPayload[PAYLOAD_BUF_SIZE];
	int recvPay_len, sendPay_len;
	int curOptionOffset;	/* used with option delta */
	int piggy;			/* Can server send piggyback? TRUE or False */
	int send_pkt_size;


	/* service */
	int fd;
	
	/* Header */
	unsigned char ver_t_tkl = 0;
	unsigned char code = 0;
	unsigned short message_id;

	/* Option */
	unsigned char delta_length = 0;
	char optionvalue[OPTIONVALUE_BUF_SIZE];


	if (argc != 2)
	{
		printf("Input error! < %s > < PORT NUMBER >\n", argv[0]);
		exit(1);
	}

	if ((serv_adr = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("socket error");
		exit(1);
	}
	puts("Socket() completed...");

	memset(&serv_adr, 0, sizeof(serv_adr));
	serv_adr.sin_family = AF_INET;
	serv_adr.sin_port = htons(atoi(argv[1]));
	serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);

	if (bind(serv_sock, (struct sockaddr *)& serv_adr, sizeof(serv_adr)) < 0)
	{
		perror("bind error");
		exit(1);
	}
	puts("Bind() completed...");

	adr_size = sizeof(struct sockaddr);

	while (1){

		puts("Waiting Request for client...");

		str_len = recvfrom(serv_sock, recvBuf, BUF_SIZE, 0, (struct sockaddr *)&clnt_adr, &adr_size);
		
		puts("Request is arrived...");
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

		
		
		
		/* init sendPacket */
		setVer(&ver_t_tkl, 1);
		message_id = getMessageID(recvHeader);
		piggy = TRUE;	// This depends on requesting file!
		initPayload(sendPayload);
		sendPay_len = 0;
		initOptionNode(sendOptionNode);
		sendToken = 0;


		switch (getT(getVer_T_Tkl(recvHeader))){
		case CON:

			
			switch (getCodeClass(getCode(recvHeader))){
			case 0:

				switch (getCodeDetail(getCode(recvHeader))){
				case 1:
					/* GET 0.01 */


					/* Option */
					curOptionOffset = 0;
					curOptionNode = recvOptionNode;
					while (curOptionNode != NULL){
						curOptionOffset += getOptionDelta(getOptionDeltaLength(curOptionNode));
						switch (curOptionOffset){
						case Uri_Path:
							getOptionValue(curOptionNode, optionvalue);

							fd = open(optionvalue, O_RDONLY);
							if (fd == -1){
								/* If server does not have this file */
								setT(&ver_t_tkl, RST);
								
								setCode(&code, 0, 0); // Empty 0.00
								piggy = FALSE;
							}
							else{
								sendPay_len = read(fd, sendPayload, PAYLOAD_BUF_SIZE);
								setT(&ver_t_tkl, ACK);
							
								setCode(&code, 2, 5); // Content 2.05
							}
							break;
						}
						curOptionNode = getNextOptionNode(curOptionNode);
					}// end of recvOption analyzing
					break;
				}
				break;
			}// end of recvCode analyzing
			


			/* piggyback */
			if (piggy){
				/* If piggyback, token is same as recv packet */
				setTkl(&ver_t_tkl, getTkl(getVer_T_Tkl(recvHeader)));
				sendToken = recvToken;
			}
			else{
				/* If seperate, token length is 0*/
				setTkl(&ver_t_tkl, 0);
			}


			/* ==== Make Packet (When T is CON) ==== */
			/* make header */
			sendHeader = makeHeader(ver_t_tkl, code, message_id);
			send_pkt_size = makePacket(sendBuf, sendHeader, sendToken, sendOptionNode, sendPayload, sendPay_len);

			/* Assume ACK doesn't have option */


			break;
		}

		sendto(serv_sock, sendBuf, send_pkt_size, 0, (struct sockaddr *)&clnt_adr, adr_size);
		printf("Packet is sending...\n");
		/*========================================*/
		splitPacket(sendBuf, send_pkt_size, recvHeader, &recvToken, recvOptionNode, recvPayload, &recvPay_len);
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
		/*===========================================*/






	}

}
