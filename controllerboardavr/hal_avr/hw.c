#include <stdint.h>
#include <avr/interrupt.h>

uint8_t critical_section_count = 0;

void enter_critical_section(void)
{
	critical_section_count++;
	cli();
	/* disable interrupts */
}

void exit_critical_section(void)
{
	if (critical_section_count)
		--critical_section_count;
	if (!critical_section_count)
		sei();
	/* enable interrupts */
}

void jump_to_bootloader()
{
	enter_critical_section();

#if !_FAST_DEBUG
	MCUSR = 0;
#if defined __AVR_ATmega644__
	asm("JMP 0xF800");
#elif defined __AVR_ATmega168__
	asm("JMP 0x3800");
#elif defined __AVR_ATmega328__ || defined __AVR_ATmega328P__
	asm("JMP 0x7800");
#else
#error "Unsupported AVR"
#endif // AVR type
#endif
}

