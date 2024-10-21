void p(char *str)
{
  printf(str);
  return;
}

void n(void)
{
  char buffer [520];

  fgets(buffer,512,stdin);
  p(buffer);
  if (*(int *)0x8049810 == 0x01025544) {
    system("/bin/cat /home/user/level5/.pass");
  }
  return;
}

void main(void)
{
  n();
  return;
}
