#include "gamma.h"

int main(int argc, char* argv[])    //На вход подается две программы и выдается результат их xor-я
{
    if (argc < 4)
    {
        printf("Недостаточно аргументов.\n");
        return EBADF;
    }
    int pipes[2][2];
    int children[2];
    pipe(pipes[0]);
    pipe(pipes[1]);
    int output = open(argv[3], O_CREAT | O_WRONLY, PERM);
    children[0] = duplicate(argv[1], pipes[0]);
    children[1] = duplicate(argv[2], pipes[1]);
    char chars[2];
    while (read(pipes[0][0], chars, 1) != 0 && read(pipes[1][0], chars+1, 1) != 0)
    {
        char temp = chars[0] ^ chars[1];
        write(output, &temp, 1);
    }
    printf("Работа программы окончена.\n");
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
		perror("Getting path error: ");
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
                perror("Ошибка открытия: ");
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
    return child;
}