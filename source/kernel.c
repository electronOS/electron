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
	unsigned long keyboard_addr; // KEYBOARD ADDRESS
	unsigned long idt_addr; // IDT ADDRESS
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
                        return; // EOL
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
	location = 0; // reset where video memory is being written
	return;
};

// CLEAR SCREEN MODULE
void clear_mod() {
	clear_screen(); // clear screen
	mod_success = 1; // success
	return;
};

int DUMMY() {}; // dummy

int strcmp(char s1[1024], char s2[1024]) {
	while (*s2) {
		if (*s1 != *s2) {
			return 1;
		};
		*s1++;
		*s2++;
	};
	return 0; // same
};

void handle_keyboard(int i) {
	if (strcmp(cmd, HELP_STR) == 0) {
		printf("help - this menu\nkernel - kernel info"); // print help menu
		NEWLINE(); // newline
	}
	else if (strcmp(cmd, KERN_STR) == 0) {
		printf("electron electronOS v0.0.5 12:42:43 EST May 31, 2018 electron/electronOS RELEASE"); // print kernel information
		NEWLINE(); // newline
	}
	else {
		run_module(0x1fd00, 0);
		NEWLINE(); // newline
	};
	printf("# "); // prompt
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
			for (int j; j < 64; j++) {
				cmd[j] = 0; // reset command
			};
			return;
		};

		cmd[cmd_count++] = key_map[(unsigned char) code]; // place in command
		video[location++] = key_map[(unsigned char) code]; // place in video memory
		video[location++] = 0x07; // attribute byte
	};
};

// ABOUT MODULE
void about() {
	printf("electron is an open source, unlicensed kernel. Type 'freedom', for knowledge on free software. "); // about
	mod_success = 1; // module successful!
	return;
};

// SHUTDOWN MODULE
void shutdown() {
	ALIVE = 1; // shutdown
	mod_success = 1; // module successful!
	return;
};

void about_freedom() {
	printf("electron is free software. There is no license in any way. Programs you use under the electron kernel, may have a separate license to electron; so check it's license before use. ");
	mod_success = 1;
	return;
};

void handle_extern(int i) {
	char about_chr[1024] = "about";
	char shutdown_chr[1024] = "shutdown";
	char clear_chr[1024] = "clear";
	char freedom_chr[1024] = "freedom";
	if (strcmp(cmd, about_chr) == 0) {
		about();
	}
	else if (strcmp(cmd, shutdown_chr) == 0) {
		shutdown();
	}
	else if (strcmp(cmd, clear_chr) == 0) {
		clear_screen();
	}
	else if (strcmp(cmd, freedom_chr) == 0) {
		about_freedom();
	}
	else {
		printf("electron: unknown command\n");
	};
};

// STARTUP
void STARTUP() {
	// the following code assumes core-utils is used.
	add_module(0x1aaaa, &handle_keyboard); // keyboard handling module
	add_module(0x1fd00, &handle_extern);
};

// SYSCALLS
int SET_ALIVE(int i) {
	ALIVE = i; // set
	return ALIVE; // return what was set
};

// BOOT MESSAGE
void BOOT_MSG() {
	NEWLINE(); // newline
	printf("electron v0.0.5");
	NEWLINE(); // newline
};

// KERNEL
void KERNEL() {
	clear_screen(); // clear the screen

	printf("       _           _                   \n\
       | |         | |                  \n\
    ___| | ___  ___| |_ _ __ ___  _ __  \n\
   / _ | |/ _ \\/ __| __| '__/ _ \\| '_ \\ \n\
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
	printf("# "); // prompt

	while(!ALIVE); // stay alive while alive
};
