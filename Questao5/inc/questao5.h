typedef struct{
	char *nome;
	float nota;
}sala;

void aloca_sala(sala *aula,int numalunos);
void desaloca_sala(sala *aula)
float calcula_media(sala *aula,int numalunos);
void ordena_alunos(sala *aula,int numalunos);
