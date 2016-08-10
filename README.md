# esperf - Elasticsearch Stress Testing Tool 

`esperf` is a multi-thread program designed for measuring Elasticsearch cluster's search, aggregation and other request capacity.
It reads the query DSL from the standard input and performs HTTP requests as the request body to the specified URL.
It is also able to modify the query string with random numbers and random strings in each request.

Usage: `esperf [-d dictionary_file] [-o omit_secs] [-r recurrence] [-t threads] [-X method] [-u user:password] url`
Options:
- `-d dictionary_file`: Newline delimited strings dictionary file 
- `-h`: Show this help
- `-r recurrence`: Number of recurrence HTTP requests per thread (default 10)
- `-o omit_secs`: Omit first <omit_secs> seconds from the statistics 
- `-t threads`: Number of threads to generate, not always a big number gives more pressure (default 5)
- `-u user:password`: Username and password for HTTP authentication 
- `-v`: Verbose outputs for debugging purpose
- `-X`: HTTP method to perform

## Downloads

- [esperf-0.1.4-linux-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.1.4/esperf-0.1.4-linux-x86_64.zip)
- [esperf-0.1.4-darwin-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.1.4/esperf-0.1.4-darwin-x86_64.zip)

## Command line usage examples

Simply retrieve the greeting message 50 times.

    $ ./esperf "http://localhost:9200"

Perform a match_all query 1000x3 times on all shards from 3 threads.

    $ echo '{"query": {"match_all": {}}}' | ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform range queries with randomly generated numbers.

    $ echo '{"query": {"range": {"my_length": {"gte": $RNUM}}}}' |  ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform term queries with randomly selected strings from the dictionary.
    
    $ echo '{"query": {"term": {"first_name": {"value": "$RDICT"}}}}' | ./esperf -r 1000 -t 3 -d ./names.txt "http://localhost:9200/_search?size=1"

Perform bulk insert requests.

    $ ./esperf -X PUT -r 1000 -t 3 -d ./names.txt "http://localhost:9200/test-index/test-type/_bulk" < bulk.txt

Your may alo refer to [ibcurl error codes](https://curl.haxx.se/libcurl/c/libcurl-errors.html) for `curl_easy_perform()` related errors.

## Example output

```
$ ./esperf -X GET -o 1 -t 5 -r 100 -d dict.txt localhost:9200/_search < body.txt
    timestamp                    success   conn_fail  http_error
    2016-08-10T21:49:52+0900         111           0           0
    2016-08-10T21:49:53+0900          90           0           0
    2016-08-10T21:49:54+0900         100           0           0
    2016-08-10T21:49:55+0900         127           0           0
    2016-08-10T21:49:56+0900          72           0           0
    
    Finished.
    
    URL:                               localhost:9200/_search
    Method:                            GET
    Input from stdin (byte):           72
    Dictionary:                        dict.txt
    First n secs to omit:              1
    
    Number of threads:                          5
    Number of recurrence/thread:              100
    Number of successful requests:            500
    Number of connection failures:              0
    Number of HTTP responses >400:              0
    
    Time taken (sec):                       4.589
    Number of requests to measure:            389
    Average successful requests/sec:      108.387
    Upload throughput (byte/sec):           10850
    Download throughput (byte/sec):       1361461
```

## How to build

### Platforms

The program is tested built and run on the following operating systems.

- Linux x86_64
- Darwin x86_64 (OS X)

### Dependencies

- cmake > 2.8
- gcc-g++ > 4.8
- libcurl-devel > 7.40

### Make

Simply run the below commands to generate the executable on the target platform.

    $ cmake ./CMakeList.txt
    $ make

## TODO:

- Provide more variations of random numbers and the geo type
