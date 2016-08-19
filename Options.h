//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_OPTIONS_H
#define ESPERF_OPTIONS_H

#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/poll.h>
#include <iomanip>
#include <vector>

using namespace std;

class Options {
public:
    unsigned int number_of_threads;
    unsigned int number_of_recurrence;
    unsigned int interval = 1;
    unsigned int iWarmupSec;
    vector<string> vsDict;
    string strDictFilename;
    string strHttpMethod = "GET";
    string strHttpUser;
    string strQuery;
    string strURL;
    bool bVerbose = false;
    int parse(int argc, char **argv);
    bool isStdinAvailable();
    void printLine(string strOtion, unsigned int value);
    void printLine(string strOtion, string value);
    void printLine(string strOtion, bool value);
    void print();
};


#endif //ESPERF_OPTIONS_H
