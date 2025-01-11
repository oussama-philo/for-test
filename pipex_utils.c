/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: olachhab <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 11:34:48 by olachhab          #+#    #+#             */
/*   Updated: 2025/01/11 15:04:49 by olachhab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

static int	count_words(const char *s, char c)
{
	int	count;
	int	in_word;

	count = 0;
	in_word = 0;
	while (*s)
	{
		if (*s != c && !in_word)
		{
			in_word = 1;
			count++;
		}
		else if (*s == c)
		{
			in_word = 0;
		}
		s++;
	}
	return (count);
}

static char	*get_next_word(const char **s, char c)
{
	const char	*start;
	const char	*end;
	char		*word;
	size_t		length;

	while (**s && **s == c)
		(*s)++;
	start = *s;
	while (**s && **s != c)
		(*s)++;
	end = *s;
	length = end - start;
	word = (char *)malloc(length + 1);
	if (!word)
		return (NULL);
	ft_memcpy(word, start, length);
	word[length] = '\0';
	return (word);
}

static void	free_alloc(char **ptr, int i)
{
	while (i >= 0)
	{
		free(ptr[i]);
		i--;
	}
	free(ptr);
}

char	**splitpath(char const *s, char c)
{
	int			i;
	const char	*ptr;
	int			word_count;
	char		**result;

	if (!s)
		return (NULL);
	word_count = count_words(s, c);
	result = malloc((word_count + 1) * sizeof(char *));
	if (!result)
		return (NULL);
	ptr = s;
	i = 0;
	while (i < word_count)
	{
		result[i] = get_next_word(&ptr, c);
		if (!result[i])
		{
			free_alloc(result, i - 1);
			return (NULL);
		}
		i++;
	}
	result[word_count] = NULL;
	return (result);
}
