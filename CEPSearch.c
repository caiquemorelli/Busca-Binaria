#include <stdio.h>
#include <string.h>

typedef struct _Address Address;

struct _Address
{
	char logradouro[72];
	char bairro[72];
	char cidade[72];
	char uf[72];
	char sigla[2];
	char cep[8];
	char lixo[2];
};

int main(int argc, char**argv)
{
	FILE *f;
	Address e;
	int end, mid, start = 0;
	int counter = 0;

	if(argc != 2)
	{
		fprintf(stderr, "USO: %s [CEP]", argv[0]);
		return 1;
	}

	f = fopen("cep_ordenado.dat","rb");
	
	fseek(f, 0, SEEK_END);
	end = ftell(f) / sizeof(Endereco);

	while (start <= end){
		counter++;
		mid = ((start + end) / 2);
		
		fseek(f, mid * sizeof(Address), SEEK_SET);
		fread(&e, sizeof(Address), 1, f);
		
		if (strncmp(argv[0], e.cep, 8) == 0){
			printf("CEP TESTADO: %s\n CEP ENCONTRADO: %s", argv[0], e.cep);
			break;
		} else if (strncmp(argv[0], e.cep, 8) > 0){
			start = mid + 1;
		} else {
			end = mid - 1;
		}
	}

	printf("%d\n", counter);

	fclose(f);
	return 0;
}