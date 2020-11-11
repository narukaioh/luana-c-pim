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
  int grupoRisco;
  Data dataNascimento;
} Paciente;

typedef struct Usuario {
  char login[10];
  char senha[10];
} Usuario;

void clear(void);
int anoAtual(void);
int mesAtual(void);
int validar(Data data);
Data dataNascimento(void);
void buscar(int id);
void buscarNome(void);
int calcularIdade(Data data);
int criarId(void);
void cadastrar(void);
int login(void);
void menu (void);
void listar(void);

int main() {
  clear();

  // cadastrar();
  // listar();
  menu();
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

void buscarNome() {
  FILE * file;
  Paciente paciente;
  int encontrados = 0;
  char nome[20];

  file = fopen("database/pacientes", "rb");

  printf("\nDigite o primeiro nome:");
  scanf("%s",&nome);

  while(1){
    fread(&paciente,sizeof(paciente),1,file);
    if(feof(file)) { break; }

    if(strcmp(nome,paciente.nome) == 0) {
      printf("\n========================================================\n\n");
      printf("\t\t BUSCA POR NOME \n");
      printf("========================================================\n\n");

      printf("\n\n");
      printf("NOME: %s %s\t", paciente.nome, paciente.sobrenome);
      printf("IDADE: %d\n\n", paciente.idade);
      printf("GRUPO RISCO: ");
      if (paciente.grupoRisco == 0) {
        printf("nao");
      } else {
        printf("sim");
      }
      printf("\tDATA NASCIMENTO: %d/%d/%d\t", paciente.dataNascimento.dia, paciente.dataNascimento.mes, paciente.dataNascimento.ano);
      printf("CPF: %s\n", paciente.cpf);

      printf("========================================================\n\n");

    }
    encontrados = 1;
  }
  if(encontrados == 0) {
    printf("\nNao encontramos nenhuma ocorrencia com esse nome.");
  }
  fclose(file);
}

int buscarUsuario() {
  FILE * file;
  Usuario usuario;
  int encontrados = 0;
  char login[10];
  char senha[10];

  file = fopen("database/usuarios", "rb");

  printf("\nCADASTRE seu LOGIN:");
  scanf("%s",&login);

  printf("\nCADASTRE seu SENHA:");
  scanf("%s",&senha);
  

  while(1){
    fread(&usuario,sizeof(usuario),1,file);
    if(feof(file)) { break; }

    if(strcmp(login, usuario.login) == 0 && strcmp(senha, usuario.senha) == 0) {
      encontrados = 1;
      break;
    }
  }
  if(encontrados == 0) {
    fclose(file);
    printf("\nLogin não autorizado!.");
    return 1;
  }
  fclose(file);
  return 0;
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

int grupoRisco (int idade) {
  if (idade > 60) {
    return 1;
  }
  return 0;
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

  printf("Data nascimento: \n");
  paciente.dataNascimento = dataNascimento();

  paciente.idade = calcularIdade(paciente.dataNascimento);

  paciente.grupoRisco = grupoRisco(paciente.idade);

  fwrite(&paciente, sizeof(paciente), 1, file);
  fclose(file);
}

void cadastrarUsuario() {
  FILE *file;
  Usuario usuario;
  file = fopen("database/usuarios", "ab");
  printf("Digite seu login: ");
  scanf("%s", &usuario.login);

  printf("Digite sua senha: ");
  scanf("%s", &usuario.senha);

  fwrite(&usuario, sizeof(usuario), 1, file);
  fclose(file);
}

int login () {
  return buscarUsuario();
}

void listar() {
  FILE * file;
  Paciente paciente;
  file = fopen("database/pacientes", "rb");
  clear();
  printf("====================================\n");
  printf("======== LISTA DE PACIENTES ========\n");
  printf("====================================\n");

  printf(" ID\t IDADE \t GRUPO RISCO\t NOME\n");
  while(1) {
    fread(&paciente, sizeof(paciente), 1, file);
    if (feof(file)) { break; }

    printf("\n");
    printf(" %d\t", paciente.id);
    printf(" %d\t", paciente.idade);
    
    if (paciente.grupoRisco == 0) {
      printf(" nao");
    } else {
      printf(" sim");
    }

    printf("\t\t %s\t\t", paciente.nome);
  }

  printf("\n\n====================================\n");

  fclose(file);
}

void menu () {
  int opcao;
    cadastrarUsuario();
    if (login() == 0) {
      do {
        clear();
        printf("================================\n");
        printf("======== MENU PRINCIPAL ========\n");
        printf("================================\n");
        printf("================================\n");
        printf("(1) == CADASTRAR PACIENTE ======\n");
        printf("(2) == LISTAR PACIENTES ========\n");
        printf("(3) == CADASTRO USUARIO ========\n");
        printf("(4) == BUSCAR PACIENTE (nome) ==\n");
        printf("(5) == SAIR ====================\n");
        printf("================================\n");
        printf("Digite sua opcao.: ");
        scanf("%d", &opcao);
        switch(opcao) {
          case 1: {
            cadastrar();
            break;
          }
          case 2: {
            listar();
            break;
          }
          case 3: {
            clear();
            printf("===========================================\n");
            printf("=========== CADASTRO DE USUARIO ===========\n");
            printf("===========================================\n");
            cadastrarUsuario();
            break;
          }
          case 4: {
            buscarNome();
            break;
          }
          default: {
            opcao = 5;
            break;
          }
        }
      } while (opcao != 4);
    }
}