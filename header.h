#include <stdio.h>
#include <stdlib.h>

#define CON 0
#define NON 1
#define ACK 2
#define RST 3


typedef struct{
	unsigned char ver_t_tkl;
	unsigned char code;
	unsigned short message_id;
}Header;

typedef struct{
	unsigned char delta_length;
	unsigned char value[16];
}Option;


int getVer(unsigned char ver_t_tkl);
int getT(unsigned char ver_t_tkl);
int getTkl(unsigned char ver_t_tkl);
int getCodeClass(unsigned char code);
int getCodeDetail(unsigned char code);

void setVer(unsigned char* ver_t_tkl, int version);
void setT(unsigned char* ver_t_tkl, int T);
void setTkl(unsigned char* ver_t_tkl, int Tkl);
void setCodeClass(unsigned char* code, int _class);
void setCodeDetail(unsigned char* code, int detail);

void setVer_T_Tkl(unsigned char* ver_t_tkl, int version, int T, int Tkl);
void setCode(unsigned char* code, int _class, int detail);


int getOptionDelta(unsigned char delta_length);
int getOptionLength(unsigned char delta_length);

void setOptionDelta(unsigned char* delta_length,int delta);
void setOptionLength(unsigned char* delta_length,int length);
void setOption(unsigned char* delta_length, int delta, int length);


void makePacket(char* buf, Header* header, Option** option, char* payload, int paylen);
/* paylen contains NULL */



int main()
{
	unsigned char a;

	setOption(&a, 11, 7);
	printf("%d %d\n", getOptionDelta(a), getOptionLength(a));
	system("pause");
}


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
	return ver_t_tkl >> 6;
	
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
	return ver_t_tkl >> 4;
}
void setTkl(unsigned char* ver_t_tkl, int Tkl){
	if (Tkl == 0 || Tkl == 4){
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
	return code >> 5;
}
int getCodeDetail(unsigned char code){
	code <<= 3;
	return code >> 3;
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

int getOptionDelta(unsigned char delta_length){
	return delta_length >> 4;
}
int getOptionLength(unsigned char delta_length){
	delta_length <<= 4;
	return delta_length >> 4;
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

void makePacket(char* buf, Header* header, Option** option, char* payload, int paylen){
	int len = 0;
	int size;
	int token; /* Assume int or zero*/
	int i;

	size = sizeof(Header);
	memcpy(&buf[len], header, size);
	len += size;

	if (size = getTkl(header->ver_t_tkl)){
		token = (int)time(NULL);
		
		memcpy(&buf[len], &token, size);
		len += size;
	}

	size = sizeof(Option);
	if (option != NULL){
		i = 0;
		while (option[i] != NULL){
			memcpy(&buf[len], option[i], size);
			len += size;
			i++;
		}
	}
	
	if (payload != NULL){
		size = 8;
		memcpy(&buf[len], "11111111", size);
		len += size;

		memcpy(&buf[len], payload, paylen);
	}

}
