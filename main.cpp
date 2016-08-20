#include <iostream>

#include "Options.h"
#include "Esperf.h"

using namespace std;

/*
 * main
 */

int main(int argc, char **argv) {

    // parse command line options_
    Options options;
    if (options.Parse(argc, argv)){
        // run esperf
        Esperf esperf(&options);
        esperf.Run();
    };

    return 0;
}
