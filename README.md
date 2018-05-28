# ABOUT
electron is a free, open source, unlicensed kernel. 

The electron kernel is designed around the x86 architecture, and the VGA video card. Support could be added for SVGA, or even newer cards, but I will only support VGA for now. It may be ported to other platforms, but I'm not sure. 

electron is just a hobby project, but I hope to grow it's functionality in the future. 

# VGA

The VGA text buffer starts at 0xb8000, and this is where all video memory is written for electron. 

Each character in a VGA font is 16 bytes, simply defining if a pixel is foreground or background, 1 or 0. 

# KERNEL

The electron kernel, as said before, only runs on x86 hardware, and I have only seen it work in Qemu. 

The kernel is based upon modules. 

Programs, are modules; they are added into the kernel at compilation.

You can use `add_module(_PORT, _POINTER)` to add a module. 

Where `_PORT` is the port you wish to place this module on, and `_POINTER` is a pointer to a function that takes an `int`, and returns  

After this, the module can be called with `run_module(_PORT, _ARG)`, where `_PORT` is the port of the module, and `_ARG` is an argument to the module. 

# LICENSE

You are free to do whatever you wish with this software, EXCEPT license it! 

I, personally, dislike licenses. I dislike licenses, because you cannot do certain actions with software; and this angers me. 

I created electron because of this, and as a hobby project. 
