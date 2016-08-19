//
// Created by Kosho Owa on 2016/08/19.
//

#include "Worker.h"

using namespace std;

void Worker::run()
{
    // init easy curl
    CURL *curl = curl_easy_init();

    if (curl){
        CURLcode cr;
        FILE *fNull;

        // do not show the response unless verbose logging
        if (!options->bVerbose) {
            fNull = fopen("/dev/null", "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fNull);
        }

        // set the method explicitly
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, options->strHttpMethod.c_str());

        // capture HTTP errors
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        // enable basic auth
        if (options->strHttpUser.empty()){
            curl_easy_setopt(curl, CURLOPT_USERPWD, options->strHttpUser.c_str());
        }

        for (int i = 0; i < options->number_of_recurrence; i++){
            // supply random numbers and strings
            string strURL = replaceRNUM(options->strURL);
            string strQuery = replaceRNUM(options->strQuery);
            if (options->vsDict.size() > 0){
                strURL = replaceRDICT(strURL);
                strQuery = replaceRDICT(strQuery);
            }

            // set the URL and the body
            curl_easy_setopt(curl, CURLOPT_URL, strURL.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strQuery.size());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, strQuery.c_str());

            // perform a request
            cr = curl_easy_perform(curl);

            // curl and HTTP errors
            switch (cr){
                case CURLE_OK:
                    long sizeUpload;
                    double sizeDownload;
                    long sizeReceivedHeader;
                    double transferTime;
                    curl_easy_getinfo(curl, CURLINFO_REQUEST_SIZE, &sizeUpload);
                    curl_easy_getinfo(curl, CURLINFO_SIZE_DOWNLOAD, &sizeDownload);
                    curl_easy_getinfo(curl, CURLINFO_HEADER_SIZE, &sizeReceivedHeader);
                    curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME, &transferTime);
                    stats->count(1, 0, 0, sizeUpload, (unsigned int) (sizeDownload + sizeReceivedHeader) , transferTime);
                    break;
                case CURLE_HTTP_RETURNED_ERROR:
                    long http_response_code;
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_response_code);
                    // cerr << "Error: HTTP response (" << http_response_code << ")" << endl;
                    stats->count(0, 0, 1, 0, 0, 0);
                    break;
                default:
                    //cerr << "Error: curl_easy_perform() returned (" << cr << ") " << curl_easy_strerror(cr) << endl;
                    stats->count(0, 1, 0, 0, 0, 0);
            }

        }
        fclose(fNull);
        curl_easy_cleanup(curl);
    }
}

Worker::Worker(Stats *stats, Options *options) : stats(stats), options(options) {}

string Worker::replaceRNUM(string strIn) {

    random_device rd;

    string str_from = "$RNUM";
    string str_subject = strIn;
    string::size_type pos = str_subject.find(str_from);

    while (pos != string::npos) {
        string str_to = to_string(rd() % 256);
        str_subject.replace(pos, str_from.size(), str_to);
        pos = str_subject.find(str_from, pos + str_to.size());
    }

    return str_subject;

}

string Worker::replaceRDICT(string strIn) {

    random_device rd;

    string str_from = "$RDICT";
    string str_subject = strIn;
    string::size_type pos = str_subject.find(str_from);

    while (pos != string::npos) {
        string str_to = options->vsDict.at(rd() % options->vsDict.size());
        str_subject.replace(pos, str_from.size(), str_to);
        pos = str_subject.find(str_from, pos + str_to.size());
    }

    return str_subject;

}