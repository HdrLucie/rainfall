void v(void)
{
  char buffer [520];
  fgets(buffer,512,stdin);
  printf(buffer);
  if (*(int *)0x804988c == 64) {
    fwrite("Wait what?!\n",1,12,stdout);
    system("/bin/sh");
  }
  return;
}

int main(void)
{
  v();
  return 0;
}
