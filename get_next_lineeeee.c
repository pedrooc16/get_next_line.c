#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 40


char	*ft_search(const char *s, int c)
{
	unsigned char	i;

	i = c;
	if (!s)
		return (NULL);
	while (*s)
	{
		if (*s == (char)i)
			return ((char *)s);
		s++;
	}
	if (i == '\0')
		return ((char *)s);
	return (NULL);
}

size_t	ft_strlen(const char *s)
{
	unsigned long int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

static char	*ft_strcpy(char *dest, const char *src)
{
	int		i;

	i = 0;
	while (src[i] != '\0')
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

char	*ft_strjoin(char *s1, char *s2)
{
	size_t	len1;
	size_t	len2;
	char	*newstring;

	if (!s1)
	{
		s1 = malloc(1 * sizeof(char));
		s1[0] = '\0';
	}
	if (!s1 || !s2)
		return (NULL);
	len1 = ft_strlen(s1);
	len2 = ft_strlen(s2);
	newstring = malloc(len1 + len2 + 1);
	if (!newstring)
		return (NULL);
	ft_strcpy(newstring, s1);
	ft_strcpy(newstring + len1, s2);
	free(s1);
	return (newstring);
}


char	*get_the_next_line(char *buffer, int * i)
{
	char 	*line;
	int		j;
	int		index;

	index = 0;
	if (buffer[index] == 0)
		return (NULL);
	while (buffer[index] && buffer[index] != '\n')
	{
		index++;
		(*i)++;
	}
	line = malloc(sizeof(char) * (index + 2));
	j = 0;
	while (buffer[j] && buffer[j] != '\n')
	{
		line[j] = buffer[j];
		j++;
	}
	if (buffer[j] == '\n')
	line[j++] = '\n';
	line[j] = '\0';
	return (line);
}

char	*refresh_buffer(char *buffer, int i)
{
	int		index;
	char 	*refresh;
	size_t	size_of_buffer;
	int	j;

	if (*buffer == 0)
	{
		free(buffer);
		return (0);
	}
	index = i;
	size_of_buffer = ft_strlen(buffer) - index;
	refresh = malloc(sizeof(char) * (size_of_buffer + 1));
	if (!refresh)
		return (NULL);
	if (buffer[index] == '\n')
	index++;
	j = 0;
	while (buffer[index])
	refresh[j++] = buffer[index++];
	refresh[j] = '\0';
	free(buffer);
	return(refresh);
}

char	*read_line(int fd, char *buffer)
{
	int		bytes;
	char	temp[BUFFER_SIZE + 1];

	while ((bytes = read(fd,temp,BUFFER_SIZE)) > 0)
	{
		temp[bytes] = '\0';
		buffer = ft_strjoin(buffer, temp);
		if (ft_search(buffer, '\n'))
		break;
	}
    if (bytes <= 0)
	{
		if (!buffer[0])
		{
			free(buffer);
			buffer = NULL;
		}
		return (buffer);
	}
	return (buffer);
}

char	*get_next_line(int fd)
{
	char 	*line;
	static char *buffer;
	int	i;
	
	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (buffer && (ft_search(buffer, '\n')))
	{
		line = get_the_next_line(buffer, &i);
		buffer = refresh_buffer (buffer, i);
		return (buffer);
	}
	buffer = read_line(fd, buffer);
	if (!buffer)
		return (NULL);
	i = 0;
	line = get_the_next_line(buffer, &i);
	buffer = refresh_buffer (buffer, i);
	return (line);
}

int main()
{
	char *line;
	int fd = open("joao.txt", O_RDONLY);
        line = get_next_line(fd);
	while (line)
	{
		printf("%s", line);
		free(line);
        line = get_next_line(fd);
	}
	free(line);
        //printf("\n");
}