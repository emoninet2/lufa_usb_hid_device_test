/*
 * serial_stdio.h
 *
 * Created: 05-Aug-15 12:50:21 PM
 *  Author: emon1
 */ 


#ifndef SERIAL_STDIO_H_
#define SERIAL_STDIO_H_

#include <stdio.h>
#include "usart.h"

#define stdio_serial_send_function		usart_transmit_byte_val
#define stdio_serial_receive_function	usart_receive_byte_ref

/**
 * putchar for streaming file out
 */
int uart_putchar(char ch, FILE *stream)
{
	if(ch=='\n')
	uart_putchar('\r',stream);
	stdio_serial_send_function((uint8_t) ch);
	return 0;
}
/**
 * get char for streaming file in
 */
int uart_getchar(FILE *stream)
{
	char ch;
	stdio_serial_receive_function( (uint8_t *) &ch);
	
	/* Echo the output back to the terminal */
	uart_putchar(ch,stream);

	return ch;
}


FILE uart_str = FDEV_SETUP_STREAM(uart_putchar, uart_getchar, _FDEV_SETUP_RW);

/**
 * initializing the stdio file stream over uart
 */
void stdio_serial_initialize()
{
	stdout = stdin = &uart_str;
}


// void clear_screen()
// {
// 	printf("%c[2J",27);
// }
//
// void cursor_home()
// {
// 	printf("%c[H",27);
// }

#endif /* SERIAL_STDIO_H_ */