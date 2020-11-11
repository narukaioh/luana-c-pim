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

void clear(void);
int anoAtual(void);
int mesAtual(void);
int validar(Data data);
Data dataNascimento(void);
void buscar(int id);
int calcularIdade(Data data);
int criarId(void)
void cadastrar(void);
void menu (void);

int main() {
  clear();
  cadastrar();
  buscar(1);
  return 0;
}

void clear () {
  for ( int i = 0; i < 50; i++ ) {
    printf("\n");
  }
  system("clear");
}

int anoAtual () {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   return info->tm_year + 1900;
}

int mesAtual() {
   time_t rawtime;
   struct tm *info;
   time( &rawtime );
   info = localtime( &rawtime );
   return info->tm_mon;
}

int validar (Data data) {
    // nao existe dia 0 no calendario, data.dia nao pode ser 0
    if (data.dia < 0 || data.dia > 31) {
      clear();
      printf("\n= Dia precisa ser maior que 0 e menor ou igual a 31. =\n");
      return 1;
    }

    // nao existe mes 0 ou maior que 12
    if (data.mes < 0 || data.mes > 12) {
      clear();
      printf("\n= Mes precisa ser maior que 0 e menor ou igual a 12. =\n");
      return 1;
    }

    /* o sistema nao aceita pessoas que tenham nascido no futuro
       nem pessoas que nasceram antes de 1901, pois pode ser fraude. */
    if (data.ano > anoAtual() || data.ano < 1901) {
      clear();
      printf("\n= Ano precisa ser menor que %d e maior que 1901. =\n", anoAtual());
      return 1;
    }

    // se o mes é fevereiro, os dias nao podem ser maiores que 29
    if (data.mes == 2 && data.dia > 29) {
      clear();
      printf("\n= Mes de fevereiro nao pode ter %d dias. =\n", data.dia);
      return 1;
    }

    // nesses meses a data do dia nao pode ser 31
    if (data.dia > 30 && (data.mes == 4 || data.mes == 6 || data.mes == 9 || data.mes == 11)) {
      clear();
      printf("\n= Abril, Junho, Setembro e Novembro =");
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
      printf("  Digite novamente \n\n");
    }

  } while (condicao == 1);

  return data;
}

void buscar(int id) {
  printf("buscando...");
}

int calcularIdade(Data data) {
  if (mesAtual() > data.mes) {
    return anoAtual() - data.ano;
  }
  return (anoAtual() - data.ano) - 1;
}

int criarId() {
  FILE * file;
	int tamanhoArquivo, tamanhoStructure;

  file = fopen("database/pacientes","r");
	fseek(file, 0, SEEK_END);
	tamanhoArquivo = ftell(file);
	fclose(file);

	tamanhoStructure = sizeof(struct paciente);

	return (tamanhoArquivo/tamanhoStructure) + 1;
}

void cadastrar() {
  FILE *file;
  Paciente paciente;
  file = fopen("database/pacientes", "ab");

  paciente.id = criarId();
  printf("= MATRICULA: %d =\n", paciente.id);
  printf("Primeiro nome: ");
  scanf("%s", &paciente.nome);

  printf("Sobrenome: ");
  scanf("%s", &paciente.sobrenome);

  printf("CPF: ");
  scanf("%s", &paciente.cpf);

  printf("Data nascimento... \n");
  paciente.dataNascimento = dataNascimento();

  paciente.idade = calcularIdade(paciente.dataNascimento);
  printf("%d", paciente.idade);

  fwrite(&paciente, sizeof(paciente), 1, file);
  fclose(file);
}

int login () {
  return 0;
}

void menu () {
  int opcao;
  if (login() == 0) {
    do {
      clear();
      printf("================================\n");
      printf("======== MENU PRINCIPAL ========\n");
      printf("================================\n");
      printf("================================\n");
      printf("(1) == CADASTRAR PACIENTE ======\n");
      printf("(2) == BUSCAR PACIENTE (id) ====\n");
      printf("(3) == BUSCAR PACIENTE (nome) ==\n");
      printf("(4) == SAIR ====================\n");
      printf("================================\n");
      printf("Digite sua opcao.: ");
      scanf("%d", &opcao);
      switch(opcao) {
        case 1: {
          cadastrar();
          break;
        }
        case 2: {
          break;
        }
        case 3: {
          break;
        }
        default: {
          opcao = 4;
          break;
        }
      }
    } while (opcao != 4);
  } else {
    clear();
    printf("================================\n");
    printf("=========== CADASTRO ===========\n");
    printf("================================\n");
  }
}