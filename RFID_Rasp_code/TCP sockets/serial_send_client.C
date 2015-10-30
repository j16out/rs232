
/**************************************************

file: demo_rx.c
purpose: simple demo that receives characters from
the serial port and print them on the screen,
exit the program by pressing Ctrl-C

compile with the command: gcc demo_rx.c rs232.c -Wall -Wextra -o2 -o test_rx

**************************************************/
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "rs232.h"
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include "ctime"
#include <sys/socket.h> // Needed for the socket functions
#include <netdb.h>      // Needed for the socket functions
#include <unistd.h>  
#include <istream>
#include <cstring>
#include <alloca.h>

using namespace std;

template <typename T> string tostr(const T& t) { 
   ostringstream os; 
   os<<t; 
   return os.str();
}


void sendstringtcp(char *msg);

int main()
{
  int i, n,
      cport_nr=1,        /* /dev/ttyS0 (COM1 on windows) */
      bdrate=9600;       /* 9600 baud */
      int seconds = 5;

  unsigned char buf[4096];
  string buftemp = "B";
  
  const char *c1; 
 
  
  bool run = false;
  bool ntag = false;
  bool time = true;


char mode[]={'8','N','1',0},
       str[2][512];


  strcpy(str[0], "r");


  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }


	while(1)
	{
	  string bufstring = "START  ";
	   clock_t endwait;
	   
	   endwait = clock () + seconds * CLOCKS_PER_SEC ;
	   run = false;
	  
		   while (clock() < endwait && time)
		   {
		   //RS232_cputs(cport_nr, str[0]);
		    //printf("sent: %s\n", str[0]);
		    n = RS232_PollComport(cport_nr, buf, 4095);

			    if(n > 0)
			    {
			      buf[n] = 0;   /* always put a "null" at the end of a string! */

			      for(i=0; i < n; i++)
			      {
				if(buf[i] < 32)  /* replace unreadable control-codes by dots */
				{
				 buf[i] = ' ';
				  //cout << "\n";
				}
			      }

			   //cout << buf << "\n";
			   string sName(reinterpret_cast<char*>(buf));
			   
			           if(sName.find("FFF") == 0)
			           ntag = true;
			           
					   if((sName.find("r") == 0 && run == false) || (run && bufstring.length() < 60))
					    {
					    bufstring.append(sName);
					    run = true;
					    }
					    
					    if( bufstring.length() <= 59)
					    time = false;
					    
					    
			    }

		    }
	   

	   cout << "\n" << bufstring;
	   
	    if(ntag)
	    {
	     ntag = false;
             }
             else
             { 
             
             c1 = bufstring.c_str();
	     char *conbuf = (char *)c1;
	   
             sendstringtcp(conbuf);
             }
            
	  }
	 
 
  return(0);
}

void sendstringtcp(char *msg)
{
    int len;
    ssize_t bytes_sent;
    ssize_t bytes_recieved;
    char incomming_data_buffer[1000];
    int socketfd ;
    int i = 0;
    int status; 
    
    
    
	    struct addrinfo host_info;       // The struct that getaddrinfo() fills up with data.
	    struct addrinfo *host_info_list; // Pointer to the to the linked list of host_info's.

	    // The MAN page of getaddrinfo() states "All  the other fields in the structure pointed
	    // to by hints must contain either 0 or a null pointer, as appropriate." When a struct
	    // is created in c++, it will be given a block of memory. This memory is not nessesary
	    // empty. Therefor we use the memset function to make sure all fields are NULL.
	    memset(&host_info, 0, sizeof host_info);


	    host_info.ai_family = AF_UNSPEC;     // IP version not specified. Can be both.
	    host_info.ai_socktype = SOCK_STREAM; // Use SOCK_STREAM for TCP or SOCK_DGRAM for UDP.

	    // Now fill up the linked list of host_info structs with google's address information.
	    status = getaddrinfo("10.20.160.168", "5556", &host_info, &host_info_list);
	    // getaddrinfo returns 0 on succes, or some other value when an error occured.
            // (translated into human readable text by the gai_gai_strerror function).
            if (status != 0)  std::cout << "getaddrinfo error" << gai_strerror(status) ;


            std::cout << "\nCreating a socket..."  << std::endl;
            // The socket descripter
            socketfd = socket(host_info_list->ai_family, host_info_list->ai_socktype,
                      host_info_list->ai_protocol);
            if (socketfd == -1)  std::cout << "socket error " ;

    
    	    std::cout << "Connect()ing..."  << std::endl;
	    status = connect(socketfd, host_info_list->ai_addr, host_info_list->ai_addrlen);
	    if (status == -1)  std::cout << "connect error" ;
    
    
	    std::cout << "sending message..."  << std::endl;
	    len = strlen(msg);
	    bytes_sent = send(socketfd, msg, len, 0);
	    std::cout << "Waiting to recieve data..."  << std::endl;
	    
	    bytes_recieved = recv(socketfd, incomming_data_buffer,1000, 0);
	    // If no data arrives, the program will just wait here until some data arrives.
	    if (bytes_recieved == 0) std::cout << "host shut down." << std::endl ;
	    if (bytes_recieved == -1)std::cout << "recieve error!" << std::endl ;
	    std::cout << bytes_recieved << " bytes recieved :" << std::endl ;
	    incomming_data_buffer[bytes_recieved] = '\0' ;
	    std::cout << incomming_data_buffer << std::endl;
	    std::cout.flush();    
 
	    std::cout << "Receiving complete" << std::endl;
	    freeaddrinfo(host_info_list);
	    
	    int close(socketfd);
    }

