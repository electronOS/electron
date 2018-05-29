/* 
 * electronOS by iH8Ra1n. 
 * NO WARRANTY OR LICENSE IN ANY WAY. 
 * created in 2018. 
 */

#include "KEY_MAP.h"
#include "MAIN.h"

// ADD MODULE
void add_module(int port, inptr ptr) {
	if (port > 131072) {
		return; // implemented buffer overflow protection
	};
	modules[port] = ptr; // assign
	is_mod[is_mod_count] = port; // assign
	is_mod_count++; // increment counter
	return;
};

// RUN MODULE
void run_module(int port, int i) {
	if (port > 131072) {
		return; // buffer overflow protection
	};
	(*modules[port])(i); // execute module
	return;
};

// INITIALIZE IDT
void IDT_INIT() {
	unsigned long keyboard_addr;
	unsigned long idt_addr;
	unsigned long iptr[2];

	// populate the IDT entry of the keyboard's interrupt
	keyboard_addr = (unsigned long) HANDLE_KEY_ASM;
	IDT[0x21].lower_bits = keyboard_addr & 0xffff;
	IDT[0x21].selector = KERNEL_SEGMENT_OFFSET;
	IDT[0x21].zero = 0;
	IDT[0x21].type_attr = INTERRUPT_GATE;
	IDT[0x21].higher_bits = (keyboard_addr & 0xffff0000) >> 16;

	/*
	 *            Ports
	 *         PIC1   PIC2
	 * Command 0x20   0xA0
	 * Data    0x21   0xA1
	 */

	// init ICW1
	WTP(0x20, 0x11);
	WTP(0xA0, 0x11);

	// init ICW2
	WTP(0x21, 0x20);
	WTP(0xA1, 0x28);

	// setup cascading for ICW3
	WTP(0x21, 0x00);
	WTP(0xA1, 0x00);

	// environment info for ICW4
	WTP(0x21, 0x01);
	WTP(0xA1, 0x01);
	// init finished

	// mask interrupts
	WTP(0x21, 0xff);
	WTP(0xA1, 0xff);

	// fill the IDT descriptor
	idt_addr = (unsigned long) IDT;
	iptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	iptr[1] = idt_addr >> 16 ;

	IDT_LOAD(iptr);
	return;
};

// INITIALIZE KEYBOARD
void KEYBOARD_INIT() {
	// 0xFD is 11111101 - enables IRQ1 (keyboard)
	WTP(0x21 , 0xFD);
	return;
};

// PRINT
void printf(const char *s) {
	unsigned int COUNT = 0; // counter
	while (s[COUNT] != '\0') {
		if (s[COUNT] == '\0') {
			return;
		};
		if (s[COUNT] == '\n') {
			NEWLINE(); // newline
			COUNT++; // increment
		};
		video[location++] = s[COUNT++]; // place character
		video[location++] = 0x07; // place attributes
	};
	return;
};

// PRINT WITH COLOR
void printc(const char *s, int color) {
	unsigned int COUNT = 0; // counter
        while (s[COUNT] != '\0') {
                if (s[COUNT] == '\0') {
                        return;
                };
                if (s[COUNT] == '\n') {
                        NEWLINE(); // newline
                        COUNT++; // increment
                };
                video[location++] = s[COUNT++]; // place character
                video[location++] = color; // place attributes
        }
        return;
};

// MAKE NEWLINE
void NEWLINE() {
	unsigned int line_size = BYTES_PER_ELEMENT * COLUMN; // calculate linesize
	location = location + (line_size - location % (line_size)); // increment line
	return;
};

// CLEAR SCREEN
void clear_screen() {
	unsigned int COUNT = 0; // counter
	while (COUNT < SCREEN) {
		video[COUNT++] = ' '; // clear
		video[COUNT++] = 0x07; // attribute
	};
	location = 0;
	return;
};

// CLEAR SCREEN MODULE
void clear_mod() {
	clear_screen();
	mod_success = 1;
	return;
};

int DUMMY() {}; // dummy

// HANDLES KEYPRESSES
void HANDLE_KEY() {
	unsigned char stat; // status
	char code; // keycode

	WTP(0x20, 0x20); // write end of interrupt

	stat = RTP(KEYBOARD_STATUS); // get status
	// lowest bit of status will be set if buffer is not empty
	if (stat & 0x01) {
		code = RTP(KEYBOARD_DATA); // get data
		if(code < 0)
			return;

		if(code == ENTER_KEY) {
			NEWLINE(); // newline
			return;
		};

		char text = key_map[(unsigned char) code]; // get text of key
		video[location++] = key_map[(unsigned char) code]; // place in video memory
		video[location++] = 0x07; // attribute byte
		NEWLINE(); // newline
		run_module(key_map[(unsigned char) code],0); // run module, (if there is no module, it will run DUMMY)
		if (text == 'k') {
			printf("electron electronOS 0.0.2rc1 17:52:41 EST May 28, 2018 x86_64 electron/electronOS"); // print kernel info
		}
		else if (text == 'h') {
			printf("h - displays help\nk - kernel info\npush a module's key to load that module"); // help
		}
		else {
			if (!mod_success) {
				printf("unrecognized command: please refer to help by pressing h"); // no module, or doesn't conform to standard. 
			};
		};
		printf("\nelectronOS# "); // place prompt
	};
};

// ABOUT MODULE
void about() {
	printf("electron is an open source, unlicensed OS. "); // about
	mod_success = 1; // module successful!
	return;
};

// SHUTDOWN MODULE
void shutdown() {
	ALIVE = 1; // shutdown
	mod_success = 1; // module successful!
	return;
};

// START COREUTILS
void coreutils_start() {
	add_module('a', &about); // add about module
	add_module('s', &shutdown); // add shutdown module
	add_module('c', &clear_mod); // add clear screen module
	return;
};

// STARTUP
void STARTUP() {
	// the following code assumes core-utils is used.
	coreutils_start(); // load core-utils, comment to disable core-utils, and get a bare kernel.
};

// SYSCALLS
int SET_ALIVE(int i) {
	ALIVE = i; // set
	return ALIVE; // return what was set
};

// BOOT MESSAGE
void BOOT_MSG() {
	printf("electron v0.0.2.1"); // REMOVE BOTH LINES IF YOU GIVE A WARRANTY
	NEWLINE();
};

// KERNEL
void KERNEL() {
	clear_screen(); // clear the screen

	for (int j; j < 131072; j++) {
		modules[j] = &DUMMY; // set pointer
	};

	STARTUP(); // startup code, add your own C
	IDT_INIT(); // initialize IDT
	KEYBOARD_INIT(); // initialize keyboard
	BOOT_MSG(); // BOOT MESSAGE
	printf("electronOS# "); // print prompt

	while(!ALIVE); // stay alive while alive
};
