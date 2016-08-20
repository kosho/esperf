//
// Created by Kosho Owa on 2016/08/19.
// Hold statistics information of HTTP requests
//

#include "Stats.h"

// Display adjustment
static const int PROGRESS_WIDTH = 9;
static const int RESULT_WIDTH = 15;

static const string PROGRESS_HEADER = "------------------------ --------- --------- -------- -------- -------- --------";
static const string RESULT_HEADER = "----------------------------------- Results ------------------------------------";

// Print progress, called by Timer every interval second
void Stats::ShowProgress() {
    char time_buff[80];
    time_t now_t = time(NULL);
    strftime(time_buff, sizeof(time_buff), "%FT%T%z", localtime(&now_t));

    u_long upload = 0;
    u_long download = 0;

    if ((success_ - prev_success_) != 0) {
        upload = static_cast<u_int>((size_upload_ - prev_size_upload_) / (success_ - prev_success_));
        download = static_cast<u_int>((size_download_ - prev_size_download_) / (success_ - prev_success_));
    }

    double response = 0.0;

    if (success_ > 0) {
        response = (time_transfer_ - prev_time_transfer_) / success_;
    }

    cout << time_buff << " " << setw(PROGRESS_WIDTH) << success_ - prev_success_ << " "
         << setw(PROGRESS_WIDTH) << error_curl_ - prev_error_curl_
         << setw(PROGRESS_WIDTH) << error_http_ - prev_error_http_
         << setw(PROGRESS_WIDTH) << upload
         << setw(PROGRESS_WIDTH) << download
         << setw(PROGRESS_WIDTH) << fixed << setprecision(4) << response << endl;

    prev_success_ = success_;
    prev_error_curl_ = error_curl_;
    prev_error_http_ = error_http_;
    prev_size_upload_ = size_upload_;
    prev_size_download_ = size_download_;
    prev_time_transfer_ = time_transfer_;

}

// Print the final result
void Stats::ShowResult() {
    if (finished_) {
        double elapsed_sec = ((clock_stop_ - clock_start_).count()) * chrono::steady_clock::period::num /
                             static_cast<double>(chrono::steady_clock::period::den);

        cout << RESULT_HEADER << endl;

        Stats::PrintLine("Total time (sec)", elapsed_sec);
        Stats::PrintLine("Number of success", static_cast<u_int>(wu_success_));
        Stats::PrintLine("Number of connection failure", static_cast<u_int>(wu_error_curl_));
        Stats::PrintLine("Number of HTTP response >400", static_cast<u_int>(wu_error_http_));
        Stats::PrintLine("Average successful requests/sec", static_cast<u_int> (wu_success_ / elapsed_sec));
        Stats::PrintLine("Upload throughput (byte/sec)", static_cast<u_int>(wu_size_upload_ / elapsed_sec));
        Stats::PrintLine("Download throughput (byte/sec)", static_cast<u_int> (wu_size_download_ / elapsed_sec));
        double time_transfer = 0.0;
        if (wu_success_ > 0) {
            time_transfer = static_cast<double> (time_transfer_ / wu_success_);
        }
        Stats::PrintLine("Average time transfer (sec)", time_transfer);
    }
}

// Return if all the requests are finished
bool Stats::IsFinished() const {
    return finished_;
}

// Safely count the number of requests and other statistics
void Stats::Count(int success, int error_curl, int error_http, u_long size_upload, u_long size_download,
                  double time_transfer) {

    // TODO: use lock guard to make entire method safe
    success_ += success;
    error_curl_ += error_curl;
    error_http_ += error_http;
    size_upload_ += size_upload;
    size_download_ += size_download;
    add_to_atomic_double(&time_transfer_, time_transfer);

    if ((chrono::steady_clock::now() - clock_start_).count() * chrono::steady_clock::period::num
        / static_cast<double>(chrono::steady_clock::period::den) > options_->warmup_sec_) {
        wu_success_ += success;
        wu_error_curl_ += error_curl;
        wu_error_http_ += error_http;
        wu_size_upload_ += size_upload;
        wu_size_download_ += size_download;
        add_to_atomic_double(&wu_time_transfer_, time_transfer);
    }

    if ((success_ + error_curl_ + error_http_) == options_->num_recurrence_ * options_->num_threads_) {
        clock_stop_ = chrono::steady_clock::now();
        finished_ = true;
    }
}

void Stats::PrintLine(string option, u_int value) {
    cout << setw(35) << right << option << ": " << setw(RESULT_WIDTH) << right << value << endl;
}

void Stats::PrintLine(string option, double value) {
    cout << setw(35) << right << option << ": " << setw(RESULT_WIDTH) << right << fixed << setprecision(5) << value
         << endl;
}

void Stats::ShowProgressHeader() {
    cout << setw(24) << left << "Timestamp" << " "
         << setw(PROGRESS_WIDTH) << right << "Success" << " " << setw(PROGRESS_WIDTH) << "Fail" << setw(PROGRESS_WIDTH)
         << "HTTP>400"
         << setw(PROGRESS_WIDTH) << "Upload" << setw(PROGRESS_WIDTH) << "Download" << setw(PROGRESS_WIDTH) << "Response"
         << endl;
    cout << PROGRESS_HEADER << endl;
}

Stats::Stats(Options *options) : options_(options) {
    success_ = 0;
    error_curl_ = 0;
    error_http_ = 0;
    size_upload_ = 0;
    size_download_ = 0;
    time_transfer_ = 0;
    wu_success_ = 0;
    wu_error_curl_ = 0;
    wu_error_http_ = 0;
    wu_size_upload_ = 0;
    wu_size_download_ = 0;
    wu_time_transfer_ = 0;
}
