#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int duplicate(char* program, int fd);  

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
    int output = open(argv[3], O_CREAT | O_WRONLY);
    children[0] = dublicate(argv[1], stream[0][1]);
    children[1] = dublicate(argv[2], stream[1][1]);
    char chars[2];
    while (read(pipes[0][0], chars, 1) != 0 && read(pipes[1][0], chars+1, 1) != 0)
    {
        char temp = chars[0] ^ chars[1];
        write(output, &temp, 1);
    }
    close(output);
    close(stream[0][0]);
    close(stream[1][0]);
    
}

int duplicate(char* program, int fd)
{
    int child = fork();
	switch (child)	
    {
        case 0:	//Потомок
            close(STDOUT_FILENO);
            dup(fd);
            if (execvp(command, arg_vec) == -1)	
            {
                perror("Ошибка открытия: ");
                exit(errno);
            }
            return;
        case -1:	//Ошибка при раздвоении
            perror("Ошибка раздвоения: ");
            break;
        default:	//Предок
            close(fd);
            break;
	}
    return child;
}