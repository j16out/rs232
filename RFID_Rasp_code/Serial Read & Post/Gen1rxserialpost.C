
/**************************************************

purpose: simple program that receives rfid data from
the serial port via ardunio and post them to server,
exit the program by pressing Ctrl-C

compile with the command: g++ rxserialpost.C rs232.c -Wall -Wextra -o2 -o serpost

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
  int i, n, cport_nr=1, bdrate=9600;        /* /dev/ttyS0 (COM1 on windows) */
           
  int seconds = 5;

  unsigned char buf[4096];
  string buftemp = "B";
  
 
   const char* c1; 
  
  bool run = false;


char mode[]={'8','N','1',0},
       str[2][512];


  strcpy(str[0], "r");


  if(RS232_OpenComport(cport_nr, bdrate, mode))
  {
    printf("Can not open comport\n");

    return(0);
  }


	while(1)
	{RS232_cputs(cport_nr, str[0]);
	  printf("sent: %s", str[0]);
	  string bufstring = "START  ";
	   clock_t endwait;
	   
	   endwait = clock () + seconds * CLOCKS_PER_SEC ;
	   run = false;
	  
		   while (clock() < endwait)
		   {
		   
		    n = RS232_PollComport(cport_nr, buf, 4095);

			    if(n > 0)
			    {
			      buf[n] = 0;   /* always put a "null" at the end of a string! */

			      for(i=0; i < n; i++)
			      {
				if(buf[i] < 32)  /* replace unreadable control-codes by dots */
				{
				  buf[i] = '.';
				  //cout << "\n";
				}
			      }

			   //cout << buf << "\n";
			   string sName(reinterpret_cast<char*>(buf));
					   if((sName.find("r") == 0 && run == false) || (run && bufstring.length() < 60))
					    {
					    bufstring.append(sName);
					    run = true;
					    }
					    
					    
			    }

		    }
	   

	    cout << "\n" << bufstring << "\n";
            if(bufstring.length() >= 60 && bufstring.length() <= 65)
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


/*typical string

FFFFFFFFFF....03/02/15 10:57:05 ..

*/
int found;
const char * c;
const char * b;
const char * a;

bool postdata = true;
string data = "curl --data \"";
string picfile = "streamer -f jpeg -o pics/";

//find time	
data.append("time=");		
string current = msg;
string hardcopy;

found=current.find("......");

current.erase (current.begin(), current.begin() + found + 6);
hardcopy = current;
current.erase (current.begin(), current.begin() +14);
current.erase (current.begin()+17, current.end());
b = current.c_str();

data.append(b);

int f = current.find("/");
current.replace(f, std::string("/").length(), ":");

f = current.find("/");
current.replace(f, std::string("/").length(), ":");


f = current.find(" ");
current.replace(f, std::string(" ").length(), "_");


b = current.c_str();
picfile.append(b);
picfile.append("___");


//find tag
data.append("&rfid=");
current = hardcopy;
current.erase (current.begin()+10, current.end());
if(current.find("FFFFFFFFFF") == 0)
	{
	postdata = false;
	}

b = current.c_str();

data.append(b);




picfile.append(b);
picfile.append(".jpeg -c /dev/video0");
//send data
	
	
	

	data.append("&feederid=");
	data.append(tostr(21));
	data.append("\" --url \"http://gaia.tru.ca/birdMOVES/postData.RA\" -v");
	
	c = data.c_str();
        a = picfile.c_str();
        if(postdata)
        {
        cout <<  "\n" << c << "\n";
        system(a);
        //system(c);
         
        }
        
        
        
        
        
        
 }

