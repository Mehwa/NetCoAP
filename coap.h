/* coap.h */
/* Made by Park Kangkyu, Chae Youngmin. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define CON 0
#define NON 1
#define ACK 2
#define RST 3

#define If_Match 1
#define Uri_Host 3
#define Uri_Port 7
#define Uri_Path 11



#define TRUE 1
#define FALSE 0

typedef struct{
	unsigned char ver_t_tkl;
	unsigned char code;
	unsigned short message_id;
}Header;

typedef struct {
	unsigned char delta_length;
	char option_value[16];
}Option;

typedef struct _optionNode{
	Option option;
	struct _optionNode* nextOptionNode;
}OptionNode;


/* Header, Token Get or Set */
int getVer(unsigned char ver_t_tkl);
int getT(unsigned char ver_t_tkl);
int getTkl(unsigned char ver_t_tkl);
int getCodeClass(unsigned char code);
int getCodeDetail(unsigned char code);
int getMessageID(Header* header);

unsigned char getVer_T_Tkl(Header* header);
unsigned char getCode(Header* header);




void setVer(unsigned char* ver_t_tkl, int version);
void setT(unsigned char* ver_t_tkl, int T);
void setTkl(unsigned char* ver_t_tkl, int Tkl);
void setCodeClass(unsigned char* code, int _class);
void setCodeDetail(unsigned char* code, int detail);

void setVer_T_Tkl(unsigned char* ver_t_tkl, int version, int T, int Tkl);
void setCode(unsigned char* code, int _class, int detail);


/* make Header */
Header* makeHeader(unsigned char ver_t_tkl, unsigned char code, unsigned short message_id);

/* init */
void initOptionNode(OptionNode * optionNode);
void initPayload(char * payload);


/* Option Get or Set */
int getOptionDelta(unsigned char delta_length);
int getOptionLength(unsigned char delta_length);

unsigned char getOptionDeltaLength(OptionNode* optionNode);
void getOptionValue(OptionNode* optionNode, char* buf);
OptionNode* get_ithOptionNode(OptionNode* optionNode, int index);
/* return ith OptionNode, for i >= 1 */
OptionNode* getNextOptionNode(OptionNode* optionNode);


void setOptionDelta(unsigned char* delta_length,int delta);
void setOptionLength(unsigned char* delta_length,int length);

void setOption(unsigned char* delta_length, int delta, int length);

/* make OptionNode or add OptionNode */
void makeOptionNode(OptionNode* optionNode, unsigned char delta_length, char* option_value);


/* Packet */
int makePacket(char* buf, Header* header, int token, OptionNode* optionNode, char* payload, int paylen);	/* paylen contains char '/0'  */
void splitPacket(char* buf, int buf_len, Header* header, int* token, OptionNode* optionNode, char* payload, int* paylen);




int getVer(unsigned char ver_t_tkl){
	return (int)(ver_t_tkl >> 6);
}

void setVer(unsigned char* ver_t_tkl, int version){

	if (version >= 0 && version <= 3){
		*ver_t_tkl <<= 2;
		*ver_t_tkl >>= 2;
		*ver_t_tkl += (unsigned char)version << 6;
		return;
	}
	puts("Invalid version setting!");
	return;
}

int getT(unsigned char ver_t_tkl){
	ver_t_tkl <<= 2;
	return (int)(ver_t_tkl >> 6);
	
}
void setT(unsigned char* ver_t_tkl, int T){
	int ver;
	int tkl;
	unsigned char result;
	if (T >= 0 && T <= 3){
		
		ver = getVer(*ver_t_tkl);
		tkl = getTkl(*ver_t_tkl);
		result = ver;
		result <<= 2;
		result += T;
		result <<= 4;
		result += tkl;
		*ver_t_tkl = result;
		return;
	}
	puts("Invalid Type setting!");
	return;
}
int getTkl(unsigned char ver_t_tkl){
	ver_t_tkl <<= 4;
	return (int)(ver_t_tkl >> 4);
}
void setTkl(unsigned char* ver_t_tkl, int Tkl){
	if (Tkl == 0 || Tkl == sizeof(int)){
		/* Assume that token length is always 4(int) or zero(none) */
		*ver_t_tkl >>= 4;
		*ver_t_tkl <<= 4;
		*ver_t_tkl += (unsigned char)Tkl;
		return;
	}
	puts("Invalid Token Length setting!");
	return;
}


