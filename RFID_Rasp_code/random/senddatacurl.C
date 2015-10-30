 #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

#include <curl/easy.h>
//g++ login.C -o login -lcurl 

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
