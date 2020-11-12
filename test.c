#include<stdio.h>
#include<stdlib.h>

int main() {
  FILE * file;
  file = fopen("t.txt", "a+");
  int res = fprintf(file, "cidade: %d mensagem: %s\n", 10, "ola mundo");
  if (res == EOF)
    printf("hmmm");
  fclose(file);
  return 0;
}