void setVer_T_Tkl(unsigned char* ver_t_tkl, int version, int T, int Tkl){
	setVer(ver_t_tkl, version);
	setT(ver_t_tkl, T);
	setTkl(ver_t_tkl, Tkl);
	return;
}

int getCodeClass(unsigned char code){
	return (int)(code >> 5);
}
int getCodeDetail(unsigned char code){
	code <<= 3;
	return (int)(code >> 3);
}


void setCodeClass(unsigned char* code, int _class){
	if (_class >= 0 && _class <= 7){
		*code <<= 3;
		*code >>= 3;
		*code += (unsigned char)_class << 5;
		return;
	}
	puts("Invalid Code Class setting!");
	return;
}
void setCodeDetail(unsigned char* code, int detail){
	if (detail >= 0 && detail <= 31){
		*code >>= 5;
		*code <<= 5;
		*code += (unsigned char)detail;
		return;
	}
	puts("Invalid Code Detail setting!");
	return;
}

void setCode(unsigned char* code, int _class, int detail){
	setCodeClass(code, _class);
	setCodeDetail(code, detail);
	return;
}

int getMessageID(Header* header){
	return (int)header->message_id;
}

unsigned char getVer_T_Tkl(Header* header){
	return header->ver_t_tkl;
}

unsigned char getCode(Header* header){
	return header->code;
}







int getOptionDelta(unsigned char delta_length){
	return (int)(delta_length >> 4);
}
int getOptionLength(unsigned char delta_length){
	delta_length <<= 4;
	return (int)(delta_length >> 4);
}

unsigned char getOptionDeltaLength(OptionNode* optionNode){
	return optionNode->option.delta_length;
}


void getOptionValue(OptionNode* optionNode, char* buf){
	int len = getOptionLength(getOptionDeltaLength(optionNode));
	memcpy(buf, optionNode->option.option_value, len);
	buf[len] = 0;
}

OptionNode* get_ithOptionNode(OptionNode* headOptionNode, int index){
	int i;
	OptionNode* curOptionNode;
	if (index < 1)
		return NULL;
	
	curOptionNode = headOptionNode;
	for (i = 1; i < index; i++)
		curOptionNode = curOptionNode->nextOptionNode;

	return curOptionNode;
}

OptionNode* getNextOptionNode(OptionNode* optionNode){
	return optionNode->nextOptionNode;
}


void setOptionDelta(unsigned char* delta_length, int delta){
	if (delta >= 0 && delta <= 15){
		*delta_length <<= 4;
		*delta_length >>= 4;
		*delta_length += (unsigned char)delta << 4;
		return;
	}
	puts("Invalid Option Delta setting!");
	return;
}
void setOptionLength(unsigned char* delta_length, int length){
	if (length >= 0 && length <= 15){
		*delta_length >>= 4;
		*delta_length <<= 4;
		*delta_length += (unsigned char)length;
		return;
	}
	puts("Invalid Option Length setting!");
	return;
}


void setOption(unsigned char* delta_length, int delta, int length){
	setOptionDelta(delta_length, delta);
	setOptionLength(delta_length, length);
	return;
}

int makePacket(char* buf, Header* header, int token, OptionNode* optionNode, char* payload, int paylen){
	/* return packet length */
	/* Assume token size is int or zero*/
	int len = 0;
	int size;
	
	int i;
	OptionNode* curOptionNode;

	size = sizeof(Header);
	memcpy(&buf[len], header, size);
	len += size;

	if (size = getTkl(header->ver_t_tkl)){
		
		memcpy(&buf[len], &token, size);
		len += size;
	}

	
	if (optionNode != NULL){
		
		i = 0;
		curOptionNode = optionNode;
		size = sizeof(Option);
		while (curOptionNode != NULL){
			memcpy(&buf[len], &(optionNode->option), size);
			len += size;
			curOptionNode = curOptionNode->nextOptionNode;
		}
	}
	
	if (payload != NULL){
		size = 8;
		memcpy(&buf[len], "11111111", size);
		len += size;

		memcpy(&buf[len], payload, paylen);
		len += paylen;
	}
	return len;

}


