//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_WORKER_H
#define ESPERF_WORKER_H

#include "Options.h"
#include "Stats.h"

#include <iostream>
#include <curl/curl.h>
#include <random>

using namespace std;

class Worker {
    Stats *stats;
    Options *options;
public:
    Worker(Stats *stats, Options *options);
    string replaceRNUM(string strIn);
    string replaceRDICT(string strIn);
    void run();
};


#endif //ESPERF_WORKER_H
