#include "interrupts.h"
#include "console.h"
#include "stdlib.h"
#include "cpustate.h"
#include "io.h"

#define GDT_ENTRIES 256
#define IDT_ENTRIES 49

static uint64_t gdt[GDT_ENTRIES];
static uint64_t idt[IDT_ENTRIES];

#include "intr_stubs.h"

void intr_routine(CpuState *state)
{
	kprintf("\n\x12\x04Interrupt %d occurred!\x12\0x7\n", state->intr);
	//while(1);
}

static void gdt_entry(int i, uint32_t base, uint32_t limit, int flags)
{
    gdt[i] = limit & 0xffffLL;
    gdt[i] |= (base & 0xffffffLL) << 16;
    gdt[i] |= (flags & 0xffLL) << 40;
    gdt[i] |= ((limit >> 16) & 0xfLL) << 48;
    gdt[i] |= ((flags >> 8 )& 0xffLL) << 52;
    gdt[i] |= ((base >> 24) & 0xffLL) << 56;
}

static void idt_entry(int i, void (*fn)(), unsigned int selector, int flags)
{
	unsigned long int handler = (unsigned long int) fn;
	idt[i] = handler & 0xffffLL;
	idt[i] |= (selector & 0xffffLL) << 16;
	idt[i] |= (flags & 0xffLL) << 40;
	idt[i] |= ((handler>> 16) & 0xffffLL) << 48;
}

static void init_gdt(void)
{
	memset(gdt, 0, sizeof(gdt));

    gdt_entry(0, 0, 0, 0);
    gdt_entry(1, 0, 0xfffff, GDTF_SEGMENT | GDTF_32_BIT |
        GDTF_CODESEG | GDTF_4K_GRAN | GDTF_PRESENT);
    gdt_entry(2, 0, 0xfffff, GDTF_SEGMENT | GDTF_32_BIT |
        GDTF_DATASEG | GDTF_4K_GRAN | GDTF_PRESENT);
    gdt_entry(3, 0, 0xfffff, GDTF_SEGMENT | GDTF_32_BIT |
        GDTF_CODESEG | GDTF_4K_GRAN | GDTF_PRESENT | GDTF_RING3);
    gdt_entry(4, 0, 0xfffff, GDTF_SEGMENT | GDTF_32_BIT |
        GDTF_DATASEG | GDTF_4K_GRAN | GDTF_PRESENT | GDTF_RING3);

	struct
	{
		uint16_t limit;
		void* pointer;
	} __attribute__((packed)) gdtp =
	{
		.limit = GDT_ENTRIES * 8 - 1,
		.pointer = gdt,
	};
	__asm__ volatile("lgdt %0" : : "m" (gdtp));
	__asm__ volatile(
		"mov $0x10, %ax;"
		"mov %ax, %ds;"
		"mov %ax, %es;"
		"mov %ax, %ss;"
		"ljmp $0x8, $.1;"
		".1:"
	);
}

#define IDT_FLAG_INTERRUPT_GATE 0xe
#define IDT_FLAG_PRESENT 0x80
#define IDT_FLAG_RING0 0x00
#define IDT_FLAG_RING3 0x60

static void init_idt()
{
	memset(idt, 0, sizeof(idt));

#define SET_ENTRY(i) idt_entry(i, intr_stub_ ## i, 0x08, IDT_FLAG_INTERRUPT_GATE | IDT_FLAG_PRESENT | IDT_FLAG_RING0)

	// System Interrupts
	SET_ENTRY(0);
	SET_ENTRY(1);
	SET_ENTRY(2);
	SET_ENTRY(3);
	SET_ENTRY(4);
	SET_ENTRY(6);
	SET_ENTRY(7);
	SET_ENTRY(8);
	SET_ENTRY(9);
	SET_ENTRY(10);
	SET_ENTRY(11);
	SET_ENTRY(12);
	SET_ENTRY(13);
	SET_ENTRY(14);
	SET_ENTRY(15);
	SET_ENTRY(16);
	SET_ENTRY(17);
	SET_ENTRY(18);

	// Hardware handler
	SET_ENTRY(32);
	SET_ENTRY(33);
	SET_ENTRY(34);
	SET_ENTRY(35);
	SET_ENTRY(36);
	SET_ENTRY(37);
	SET_ENTRY(38);
	SET_ENTRY(39);
	SET_ENTRY(40);
	SET_ENTRY(41);
	SET_ENTRY(42);
	SET_ENTRY(43);
	SET_ENTRY(44);
	SET_ENTRY(45);
	SET_ENTRY(46);
	SET_ENTRY(47);

	// System call
	SET_ENTRY(48);

#undef SET_ENTRY
	struct
	{
		uint16_t limit;
		void* pointer;
	} __attribute__((packed)) idtp =
	{
		.limit = IDT_ENTRIES * 8 - 1,
		.pointer = idt,
	};
	__asm__ volatile("lidt %0" : : "m" (idtp));

	// Master-PIC initialisieren
	outb(0x20, 0x11); // Initialisierungsbefehl fuer den PIC
	outb(0x21, 0x20); // Interruptnummer fuer IRQ 0
	outb(0x21, 0x04); // An IRQ 2 haengt der Slave
	outb(0x21, 0x01); // ICW 4

	// Slave-PIC initialisieren
	outb(0xa0, 0x11); // Initialisierungsbefehl fuer den PIC
	outb(0xa1, 0x28); // Interruptnummer fuer IRQ 8
	outb(0xa1, 0x02); // An IRQ 2 haengt der Slave
	outb(0xa1, 0x01); // ICW 4

	// Alle IRQs aktivieren (demaskieren)
	outb(0x20, 0x0);
	outb(0xa0, 0x0);
}

void intr_init(void)
{
	init_gdt();
	init_idt();
}
