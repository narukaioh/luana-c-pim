#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct data {
  int dia;
  int mes;
  int ano;
} Data;

typedef struct paciente {
  int id;
  int idade;
  char nome[15];
  char sobrenome[15];
  char cpf[15];
  Data dataNascimento;
} Paciente;

Paciente cadastrar(void);
Data dataNascimento(void);
int validar(Data data);

int anoAtual () {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   return info->tm_year + 1900;
}

int validar (Data data) {

    // nao existe dia 0 no calendario, data.dia nao pode ser 0
    if (data.dia < 0 || data.dia > 31) {
      printf("\n= Dia precisa ser maior que 0 e menor ou igual a 31. =\n");
      return 1;
    }

    // nao existe mes 0 ou maior que 12
    if (data.mes < 0 || data.mes > 12) {
      printf("\n= Mes precisa ser maior que 0 e menor ou igual a 12. =\n");
      return 1;
    }

    /* o sistema nao aceita pessoas que tenham nascido no futuro
       nem pessoas que nasceram antes de 1901, pois pode ser fraude. */
    if (data.ano > anoAtual() || data.ano < 1901) {
      printf("\n= Ano precisa ser menor que %d e maior que 1901. =\n", anoAtual());
      return 1;
    }

    // se o mes é fevereiro, os dias nao podem ser maiores que 29
    if (data.mes == 2 && data.dia > 29) {
      printf("\n= Mes de fevereiro nao pode ter %d dias. =\n", data.dia);
      return 1;
    }

    // nesses meses a data do dia nao pode ser 31
    if (data.dia > 30 && (data.mes == 4 || data.mes == 6 || data.mes == 9 || data.mes == 11)) {
      printf("\n= Abril, Junho, Setembro Novembro =\n");
      printf("\n= Esses meses nao podem ter 31 dias. =\n");
      return 1;
    }
    return 0;
}

Data dataNascimento() {
  Data data;
  int condicao = 1;
  do {
    printf("  Digite o dia: ");
    scanf("%d", &data.dia);
    printf("  Digite o mes: ");
    scanf("%d", &data.mes);
    printf("  Digite o ano: ");
    scanf("%d", &data.ano);

    condicao = validar(data);
    if (condicao == 1) {
      printf("\n===== DATA INVALIDA =====\n");
      printf("  Digite novamente \n");
    }

  } while (condicao == 1);

  return data;
}

Paciente cadastrar() {
  FILE *file;
  Paciente paciente;


  printf("Primeiro nome: ");
  scanf("%s", &paciente.nome);

  printf("Sobrenome: ");
  scanf("%s", &paciente.sobrenome);

  printf("Data nascimento... \n");
  paciente.dataNascimento = dataNascimento();

  printf("CPF: ");
  scanf("%s", &paciente.cpf);

  return paciente;
}

int main()
{
  Paciente paciente;
  paciente = cadastrar();

  return 0;
}