#include "module.h"
#include "extern.h"
#include "stdio.h"

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

// EXTERNAL COMMAND HANDLER
void handle_extern(int i) {
	char about_chr[1024]    = "about";
	char shutdown_chr[1024] = "shutdown";
	char clear_chr[1024]    = "clear";
	char freedom_chr[1024]  = "freedom";
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
	return;
};
