
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

# define BUFFER_SIZE 1024

void	Swap(int *n1, int *n2)
{
	int tmp = 0;

	tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

char	*Sub(const char *buf1, const char *buf2)
{
	char *res = 0;
	int	len1 = 0, len2 = 0, raise = 0;
	int size = 0, neg = 0;
	
	if (buf1[0] == '-')
	{
		neg = 1;
		buf1++;
	}
	if (buf2[0] == '-')
		buf2++;
		
	len1 = strlen(buf1);
	len2 = strlen(buf2);
	if (len1 == 0 || len2 == 0)
		return 0;
	if ((len1 < len2) || (len1 == len2 && strncmp(buf1, buf2, len1) < 0))
	{
		neg ^= 1;
		char *tmp = (char *)buf1;
		buf1 = buf2;
		buf2 = tmp;
		Swap(&len1, &len2);
	}
	if (neg)
		size = len1 + 1;
	else
		size = len1;

	res = malloc(sizeof(char) * (size + 1));
	if (!res)
	{
		perror("malloc failed!");
		exit(1);
	}
	memset(res, 0, sizeof(char) * (size + 1));
	res[size] = '\0';
	while (size--)
	{
		int tmp1 = 0, tmp2 = 0, num = 0;

		if (--len1 >= 0)
			tmp1 = buf1[len1] - '0';
		if (--len2 >= 0)
			tmp2 = buf2[len2] - '0';

		num = tmp1 - tmp2 - raise;
		if (num < 0)
		{
			num += 10;
			raise = 1;
		}
		else
			raise = 0;

		num = num % 10;
		res[size] = (char)(num + '0');
	}
	if(neg)
		res[0] = '-';
	return res;	
}

char	*Add(const char *buf1, const char *buf2)
{
	char *res = 0;
	int	len1 = 0, len2 = 0, raise = 0;
	int size = 0, neg = 0;
	
	if (buf1[0] == '-' && buf2[0] == '-')
	{
		buf1++;
		buf2++;
		neg = 1;
	}
	len1 = strlen(buf1);
	len2 = strlen(buf2);
	if (len1 == 0 || len2 == 0)
		return 0;
	if (len1 >= len2)
		size = len1 + 1;
	else
		size = len2 + 1;
	if (neg)
		size++;

	res = malloc(sizeof(char) * (size + 1));
	if (!res)
	{
		perror("malloc failed!");
		exit(1);
	}
	memset(res, 0, sizeof(char) * (size + 1));
	res[size] = '\0';
	while (size--)
	{
		int tmp1 = 0, tmp2 = 0, num = 0;

		if (--len1 >= 0)
			tmp1 = buf1[len1] - '0';
		if (--len2 >= 0)
			tmp2 = buf2[len2] - '0';

		num = tmp1 + tmp2 + raise;
		if (num >= 10)
			raise = 1;
		else
			raise = 0;

		num = num % 10;
		res[size] = (char)(num + '0');
	}
	if (neg)
		res[0] = '-';
	return res;	
}

int valid_check(const char *buf)
{
	while (*buf)
	{
		if (!isdigit(*buf) && !isblank(*buf) && *buf != '-' && *buf != '+')
			return 0;
		buf++;
	}
	return 1;
}

int	main(void)
{
	char buf1[BUFFER_SIZE] = { 0 };
	char buf2[BUFFER_SIZE] = { 0 };
	char *res = 0;
	char op = 0;

	printf("Big_Calulator> ");
	fgets(buf1, BUFFER_SIZE, stdin);	
	buf1[strlen(buf1) - 1] = '\0';
	printf("Big_Calulator> ");
	fgets(buf2, BUFFER_SIZE, stdin);	
	buf2[strlen(buf2) - 1] = '\0';

	if (!valid_check(buf1) || !valid_check(buf2))
	{
		puts("invalid argument!");
		return 1;
	}
	if (buf1[0] == '-' && buf2[0] != '-' || buf1[0] != '-' && buf2[0] == '-') 
		op = '-';
	else
		op = '+';

	if (op == '+')
		res = Add(buf1, buf2);
	else if (op == '-') 
		res = Sub(buf1, buf2);
	
	int i = 0;
	while(res[i])
	{
		if (res[i] == '0' && res[i + 1] != '\0')
			i++;
		else
			break;
	}

	printf("Result: %s\n", &res[i]);

	free(res);
	return 0;
}

