/*
 */

#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{

    // Insert code
    unsigned char a;

    PORTB=0x00;
    DDRB=0xFF;

    PORTC=0xFF;
    DDRC=0x00;

    a = 0B00000001;
    while(1)
    {
        a = a<<1 | a>>7;
        PORTB=a;
        delay_ms(500);
    }
    ;

    return 0;
}
