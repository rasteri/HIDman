
#define PORT_KEY 0
#define PORT_MOUSE 1

#define CLOCK 0
#define DATA 1

typedef struct ps2port {
	
	uint8_t state;
	uint8_t port;
	uint8_t data;
	uint8_t bitnum;
	uint8_t parity;
	
} ps2port;



void OutPort (unsigned char port, unsigned char channel, bool val);

bool GetPort (unsigned char port, unsigned char channel);

/*States

IDLE - CLOCK/DATA high

SEND_CLOCK_LOW - Check for host transmission - if not, Write next value to data, drive clock line low, set state to SEND_CLOCK_HIGH
SEND_CLOCK_HIGH - Make clock high, perhaps calculate next value

*/

#define S_INIT 0
#define S_IDLE 1
#define S_SEND_CLOCK_LOW 2
#define S_SEND_CLOCK_HIGH 3
#define S_RECIEVE_CLOCK_LOW 4
#define S_RECIEVE_CLOCK_HIGH 5
#define S_INHIBIT 6

void ps2stuff(ps2port *port);