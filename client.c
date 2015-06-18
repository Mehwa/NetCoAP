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
#include <signal.h>

#define BUF_SIZE 256
#define OPTIONVALUE_BUF_SIZE 20
#define PAYLOAD_BUF_SIZE 100
#define VERSION 1



void handler(int sig)
{
	printf("\n<><> Thank you! for Using Client Program <><>\n");
	exit(1);
}

int main(int argc, char *argv[]){
	/* socket */
	int clnt_sock;
	struct sockaddr_in serv_adr;
	int adr_size;

	/* packet */
	char recvBuf[BUF_SIZE], sendBuf[BUF_SIZE];


	/* coap Packet*/
	Header recvHeader, *sendHeader;
	OptionNode recvOptionNode, *sendOptionNode, *curOptionNode;
	int send_pkt_size;
	int recv_pkt_size;

	/* service */
	int fd;

	/* Header */
	unsigned char ver_t_tkl = 0;
	unsigned char code = 0;
	unsigned short message_id;

	/* Option */
	unsigned char delta_length = 0;
	char optionvalue[OPTIONVALUE_BUF_SIZE];

	/* others */
	int curOptionOffset;	/* used with option delta */
	int piggy;			/* Can server send piggyback? TRUE or False */
	int recvToken, sendToken;
	char recvPayload[PAYLOAD_BUF_SIZE], sendPayload[PAYLOAD_BUF_SIZE];
	int recvPay_len, sendPay_len;

	int recv_pkt_num = 0;
	int option_num;
	int i;

	_initOptionNode(&recvOptionNode);
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

	adr_size = sizeof(struct sockaddr);

	signal(SIGINT, handler);
	while (1){
		setVer_T_Tkl(&ver_t_tkl, 1, CON, 4);
		setCode(&code, 0, 1);
		puts("\n\n==========================================");
		puts(" Exit <== Ctrl + c");
		puts("==========================================");
		puts("============== Auto Setting ==============");
		puts("= [Ver: 1] [T: CON] [Tkl: 4] [Code: GET] =");
		puts("= [Option Delta: 11]                     =");
		puts("==========================================");

		printf("Input Message ID > ");
		scanf("%d", (int *)&message_id);

		sendHeader = makeHeader(ver_t_tkl, code, message_id);

		printf("Input Token Value > ");
		scanf("%d", &sendToken);

		printf("Input Uri_Path (Max: 16) > ");
		scanf("%s", sendPayload);


		setOption(&delta_length, 11,(int)strlen(sendPayload));
		sendOptionNode = makeOptionNode(delta_length, sendPayload);




		send_pkt_size = makePacket(sendBuf, sendHeader, sendToken, sendOptionNode, "wow", 4);


		

		sendto(clnt_sock, sendBuf, send_pkt_size, 0, (struct sockaddr *)&serv_adr, adr_size);
		

		puts("Packet sending...\n");



		recv_pkt_size = recvfrom(clnt_sock, recvBuf, BUF_SIZE, 0, (struct sockaddr *)&serv_adr, &adr_size);

		recv_pkt_num++;

		freeHeader(sendHeader);
		freeOptionNode(sendOptionNode);

		puts("Response is arrived...\n");

		initOptionNode(&recvOptionNode);
		splitPacket(recvBuf, recv_pkt_size, &recvHeader
			, &recvToken, &recvOptionNode, &option_num
			, recvPayload, &recvPay_len);

		/*================ Print recvPacket ============================*/
		printf("=== #%d === received CoAP Packet ============\n", recv_pkt_num);
		printf("[Ver: %d] [T: %d] [Tkl: %d] [Code: %d.%02d] [MessageID: %d]\n"
			, getVer(getVer_T_Tkl(&recvHeader))
			, getT(getVer_T_Tkl(&recvHeader))
			, getTkl(getVer_T_Tkl(&recvHeader))
			, getCodeClass(getCode(&recvHeader))
			, getCodeDetail(getCode(&recvHeader))
			, getMessageID(&recvHeader));

		if (getTkl(getVer_T_Tkl(&recvHeader)))
			printf("[Token value: %d]\n", recvToken);


		curOptionNode = &recvOptionNode;
		for (i = 0; i < option_num; i++){
			printf("[Option Delta: %d] [Option Length: %d]\n"
				, getOptionDelta(getOptionDeltaLength(curOptionNode))
				, getOptionLength(getOptionDeltaLength(curOptionNode)));

			getOptionValue(curOptionNode, optionvalue);
			printf("[Option value: %s]\n", optionvalue);
			curOptionNode = getNextOptionNode(curOptionNode);
		}

		if (recvPay_len){
			recvPayload[recvPay_len] = 0;
			printf("[Payload: %s]\n", recvPayload);
			printf("[payload_length: %d]\n", recvPay_len);
		}
	}


}
