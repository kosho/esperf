//
// Created by Kosho Owa on 2016/08/19.
// Parse and hold command line options
//

#include "Options.h"

static const string COMMAND_LINE_OPTIONS_MSG = "Usage esperf [-d dictionary_file] [-r recurrence] [-t num_threads] [-w warm_up_sec] [-X method] url";
static const string OPTIONS_HEADER = "----------------------------------- Options ------------------------------------";

// Parse command line options_ and keep by instance variables
int Options::Parse(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv,"vhX:d:i:w:r:t:u:")) != EOF)
        switch(opt)
        {
            case 'd':
                dict_filename_ = optarg;
                break;
            case 'i':
                interval_sec_ = (u_int) atoi(optarg);
                break;
            case 'w':
                warmup_sec_ = (u_int) atoi(optarg);
                break;
            case 'r':
                num_recurrence_ =  (u_int) atoi(optarg);
                break;
            case 't':
                num_threads_ = (u_int) atoi(optarg);
                break;
            case 'u':
                http_user_ = optarg;
                break;
            case 'v':
                verbose_ = true;
                break;
            case 'X':
                http_method_ = optarg;
                break;
            case 'h':
            case ':':
            default:
                cout << COMMAND_LINE_OPTIONS_MSG;
                return -1;
        }

    // Get url from command line
    if (!argv[optind]) {
        cout << "Error: URL missing" << endl;
        return -1;
    }else {
        request_url_ = argv[optind];
    }

    // Read dictionary
    if (dict_filename_.size() > 0) {
        ifstream if_dict(dict_filename_);
        string str_line;
        while (getline(if_dict, str_line))
            dict_.push_back(str_line);
    }

    // Construct request body from stdin
    if (IsStdinAvailable()) {
        request_body_ = "";
        for (string str_line; getline(cin, str_line);) {
            request_body_.append(str_line);
            request_body_.append("\n");
        }
    }
    return 1;
}

void Options::PrintLine(const string otion, const u_int value)
{
    cout << setw(35) << right << otion << ": " << setw(15) << right << value << endl;
}

void Options::PrintLine(const string otion, const string value)
{
    cout << setw(35) << right << otion << ": " << value << endl;
}

void Options::PrintLine(const string otion, const bool value)
{
    if (value){
        cout << setw(35) << right << otion << ": " << "true" << endl;
    }else{
        cout << setw(35) << right << otion << ": " << "false" << endl;
    }
}

// Print parsed options
void Options::Print()
{
    cout << OPTIONS_HEADER << endl;
    PrintLine("Number of threads", num_threads_);
    PrintLine("Number of recurrence", num_recurrence_);
    PrintLine("Interval (sec)", interval_sec_);
    PrintLine("Warm-up (sec)", warmup_sec_);
    PrintLine("Dictionary", dict_filename_);
    PrintLine("URL", request_url_);
    PrintLine("HTTP Method", http_method_);
    if (verbose_) PrintLine("HTTP User", http_user_);
    if (verbose_) PrintLine("Verbose", verbose_);
    PrintLine("Body", request_body_);
}

// Check if any standard input is available
bool Options::IsStdinAvailable() {
    struct pollfd fds;
    fds.fd = 0; // stdin
    fds.events = POLLIN;

    if (poll(&fds, 1, 0)){
        return true;
    }
    return false;
}