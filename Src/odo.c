#include "odo.h"
#include "usart.h"
#include "tim.h"

static void convert2hex(uint16_t x, char *res);

void send_counter(void){
	uint8_t	buf[7];

	convert2hex(tim_get_pulse_count(),(char *)buf);
	buf[4] = 0x0D;
	buf[5] = 0x0A;
	
	#ifdef DISABLE_NRST
	uart_send_bin(USART2,buf,6);
	#endif

	#ifdef DEBUG_ENABLED
	DEBUG("Reply [%d]: 0x%0X:0x%0X:0x%0X:0x%0X:0x%0X:0x%0X",tim_get_pulse_count(),buf[0],buf[1],buf[2],buf[3],buf[4],buf[5]);
	buf[4] = 0;
	DEBUG("String: %s",buf);
	#endif
};

void reset_counter(void){
	tim_reset_counter();
}

static void convert2hex(uint16_t x, char *res){
	res[0] = TO_HEX(((x & 0xF000) >> 12));   
	res[1] = TO_HEX(((x & 0x0F00) >> 8));
	res[2] = TO_HEX(((x & 0x00F0) >> 4));
	res[3] = TO_HEX((x & 0x000F));
}
