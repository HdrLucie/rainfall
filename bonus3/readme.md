# bonus3

This one's pretty easy (once you understand it)!
There's only the main function. This takes one argument. 

In our main function, we see a call to the `fopen` on the password we need. 
Then a check is made: if the file couldn't be opened or if there are not two arguments, and -1 is returned. 
Otherwise, the `fread` function is called twice and the returns from `fread` are given to a buffer1 and a buffer2. 
Finally, compare our argv[1] and buffer1 with `strcmp`. If our buffer and our argv[1] are identical, we execute ‘/bin/sh/.’. 

However, for our buffer and argv[1] to be identical, we need to pass the flag as a parameter... How do we do this?  
In fact, after the first fread, we see a call to `atoi` with our argv[1], the return from `atoi` is given to iVar1.  
And it's thanks to `atoi` that we're going to be able to enter the if. We want iVar1 to be equal to 0.   
And to make iVar1 equal to 0, all we have to do is pass an empty string to `atoi`...  

```bash
bonus3@RainFall:~$ ./bonus3 ""
$ whoami
end
$ cat /home/user/end/.pass
3321b6f81659f9a71c76616f606e4b50189cecfea611393d5d649f75e157353c
bonus3@RainFall:~$ su end
Password: 
end@RainFall:~$ ls
end
end@RainFall:~$ cat end 
Congratulations graduate!
end@RainFall:~$ 
```

And that's it!
