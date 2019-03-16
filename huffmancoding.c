#include <stdio.h>
#include <string.h>

#define MAXSIZE 100

void init(void);
void createht(void);
void huffcode(void);
void reverse(char *);

typedef struct
{
	int weight;
	int parent;
	int lchild, rchild;
} Huffnode;

typedef struct
{
	char w;
	char code[10];
} Huffcode;

Huffnode ht[MAXSIZE * 2 - 1];
Huffcode hc[MAXSIZE];

int n; //编码字符的个数

void init()
{
	char ch;
	int i = 0;
	FILE *fp = fopen("text/in.txt", "r");
	if (fp == NULL)
	{
		printf("can't open the input file\n");
		exit(0);
	}

	ht[i].parent = ht[i].lchild = ht[i].rchild = -1;

	while ((ch = fgetc(fp)) != EOF)
	{
		if (ch == '\n')
		{
			i++;
			ht[i].parent = ht[i].lchild = ht[i].rchild = -1;
		}
		else if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))
		{
			hc[i].w = ch;
		}
		else if (ch >= '0' && ch <= '9')
		{
			ht[i].weight = ht[i].weight * 10 + ch - '0';
		}
	}

	n = i;

	if (fclose(fp))
	{
		printf("can't close the file\n");
		exit(0);
	}
}

void createht()
{
	int mini, minj;
	int k, s;
	int f;

	for (k = n; k < (2 * n - 1); k++)
	{
		f = 0; //remenber!!!不要放错了地方
		for (s = 0; s < k; s++)
		{ //寻找权值最小的两个结构体
			if (ht[s].parent == -1)
			{
				if (f == 0)
				{
					mini = s;
					f++;
				}
				else if (f == 1)
				{
					if (ht[s].weight < ht[mini].weight)
					{
						minj = mini;
						mini = s;
					}
					else
						minj = s;
					f++;
				}
				else
				{
					if (ht[s].weight < ht[mini].weight)
					{
						minj = mini;
						mini = s;
					}
					else if (ht[s].weight < ht[minj].weight)
						minj = s;
				}
			}
		}
		ht[k].weight = (ht[mini].weight + ht[minj].weight);
		ht[k].parent = -1;
		ht[k].lchild = mini;
		ht[k].rchild = minj;

		ht[mini].parent = ht[minj].parent = k;
	}
}

void huffcode()
{
	int i, j, t;
	int length;
	for (i = 0; i < n; i++)
	{
		j = i;
		length = 0;
		while ((ht[j].parent) != -1)
		{ //是循环，而不是判断，逻辑不能错
			t = ht[j].parent;
			if (ht[t].lchild == j)
				hc[i].code[length++] = 0 + '0';
			if (ht[t].rchild == j)
				hc[i].code[length++] = 1 + '0';
			j = t;
		}
		hc[i].code[length] = '\0';

		reverse(hc[i].code);
	}
}

void reverse(char *cp)
{
	int i = 0;
	int l = (strlen(cp) - 1);
	char temp;
	for (; i < l; i++, l--)
	{
		temp = cp[i];
		cp[i] = cp[l];
		cp[l] = temp;
	}
}

void main(void)
{
	int i;

	init();
	createht();
	huffcode();

	for (i = 0; i < n; i++)
	{
		printf("the w is %c,the code is %s\n", hc[i].w, hc[i].code);
	}
}
