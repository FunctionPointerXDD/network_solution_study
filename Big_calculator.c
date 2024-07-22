
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char	*Sub(const char *buf1, const char *buf2)
{
	char *res = 0;
	int	len1 = 0, len2 = 0, raise = 0;
	int size = 0;
	
	res = malloc(sizeof(char) * 102);
	if (!res)
	{
		puts("malloc failed!");
		exit(1);
	}
	memset(res, 0, sizeof(char) * 102);

	if (buf1[0] == '-')
		buf1++;
	if (buf2[0] == '-')
		buf2++;
		
	len1 = strlen(buf1);
	len2 = strlen(buf2);
	if (len1 == 0 || len2 == 0)
		return 0;
	if (len1 <= len2)
	{
		if(buf1[len1 - 1] < buf2[len2 - 1])
		{
			res[0] = '-';
			char *tmp = (char *)buf1;
			buf1 = buf2;
			buf2 = tmp;
			puts("n1 < n2");
		}
	}
	if (len1 >= len2)
		size = len1;
	else
		size = len2;
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
	return res;	
}

char	*Add(const char *buf1, const char *buf2)
{
	char *res;
	int	len1 = 0, len2 = 0, raise = 0;
	int size = 0;
	
	res = malloc(sizeof(char) * 101 + 1);
	if (!res)
	{
		puts("malloc failed!");
		exit(1);
	}
	memset(res, 0, sizeof(char) * 102);

	len1 = strlen(buf1);
	len2 = strlen(buf2);
	if (len1 >= len2)
		size = len1;
	else
		size = len2;
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
	return res;	
}

int check_digit(const char *buf)
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
	char buf1[102] = { 0 };
	char buf2[102] = { 0 };
	char *res = 0;
	char op = 0;

	fgets(buf1, 102, stdin);	
	buf1[strlen(buf1) - 1] = '\0'; //개행 제거, //최대 입력 자릿수: 100
	fgets(buf2, 102, stdin);	
	buf2[strlen(buf2) - 1] = '\0'; //개행 제거, //최대 입력 자릿수: 100

	if (!check_digit(buf1) || !check_digit(buf2))
	{
		puts("invalid argument!");
		return 1;
	}
	//parse
	if ((buf1[0] == '-' && buf2[0] != '-') || (buf1[0] != '-' && buf2[0] == '-'))
		op = '-';

	if (op == '+')
		res = Add(buf1, buf2);
	else if (op == '-') 
		res = Sub(buf1, buf2);

	printf("Result: %s\n", res);

	free(res);
	return 0;
}
