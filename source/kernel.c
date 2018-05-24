/* 
 * electronOS by iH8Ra1n. 
 * NO WARRANTY OR LICENSE IN ANY WAY. 
 * created in 2018. 
 */

#include "KEY_MAP.h"
#define LINE 25
#define COLUMN 80
#define BYTES_PER_ELEMENT 2
#define SCREEN BYTES_PER_ELEMENT * COLUMN * LINE

#define KEYBOARD_DATA 0x60
#define KEYBOARD_STATUS 0x64
#define IDT_SIZE 256
#define INTERRUPT_GATE 0x8e
#define KERNEL_SEGMENT_OFFSET 0x08

#define ENTER_KEY 0x1C

extern unsigned char keyboard[128];
extern void HANDLE_KEY_ASM();
extern char RTP(unsigned short port);
extern void WTP(unsigned short port, unsigned char data);
extern void IDT_LOAD(unsigned long *iptr);

unsigned int location = 0; // Cursor location
char *video = (char*) 0xb8000; // video memory begins at address 0xb8000
int ALIVE;
int mod_success;

typedef (*inptr)(int);
inptr modules[16384];

int is_mod_count;
int is_mod[16384];

void add_module(int port, inptr ptr) {
	modules[port]=ptr;
	is_mod[is_mod_count]=port;
	is_mod_count++;
	return;
};

void run_module(int port, int i) {
	(*modules[port])(i);
	return;
};

struct IDT_entry {
	unsigned short int lower_bits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int higher_bits;
};

struct IDT_entry IDT[IDT_SIZE];

void IDT_INIT() {
	unsigned long keyboard_addr;
	unsigned long idt_addr;
	unsigned long iptr[2];

	// populate the IDT entry of the keyboard's interrupt
	keyboard_addr = (unsigned long)HANDLE_KEY_ASM;
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
	WTP(0x20 , 0x11);
	WTP(0xA0 , 0x11);

	// init ICW2
	WTP(0x21 , 0x20);
	WTP(0xA1 , 0x28);

	// setup cascading for ICW3
	WTP(0x21 , 0x00);
	WTP(0xA1 , 0x00);

	// environment info for ICW4
	WTP(0x21 , 0x01);
	WTP(0xA1 , 0x01);
	// init finished

	// mask interrupts 
	WTP(0x21 , 0xff);
	WTP(0xA1 , 0xff);

	// fill the IDT descriptor
	idt_addr = (unsigned long) IDT;
	iptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	iptr[1] = idt_addr >> 16 ;

	IDT_LOAD(iptr);
	return;
}

void KEYBOARD_INIT() {
	// 0xFD is 11111101 - enables IRQ1 (keyboard)
	WTP(0x21 , 0xFD);
	return;
}

void printf(const char *s) {
	unsigned int COUNT = 0;
	while (s[COUNT] != '\0') {
		if (s[COUNT] == '\n') {
			NEWLINE();
			COUNT++;
		};
		video[location++] = s[COUNT++];
		video[location++] = 0x07;
	}
	return;
}

void NEWLINE() {
	unsigned int line_size = BYTES_PER_ELEMENT * COLUMN;
	location = location + (line_size - location % (line_size));
	return;
}

void clear_screen() {
	unsigned int COUNT = 0;
	while (COUNT < SCREEN) {
		video[COUNT++] = ' ';
		video[COUNT++] = 0x07;
	}
	return;
}

int DUMMY(){};

void HANDLE_KEY() {
	unsigned char stat;
	char code;

	WTP(0x20, 0x20); // write end of interrupt

	stat = RTP(KEYBOARD_STATUS);
	// lowest bit of status will be set if buffer is not empty 
	if (stat & 0x01) {
		code = RTP(KEYBOARD_DATA);
		if(code < 0)
			return;

		if(code == ENTER_KEY) {
			NEWLINE();
			return;
		}

		char text=key_map[(unsigned char) code];
		video[location++] = key_map[(unsigned char) code];
		video[location++] = 0x07;
		NEWLINE();
		run_module(key_map[(unsigned char) code],0);
		if (text == 'k') {
			printf("electron electronOS 0.0.0-alpha 5:21 PM May 24, 2018 x86_64 electron/electronOS");
		}
		else if (text == 'h') {
			printf("h - displays help\nk - kernel info\npush a module's key to load that module");
		}
		else {
			run_module(key_map[(unsigned char) code],0);
			if(!mod_success){
				printf("unrecognized command: please refer to help by pressing h");
			};
		};
		printf("\nelectronOS# "); // place prompt
	}
}

void about(){
	printf("electron is an open source, unlicensed OS. ");
	mod_success=1;
	return;
};

void coreutils_start(){
	add_module('a',&about);
	return;
};


void STARTUP(){
	// the following code assumes core-utils is used. 
	coreutils_start(); // load core-utils, comment to disable core-utils, and get a bare kernel. 
};

// SYSCALLS
int SET_ALIVE(int i){
	ALIVE=i;
	return ALIVE;
};

void KERNEL() {
	clear_screen();

	for (int j; j < 16384; j++){
		modules[j]=&DUMMY;
	};
	STARTUP();

	IDT_INIT();
	KEYBOARD_INIT();
	printf("electronOS# ");

	while(!ALIVE);
}
