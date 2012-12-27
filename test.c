#include<stdio.h>

#define MAX 10

typedef struct {
	int i;
	char w;
	char t[10];
}node;

node ht[MAX];

int n;

main() {
	int j;
	
	printf("the n is %d\n",n);

	for(j=0;j<=MAX;j++)	{
		printf("the i is %d,the w is %c,the t is %s\n",ht[j].i,ht[j].w,ht[j].t);
	}
}
