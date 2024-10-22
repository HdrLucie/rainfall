void o(void)
{
  system("/bin/sh");
                    /* WARNING: Subroutine does not return */
  _exit(1);
}

void n(void)
{
  char buffer [520];

  fgets(buffer,0x200,stdin);
  printf(buffer);
                    /* WARNING: Subroutine does not return */
  exit(1);
}

int main(void)
{
  n();
  return 0;
}
