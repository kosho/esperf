//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_WORKER_H
#define ESPERF_WORKER_H

#include <iostream>
#include <curl/curl.h>
#include <random>
#include <sstream>

#include "Options.h"
#include "Stats.h"

using namespace std;

class Worker {
public:
    Worker(Stats *stats, Options *options);

    void Run();

private:
    Stats *stats_;
    Options *options_;

    string ReplaceRNUM(string in);

    string ReplaceRNUMEx(string in);

    string ReplaceRDICT(string in);
};


#endif //ESPERF_WORKER_H
