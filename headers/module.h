typedef (*inptr)(int); // for modules
inptr modules[131072]; // modules

int is_mod_count; // count
int *is_mod = (int*) 0xfffb38; // make is_mod unusable for a buffer overflow
void add_module(int port, inptr ptr); // add module
void run_module(int port, int i); // run module
