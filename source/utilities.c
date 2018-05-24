void ls(){
	// unused function, will print files in pwd when filesystem support is added
	printf("Contents of /\nkernel");
	return;
};
void about(){
	printf("electron\n");
	printf("electron is a freely available, open source, unlicensed alternative to Unix and Linux, that is still in alpha. \n");
	return;
};

void coreutils_start(){
	printf("core-utils v0.0.0a\n");
	add_module('l',&ls);
	add_module('a',&about);
	return;
};
