//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_TIMER_H
#define ESPERF_TIMER_H

#include "Options.h"
#include "Stats.h"

#include <iostream>
#include <thread>

using namespace std;

class Timer {
    Stats *stats;
    Options *options;
public:
    Timer(Stats *stats, Options *options);
    void start();
};

#endif //ESPERF_TIMER_H
