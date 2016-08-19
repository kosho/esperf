//
// Created by Kosho Owa on 2016/08/19.
//

#include "Stats.h"

void Stats::showProgress()
{
    char time_buff[80];
    time_t now_t = time(NULL);
    strftime(time_buff, sizeof(time_buff), "%FT%T%z", localtime(&now_t));
    cout << time_buff << " " << setw(iProgressWidth) << ulSuccess - prevSuccess << " " << setw(iProgressWidth) << ulErrorCurl - prevErrorCurl << setw(iProgressWidth) << ulErrorHttp - prevErrorHttp
         << setw(iProgressWidth) << ulSizeUpload - prevSizeUpload << setw(iProgressWidth) << ulSizeDownload - prevSizeDownload << setw(iProgressWidth) << fixed << setprecision(5) << (dTimeTransfer - prevTimeTransfer)/ulSuccess << endl;

    prevSuccess = ulSuccess;
    prevErrorCurl = ulErrorCurl;
    prevErrorHttp = ulErrorHttp;
    prevSizeUpload = ulSizeUpload;
    prevSizeDownload = ulSizeDownload;
    prevTimeTransfer = dTimeTransfer.load();

}

void Stats::showResult()
{
    if (finished) {
        double elapsedSeconds = ((clock_end - clock_start).count()) * chrono::steady_clock::period::num /
                                static_cast<double>(chrono::steady_clock::period::den);
        // TODO: calculate and print
        cout << "---------------------- Result ----------------------" << endl;

        Stats::printLine("Total time (sec)", elapsedSeconds);
        Stats::printLine("Number of success", (unsigned int) ulAwSuccess.load());
        Stats::printLine("Number of connection failure", (unsigned int) ulAwErrorCurl.load());
        Stats::printLine("Number of HTTP response >400", (unsigned int)ulAwErrorHttp.load());
        Stats::printLine("Average successful requests/sec", (double) ulAwSuccess.load()/ elapsedSeconds);
        Stats::printLine("Upload throughput (byte/sec)", (unsigned int) (ulAwSizeUpload.load() /elapsedSeconds ));
        Stats::printLine("Download throughput (byte/sec)", (unsigned int) (ulAwSizeDownload.load() /elapsedSeconds));
        Stats::printLine("Average time transfer (sec)", (double) (dTimeTransfer.load()/ulAwSuccess.load()));
    }
}

bool Stats::isFinished() const {
    return finished;
}

Stats::Stats(Options *options) : options(options) {
    ulSuccess = 0;
    ulErrorCurl = 0;
    ulErrorHttp = 0;
    ulSizeUpload = 0;
    ulSizeDownload = 0;
    dTimeTransfer = 0;
    ulAwSuccess = 0;
    ulAwErrorCurl = 0;
    ulAwErrorHttp = 0;
    ulAwSizeUpload = 0;
    ulAwSizeDownload = 0;
    dAwTimeTransfer = 0;
}

void Stats::count(int success, int errorCurl, int errorHttp, unsigned long sizeUpload, unsigned long sizeDownload,
                  double timeTransfer) {

    // TODO: use lock guard to make entire method safe

//    cout << "count: " << success << " " << errorCurl << " " << errorHttp << " " << sizeUpload << " " << sizeDownload << " " << timeTransfer << endl;

    ulSuccess.operator+=(success);
    ulErrorCurl.operator+=(errorCurl);
    ulErrorHttp.operator+=(errorHttp);
    ulSizeUpload.operator+=(sizeUpload);
    ulSizeDownload.operator+=(sizeDownload);
    add_to_atomic_double(&dTimeTransfer, timeTransfer);


    if ((chrono::steady_clock::now() - clock_start).count() * chrono::steady_clock::period::num /static_cast<double>(chrono::steady_clock::period::den) > options->iWarmupSec ) {
        ulAwSuccess.operator+=(success);
        ulAwErrorCurl.operator+=(errorCurl);
        ulAwErrorHttp.operator+=(errorHttp);
        ulAwSizeUpload.operator+=(sizeUpload);
        ulAwSizeDownload.operator+=(sizeDownload);
        add_to_atomic_double(&dAwTimeTransfer, timeTransfer);
    }

    if ((ulSuccess + ulErrorCurl + ulErrorHttp) == options->number_of_recurrence * options->number_of_threads ){
        clock_end = chrono::steady_clock::now();
        finished = true;
    }
}

void Stats::printLine(string strOtion, unsigned int value) {
    cout << setw(35) << right << strOtion << ": " << setw(iResultWidth) << right << value << endl;
}

void Stats::printLine(string strOtion, string value) {
    cout << setw(35) << right << strOtion << ": " << value << endl;
}

void Stats::printLine(string strOtion, bool value) {
    if (value){
        cout << setw(35) << right << strOtion << ": " << "true" << endl;
    }else{
        cout << setw(35) << right << strOtion << ": " << "false" << endl;
    }
}

void Stats::printLine(string strOtion, double value) {
    cout << setw(35) << right << strOtion << ": " << setw(iResultWidth) << right << fixed << setprecision(5) << value << endl;
}

void Stats::showProgressHeader() {
    cout << setw(24) << left << "Timestamp" << " "
         << setw(iProgressWidth) << right << "Success" << " " << setw(iProgressWidth) << "Fail" << setw(iProgressWidth) << "HTTP>400"
         << setw(iProgressWidth) << "Upload(byte)" << setw(iProgressWidth) << "Download(byte)" << setw(iProgressWidth) << "Avg Response" << endl;
}