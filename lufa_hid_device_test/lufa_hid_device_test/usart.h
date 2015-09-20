/*
 * uart.h
 *
 * Created: 04-Aug-15 11:11:40 PM
 *  Author: emon1
 */ 


#ifndef UART_H_
#define UART_H_


/**
 * initialize the usart baud rate
 * @param baud_rate the baud rate for teh usart module
 */
void usart_set_baud_rate(unsigned long baud_rate){
	unsigned int ubrr = (F_CPU/16/baud_rate)-1;
	UBRR1H = (unsigned char)(ubrr>>8);
	UBRR1L = (unsigned char)ubrr;
}
/**
 * Settings for the USART module
 * @param mode		0->asynnchronous mode, 1-> synchronous mode
 * @param parity	0->disabled, 2-> even parity, 3-> odd parity
 * @param stop_bits	0->1 stop bit, 1-> 2 stop bits
 * @param char_size	0->5-bit, 1->6-bit, 2->7-bit, 3->8-bit,7->9-bit
 * @param pol		0->tx or rising, rx on falling, 1-> tx on falling, rx on rising
 */
void usart_setup(uint8_t mode, uint8_t parity, uint8_t stop_bits, uint8_t char_size,uint8_t pol){
	 UCSR1C =  (mode<<6) | (parity<<4) | (stop_bits<<3) |(char_size<<1) | (pol<<0);
 }

/**
 * enable the USART tx and rx
 */
void usart_enable(){
	 UCSR1B = (1<<RXEN1)|(1<<TXEN1);
 }
 
/**
 * transmit byte via pass by reference
 * @param data address of data byte to be sent
 */
void usart_transmit_byte_ref(uint8_t *data){
	 while (!(UCSR1A & (1<<UDRE1)));
	 UDR1 = *data;
 }

/**
 * transmit byte via pass by value
 * @param data data byte to be sent
 */
void usart_transmit_byte_val(uint8_t data){
	 while (!(UCSR1A & (1<<UDRE1)));
	 UDR1 = data;
 }

/**
 * receive byte via reference
 * @param data address of data byte into which the data shall be recieved
 */
void usart_receive_byte_ref(uint8_t *data){
	 while (!(UCSR1A & (1<<RXC1)));
	 *data = UDR1;
 }

/**
 * receive byte by value
 @return data
 */
 uint8_t usart_receive_byte_val(){
	 while (!(UCSR1A & (1<<RXC1)));
	 return UDR1;
 }

#endif /* UART_H_ */