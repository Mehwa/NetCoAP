#해결해야할 문제점들

======통신방식=====================================================

1. CON 메세지에 대한 timeout

2. 피기백 아닌 패킷 처리(액만 먼저 오고 데이터가 나중에 오는 경우)

3. MID, Token num 시퀀스 확인

4. GET 방식의 통신 구현 (POST, DELETE, PUT은 연기)

5. CON / NON / ACK / RESET 네가지 메세지 타입에 대한 설정

======패킷 구현=====================================================

======라이브러리 구현 부분==========================================

@@1. 해더 구현

@@2. 패킷 구현

@@3. 사용 함수 구현

4. 메이크 패킷 함수 구현

5. 패킷 분석

======문서 정리=====================================================

1. 메시지 타입 정리 (CON / NON / ACK / RESET)

2. UDP 패킷 정리 (버퍼에 해더와 패킷을 담는 방법)

3. 예제

@@4. Message Format

@@5. 코드와 옵션의 델타부 정리

 6. 통신 프로토콜 정리
