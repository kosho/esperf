//
// Created by Kosho Owa on 2016/08/19.
//

#ifndef ESPERF_ESPERF_H
#define ESPERF_ESPERF_H

#include <thread>

#include "Options.h"
#include "Stats.h"
#include "Worker.h"
#include "Timer.h"

using namespace std;

class Esperf {
public:
    Esperf(Options *options);
    void Run();
private:
    Options *options_;
};


#endif //ESPERF_ESPERF_H
