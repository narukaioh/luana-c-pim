#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

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
  int comorbidades;
  Data dataNascimento;
} Paciente;

typedef struct Usuario {
  char login[10];
  char senha[10];
} Usuario;

void imprimirCabecalho (char * cabecalho);
void clear(void);
int anoAtual(void);
int mesAtual(void);
int validar(Data data);
Data dataNascimento(void);
void buscarNome(char * nome);
int buscarUsuario(void);
int calcularIdade(Data data);
int criarPacienteId(void);
int descobrirComorbidades(void);
int grupoRisco (int idade);
int pacienteCritico(Paciente paciente);
void cadastrar(void);
void cadastrarUsuario(void);
int login(void);
void listar(char * database, char * titulo);
void menu (void);
int imprimirCabecalhoMenu(void);

int main() {
  clear();
  menu();
  return 0;
}

void imprimirCabecalho (char * cabecalho) {
  clear();
  printf("==================================\n");
  printf("\t%s\n", cabecalho);
  printf("==================================\n\n");
}

void clear () {
  for (int i=0; i < 3; i++) {
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

void buscarNome(char * nome) {
  FILE * file;
  Paciente paciente;
  int encontrados = 0;

  file = fopen("database/pacientes", "rb");
  
  while(1){
    fread(&paciente,sizeof(paciente),1,file);
    if(feof(file)) { break; }

    if(strcmp(nome,paciente.nome) == 0) {
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

      encontrados = 1;
    }
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

  printf("\nEntre com seu LOGIN:");
  scanf("%s", &login);

  printf("\nEntre com sua SENHA:");
  scanf("%s", &senha);

  while(1){
    fread(&usuario, sizeof(usuario), 1, file);
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

int criarPacienteId() {
  FILE * file;
	int tamanhoArquivo, tamanhoStructure;

  file = fopen("database/pacientes","r");
	fseek(file, 0, SEEK_END);
	tamanhoArquivo = ftell(file);
	fclose(file);

	tamanhoStructure = sizeof(struct paciente);

	return (tamanhoArquivo/tamanhoStructure) + 1;
}

int descobrirComorbidades() {
  int opcao = 0;
  do {
    scanf("%d", &opcao);
  } while (opcao != 1 && opcao != 2);
  return opcao;
}

int grupoRisco (int idade) {
  if (idade >= 65) {
    return 0;
  }
  return 1;
}

int pacienteCritico(Paciente paciente) {
  // Se tem comorbidades e se é grupo de risco
  if (paciente.comorbidades == 1 && paciente.grupoRisco == 0) {
    return 0;
  }
  return 1;
}

void cadastrar() {
  FILE *file;
  FILE *file_paciente_critico;

  Paciente paciente;
  file = fopen("database/pacientes", "ab");
  file_paciente_critico = fopen("database/pacientes_criticos", "ab");

  paciente.id = criarPacienteId();
  printf("[ MATRICULA: %d ]\n", paciente.id);
  printf("\nPrimeiro nome: ");
  scanf("%s", &paciente.nome);

  printf("\nSobrenome: ");
  scanf("%s", &paciente.sobrenome);

  printf("\nCPF: ");
  scanf("%s", &paciente.cpf);

  printf("\nTem alguma dessas comorbidades? (S/N)\n");
  printf("diabetes, hipertensao, cancer...\n\n");
  printf("1) Sim\n2) Nao\n>> ");
  paciente.comorbidades = descobrirComorbidades();

  printf("\nData nascimento: \n");
  paciente.dataNascimento = dataNascimento();

  paciente.idade = calcularIdade(paciente.dataNascimento);

  paciente.grupoRisco = grupoRisco(paciente.idade);


  if (pacienteCritico(paciente) == 0) {
    fwrite(&paciente, sizeof(paciente), 1, file_paciente_critico);
  }

  fwrite(&paciente, sizeof(paciente), 1, file);
  fclose(file);
  fclose(file_paciente_critico);
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
  imprimirCabecalho("LOGIN");
  return buscarUsuario();
}

void listar(char * database, char * titulo) {
  FILE * file;
  Paciente paciente;
  char pause;
  printf("\n\n %s \n\n", database);
  file = fopen(database, "rb");
  printf("==============================================================\n");
  printf("\t\t\t %s\n", titulo);
  printf("==============================================================\n");

  printf(" ID\t %-10s \t IDADE \t %-10s \t GRUPO RISCO\n", "NOME", "CPF");
  while(1) {
    fread(&paciente, sizeof(paciente), 1, file);
    if (feof(file)) { break; }

    printf("\n");
    printf(" %d\t", paciente.id);
    printf(" %-10s\t", paciente.nome);
    printf(" %d\t", paciente.idade);
    printf(" %-10s\t", paciente.cpf);
    
    if (paciente.grupoRisco == 0) {
      printf(" sim");
    } else {
      printf(" nao");
    }

  }

  printf("\n\n==============================================================\n");
  printf("\nDeseja sair da listagem? (S/N) ");
  fclose(file);
}

void menu () {
  int opcao;
  do {
    if (login() == 0) {
      do {
        opcao = imprimirCabecalhoMenu();
        switch(opcao) {
          case 1: {
            imprimirCabecalho("CADASTRAR PACIENTE");
            cadastrar();
            break;
          }
          case 2: {
            char condicaoListarP = 'N';
            do {
              listar("database/pacientes", "LISTA DE PACIENTES");
              scanf("%c", &condicaoListarP);
            } while(condicaoListarP != 'S');
            break;
          }
          case 3: {
            imprimirCabecalho("CADASTRAR USUARIO");
            cadastrarUsuario();
            break;
          }
          case 4: {
            char condicaoListar = 'N';
            char nome[20];
            int opcaoBusca;

            do {
              imprimirCabecalho("BUSCAR PACIENTE (nome)");
              printf("\n1) Digite o primeiro nome:");
              printf("\n2) Sair da busca.\n\n>> ");
              scanf("%d", &opcaoBusca);
              switch(opcaoBusca) {
                case 1: {
                  fflush(stdout);
                  printf("\n\nDigite o nome do paciente: ");
                  scanf("%s", &nome);
                  clear();
                  buscarNome(nome);
                  break;
                }
                case 2: {
                  condicaoListar = 'S';
                  break;
                }
              }
              system("read -n 1 -s -p \"Aperte qualquer tecla para continuar...\n\"");
            } while(condicaoListar != 'S');
            break;
          }
          case 5: {
            char condicaoListar = 'N';
            do {
              listar("database/pacientes_criticos", "LISTA DE PACIENTES CRITICOS");
              scanf("%c", &condicaoListar);
            } while(condicaoListar != 'S');
            break;
          }
          default: {
            opcao = 6;
            break;
          }
        }
      } while (opcao != 6);
    } else {
      imprimirCabecalho("CADASTRO DE USUARIO");
      cadastrarUsuario();
    }
  } while (opcao != 6);
}

int imprimirCabecalhoMenu() {
  int opcao;
  clear();
  printf("===================================\n");
  printf("======== MENU PRINCIPAL ===========\n");
  printf("===================================\n");
  printf("===================================\n");
  printf("(1) == CADASTRAR PACIENTE =========\n");
  printf("(2) == LISTAR PACIENTES ===========\n");
  printf("(3) == CADASTRO USUARIO ===========\n");
  printf("(4) == BUSCAR PACIENTE ============\n");
  printf("(5) == LISTAR PACIENTES CRITICOS ==\n");
  printf("(6) == SAIR =======================\n");
  printf("===================================\n");
  printf("Digite sua opcao.: ");
  scanf("%d", &opcao);
  return opcao;
}