#include<stdio.h>
#include<string.h>

#define MAXSIZE 100

typedef struct {
	int weight;
	int parent;
	int lchild,rchild;
}Huffnode;

typedef struct {
	char w;
	char code[10];
}Huffcode;

Huffnode ht[MAXSIZE*2-1];
Huffcode hc[MAXSIZE];

int n;  //编码字符的个数

void init(Huffnode *htp,Huffcode *hcp)	{
	char ch;
	int i=0;
	FILE *fp=fopen("text/in.txt","r");
	if(fp==NULL)  {
		printf("can't open the input file\n");
		exit(0);
	}

	while(ch=fgetc(fp)!=EOF)	{
		if(ch=='\n')	{
			i++;
			htp[i].parent=htp[i].lchild=htp[i].rchild=-1;
			htp[i].weight=0;
		}
		else if((ch>='a'&&ch<='z')||(ch>='A'&&ch<='Z'))	{
			hcp[i].w=ch;
		}
		else if(ch>='0'&&ch<='9')	{
			htp[i].weight=htp[i].weight*10+ch-'0';
		}	
	}
	
	n=i;

	if(fclose(fp))	{
		printf("can't close the file\n");
		exit(0);
	}
}

void createht(Huffnode *htp)	{
	int mini,minj;
	int k,s;
	int f;
	
	for(k=n;k<=(2*n-1);k++)	{
		for(s=0;s<n;s++)	{
			f=0;
			if(htp[s].parent==-1)	{
				if(f==0)  { mini=s; f++; }
				else if(f==1) {
					if(htp[s].weight<htp[mini].weight)  { 
						minj=mini; mini=s; 
					} 
					else 
						minj=s;
					f++;
				}
				else	{
					if(htp[s].weight<htp[mini].weight)	{
						minj=mini; mini=s;
					}
					else if(htp[s].weight<htp[minj].weight)	
						minj=s;
				} 
			}
		}
	htp[n].weight=htp[mini].weight+htp[minj].weight;
	htp[n].parent=-1;
	htp[n].lchild=mini;
	htp[n].rchild=minj;
	
	htp[mini].parent=htp[minj].parent=n;
	}
}


void huffcode(Huffnode *htp,Huffcode *hcp)	{
	int i,j,t;
	int length;
	for(i=0;i<n;i++)	{
		j=i;
		length=0;
		if(htp[j].parent!=-1)	{
			t=htp[j].parent;
			if(htp[t].lchild==i)  hcp[i].code[length]=0+'0';
			if(htp[t].rchild==i)  hcp[i].code[length]=1+'0';
			length++;
			j=i=t;
		}
		else
			hcp[i].code[length]='\n';
			
	reverse(hcp[i].code);	
	}
}

void reverse(char *cp)	{
	int i=0;
	int l=strlen(cp);
	char temp;
	for(;i<l;i++,l--)	{
		temp=cp[i];
		cp[i]=cp[l];
		cp[l]=temp;
	}
}


void main(void)	{
	int i;

	init(ht,hc);
	createht(ht);
	huffcode(ht,hc);

	for(i=0;i<n;i++)	{
		printf("the w is %c,the code is %s\n",hc[i].w,hc[i].code);
	}
}
