bits 32
section .text
	align 4
	dd 0x1BADB002; multiboot magic
	dd 0x00; flags
	dd - (0x1BADB002 + 0x00); checksum

global DEVICE_START
global HANDLE_KEY_ASM
global HANDLE_KEY
global RTP
global WTP
global IDT_LOAD

extern KERNEL; KERNEL is the electronOS kernel
extern HANDLE_KEY; HANDLE_KEY handles the keyboard

RTP:
	mov edx, [esp + 4];
	in al, dx; AL is the lower 8 bits of the EAX. DX is the lower 16 bits of the EDX. 
	ret;

WTP:
	mov edx, [esp + 4];
	mov al, [esp + 4 + 4];
	out dx, al;
	ret;

IDT_LOAD:
	mov edx, [esp + 4];
	lidt [edx];
	sti; enable interrupts
	ret;

HANDLE_KEY_ASM:
	call HANDLE_KEY; Run HANDLE_KEY, handles keypress. 
	iretd;

DEVICE_START:
	cli;
	mov esp, STACK;
	call KERNEL;
	hlt;

section .bss
resb 16777216; 1 MB for the stack. 
STACK:
