#define F_CPU 1000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define FOSC 1000000
#define BAUDRATE 9600
#define MYUBRR FOSC/8/BAUDRATE-1

char buffer[100];

void init_IO()
{
	DDRC = 0x00;
	/*PORTB0 -> LED*/
	DDRB = 0x01;
}

void init_ADC()
{
	/*capacitor at AREF, right adjusted, ADC5*/
	ADMUX = (1 << REFS0) | (1 << MUX2) | (1 << MUX0);
	/*Prescaler = 8*/
	ADCSRA = (1 << ADEN) | (1 << ADPS1) | (1 << ADPS0);		
}

void init_USART(unsigned int ubrr)
{
	UBRR0H = (unsigned char) (ubrr >> 8);
	UBRR0L = (unsigned char) ubrr;
	/*double speed*/
	UCSR0A |= (1 << U2X0);
	/* transmit and receive enable*/
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);
	/* asynchronous, no parity, 1 stop bit, 8 data bits*/
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void ADC_read(uint16_t *analog_value)
{
	ADCSRA |= (1 << ADSC);
	while(ADCSRA & (1 << ADSC));
	*analog_value = ADC;
}

void blink_LED(void)
{
	PORTB |= (1 << PORTB0);
	_delay_ms(1000);
	PORTB &= ~(1 << PORTB0);
	_delay_ms(1000);
}

void USART_transmit_buffer(char *buffer)
{
	unsigned char i = 0;
	while(buffer[i] != '\0')
	{
		while(!(UCSR0A & (1 << UDRE0)));
		UDR0 = buffer[i];
		i++;
	}
}

int main(void)
{
	init_IO();
	init_ADC();
	init_USART(MYUBRR);
	uint16_t adc_value;
    while (1) 
    {
		ADC_read(&adc_value);
		sprintf(buffer,"%d\r\n",adc_value);
		USART_transmit_buffer(buffer);
		_delay_ms(250);
    }
}