void splitPacket(char* buf, int pkt_len, Header* header, int* token, OptionNode* optionNode, char* payload, int* paylen){
	int len = 0;
	int size;
	OptionNode* curOptionNode;
	OptionNode* newOptionNode;
	OptionNode* delOptionNode;

	if (header != NULL)
		free(header);
	header = (Header *)malloc(sizeof(Header));
	if (header == NULL){
		puts("header malloc error!\n");
		exit(1);
	}


	size = sizeof(Header);
	memcpy(header, &buf[len], size);
	len += size;


	if (size = getTkl(header->ver_t_tkl)){
		memcpy(token, &buf[len], size);
		len += size;
	}
	else
		*token = 0;
 

	if (optionNode != NULL){
		curOptionNode = optionNode;
		while (curOptionNode != NULL){
			delOptionNode = curOptionNode;
			curOptionNode = curOptionNode->nextOptionNode;
			free(delOptionNode);
		}
	}
	optionNode = NULL;

	
	size = sizeof(Option);
	while (strncmp(&buf[len], "11111111", 8)){
		/* if packet don't have payload */
		if (len == pkt_len){
			if (payload != NULL)
				free(payload);
			payload = NULL;
			*paylen = 0;
			return;
		}
		/* If there are options */
		newOptionNode = (OptionNode *)malloc(sizeof(OptionNode));
		if (newOptionNode == NULL){
			puts("OptionNode malloc error!\n");
			exit(1);
		}
		
		memcpy(&(newOptionNode->option), &buf[len], size);
		len += size;
		newOptionNode->nextOptionNode = NULL;

		
		if (optionNode == NULL)
			optionNode = newOptionNode;
		else{
			while (curOptionNode->nextOptionNode != NULL)
				curOptionNode = curOptionNode->nextOptionNode;
			curOptionNode->nextOptionNode = newOptionNode;
		}
	}
	
	len += 8;

	/* read payload */
	if (payload == NULL)
		free(payload);

	size = pkt_len - len;
	payload = (char *)malloc(sizeof(size));
	if (payload == NULL){
		puts("payload malloc error!\n");
		exit(1);
	}
	memcpy(payload, &buf[len], size);

	*paylen = size;

	return;
}


Header* makeHeader(unsigned char ver_t_tkl, unsigned char code, unsigned short message_id){
	
	Header* header;

	if (header != NULL)
		free(header);

	header = (Header *)malloc(sizeof(Header));
	if (header == NULL){
		puts("header malloc error!\n");
		exit(1);
	}
	header->ver_t_tkl = ver_t_tkl;
	header->code = code;
	header->message_id = message_id;
	
	

	return header;
}

void makeOptionNode(OptionNode* optionNode, unsigned char delta_length, char* option_value){

	OptionNode* newOptionNode;
	OptionNode* curOptionNode;
	newOptionNode = (OptionNode *)malloc(sizeof(OptionNode));
	if (newOptionNode == NULL){
		puts("OptionNode malloc error!\n");
		exit(1);
	}
	newOptionNode->nextOptionNode = NULL;
	newOptionNode->option.delta_length = delta_length;
	memcpy(newOptionNode->option.option_value, option_value, getOptionLength(delta_length));


	if (optionNode == NULL)
		optionNode = newOptionNode;
	else{
		while (curOptionNode->nextOptionNode != NULL)
			curOptionNode = curOptionNode->nextOptionNode;
		curOptionNode->nextOptionNode = newOptionNode;
	}
	return;
}


void initOptionNode(OptionNode * optionNode){
	OptionNode* curOptionNode;
	OptionNode* delOptionNode;

	if (optionNode != NULL){
		curOptionNode = optionNode;
		while (curOptionNode != NULL){
			delOptionNode = curOptionNode;
			curOptionNode = curOptionNode->nextOptionNode;
			free(delOptionNode);
		}
	}
	optionNode = NULL;
}
void initPayload(char * payload){
	if (payload != NULL)
		free(payload);
	payload = NULL;
}
