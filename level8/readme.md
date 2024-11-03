# level8

This level introduces heap exploitation through memory layout manipulation.

There are two global variables:
- `auth`: a pointer to an authentication structure
- `service`: a string pointer

The program implements a simple command line interface in its `main` function that recognizes 4 commands:
- `auth `: allocates a small buffer and copies the provided string into it
- `reset`: frees the auth buffer
- `service`: duplicates its argument into a new buffer
- `login`: spawns a shell if a specific value in the auth structure is non-zero

The interesting part is in the way memory is handled. Looking at the disassembly, we can see that `auth` only gets allocated 4 bytes with `malloc(4)`, even though it's meant to hold a 32-byte string and an integer. The string copy is protected by a length check (`strlen <= 30`), so we can't overflow it directly.

However, since `service` uses `strdup` to allocate new buffers, and malloc tends to allocate contiguous memory blocks, we can manipulate the heap layout to get a `service` buffer to land exactly 32 bytes (0x20) after `auth`. When this happens, the service string data will overwrite what would be the integer field in the auth structure, allowing us to pass the login check.

Here's how to exploit it:
```
level8@RainFall:~$ ./level8 
(nil), (nil)          # Both pointers start as NULL
auth                  # Allocate auth buffer at 0x804a008
0x804a008, (nil)
service               # First service allocation at 0x804a018
0x804a008, 0x804a018
service               # Second service lands at 0x804a028 (auth + 0x20)
0x804a008, 0x804a028 # This overwrites auth->n with non-zero data
login                 # Success!
$ whoami
level9
```

We actually found this solution by just trying stuff, after having noticed that the Ghidra disassembly had this line:
```c
if (*(int *)(auth + 0x20) == 0) {
```

In the end, because the assembly had a lot of inlined calls, it took way longer to understand what the code was doing and disassemble it than it took to just break it lol