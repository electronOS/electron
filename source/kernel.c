/*
 * electronOS by iH8Ra1n.
 * NO WARRANTY OR LICENSE IN ANY WAY.
 * created in 2018.
 */

#include "KEY_MAP.h"
#include "main.h"
#include "module.h"
#include "stdio.h"
#include "extern.h"

// INITIALIZE IDT
void IDT_INIT() {
	unsigned long keyboard_addr; // KEYBOARD ADDRESS
	unsigned long idt_addr; // IDT ADDRESS
	unsigned long iptr[2];

	// populate the IDT entry of the keyboard's interrupt
	keyboard_addr = (unsigned long) HANDLE_KEY_ASM; // keyboard address
	IDT[0x21].lower_bits = keyboard_addr & 0xffff; // IDT's lower bits
	IDT[0x21].selector = KERNEL_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].higher_bits = (keyboard_addr & 0xffff0000) >> 16; // IDT's higher bits

	/*
	 *            Ports
	 *         PIC1   PIC2
	 * Command 0x20   0xA0
	 * Data    0x21   0xA1
	 */

	// init ICW1
	WTP(0x20, 0x11); // PIC1 COMMAND
	WTP(0xA0, 0x11); // PIC2 COMMAND

	// init ICW2
	WTP(0x21, 0x20); // PIC1 DATA
	WTP(0xA1, 0x28); // PIC2 DATA

	// setup cascading for ICW3
	WTP(0x21, 0x00); // PIC1 DATA
	WTP(0xA1, 0x00); // PIC2 DATA

	// environment info for ICW4
	WTP(0x21, 0x01); // PIC1 DATA
	WTP(0xA1, 0x01); // PIC2 DATA
	// init finished

	// mask interrupts
	WTP(0x21, 0xff); // PIC1 DATA
	WTP(0xA1, 0xff); // PIC2 DATA

	// fill the IDT descriptor
	idt_addr = (unsigned long) IDT;
	iptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	iptr[1] = idt_addr >> 16 ;

	IDT_LOAD(iptr); // load IDT
	return;
};

// INITIALIZE KEYBOARD
void KEYBOARD_INIT() {
	// 0xFD is 11111101 - enables IRQ1 (keyboard)
	WTP(0x21 , 0xFD); // enable IRQ1 (keyboard)
	return;
};

// MAKE NEWLINE
void NEWLINE() {
	unsigned int line_size = BYTES_PER_ELEMENT * COLUMN; // calculate linesize
	location = location + (line_size - location % (line_size)); // increment line
	return;
};

int DUMMY() {}; // dummy

void handle_keyboard(int i) {
	if (strcmp(cmd, HELP_STR) == 0) {
		printf("help - this menu\nkernel - kernel info"); // print help menu
		NEWLINE(); // newline
	}
	else if (strcmp(cmd, KERN_STR) == 0) {
		printf("electron electronOS v0.0.6 15:57:03 EST June 2, 2018 electron/electronOS RELEASE"); // print kernel information
		NEWLINE(); // newline
	}
	else {
		run_module(0x1fd00, 0);
		NEWLINE(); // newline
	};
	printc("electron ", 0x02); // prompt
	printc("# ", 0x04); // prompt
	return;
};

// HANDLES KEYPRESSES
void HANDLE_KEY() {
	unsigned char stat; // status
	char code; // keycode

	WTP(0x20, 0x20); // write end of interrupt

	stat = RTP(KEYBOARD_STATUS); // get status
	// lowest bit of status will be set if buffer is not empty
	if (stat & 0x01) {
		code = RTP(KEYBOARD_DATA); // get data
		if (code < 0)
			return;

		if (code == ENTER_KEY) {
			cmd_count = 0; // write to the correct place
			NEWLINE(); // newline
			run_module(0x1aaaa, 0); // handle keyboard input
			for (int j; j < 1024; j++) {
				cmd[j] = 0; // reset command
			};
			return;
		};

		cmd[cmd_count++] = key_map[(unsigned char) code]; // place in command
		video[location++] = key_map[(unsigned char) code]; // place in video memory
		video[location++] = 0x07; // attribute byte
	};
};

// STARTUP
void STARTUP() {
	add_module(0x1aaaa, &handle_keyboard); // keyboard handling module
	add_module(0x1fd00, &handle_extern); // handle custom commands (extern.c)
	return;
};

// SYSCALLS
int SET_ALIVE(int i) {
	ALIVE = i; // set
	return ALIVE; // return what was set
};

// BOOT MESSAGE
void BOOT_MSG() {
	NEWLINE(); // newline
	printf("electron 0.1.1");
	NEWLINE(); // newline
	return;
};

// KERNEL
void KERNEL() {
	clear_screen(); // clear the screen

	printf("        _           _\n\
       | |         | |\n\
    ___| | ___  ___| |_ _ __ ___  _ __\n\
   / _ | |/ _ \\/ __| __| '__/ _ \\| '_ \\\n\
  |  __| |  __| (__| |_| | | (_) | | | |\n\
   \\___|_|\\___|\\___|\\__|_|  \\___/|_| |_|\n"); // boot message

	for (int j; j < 131072; j++) {
		modules[j] = &DUMMY; // set pointer
	};

	STARTUP(); // startup code, add your own C
	printf("[KERNEL] initializing IDT\n"); // kernel messages
	IDT_INIT(); // initialize IDT
	printf("[KERNEL] initializing keyboard"); // kernel messages
	KEYBOARD_INIT(); // initialize keyboard
	NEWLINE();
	BOOT_MSG(); // BOOT MESSAGE
	printc("electron", 0x02); // prompt
	printc("# ", 0x04); // prompt

	while(!ALIVE); // stay alive while alive
};
