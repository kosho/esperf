//
// Created by Kosho Owa on 2016/08/19.
// Parse and hold command line options
//

#include "Options.h"

static const string COMMAND_LINE_OPTIONS_MSG = "Usage: esperf [-v] [-d dictionary_file] [-i interval_sec] [-w warm_up_sec] [-r recurrence] [-t num_threads] [-u user:password] [-T timeout] [-X method] url";
static const string OPTIONS_HEADER = "----------------------------------- Options ------------------------------------";

// Parse command line options and keep by instance variables
int Options::Parse(int argc, char **argv)
{
    int opt;
    while ((opt = getopt(argc, argv,"vhX:d:i:w:T:r:t:u:")) != EOF)
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
            case 'T':
                timeout_sec_ = static_cast<u_int>(atoi(optarg));
                break;
            case 'v':
                verbose_ = true;
                break;
            case 'X':
                http_method_ = optarg;
                break;
            default:
                cout << COMMAND_LINE_OPTIONS_MSG << endl;
                return EXIT_FAILURE;
        }

    // Get url from command line
    if (!argv[optind]) {
        cout << "Error: URL missing" << endl;
        cout << COMMAND_LINE_OPTIONS_MSG << endl;
        return EXIT_FAILURE;
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
    return EXIT_SUCCESS;
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
    PrintLine("Timeout (sec)", timeout_sec_);
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

    if (poll(&fds, 1, poll_timeout)){
        return true;
    }
    return false;
}
