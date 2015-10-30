#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <unistd.h>

int fd1;
 unsigned char buff[4096];
int rd,nbytes,tries;

int main()
{	while(1)
        {
	fd1=open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
	fcntl(fd1, F_SETFL,0);
	rd=read(fd1,buff,100);
	if(rd > 1)
	{
	printf("Bytes sent are %s\n",buff);
	}
	close(fd1);
	}
	return 0;
}

