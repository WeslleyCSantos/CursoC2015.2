#include <stdio.h>

int main(){
	int i,soma=0;
	for(i=0;i<=1000;i++)
		if(i%3==0 && i%7==0)
			soma +=i;
	printf("A soma dos multiplos de 3 e 7 e igual a : %d\n",soma);
	return 0;

}
