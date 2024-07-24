
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

# define BUFFER_SIZE 1024

void	Swap(int *n1, int *n2);
char	*Sub(const char *buf1, const char *buf2);
char	*Add(const char *buf1, const char *buf2);
int		valid_check(const char *buf);
void	PrintResult(const char *res);

int	main(void)
{
	char buf1[BUFFER_SIZE] = { 0 };
	char buf2[BUFFER_SIZE] = { 0 };
	char *res = NULL;

	while (1)
	{
		printf("\nNum1> ");
		fgets(buf1, BUFFER_SIZE, stdin);
		if (strncmp(buf1, "exit\n", 6) == 0)
			break ;
		printf("Num2> ");
		fgets(buf2, BUFFER_SIZE, stdin);	
		if (strncmp(buf1, "\n", 2) == 0 && strncmp(buf2, "\n", 2) == 0)
		{
			printf("\033[2J\033[1;1H");
			continue;
		}
		buf1[strlen(buf1) - 1] = '\0';
		buf2[strlen(buf2) - 1] = '\0';

		if (!valid_check(buf1) || !valid_check(buf2))
		{
			puts("invalid argument!");
			continue ;
		}
		if ((buf1[0] == '-' && buf2[0] != '-') || (buf1[0] != '-' && buf2[0] == '-')) 
			res = Sub(buf1, buf2);
		else
			res = Add(buf1, buf2);

		PrintResult(res);
		free(res);
	}
	return 0;
}

void	Swap(int *n1, int *n2)
{
	int tmp = 0;

	tmp = *n1;
	*n1 = *n2;
	*n2 = tmp;
}

char	*Sub(const char *buf1, const char *buf2)
{
	char	*res = 0;
	int		size = 0, neg = 0, len1 = 0, len2 = 0, raise = 0;
	
	if (buf1[0] == '-')
		neg = 1;
	while (!isdigit(*buf1))
		buf1++;
	while (!isdigit(*buf2))
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
	char	*res = 0;
	int		size = 0, neg = 0, len1 = 0, len2 = 0, raise = 0;
	
	if (buf1[0] == '-' && buf2[0] == '-')
		neg = 1;
	while (!isdigit(*buf1))
		buf1++;
	while (!isdigit(*buf2))
		buf2++;
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
	int sign = 0;
	const char *tmp = buf;

	if (*tmp == '\0')
		return 0;
	while (*tmp == '-' || *tmp == '+')
	{
		sign++;
		if (sign > 1 || *(tmp + 1) == '\0')
			return 0;
		tmp++;
	}
	tmp = ++buf;
	while (*tmp)
	{
		if (!isdigit(*tmp) && !isblank(*tmp))
			return 0;
		tmp++;
	}
	return 1;
}

void	PrintResult(const char *res)
{
	int i = 0;
	int neg = 0;

	if (res[0] == '-')
	{
		neg = 1;
		i++;
	}
	while(res[i])
	{
		if (res[i] == '0' && res[i + 1] != '\0')
			i++;
		else
			break;
	}

	if (neg)
		printf("Result: -%s\n", &res[i]);
	else
		printf("Result: %s\n", &res[i]);
}


