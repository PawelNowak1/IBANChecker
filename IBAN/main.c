#include <stdio.h>
#include <string.h>

#define charToChange 4
#define maxSizeOfIBAN 90
#define modulo 97
#define incorrectIBANFormat -1

void moveFirstCharacters(char tab[])
{
	int length = strlen(tab);
	char temp[charToChange];
	int i;
	for (i = 0; i < charToChange; i++)
	{
		temp[i] = tab[i];
	}

	for (i = 0; i < length - charToChange; i++)
	{
		tab[i] = tab[i + charToChange];
	}

	for (i = 0; i < charToChange; i++)
	{
		tab[length - charToChange + i] = temp[i];
	}
}

int mod(int tab[], int num, int size) // https://www.geeksforgeeks.org/how-to-compute-mod-of-a-big-number/
{
	int res = 0; 
	int i;
	for (i = 0; i < size; i++)
		res = (res * 10 + tab[i]) % 97;

	return res;
}
int getRemainder(char tab[])
{
	const int changeConst = 55;
	int intTab[maxSizeOfIBAN];
	int i, j = 0, length = strlen(tab);

	for (i = 0; i < length; i++)
	{
		if (tab[i] >= 'A' && tab[i] <= 'Z')
		{
			intTab[j] = (tab[i] - changeConst) / 10;
			j++;
			intTab[j] = (tab[i] - changeConst) % 10;
		}
		else
		{
			intTab[j] = tab[i] - '0';
		}
		j++;
	}

	return mod(intTab, modulo, j);
}

void removeSpaces(char* source) // https://stackoverflow.com/questions/1726302/removing-spaces-from-a-string-in-c
{
	char* i = source;
	char* j = source;
	while (*j != 0)
	{
		*i = *j++;
		if (*i != ' ')
			i++;
	}
	*i = 0;
}

int compareFormat(char format[], char tab[])
{
	int length = strlen(tab);
	int length2 = strlen(format);

	if(length != strlen(format))
		return incorrectIBANFormat;
	int i;

	for (i = 2; i < length; i++)
	{
		//N – cyfra, A – litera, X – znak(litera lub cyfra).
		if (format[i] == 'A')
		{
			if (tab[i] > 'Z' || tab[i] < 'A')
				return -1;
		}
		else if (format[i] == 'N')
		{
			if (tab[i] > '9' || tab[i] < '0')
				return -1;
		}
		else if (format[i] == 'X')
		{
			if(tab[i] < '0')
				return -1;
			if (tab[i] > '9' && tab[i] < 'A')
				return -1;
			if (tab[i] > 'Z')
				return -1;
		}
		else if (tab[i] != format[i])
			return -1;
	}
	return 0;
}

int checkFormat(char tab[])
{
	const char file[] = "IBAN.txt";

	FILE * pFile;
	char buffer[maxSizeOfIBAN];

	pFile = fopen(file, "r");
	if (pFile == NULL) perror("Error opening file");
	else
	{
		while (!feof(pFile))
		{
			if (fgets(buffer, maxSizeOfIBAN, pFile) == NULL) break;

			int len = strlen(buffer);
			if (buffer[len - 1] == '\n')
				buffer[len - 1] = '\0';
			if (buffer[0] == tab[0] && buffer[1] == tab[1])
				return compareFormat(buffer, tab);
		}
		fclose(pFile);
	} 
	return incorrectIBANFormat;
}

int isIBANgood(char tab[])
{
	/*
	1. WeŸ pe³ny numer konta (razem z kodem kraju), bez spacji.
    2. SprawdŸ, czy zgadza siê d³ugoœæ numeru dla danego kraju.
    3. Przenieœ 4 pierwsze znaki numeru na jego koniec.
    4. Przekszta³æ litery w numerze konta na ci¹gi cyfr, zamieniaj¹c 'A' na '10', 'B' na '11' itd., a¿ do 'Z' na '35' (dla Polski 2521).
    5. Potraktuj otrzymany ci¹g znaków jak liczbê i wylicz resztê z dzielenia przez 97.
    6. Jeœli reszta jest równa 1, to numer konta ma prawid³owe cyfry kontrolne.
	*/
	const int correctResult = 1;

	removeSpaces(tab); // 1
	if (checkFormat(tab) == incorrectIBANFormat) // 2
	{
		printf("Incorrect IBAN number!");
		return 0;
	}

	moveFirstCharacters(tab); // 3

	if (getRemainder(tab) == correctResult) // 4, 5, 6
		printf("Correct IBAN number!");
	else 
		printf("Incorrect IBAN number!");
	
	return 0;
}


int main()
{
	char tab[] = "PL60 1020 1026 0000 0422 7020 1111";
	isIBANgood(tab);
}