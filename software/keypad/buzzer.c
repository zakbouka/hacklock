#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "io.h"
#include "buzzer.h"

IMPORT_IO(BUZZER);

void buzzer_init(void)
{
    // Set up Timer1 - CTC and /256 prescaler
    TCCR1A = 0;
    TCCR1B = _BV(WGM12) | _BV(CS12);
    TCNT1 = 0;
    OCR1A = 4;
    OCR1B = 0xFFFF;
}

void buzzer_start(uint16_t ticks)
{
    cli();
    TIMSK &= ~_BV(OCIE1A);
    OCR1A = ticks;
    TIMSK |= _BV(OCIE1A);
    TCNT1 = 0;
    sei();
}

void buzzer_stop(void)
{
    cli();
    TIMSK &= ~_BV(OCIE1A);
    sei();
}

ISR(TIMER1_COMPA_vect)
{
    IO_TOGGLE(BUZZER);
}

void buzzer_signal_boot(void)
{
    buzzer_start(TONE_LOW);
    _delay_ms(200);
    buzzer_start(TONE_MID);
    _delay_ms(200);
    buzzer_start(TONE_HIGH);
    _delay_ms(200);
    buzzer_stop();
}
