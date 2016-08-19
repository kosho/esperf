#include <iostream>

#include "Options.h"
#include "Esperf.h"

using namespace std;

/*
 * main
 */

int main(int argc, char **argv) {

    // parse command line options
    Options options;
    if (options.parse(argc, argv)){
        // run Esperf
        Esperf esperf(&options);
        esperf.run();
    };

    return 0;
}
