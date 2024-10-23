void n(void)
{
  system("/bin/cat /home/user/level7/.pass");
}

void m(void)
{
  puts("Nope");
}

void main(int argc,char **argv)
{
  char *buffer;
  void (**functionPtr)(void);

  buffer = (char *)malloc(64);
  functionPtr = (void (*)(void))malloc(4);
  *functionPtr = &m;
  strcpy(buffer,argv[1]);
  (**functionPtr)();
}
