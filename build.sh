echo "electron by iH8Ra1n. "
echo "this script will build the LATEST electron, from source, and deposit it into the 'kernel' executable. "
mkdir tmp
nasm -f elf32 source/boot.asm -o tmp/asm.o
gcc -fno-stack-protector -m32 -c source/kernel.c -o tmp/kernel.o -I headers
ld -m elf_i386 -T link.ld -o kernel tmp/*.o
echo "done"
