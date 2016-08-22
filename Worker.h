//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_WORKER_H
#define ESPERF_WORKER_H

#include <iostream>
#include <curl/curl.h>
#include <random>
#include <sstream>
#include <thread>

#include "Options.h"
#include "Stats.h"

using namespace std;

class Worker {
public:
    Worker(Stats *stats, Options *options);

    Worker(Stats *stats_, Options *options_, mutex *mtx_for_cout_);

    void Run();

private:
    Stats *stats_;
    Options *options_;
    mutex *mtx_for_cout_;

    string ReplaceRNUM(const string in);

    string ReplaceRNUMEx(const string in);

    string ReplaceRDICT(const string in);

    void safe_cout(const string msg);

    void safe_cerr(const string msg);

};


#endif //ESPERF_WORKER_H
