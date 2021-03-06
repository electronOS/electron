echo "electron by iH8Ra1n. "
echo "this script will build the LATEST electron, from source, and deposit it into the 'kernel' executable. "
mkdir tmp
nasm -f elf32 source/boot.asm -o tmp/asm.o
gcc -fno-stack-protector -m32 -c source/kernel.c -o tmp/kernel.o -I headers
gcc -fno-stack-protector -m32 -c source/module.c -o tmp/module.o -I headers
gcc -fno-stack-protector -m32 -c source/stdio.c -o tmp/stdio.o -I headers
gcc -fno-stack-protector -m32 -c source/extern.c -o tmp/extern.o -I headers
ld -m elf_i386 -T link.ld -o kernel tmp/*.o --allow-multiple-definition
echo "done"
