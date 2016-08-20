//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_TIMER_H
#define ESPERF_TIMER_H

#include <iostream>
#include <thread>

#include "Options.h"
#include "Stats.h"

using namespace std;

class Timer {
public:
    Timer(Stats *stats, Options *options);

    void Start();

private:
    Stats *stats_;
    Options *options_;
};

#endif //ESPERF_TIMER_H
