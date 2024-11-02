For the first level (level0, haha...) we have a binary file `./level0` with the SUID of level1.
It needs an argument, else it segfaults. Indeed, when you disassemble the binary file with GDB, we can see a call to atoi that takes argv[1] as a parameter.
Just below atoi, we can see a call to cmp : it checks if the params we passed is equal to 0x1a7 (423).
So we just need to run the program with 423 as argument, then a call to execv is made with /bin/sh.
Now, we can display the password !
