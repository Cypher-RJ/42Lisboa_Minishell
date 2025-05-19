#include "../../includes/minishell.h"

static int count_words(char *str)
{
    int count = 0;
    int i = 0;
    int in_word = 0;
    char quote_type = 0;

    while (str[i])
    {
        // Handle quoted segments
        if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i-1] != '\\'))
        {
            // If we're not inside a quote, start a quote
            if (!quote_type)
            {
                if (!in_word)
                {
                    in_word = 1;
                    count++;  // Start a new word if we weren't in one
                }
                quote_type = str[i];
            }
            // If we're inside a matching quote, end the quote
            else if (str[i] == quote_type)
            {
                quote_type = 0;
            }
            // If we're inside a different quote, it's just a character
        }
        // Handle spaces (only meaningful outside quotes)
        else if (str[i] == ' ' && !quote_type)
        {
            in_word = 0;  // End the current word
        }
        // Handle any other character
        else if (!in_word && !quote_type)
        {
            in_word = 1;
            count++;  // Start a new word
        }
        i++;
    }

    return count;
}

char	**split_cmds(char *input)
{
	char	**args;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!input || input[0] == '\0' || is_only_spaces(input))
		return (NULL);
	if (check_syntax(input) || check_syntax_redir(input))
		return (NULL);
	args = malloc(sizeof(char *) * (count_words(input) + 1));
	if (!args)
		return (NULL);
	while (input[i])
	{
		while (input[i] == ' ')
			i++;
		if (!handle_segment(input, &args[j], &i))
		{
			ft_free_split(args);
			return (NULL);
		}
		if (args[j])
			j++;
		if (input[i] == '|')
			i++;
	}
	args[j] = NULL;
	return (args);
}

char **ft_split_quotes(char *str)
{
    int     i = 0;
    int     k = 0;
    int     start;
    char    **result;
    char    in_quote = 0;  // Track quote state: 0=none, '=single, "=double

    if (!str)
        return (NULL);
        
    // Count words to allocate memory
    int word_count = 0;
    int in_word = 0;
    int j = 0;
    
    while (str[j])
    {
        // Handle quote characters
        if ((str[j] == '\'' || str[j] == '"') && (j == 0 || str[j-1] != '\\'))
        {
            if (!in_quote)
            {
                if (!in_word)
                {
                    in_word = 1;
                    word_count++;
                }
                in_quote = str[j];
            }
            else if (str[j] == in_quote)
            {
                in_quote = 0;
            }
        }
        // Handle spaces (only if not in quotes)
        else if ((str[j] == ' ' || str[j] == '\t') && !in_quote)
        {
            if (in_word)
                in_word = 0;
        }
        // Handle regular characters
        else if (!in_word)
        {
            in_word = 1;
            word_count++;
        }
        j++;
    }
    
    // Allocate result array
    result = malloc(sizeof(char *) * (word_count + 1));
    if (!result)
        return (NULL);
    
    // Reset for actual parsing
    in_word = 0;
    in_quote = 0;
    
    while (str[i])
    {
        // Skip whitespace between words (but not in quotes)
        if ((str[i] == ' ' || str[i] == '\t') && !in_quote)
        {
            if (in_word)
            {
                result[k++] = ft_substr(str, start, i - start);
                in_word = 0;
            }
            i++;
            continue;
        }
        
        // Start a new word if we're not in one
        if (!in_word)
        {
            start = i;
            in_word = 1;
        }
        
        // Handle quotes
        if ((str[i] == '\'' || str[i] == '"') && (i == 0 || str[i-1] != '\\'))
        {
            if (!in_quote)
                in_quote = str[i];
            else if (str[i] == in_quote)
                in_quote = 0;
        }
        
        i++;
        
        // If end of string and we're in a word, add the final word
        if (!str[i] && in_word)
            result[k++] = ft_substr(str, start, i - start);
    }
    
    result[k] = NULL;
    return (result);
}
