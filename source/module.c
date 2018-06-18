/*
 * electronOS by iH8Ra1n.
 * NO WARRANTY OR LICENSE IN ANY WAY.
 * created in 2018.
 * ASSUME ALL FILES WERE CREATED BY IH8RA1N UNLESS STATED OTHERWISE! 
 */

#include "module.h"

// ADD MODULE
void add_module(int port, inptr ptr) {
	if (port > 131072) {
		return; // implemented buffer overflow protection
	};
	if (is_mod_count > 131072) {
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
