/*
 * electronOS by iH8Ra1n.
 * NO WARRANTY OR LICENSE IN ANY WAY.
 * created in 2018.
 * ASSUME ALL FILES WERE CREATED BY IH8RA1N UNLESS STATED OTHERWISE! 
 */

#include "module.h"
#include "extern.h"
#include "stdio.h"
#include "main.h"

// CLEAR SCREEN COMMAND / MODULE
void clear_mod() {
	clear_screen(); // clear screen
	mod_success = 1; // success
	return;
};

// ABOUT COMMAND / MODULE
void about() {
	printf("electron is an open source, unlicensed kernel. Type 'freedom', for knowledge on free software. "); // about
	mod_success = 1; // module successful!
	return;
};

// SHUTDOWN COMMAND / MODULE
void shutdown() {
	ALIVE = 1; // shutdown
	mod_success = 1; // module successful!
	return;
};

// FREEDOM COMMAND / MODULE
void about_freedom() {
	printf("electron is free software. There is no license in any way. Programs you use under the electron kernel, may have a separate license to electron; so check its license before use. ");
	mod_success = 1;
	return;
};

// Array TO Integer
int atoi(char *s) {
	int result = 0;
	for (int count = 0; s[count] != '\0'; count++)
		result = result * 10 + s[count] - '0'; // process string into integer
	return result;
}

// Implementation of itoa()
char* itoa(int k, char* s, int base) {
	int count = 0;
	int is_negative = 0;
	// If k == 0, it would just return null. 
	if (k == 0) {
		s[count++] = '0';
		s[count] = '\0';
		return count;
	};
	
	// Process negative numbers
	if (k < 0 && base == 10) {
		is_negative = 1;
		k = -k;
	};

	// Process individual digits
	while (k != 0) {
		int remain = k % base; // Remainder
		s[count++] = (remain > 9) ? (remain-10) + 'a' : remain + '0'; // Get character
		k = k/base; // Divide, for the next remainder. 
	};

	// If the number is negative, append '-'.
	if (is_negative)
		s[count++] = '-';

	s[count] = '\0'; // Append string terminator

	return s;
}

void add_handle() {
	int k;
	for (int j = 0; j < 1024; j++) {
		if (cmd[j] == ' ') {
			k = j;
		};
	};
	char a1[512];
	for (int j = 0; j < k; j++) {
        	if (k < 512) {
        	        a1[j] = cmd[j];
        	};
	};
	char a2[512];
	for (int j = 0; j < k; j++) {
        	if (k < 512) {
        	        a2[j] = cmd[j + k + 1];
        	};
	};
	int i1;
	int i2;
	i1 = atoi(a1);
	i2 = atoi(a2);
	char *s[1024];
	itoa(i1 + i2, &s, 10);
	printf(s);
	add_module(0x1aaaa, &handle_keyboard);
};

void add() {
	add_module(0x1aaaa, &add_handle);
	return;
};

void sub_handle() {
	int k;
	for (int j = 0; j < 1024; j++) {
		if (cmd[j] == ' ') {
			k = j;
		};
	};
	char a1[512];
	for (int j = 0; j < k; j++) {
      		if (k < 512) {
        		a1[j] = cmd[j];
        	};
	};
	char a2[512];
	for (int j = 0; j < k; j++) {
        	if (k < 512) {
        		a2[j] = cmd[j + k + 1];
        	};
	};
	int i1;
	int i2;
	i1 = atoi(a1);
	i2 = atoi(a2);
	char *s[1024];
	itoa(i1 - i2, &s, 10);
	printf(s);
	add_module(0x1aaaa, &handle_keyboard);
};

void sub() {
	add_module(0x1aaaa, &sub_handle);
	return;
};

void mlt_handle() {
	int k;
	for (int j = 0; j < 1024; j++) {
		if (cmd[j] == ' ') {
			k = j;
		};
	};
	char a1[512];
	for (int j = 0; j < k; j++) {
		if (k < 512) {
        		a1[j] = cmd[j];
        	};
	};
	char a2[512];
	for (int j = 0; j < k; j++) {
		if (k < 512) {
        		a2[j] = cmd[j + k + 1];
        	};
	};
	int i1;
	int i2;
	i1 = atoi(a1);
	i2 = atoi(a2);
	char *s[1024];
	itoa(i1 * i2, &s, 10);
	printf(s);
	add_module(0x1aaaa, &handle_keyboard);
};

void mlt() {
	add_module(0x1aaaa, &mlt_handle);
	return;
};

void ls() {
	const char about_ch[1024]     = "about";
	const char shutdown_ch[1024]  = "shutdown";
	const char clear_ch[1024]     = "clear";
	const char freedom_ch[1024]   = "freedom";
	const char add_ch[1024]       = "add";
	const char sub_ch[1024]       = "subtract";
	const char mlt_ch[1024]       = "multiply";
	const char ls_ch[1024]        = "ls";
	const char ls_def[1024];
	const char *ls_ptr[1024];
	for (int k; k < 1024; k++) {
		ls_ptr[k] = &ls_def;
	};
	ls_ptr[0] = &about_ch;
	ls_ptr[1] = &shutdown_ch;
	ls_ptr[2] = &clear_ch;
	ls_ptr[3] = &freedom_ch;
	ls_ptr[4] = &add_ch;
	ls_ptr[5] = &sub_ch;
	ls_ptr[6] = &mlt_ch;
	ls_ptr[7] = &ls_ch;
	for (int ls_c = 0; ls_c < 8; ls_c++) {
		printf(ls_ptr[ls_c]);
        	NEWLINE();
	};
	return;
};

// EXTERNAL COMMAND HANDLER
void handle_extern(int i) {
	const char about_chr[1024]    = "about";
	const char shutdown_chr[1024] = "shutdown";
	const char clear_chr[1024]    = "clear";
	const char freedom_chr[1024]  = "freedom";
	const char add_chr[1024]      = "add";
	const char sub_chr[1024]      = "subtract";
	const char mlt_chr[1024]      = "multiply";
	const char ls_chr[1024]       = "ls";
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
	else if (strcmp(cmd, add_chr) == 0) {
		add();
	}
	else if (strcmp(cmd, sub_chr) == 0) {
		sub();
	}
	else if (strcmp(cmd, mlt_chr) == 0) {
		mlt();
	}
	else if (strcmp(cmd, ls_chr) == 0) {
		ls();
	}
	else {
		printf("electron: unknown command\n");
	};
	return;
};
