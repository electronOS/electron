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

extern char RTP(unsigned short port); // read port
extern void HANDLE_KEY_ASM(); // assembly key handler
extern void WTP(unsigned short port, unsigned char data); // write port
extern void IDT_LOAD(unsigned long *iptr); // load idt
extern void handle_module(char command[1024]);
int location = 0; // Cursor location
char *video = (char*) 0xb8000; // video memory begins at address 0xb8000
int ALIVE; // if machine is alive
int mod_success; // module success
int cmd_count=0; // command write location

char cmd[1024]=""; // command
char HELP_STR[1024]="help"; // help string
char KERN_STR[1024]="kernel"; // kernel string (for kernel info)

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

void add_module(int port, inptr ptr); // add module
void run_module(int port, int i); // run module
void IDT_INIT(); // initialize IDT
void KEYBOARD_INIT(); // initialize keyboard
void printf(const char *s); // print data to VGA Text Buffer
void NEWLINE(); // newline in VGA Text Buffer
void clear_screen(); // clear screen
void clear_mod(); // clear screen module
void HANDLE_KEY(); // handle keyboard input (C)
void about(); // about module
void shutdown(); // shutdown module
void coreutils_start(); // add coreutils to module list
void STARTUP(); // startup
void BOOT_MSG(); // boot message
void handle_keyboard(int i);
int strcmp(char s1[1024], char s2[1024]);
int DUMMY(); // dummy function
int SET_ALIVE(int i); // SET_ALIVE syscall
