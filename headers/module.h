typedef (*inptr)(int); // for modules
inptr modules[131072]; // modules

int is_mod_count; // count
int is_mod[131072]; // for modules
void add_module(int port, inptr ptr); // add module
void run_module(int port, int i); // run module
