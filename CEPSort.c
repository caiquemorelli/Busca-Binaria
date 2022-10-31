#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct _Address Address;

struct _Address {
  char logradouro[72];
  char bairro[72];
  char cidade[72];
  char uf[72];
  char sigla[2];
  char cep[8];
  char lixo[2];
};

void sort(long, int, FILE *);
void intercala(char *, char *, char *);
void test(char *);
char *criaNomeArquivosIntercala(int);

int main(void) {
  FILE *input;
  long size, numRegistros;
  int numArq;
  char nomeArqResultante[128];

  printf("Entre com a quantidade de divisões no arquivo: ");
  scanf("%d", &numArq);

  if (numArq % 2 != 0) {
    printf("A quantidade de arquivos precisa ser um número par");
    return 0;
  }

  input = fopen("cep.dat", "rb");

  fseek(input, 0, SEEK_END);

  size = ftell(input);
  numRegistros = size / sizeof(Endereco);

  rewind(input);

  sort(numRegistros, numArq, input);

  fclose(input);

  sprintf(nomeArqResultante, "%s", criaNomeArquivosIntercala(numArq));

  test(nomeArqResultante);

  return 0;
}

int compara(const void *e1, const void *e2) {
  return strncmp(((Address *)e1)->cep, ((Address *)e2)->cep, 8);
}

void sort(long numRegistros, int numArq, FILE *input) {
  int sizeoutput;
  FILE *output;
  char nomeArq[32];
  int i;

  for (i = 0; i < numArq; i++) {
    Address *e;

    if (i == 0) {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);

      output = fopen(nomeArq, "wb");

      sizeoutput = ((numRegistros / numArq) + (numRegistros % numArq));

      e = (Address *)malloc(sizeoutput * sizeof(Address));

      fread(e, sizeof(Address), sizeoutput, input);

      qsort(e, sizeoutput, sizeof(Address), compara);

      fwrite(e, sizeof(Address), sizeoutput, output);
    } else {
      snprintf(nomeArq, sizeof(char) * 32, "cep-%d.dat", i + 1);

      output = fopen(nomeArq, "wb");

      sizeoutput = numRegistros / numArq;

      e = (Address *)malloc(sizeoutput * sizeof(Address));

      fread(e, sizeof(Address), sizeoutput, input);

      qsort(e, sizeoutput, sizeof(Address), compara);

      fwrite(e, sizeof(Address), sizeoutput, output);
    }

    free(e);
    fclose(output);
  }
}

char *criaNomeArquivosIntercala(int numArquivos) {
  int i, j;
  int n = 1, passos = 1, controle = 1;
  char arq1[128] = "", arq2[128] = "";

  for (i = 1; i < ((numArquivos * 2) - 1); i++) {
    int pos = 0;

    for (j = 0; j < passos; j++) {
      if (controle == 1) {
        pos += sprintf(&arq1[pos], "%d", n);

        if (j < passos - 1) {
          pos += sprintf(&arq1[pos], "-");
        }
      } else {
        pos += sprintf(&arq2[pos], "%d", n);

        if (j < passos - 1) {
          pos += sprintf(&arq2[pos], "-");
        }
      }

      if (n == numArquivos) {
        passos = passos * 2;
        n = 1;
        break;
      }

      n++;
    }

    if (controle == 2) {
      char nomeArqA[128], nomeArqB[128], nomeArqoutput[128];

      sprintf(&nomeArqA[0], "cep-%s.dat", arq1);
      sprintf(&nomeArqB[0], "cep-%s.dat", arq2);
      sprintf(&nomeArqoutput[0], "cep-%s-%s.dat", arq1, arq2);

      controle = 0;

      intercala(nomeArqA, nomeArqB, nomeArqoutput);

      if (i == ((numArquivos * 2) - 2)) {
        return nomeArqoutput;
      }

      strcpy(arq1, "");
      strcpy(arq2, "");
    }

    controle++;
  }

  return "";
}

void intercala(char *nomeArqinputA, char *nomeArqinputB,
               char *nomeArqoutput) {

  FILE *output, *inputA, *inputB;
  Address ea, eb;

  inputA = fopen(nomeArqinputA, "rb");
  inputB = fopen(nomeArqinputB, "rb");
  output = fopen(nomeArqoutput, "wb");

  fread(&ea, sizeof(Address), 1, inputA);
  fread(&eb, sizeof(Address), 1, inputB);

  while (!feof(inputA) && !feof(inputB)) {
    if (compara(&ea, &eb) < 0) {
      fwrite(&ea, sizeof(Address), 1, output);
      fread(&ea, sizeof(Address), 1, inputA);
    } else {
      fwrite(&eb, sizeof(Address), 1, output);
      fread(&eb, sizeof(Address), 1, inputB);
    }
  }

  while (!feof(inputA)) {
    fwrite(&ea, sizeof(Address), 1, output);
    fread(&ea, sizeof(Address), 1, inputA);
  }

  while (!feof(inputB)) {
    fwrite(&eb, sizeof(Address), 1, output);
    fread(&eb, sizeof(Address), 1, inputB);
  }

  fclose(inputA);
  fclose(inputB);

  remove(nomeArqinputA);
  remove(nomeArqinputB);

  fclose(output);
}

void test(char *nomeArqResultante) {
  FILE *input;
  Address e;
  int i;

  input = fopen(nomeArqResultante, "rb");

  fread(&e, sizeof(Address), 1, input);

  for (i = 0; i < 50; i++) {
    printf("\n%.8s", e.cep);
    fread(&e, sizeof(Address), 1, input);
  }

  fclose(input);
}