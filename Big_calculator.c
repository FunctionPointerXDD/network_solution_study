
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char	*Sub(const char *buf1, const char *buf2)
{
	char *res = 0;
	int	len1 = 0, len2 = 0, raise = 0, sign = 0;
	
	res = malloc(sizeof(char) * 101 + 1);
	len1 = strlen(buf1) - 1;
	len2 = strlen(buf2) - 1;
	if (len1 < 0 || len2 < 0)
		return 0;
	if (len1 <= len2)
	{
		if(atoi(buf1[len1]) < atoi(buf2[len2]))
			res[0] = '-';
	}

	return 0;
}

char	*Add(const char *buf1, const char *buf2)
{
	char *res = 0;
	int	len1 = 0, len2 = 0, raise = 0, sign = 0;
	
	res = malloc(sizeof(char) * 101 + 1);
	len1 = strlen(buf1) - 1;
	len2 = strlen(buf2) - 1;
	if (len1 < 0 || len2 < 0)
		return 0;
	


	return res;	
}

int check_digit(const char *buf)
{
	while (*buf)
	{
		if (!isdigit(*buf++))
			return 0;
	}
	return 1;
}

int	main(void)
{
	char *buf1 = 0;
	char *buf2 = 0;
	char *res = 0;
	char op[2] = { 0 };

	buf1 = malloc(sizeof(char) * 101 + 1); // char * 101 자리 할당 , +1 은 널값
	buf2 = malloc(sizeof(char) * 101 + 1); // char * 101 자리 할당 , +1 은 널값
	if (!buf1 || !buf2)
		exit(1);

	fgets(buf1, 102, stdin);	
	fgets(buf2, 102, stdin);	
	buf1[strlen(buf1) - 1] = '\0'; //개행 제거, //최대 입력 자릿수: 100
	buf2[strlen(buf2) - 1] = '\0'; //개행 제거, //최대 입력 자릿수: 100

	strncpy(op, "+", 2);

	if (!check_digit(buf1) || !check_digit(buf2))
	{
		puts("invalid argument!");
		return 1;
	}

	if (strncmp(op, "+", 2) == 0)
		res = Add(buf1, buf2);
	else 
		res = Sub(buf1, buf2);

	int cnt1 = printf("%s\n", buf1);
	int cnt2 = printf("%s\n", buf2);
	printf("%d\n", cnt1);
	printf("%d\n", cnt2);

	free(buf1);
	free(buf2);
	free(res);

	return 0;
}
