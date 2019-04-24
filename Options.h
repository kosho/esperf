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
    u_int num_threads_ = 1;
    u_int num_recurrence_ = 1;
    u_int interval_sec_ = 1;
    u_int warmup_sec_;
    u_int timeout_sec_;
    vector<string> dict_;
    string dict_filename_;
    string http_method_ = "GET";
    string http_user_;
    string http_header_;
    string request_body_;
    string request_url_;
    bool verbose_ = false;
    // timeout msec to check if stdin is available
    u_int poll_timeout = 100;

    int Parse(int argc, char **argv);

    void Print();

private:
    bool IsStdinAvailable();

    void PrintLine(const string otion, const u_int value);

    void PrintLine(const string otion, const string value);

    void PrintLine(const string otion, const bool value);
};

#endif //ESPERF_OPTIONS_H
