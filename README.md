# electron

electron is a free, and open source kernel; which is unlicensed. 

It is based on the x86 architecture, and the VGA video card. Support may be added for SVGA. 

# VGA

The VGA text buffer starts at 0xb8000, and this is where electron puts output. 

Each character in a VGA font is 16 bytes, with each bit simply defining if a pixel is foreground or background.

# KERNEL

The electron kernel, as said before, only runs on x86 hardware, and I have only seen it work in Qemu.

The kernel, is in `KERNEL`. It calls many other functions, like `BOOT_MSG` (a boot message), and `IDT_INIT` (initalizes the IDT).

The kernel is based upon modules.

Programs, are modules; they are added into the kernel at compilation.

You can use `add_module(_PORT, _POINTER)` to add a module.

Where `_PORT` is the port you wish to place this module at, and `_POINTER` is a pointer to a function that takes an `int` as an argument, and returns `void`.

After this, the module can be called with `run_module(_PORT, _ARG)`, where `_PORT` is the port of the module, and `_ARG` is an argument to be passed to the module.

# LICENSE

electron is not licensed, and you cannot license it. 

# THANKS TO

@iHateRa1n - many security bugs. 
@th0ex     - 1 security bug, arbitrary write to kernel memory. 
