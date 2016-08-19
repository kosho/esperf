//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_ESPERF_H
#define ESPERF_ESPERF_H

#include "Options.h"
#include "Stats.h"
#include "Worker.h"
#include "Timer.h"

#include <thread>

using namespace std;

class Esperf {
    Options *options;
public:
    Esperf(Options *options);
    void run();
};


#endif //ESPERF_ESPERF_H
