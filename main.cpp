#include <iostream>
#include <fstream>
#include <thread>
#include <unistd.h>
#include <random>
// necessary to include on Linux
#include <mutex>
#include <sys/poll.h>
#include <curl/curl.h>

int i_success = 0;
int i_failure = 0;
int i_http_errors = 0;

std::mutex mtx;

// TODO: store statistics for benchmarking mode by using curl_easy_getinfo()
// TODO: show version number

// replace $RNUM  in `str_in` with a random number
std::string replace_with_random_number(std::string str_in) {

    std::random_device rd;

    std::string str_from = "$RNUM";
    std::string str_subject = str_in;
    std::string::size_type pos = str_subject.find(str_from);

    while (pos != std::string::npos) {
        std::string str_to = std::to_string(rd() % 256);
        str_subject.replace(pos, str_from.size(), str_to);
        pos = str_subject.find(str_from, pos + str_to.size());
    }

    return str_subject;
}

// TODO: more variations of random numbers, geo type

// replace $RDICT in `str_in` with randomly selected strings from the dictionary `vs_dict`
std::string replace_from_random_dict(std::string str_in, std::vector<std::string>& vs_dict) {

    std::random_device rd;

    std::string str_from = "$RDICT";
    std::string str_subject = str_in;
    std::string::size_type pos = str_subject.find(str_from);

    while (pos != std::string::npos) {
        std::string str_to = vs_dict.at(rd() % vs_dict.size());
        str_subject.replace(pos, str_from.size(), str_to);
        pos = str_subject.find(str_from, pos + str_to.size());
    }

    return str_subject;

}

// count successful requests with mutex locking
void count_success(){
    mtx.lock();
    i_success++;
    mtx.unlock();
}

// count failed requests with mutex locking
void count_failure(){
    mtx.lock();
    i_failure++;
    mtx.unlock();
}

// count HTTP errors > 400 with mutex locking
void count_http_errors(){
    mtx.lock();
    i_http_errors++;
    mtx.unlock();
}

// check if any inputs from stdin
bool is_stdin_available(){

    struct pollfd fds;
    fds.fd = 0; // stdin
    fds.events = POLLIN;

    if (poll(&fds, 1, 0) == 1){
        return true;
    }

    return false;

}

// the worker thread which performs requests to the http server
void worker(bool verbose, std::string http_method, std::string url, std::string http_user, std::string query, int max_recurrence, bool dots, std::vector<std::string>& vs_dict) {
    // consider mutex locking while verbose logging

    if(verbose) {
        mtx.lock();
        std::cout << std::this_thread::get_id() << " thread started" << std::endl;
        mtx.unlock();
    }

    // init easy curl
    CURL *curl = curl_easy_init();

    if (curl) {
        CURLcode res;
        FILE *f_null;

        // do not show the response unless verbose logging
        if (!verbose) {
            f_null = fopen("/dev/null", "wb");
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, f_null);
        }

        // set the method explicitly
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, http_method.c_str());

        // capture HTTP errors
        curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);

        // enable basic auth
        if (!http_user.empty()){
            curl_easy_setopt(curl, CURLOPT_USERPWD, http_user.c_str());
        }

                // repeat requests
        for (int j = 0; j < max_recurrence; j++) {

            // supply random numbers and strings
            std::string randomized_url = replace_with_random_number(url);
            std::string randomized_query = replace_with_random_number(query);

            if(vs_dict.size() > 0) {
                randomized_url = replace_from_random_dict(randomized_url, vs_dict);
                randomized_query = replace_from_random_dict(randomized_query, vs_dict);
            }

            // set the URL and the body
            curl_easy_setopt(curl, CURLOPT_URL, randomized_url.c_str());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, randomized_query.size());
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, randomized_query.c_str());

            if(verbose){
                mtx.lock();
                std::cout << std::this_thread::get_id() << " URL: " << randomized_url << std::endl;
                std::cout << std::this_thread::get_id() << " Method: " << http_method << std::endl;
                std::cout << std::this_thread::get_id() << " Auth: "  << http_user << std::endl;
                std::cout << std::this_thread::get_id() << " Body: " << randomized_query << std::endl;
                mtx.unlock();
            }

            // perform a request
            res = curl_easy_perform(curl);

            if (dots) {
                std::cout << ".";
            }

            // curl and HTTP errors
            switch (res){
                case CURLE_OK:
                    count_success();
                    break;
                case CURLE_HTTP_RETURNED_ERROR:
                    long http_response_code;
                    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, std::ref(http_response_code));
                    mtx.lock();
                    std::cerr << "Error: HTTP response (" << http_response_code << ")" << std::endl;
                    mtx.unlock();
                    count_http_errors();
                    break;
                default:
                    mtx.lock();
                    std::cerr << "Error: curl_easy_perform() returned (" << res << ") " << curl_easy_strerror(res) << std::endl;
                    mtx.unlock();
                    count_failure();
            }

        }
        if(!verbose) fclose(f_null);
        curl_easy_cleanup(curl);
    }
}

