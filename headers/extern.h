void handle_extern(int i); // external commands
void about_freedom(); // about freedom
void clear_mod(); // clear command
void shutdown(); // shutdown command
char *cmd = (char*)0xfffaff; // cmd starts at 0xfffbff to prevent a buffer overflow discovered by iH8Ra1n. (github.com/iHateRa1n & twitter.com/iHateRa1n)
int ALIVE; // if machine is alive
int mod_success; // module success
