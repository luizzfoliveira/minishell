#include "../minishell.h"

void	*ft_calloc(size_t nmemb, size_t size)
{
	size_t	i;
	char	*alloced;

	if (nmemb == 0 || size == 0)
	{
		alloced = malloc(1);
		*alloced = 0;
		return (alloced);
	}
	alloced = malloc(nmemb * size);
	if (!alloced)
		return (0);
	i = 0;
	while (i < nmemb * size)
	{
		alloced[i] = 0;
		i++;
	}
	return (alloced);
}
