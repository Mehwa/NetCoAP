


#네트워크 실습 파이널 프로젝트

#코앱 프로토콜 시뮬레이터

@코앱 개요
사물인터넷을 위한 경량 패킷을 가진 머신투머신 프로토콜 

@통신 방식

 - GET, POST, DELETE, PUT 네가지 통신 방법

	+------+--------+-----------+
	| Code | Name   | Reference |
     +------+--------+-----------+
     | 0.01 | GET    | [RFC7252] |
     | 0.02 | POST   | [RFC7252] |
     | 0.03 | PUT    | [RFC7252] |
     | 0.04 | DELETE | [RFC7252] |
     +------+--------+-----------+

 - 타임아웃

 - 피기백

 - MID, Token num 시퀀스 확인

 - 메세지 타입에 따른 통신 규약 

@메세지 포멧

					Message Format

    0                   1                   2                   3
    0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |Ver| T |  TKL  |      Code     |          Message ID           |
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Token (if any, TKL bytes) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |   Options (if any) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   |1 1 1 1 1 1 1 1|    Payload (if any) ...
   +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

@메세지 타입
 
 - CON / NON / ACK / RESET

@옵션 포멧

					Option Format

			0   1   2   3   4   5   6   7
		   +---------------+---------------+
		   |               |               |
		   |  Option Delta | Option Length |   1 byte
		   |               |               |
		   +---------------+---------------+
		   \                               \
		   /         Option Delta          /   0-2 bytes
		   \          (extended)           \
		   +-------------------------------+
		   \                               \
		   /         Option Length         /   0-2 bytes
		   \          (extended)           \
		   +-------------------------------+
		   \                               \
		   /                               /
		   \                               \
		   /         Option Value          /   0 or more bytes
		   \                               \
		   /                               /
		   \                               \
		   +-------------------------------+

@코드 타입

		+------+------------------------------+-----------+
		| Code | Description                  | Reference |
		+------+------------------------------+-----------+
		| 2.01 | Created                      | [RFC7252] |
		| 2.02 | Deleted                      | [RFC7252] |
		| 2.03 | Valid                        | [RFC7252] |
		| 2.04 | Changed                      | [RFC7252] |
		| 2.05 | Content                      | [RFC7252] |
		| 4.00 | Bad Request                  | [RFC7252] |
		| 4.01 | Unauthorized                 | [RFC7252] |
		| 4.02 | Bad Option                   | [RFC7252] |
		| 4.03 | Forbidden                    | [RFC7252] |
		| 4.04 | Not Found                    | [RFC7252] |
		| 4.05 | Method Not Allowed           | [RFC7252] |
		| 4.06 | Not Acceptable               | [RFC7252] |
		| 4.12 | Precondition Failed          | [RFC7252] |
		| 4.13 | Request Entity Too Large     | [RFC7252] |
		| 4.15 | Unsupported Content-Format   | [RFC7252] |
		| 5.00 | Internal Server Error        | [RFC7252] |
		| 5.01 | Not Implemented              | [RFC7252] |
		| 5.02 | Bad Gateway                  | [RFC7252] |
		| 5.03 | Service Unavailable          | [RFC7252] |
		| 5.04 | Gateway Timeout              | [RFC7252] |
		| 5.05 | Proxying Not Supported       | [RFC7252] |
		+------+------------------------------+-----------+



		  +--------+------------------+-----------+
		  | Number | Name             | Reference |
		  +--------+------------------+-----------+
		  |      0 | (Reserved)       | [RFC7252] |
		  |      1 | If-Match         | [RFC7252] |
		  |      3 | Uri-Host         | [RFC7252] |
		  |      4 | ETag             | [RFC7252] |
		  |      5 | If-None-Match    | [RFC7252] |
		  |      7 | Uri-Port         | [RFC7252] |
		  |      8 | Location-Path    | [RFC7252] |
		  |     11 | Uri-Path         | [RFC7252] |
		  |     12 | Content-Format   | [RFC7252] |
		  |     14 | Max-Age          | [RFC7252] |
		  |     15 | Uri-Query        | [RFC7252] |
		  |     17 | Accept           | [RFC7252] |
		  |     20 | Location-Query   | [RFC7252] |
		  |     35 | Proxy-Uri        | [RFC7252] |
		  |     39 | Proxy-Scheme     | [RFC7252] |
		  |     60 | Size1            | [RFC7252] |
		  |    128 | (Reserved)       | [RFC7252] |
		  |    132 | (Reserved)       | [RFC7252] |
		  |    136 | (Reserved)       | [RFC7252] |
		  |    140 | (Reserved)       | [RFC7252] |
		  +--------+------------------+-----------+

@구현 시뮬레이터
코앱을 이용한 클라이언트의 서버 배터리 정보 요청에 대한 서비스

@제한된 구현

 - CON 메세지 타입
 
 - GET 방식의 통신

 - MID, Token num을 인풋을 통해 받음

 - 타임 아웃

 - 피기백

 - UDP 패킷에 모든 정보를 담아 보내고 받는 쪽에서 이를 분석

@라이브러리를 통한 패킷 타입, 사용 함수 제공 (패킷 설정, 생성, 분석 등)

@배터리 정보 출력 프로그램

@서버와 클라이언트

