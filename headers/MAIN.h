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

extern void HANDLE_KEY_ASM(); // assembly key handler
extern char RTP(unsigned short port); // read port
extern void WTP(unsigned short port, unsigned char data); // write port
extern void IDT_LOAD(unsigned long *iptr); // load idt
int location = 0; // Cursor location
char *video = (char*) 0xb8000; // video memory begins at address 0xb8000
char KERNEL_LOG[80*25];
int ALIVE; // if machine is alive
int mod_success; // module success

typedef (*inptr)(int); // for modules
inptr modules[131072]; // modules

int is_mod_count; // count
int is_mod[131072]; // for modules

struct IDT_entry {
	unsigned short int lower_bits;
	unsigned short int selector;
	unsigned char zero;
	unsigned char type_attr;
	unsigned short int higher_bits;
};

struct IDT_entry IDT[IDT_SIZE];

void add_module(int port, inptr ptr);
void run_module(int port, int i);
void IDT_INIT();
void KEYBOARD_INIT();
void printf(const char *s);
void NEWLINE();
void clear_screen();
void clear_mod();
void HANDLE_KEY();
void about();
void shutdown();
void coreutils_start();
void STARTUP();
void BOOT_MSG();
void test_mod(int k);
void log(const char *s);

int DUMMY();
int SET_ALIVE(int i);
