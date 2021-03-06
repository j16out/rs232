//example of how to post data to the gaia server ***WITHOUT*** the use of system commands as used in the Gen2 script
//enter line into terminal to compile:   g++ post_data_curl.C -o post -lcurl
//the -lcurl links the curl libraries

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include <curl/easy.h>
 

int main()
{

curl_global_init( CURL_GLOBAL_ALL );
CURL * myHandle = curl_easy_init ( );


curl_easy_setopt(myHandle, CURLOPT_URL, "http://gaia.tru.ca/birdMOVES/postData.RA");
// Next we tell LibCurl what HTTP POST data to submit
char *data="time=00:00:00&rfid=ffffffffff&feederid=16";
curl_easy_setopt(myHandle, CURLOPT_POSTFIELDS, data);
curl_easy_perform( myHandle );

curl_easy_cleanup( myHandle );


return 0;
}
