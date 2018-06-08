#define LINE 25
#define COLUMN 80
#define BYTES_PER_ELEMENT 2
#define SCREEN BYTES_PER_ELEMENT * COLUMN * LINE

int location = 0; // Cursor location
char *video = (char*) 0xb8000; // video memory begins at address 0xb8000

void printc(const char *s, int color); // print data with color to VGA Text Buffer
void printf(const char *s); // print data to VGA Text Buffer
void clear_screen(); // clear screen
int strcmp(char s1[1024], char s2[1024]); // strcmp
int strlen(char s[]);
