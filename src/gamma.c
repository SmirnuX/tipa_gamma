#include "gamma.h"

int main(int argc, char* argv[])    
{
    if (argc < 4)
    {
        printf("Недостаточно аргументов.\n");
        return EBADF;
    }
    int pipes[2][2];    //Трубы
    if (pipe(pipes[0]) != 0 || pipe(pipes[1]) != 0)
    {
        perror("Ошибка создания трубы: ");
        close_pipes((int**)pipes);
        return errno;
    }
    int output = open(argv[3], O_CREAT | O_WRONLY | O_TRUNC, PERM);
    if (output < 0)
    {
        perror("Ошибка создания выходного файла: ");
        close_pipes((int**)pipes);
        return errno;
    }
    if (duplicate(argv[1], pipes[0]) != 0 || duplicate(argv[2], pipes[1]) != 0)
    {
        close_pipes((int**)pipes);
        close(output);
        return errno;
    }
    char chars[2];
    int count[2];
    int total = 0;
    count[0] = read(pipes[0][0], chars, 1);
    count[1] = read(pipes[1][0], chars+1, 1);
    while (count[0] != 0 && count[1] != 0)
    {
        
        if (count[0] < 0 || count[1] < 0)
        {
            perror("Ошибка чтения из труб: ");
            break;
        }
        char temp = chars[0] ^ chars[1];
        if (write(output, &temp, 1) != 1)
        {
            perror("Ошибка записи в файл: ");
            break;
        }
        else
            total++;
        count[0] = read(pipes[0][0], chars, 1);
        count[1] = read(pipes[1][0], chars+1, 1);
    }
    printf("Работа программы окончена, записано %i байт.\n", total);
    close(output);
    close(pipes[0][0]);
    close(pipes[1][0]);
    return 0;
}

int duplicate(char* program, int* _pipe)
{
    char* command;
    char** arg_vec = string_parser(program, "\n ");
    command = arg_vec[0];
    char path[PATH_MAX];
    if (getcwd(path, PATH_MAX) == NULL)	{
		perror("Ошибка получения пути: ");
		return errno;
	}
    arg_vec[0] = path;
    if (command == NULL)
        return -1;
    int child;
	switch (child = fork())	
    {
        case 0:	//Потомок
            close(STDOUT_FILENO);
            dup(_pipe[1]);
            close(_pipe[0]);
            if (execvp(command, arg_vec) == -1)	
            {
                perror("Ошибка открытия программы: ");
                exit(errno);
            }
            return -1;
        case -1:	//Ошибка при раздвоении
            perror("Ошибка раздвоения: ");
            break;
        default:	//Предок
            close(_pipe[1]);
            break;
	}
    //Очистка памяти
    free(command);
    for(int i = 1; arg_vec[i] != NULL; i++)
        free(arg_vec[i]);
    free(arg_vec);
    return 0;
}

void close_pipes(int** pipes)
{
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            close(pipes[i][j]);
}