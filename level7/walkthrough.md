# level7

We continue the heap overflow saga. 
So we have a binary file (as always) with several functions : especially `main` and `m` functions.
Our program take two arguments which are copied with `strcpy` function, in two strings that are malloc'd at the beginning of `main` function. 

Then, we can see a call to fopen, which takes path of the level8 pass file. Then, the content of the file is given to the global variable `c`. 
So, we would like to print the content of `c` ! Luckily, `m` function is here. Indeed, we can see a call to printf in `m` function, which prints the content of `c` and the return of `time` function.  

We need to find a way to call `m` function in order to print our pass; and this way is the call to `puts` (thank you)!

The thing we have to do, is to write `m` address: `0x080484f4` in place of `puts` adress in the GOT which is: `0x8049928`. 
To overflow, we use calls to `strcpy`, we use the first one to replace the address of the second `strcpy` with `puts` address and we give the second one the address of `m`. 

This is a dump of the heap with the arguments `AAAA` (41414141) and `BBBB` (42424242).
```
x/16xw 0x804a008
0x804a008:	0x00000001	0x0804a018	0x00000000	0x00000011
0x804a018:	0x41414141	0x00000000	0x00000000	0x00000011
0x804a028:	0x00000002	0x0804a038	0x00000000	0x00000011
0x804a038:	0x42424242	0x00000000	0x00000000	0x00020fc1
```

We can see that there are 20 bytes between our first string and the second. So we need to write 20 characters before typing the address of `puts` in the GOT. And just have to give the address of `m` as second argument. 
And here we are :
```bash 
./level7 $(python -c 'print "A" * 20 + "\x08\x04\x99\x28"[::-1] + " " + "\x08\x04\x84\xf4"[::-1]')
```
