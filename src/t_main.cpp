#include <string> 
#include <iostream> 
#include <curl/curl.h> 

using namespace std;
int main()
{
	CURL *curl;
	CURLcode ret;

	curl = curl_easy_init();
	string chunk;

	if (curl == NULL) {
		cerr << "curl_easy_init() failed" << endl;
		return 1;
	}

	curl_easy_setopt(curl, CURLOPT_URL, "http://157.7.242.70/face/detect/");
	ret = curl_easy_perform(curl);

	if (ret != CURLE_OK) {
		cerr << "curl_easy_perform() failed." << endl;
		return 1;
	}


	return 0;
}

