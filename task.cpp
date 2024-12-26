#include <iostream>

using namespace std;

int** createG(int n)
{
	int** G;
	G = new int* [n];
	for (int i = 0; i < n; i++)
	{
		G[i] = new int[n];
	}
	for (int i = 0; i < n; i++)
	{
		for (int j = i; j < n; j++)
		{
			G[i][j] = rand() % 2;
			G[j][i] = G[i][j];
			if (i == j) G[i][j] = 0;
		}
	}
	return G;
}

void printG(int** G, int size)
{
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			printf("%d",G[i][j]);
		}
		printf("\n");
	}
}

int** Decarta(int** G1, int** G2, int v1, int v2)
{
	int** Gtemp = createG(v1 * v2);
	for (int i1 = 0; i1 < v1; i1++)
	{
		for (int i2 = 0; i2 < v2; i2++)
		{
			for (int j1 = 0; j1 < v1; j1++)
			{
				for (int j2 = 0; j2 < v2; j2++)
				{
					int index1 = i1 * v2 + i2;
					int index2 = j1 * v2 + j2;

					if ((i1 == j1 && G2[i2][j2] == 1) || (i2 == j2 && G1[i1][j1] == 1))
					{
						Gtemp[index1][index2] = 1;
					}
					else
					{
						Gtemp[index1][index2] = 0;
					}
				}
			}

		}
	}
	return Gtemp;
}

void adjacencyMatrixToList(int** matrix, int n) {
	int** adjList = new int* [n];
	int* Gtemp = new int[n];


	for (int i = 0; i < n; ++i) {
		Gtemp[i] = 0;
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] == 1) {
				Gtemp[i]++;
			}
		}
		adjList[i] = new int[Gtemp[i]];
	}


	for (int i = 0; i < n; ++i) {
		int index = 0;
		for (int j = 0; j < n; ++j) {
			if (matrix[i][j] == 1) {
				adjList[i][index++] = j;
			}
		}
	}

	printf("Список смежности:\n");
	for (int i = 0; i < n; ++i) {
		cout << "Вершина " << i << ": ";
		for (int j = 0; j < Gtemp[i]; ++j) {
			cout << adjList[i][j] << " ";
		}
		printf("\n");;
	}
}

int** delV(int** G, int size, int v)
{
	int** Gtemp = createG(size - 1);
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			if (i < v && j < v) Gtemp[i][j] = G[i][j];
			if (i > v && j > v) Gtemp[i - 1][j - 1] = G[i][j];
			if (i > v && j < v) Gtemp[i - 1][j] = G[i][j];
			if (i < v && j > v) Gtemp[i][j - 1] = G[i][j];
		}
	}
	for (int i = 0; i < size; i++)
	{
		free(G[i]);
	}
	free(G);

	return Gtemp;
}

int** unionV(int** G, int size, int v1, int v2)
{
	for (int i = 0; i < size; i++)
	{
		if (G[v2][i] == 1)
		{
			G[v1][i] = G[v2][i];
			G[i][v1] = G[i][v2];

		}
	}
	G = delV(G, size, v2);
	return G;
}

int** splitV(int** G, int size, int v1)
{
	int** Gtemp = createG(size + 1);
	for (int i = 0; i < size; i++)
	{
		Gtemp[i][size] = G[i][v1];
		Gtemp[size][i] = G[v1][i];
	}
	for (int i = 0; i < size; i++)
	{
		for (int j = 0; j < size; j++)
		{
			Gtemp[i][j] = G[i][j];
		}
	}
	for (int i = 0; i < size; i++)
	{
		free(G[i]);
	}
	free(G);
	G = NULL;
	return Gtemp;

}

int** contrE(int** G, int size, int v1, int v2)
{
	if (G[v1][v2] != 1)
	{
		printf("Ребра нет");
		return G;
	}
	else
	{
		G[v1][v2] = 0;
		G[v2][v1] = 0;
		for (int i = 0; i < size; i++)
		{
			if (G[v2][i] == 1)
			{
				G[v1][i] = G[v2][i];
				G[i][v1] = G[i][v2];
			}
		}
		G = delV(G, size, v2);
	}

	return G;
}

int** unionG(int** G1, int** G2, int size1, int size2)
{
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;

	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;

	int** Gtemp = createG(sizemax);

	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			Gtemp[i][j] = Gmin[i][j] | Gmax[i][j];
		}
	}

	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			Gtemp[i][j] = Gmax[i][j];
			Gtemp[j][i] = Gtemp[i][j];
		}
	}
	return Gtemp;
}

int** intersectionG(int** G1, int** G2, int size1, int size2)
{
	int sizemin = (size1 < size2) ? size1 : size2;
	int** Gtemp = createG(sizemin);

	for (int i = 0; i < sizemin; i++)
	{
		for (int j = 0; j < sizemin; j++)
		{
			Gtemp[i][j] = G1[i][j] & G2[i][j];
		}
	}
	return Gtemp;
}

int** xorG(int** G1, int** G2, int size1, int size2)
{
	int sizemax = (size1 > size2) ? size1 : size2;
	int sizemin = (size1 < size2) ? size1 : size2;

	int** Gmax = (size1 > size2) ? G1 : G2;
	int** Gmin = (size1 < size2) ? G1 : G2;

	int** Gtemp = createG(sizemax);

	for (int i = sizemin; i < sizemax; i++)
	{
		for (int j = sizemin; j < sizemax; j++)
		{
			Gtemp[i - sizemin][j - sizemin] = Gmax[i][j];
		}
	}
	return Gtemp;
}

int main()
{
	setlocale(LC_ALL, "");
	int n1, n2;
	int nG1 = 3, nG2 = 5;
	printf("Кол-во вершин 1 графа\n");
	scanf_s("%d",&nG1);
	printf("Кол-во вершин 2 графа\n");
	scanf_s("%d", &nG2);
	int** G1 = createG(nG1);
	int** G2 = createG(nG2);
	adjacencyMatrixToList(G1, nG1);
	printf("1 граф\n");
	printG(G1, nG1);
	printf("2 граф\n");
	printG(G2, nG2);
	printf("отождествление\n");
	scanf_s("%d", &n1);
	scanf_s("%d", &n2);
	G2 = unionV(G2, nG2, n1, n2);
	nG2--;
	printG(G2, nG2);
	printf("стягивание ребра\n");
	scanf_s("%d", &n1);
	scanf_s("%d", &n2);
	G2 = contrE(G2, nG2, n1, n2);
	nG2--;
	printG(G2, nG2);
	printf("расщепление ребра\n");
	scanf_s("%d", &n1);
	G1 = splitV(G1, nG1, n1);
	nG1++;
	printG(G1, nG1);
	printf("обьединение графа\n");
	int** G3 = unionG(G1, G2, nG1, nG2);
	int nG3 = (nG1 > nG2) ? nG1 : nG2;
	printG(G3, nG3);
	printf("пересечение графа\b");
	int** G4 = intersectionG(G1, G2, nG1, nG2);
	int nG4 = (nG1 < nG2) ? nG1 : nG2;
	printG(G4, nG4);
	printf("кольцевая сумма\n");
	int** G5 = xorG(G1, G2, nG1, nG2);
	int nG5 = nG2 - nG1;
	printG(G5, nG5);

	printf("Декартово произведения\n");
	printf("1 граф\n");
	printG(G1, nG1);
	printf("2 граф\n");
	printG(G2, nG2);
	int** G6 = Decarta(G1, G2, nG1, nG2);
	int nG6 = nG2 * nG1;
	printG(G6, nG6);
}