#include <stdio.h>
#include <sys/types.h>


/*
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

Option Delta:  4-bit unsigned integer.  A value between 0 and 12
      indicates the Option Delta.  Three values are reserved for special
      constructs:

      13:  An 8-bit unsigned integer follows the initial byte and
         indicates the Option Delta minus 13.

      14:  A 16-bit unsigned integer in network byte order follows the
         initial byte and indicates the Option Delta minus 269.

      15:  Reserved for the Payload Marker.  If the field is set to this
         value but the entire byte is not the payload marker, this MUST
         be processed as a message format error.

      The resulting Option Delta is used as the difference between the
      Option Number of this option and that of the previous option (or
      zero for the first option).  In other words, the Option Number is
      calculated by simply summing the Option Delta values of this and
      all previous options before it.

   Option Length:  4-bit unsigned integer.  A value between 0 and 12
      indicates the length of the Option Value, in bytes.  Three values
      are reserved for special constructs:

      13:  An 8-bit unsigned integer precedes the Option Value and
         indicates the Option Length minus 13.

      14:  A 16-bit unsigned integer in network byte order precedes the
         Option Value and indicates the Option Length minus 269.

      15:  Reserved for future use.  If the field is set to this value,
         it MUST be processed as a message format error.

	Option Value Formats
	  4 types - empty, opaque, uint, string
*/
//simple 4 bytes light header
typedef struct CoAPHeader{
	unsigned int ver:1;	//version
	unsigned int T:2;	//Type	
					//0 : Confirmable, 1 : Non-confirmable, 2 : Acknowledgement, 3 : Reset.
	unsigned int TKL:4;	//Token Length
					//Indicates the length of the variable-length Token field (0-8 bytes).
					//Lengths 9-15 are reserved, MUST NOT be sent, 
					//and MUST be processed as a message format error.
	unsigned int code:8;//3-bit calss(MSB) and 5-bit detail(LSB)
					//c.dd  c(0to7 MSB), dd(00to31 LSB)
					//The class  0 : request, 2 : success response, 4 : client error response
                         //           5 : server error response		
					//(All other class values are reserved.)
					//Code 0.00 indicates an Empty message.
	unsigned int MID:16;//Message ID
					//Used to detect message duplication and to match messages of type
      				//Acknowledgement/Reset to messages of type Confirmable/Non-confirmable.
	unsigned int Token:64;//0 to 8 bytes is determined by TKL value.
					//used to correlate requests and responses.
	unsigned int Op:32;	//Options

}
int main(){
	
	
}
