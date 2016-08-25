# esperf - Elasticsearch Performance Testing Tool 

`esperf` is a multi-thread program designed for measuring Elasticsearch cluster's search, aggregation and other request capacity.
It reads the query DSL from the standard input and performs HTTP requests as the request body to the specified URL.
It is also able to modify the query string with random numbers and random strings in each request.

Usage: `esperf [-d dictionary_file] [-o omit_secs] [-r recurrence] [-t threads] [-X method] [-u user:password] url`  
Options:  
- `-d dictionary_file`: Newline delimited strings dictionary file 
- `-h`: Show this help
- `-r recurrence`: Number of recurrence HTTP requests per thread (default 1)
- `-t threads`: Number of threads to generate, not always a big number gives more pressure (default 1)
- `-u user:password`: Username and password for HTTP authentication 
- `-v`: Verbose outputs for debugging purpose
- `-w warm-up`: `warm-up` seconds to omit from the statistics (default 0)
- `-T timeout`: Maximum `timeout` seconds to transfer completion (default 0 - unlimited)
- `-X`: HTTP method to perform (default GET)

`libcurl` is necessary to be installed on the local system. `sudo yum install libcurl` or `sudo apt-get install libcurl4-openssl-dev` to install.

## Downloads

- [esperf-0.2.2-linux-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.2/esperf-0.2.2-linux-x86_64.zip)
- [esperf-0.2.2-darwin-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.2/esperf-0.2.2-darwin-x86_64.zip)

## Command line usage examples

Simply retrieve the greeting message.

    $ ./esperf "http://localhost:9200"

Perform a match_all query 1000x3 times on all shards from 3 threads.

    $ echo '{"query": {"match_all": {}}}' | ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform range queries with randomly generated numbers (0 to 99).

    $ echo '{"query": {"range": {"my_length": {"gte": $RNUM(100)}}}}' |  ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform term queries with randomly selected strings from the dictionary.
    
    $ echo '{"query": {"term": {"first_name": {"value": "$RDICT"}}}}' | ./esperf -r 1000 -t 3 -d ./names.txt "http://localhost:9200/_search?size=1"

Perform bulk insert requests.

    $ ./esperf -X PUT -r 1000 -t 3 -d ./names.txt "http://localhost:9200/test-index/test-type/_bulk" < bulk.txt

You may aloso refer to [ibcurl error codes](https://curl.haxx.se/libcurl/c/libcurl-errors.html) for `curl_easy_perform()` related errors.

## Example output

```
$ echo '{"query": {"term": {"first_name": {"value": "$RDICT"}}}}' | ./esperf -t 10 -r 10000 -w 1 -d ./dict.txt localhost:9200/_search
Timestamp                  Success      Fail HTTP>400   Upload Download Response
------------------------ --------- --------- -------- -------- -------- --------
2016-08-23T21:51:39+0900      3061         0        0      186      211   0.0032
2016-08-23T21:51:40+0900      3182         0        0      186      211   0.0031
2016-08-23T21:51:41+0900      3757         0        0      186      211   0.0026
----------------------------------- Options ------------------------------------
                  Number of threads:              10
               Number of recurrence:           10000
                     Interval (sec):               1
                      Warm-up (sec):               1
                      Timeout (sec):               0
                         Dictionary: ./dict.txt
                                URL: localhost:9200/_search
                        HTTP Method: GET
                               Body: {"query": {"term": {"first_name": {"value": "$RDICT"}}}}

----------------------------------- Results ------------------------------------
           Time after warm-up (sec):         2.00114
                  Number of success:            6946
       Number of connection failure:               0
       Number of HTTP response >400:               0
    Average successful requests/sec:            3471
       Upload throughput (byte/sec):          645610
     Download throughput (byte/sec):          732470
        Average time transfer (sec):         0.00281
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
