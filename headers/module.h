/*
 * electronOS by iH8Ra1n.
 * NO WARRANTY OR LICENSE IN ANY WAY.
 * created in 2018.
 * ASSUME ALL FILES WERE CREATED BY IH8RA1N UNLESS STATED OTHERWISE! 
 */

typedef (*inptr)(int); // for modules
inptr *modules = (inptr*) 0xf00000; // modules

int is_mod_count; // count
int *is_mod = (int*) 0xfffb38; // make is_mod unusable for a buffer overflow
void add_module(int port, inptr ptr); // add module
void run_module(int port, int i); // run module
