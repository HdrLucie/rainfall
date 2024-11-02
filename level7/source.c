char c[69];

void m(void)
{
  time_t tVar1;

  tVar1 = time((time_t *)0x0);
  printf("%s - %d\n",c,tVar1);
  return;
}

int main(int argc,char **argv)
{
  char **puVar1;
  char *pcVar1;
  char **puVar2;
  FILE *file;

  puVar1 = (char **)malloc(8);
  *puVar1 = (char *)0x1;
  pcVar1 = (char *)malloc(8);
  puVar1[1] = pcVar1;
  puVar2 = (char **)malloc(8);
  *puVar2 = (char *)0x2;
  pcVar1 = (char *)malloc(8);
  puVar2[1] = pcVar1;
  strcpy(puVar1[1],argv[1]);
  strcpy(puVar2[1],argv[2]);
  file = fopen("/home/user/level8/.pass","r");
  fgets(c,0x44,file);
  puts("~~");
  return (0);
}

