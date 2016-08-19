//
// Created by Kosho Owa on 2016/08/19.
//

#include "Options.h"

int Options::parse(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc,argv,"vhX:d:i:w:r:t:u:")) != EOF)
        switch(opt)
        {
            case 'd':
                strDictFilename = optarg;
                break;
            case 'i':
                interval = (unsigned int) atoi(optarg);
                break;
            case 'w':
                iWarmupSec = (unsigned int) atoi(optarg);
                break;
            case 'r':
                number_of_recurrence =  (unsigned int) atoi(optarg);
                break;
            case 't':
                number_of_threads = (unsigned int) atoi(optarg);
                break;
            case 'u':
                strHttpUser = optarg;
                break;
            case 'v':
                bVerbose = true;
                break;
            case 'X':
                strHttpMethod = optarg;
                break;
            case 'h':
            case ':':
            default:
                cout << "usage: " << "esperf" << " [-d dictionary_file] [-r recurrence] [-t num_threads] [-w warm_up_sec] [-X method] url";
                return -1;
        }

    // get url from command line
    if (!argv[optind]) {
        cout << "Error: URL missing" << endl;
        return -1;
    }else {
        strURL = argv[optind];
    }

    // read dictionary
    if (strDictFilename.size() > 0) {
        ifstream if_dict(strDictFilename);
        string str_line;
        while (getline(if_dict, str_line))
            vsDict.push_back(str_line);
    }

    // read request body from stdin
    if (isStdinAvailable()) {
        strQuery = "";
        for (string str_line; getline(cin, str_line);) {
            strQuery.append(str_line);
            strQuery.append("\n");
        }
    }
    return 1;
}

void Options::printLine(string strOtion, unsigned int value)
{
    cout << setw(35) << right << strOtion << ": " << setw(15) << right << value << endl;
}

void Options::printLine(string strOtion, string value)
{
    cout << setw(35) << right << strOtion << ": " << value << endl;
}

void Options::printLine(string strOtion, bool value)
{
    if (value){
        cout << setw(35) << right << strOtion << ": " << "true" << endl;
    }else{
        cout << setw(35) << right << strOtion << ": " << "false" << endl;
    }
}

void Options::print()
{
    cout << "---------------------- Options ---------------------" << endl;
    printLine("Number of threads", number_of_threads);
    printLine("Number of recurrence", number_of_recurrence);
    printLine("Interval (sec)", interval);
    printLine("Warm-up (sec)", iWarmupSec);
    printLine("Dictionary", strDictFilename);
    printLine("URL", strURL);
    printLine("HTTP Method", strHttpMethod);
    if (bVerbose) printLine("HTTP User", strHttpUser);
    if (bVerbose) printLine("Verbose", bVerbose);
    printLine("Body", strQuery);
}

bool Options::isStdinAvailable() {
    struct pollfd fds;
    fds.fd = 0; // stdin
    fds.events = POLLIN;

    if (poll(&fds, 1, 0)){
        return true;
    }
    return false;
}