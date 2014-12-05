#include "stdlib.h"
#include "console.h"
#include "interrupts.h"


void init(void)
{
	ksetcolor(COLOR_BLACK, COLOR_LIGHTGRAY);
	kclear();
	kputs("Hello World!\nSecond Line!\n");
	kputs("Hey \x12\x05User\x12\x07!\n");
	
	char buffer[64];
	kputs("itoa test:\n");
	kputs("10 =   "); kputs(itoa(10, buffer, 10)); kputc('\n');
	kputs("10 = 0x"); kputs(itoa(10, buffer, 16)); kputc('\n');
	kputs("10 = 0b"); kputs(itoa(10, buffer, 2)); kputc('\n');
	kputs("10 = 0o"); kputs(itoa(10, buffer, 8)); kputc('\n');
	
	kputs("printf test:\n");
	kprintf("This %s %c test line.\n", "is", 'a');
	kprintf("Numbers: %d %i %x %b\n", 15, 15, 15, 15);
	
	kputs("Initialize interrupts: ");
	intr_init();
	kputs("success.\n");
	/*
	kputs("scroll-test:\n");
	for(int i = 0; i < 100; i++)
	{
		kprintf("They see me scrolling, they hating! %i\n", i);
	}
	for(int i = 0; i < 272; i++)
	{
		kprintf("x");
	}
	*/
	
	kputs("Trigger INT0: ");
	__asm__ volatile("int $0x00");
	kputs("success.\n");
}