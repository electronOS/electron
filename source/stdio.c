#include "stdio.h"

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
		if (location >= 0xfa0) {
			 return;
		};
		video[location++] = s[COUNT++]; // place character
		video[location++] = 0x07; // place attributes
	};
	return;
};

// GET LENGTH OF STRING UP TO A NULL
int strlen(char s[]) {
	int n;
	while (*s) {
		if (*s != 0) {
			n++;
		}
		else {
			break;
		};
		*s++;
	};
	return n;
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
                if (location > 0xfa0) {
					return;
				};
                video[location++] = s[COUNT++]; // place character
                video[location++] = color; // place attributes
        }
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

int strcmp(const char s1[1024], const char s2[1024]) {
	while (*s2) {
		if (*s1 != *s2) {
			return 1;
		};
		*s1++;
		*s2++;
	};
	return 0; // same
};
