#include <stdio.h>

int main(int argv, char **argc, char **argenv)
{
	int	i = -1;
	while (argenv[++i] != 0)
		printf("%s\n", argenv[i]);
}