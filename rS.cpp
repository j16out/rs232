#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>



int main()
{
int fd1;
int fd2;
char *buff,*buffer,*bufptr;
int wr,rd,nbytes,tries;

fd1=open("/dev/ttyACM1", O_RDWR | O_NOCTTY | O_NDELAY);

if (fd1 == -1 )

{

perror("open_port: Unable to open /dev/ttyACM1 – ");

}

else

{

fcntl(fd1, F_SETFL,0);

printf("Port 1 has been sucessfully opened and %d is the file description\n",fd1);

}









return 0;
}

