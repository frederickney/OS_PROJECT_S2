/**
 * @project OS PROJECT S2
 * @file utils.c
 * @author Frederick NEY et Stephane OVERLEN
 * @date 21/07/2016 10:43
 * @revision 1.0.1
 */

#include <string.h>
#include "utils.h"

void reverse(char *string)
{
	int i, j;
	char c;
	for (i = 0, j = strlen(string)-1; i<j; i++, j--) {
		c = string[i];
		string[i] = string[j];
		string[j] = c;
	}
}

char *itoa(int n, char *string)
{
	int i, sign;

	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		string[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		string[i++] = '-';
	string[i++] = '\0';
	reverse(string);
	return string;
}