// the timer thread which shows number of requests performed every each second
void timer(int max_threads, int max_recurrence, bool dots){

    int i_prev_success = 0;
    int i_prev_failure = 0;
    int i_prev_http_errors = 0;
    char time_buff[80];

    while(true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        mtx.lock();
        if(!dots){
            std::time_t now_t = std::time(NULL);
            std::strftime(time_buff, sizeof(time_buff), "%FT%T%z", std::localtime(&now_t));
            std::cout << time_buff << " " << i_success - i_prev_success << " " << i_failure - i_prev_failure << " " << i_http_errors - i_prev_http_errors << std::endl;
        }else{
            std::cout << std::endl;
        }
        i_prev_success = i_success;
        i_prev_failure = i_failure;
        i_prev_http_errors = i_http_errors;
        mtx.unlock();
        if (i_success + i_failure + i_http_errors == max_recurrence * max_threads) break;
    }
}

int main(int argc, char **argv) {

    // default settings
    int max_threads = 5;
    int max_recurrence = 10;
    bool dots = false;
    bool verbose = false;
    std::vector<std::string> vs_dict;
    std::string dict_filename;
    std::string http_method = "GET";
    std::string http_user;

    std::string query;

//    std::string query = "{\n"
//            "  \"query\": {\n"
//            "    \"range\": {\n"
//            "      \"mag\": {\n"
//            "        \"gte\": $RNUM\n"
//            "      }\n"
//            "    }\n"
//            "  }\n"
//            "}";

    // parse command line options
    // -d: show dots instead of metrics
    // -h: show help
    // -r val: number of recurrence per thread
    // -t val: threads to generate
    // -u val: http basic authentication username and password
    // -v : show verbose outputs for debugging purpose
    // -X val: one of GET, PUT or POST method (default GET)

    int opt;
    while ((opt = getopt(argc,argv,"ivhX:d:r:t:u:")) != EOF)
        switch(opt)
        {
            case 'd':
                dict_filename = optarg;
                break;
            case 'i':
                dots = true;
                break;
            case 'r':
                max_recurrence = std::atoi(optarg);
                break;
            case 't':
                max_threads = std::atoi(optarg);
                break;
            case 'u':
                http_user = optarg;
                break;
            case 'v':
                verbose = true;
                break;
            case 'X':
                http_method = optarg;
                break;
            case 'h':
            case ':':
            default:
                std::cout << "usage: " << "esperf" << " [-d dictionary_file] [-r max_recurrence] [-t num_threads] [-X method] url";
                abort();
        }

    // get url from command line
    std::string url;
    if (!argv[optind]) {
        std::cout << "Error: URL missing" << std::endl;
        abort();
    }else {
        url = argv[optind];
    }

    // read dictionary
    if (dict_filename.size() > 0) {
        std::ifstream if_dict(dict_filename);
        std::string str_line;
        while (getline(if_dict, str_line))
            vs_dict.push_back(str_line);
    }

    // read request body from stdin
    if (is_stdin_available()) {
        query = "";
        for (std::string str_line; std::getline(std::cin, str_line);) {
            query.append(str_line);
            query.append("\n");
        }
    }

    // create threads
    std::thread th_timer(timer, max_threads, max_recurrence, dots);
    std::thread *th_worker;
    th_worker = new std::thread[max_threads];

    // run threads
    for (int i = 0; i < max_threads; i++) {
        th_worker[i] = std::thread(worker, verbose, http_method, url, http_user, query, max_recurrence, dots, std::ref(vs_dict));
    }
    for (int i = 0; i < max_threads; i++) {
        th_worker[i].join();
    }
    th_timer.join();

    if (verbose) std::cout << "Total number of requests performed: " << i_success << std::endl;
    return 0;
}
