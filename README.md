# esperf - Elasticsearch Performance Testing Tool 

`esperf` is a multi-thread program designed for measuring Elasticsearch cluster's search, aggregation and other request capacity.
It reads the query DSL from the standard input and performs HTTP requests as the request body to the specified URL.
It is also able to modify the query string with random numbers and random strings in each request.

Usage: `esperf [-d dictionary_file] [-o omit_secs] [-r recurrence] [-t threads] [-X method] [-u user:password] url`  
Options:  
- `-d dictionary_file`: Newline delimited strings dictionary file 
- `-h`: Show this help
- `-r recurrence`: Number of recurrence HTTP requests per thread (default 10)
- `-w warm-up`: `warm-up` seconds to omit from the statistics (default 0)
- `-t threads`: Number of threads to generate, not always a big number gives more pressure (default 5)
- `-u user:password`: Username and password for HTTP authentication 
- `-v`: Verbose outputs for debugging purpose
- `-X`: HTTP method to perform (default GET)

## Downloads

- [esperf-0.2.0-linux-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.0/esperf-0.2.0-linux-x86_64.zip)
- [esperf-0.2.0-darwin-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.0/esperf-0.2.0-darwin-x86_64.zip)

## Command line usage examples

Simply retrieve the greeting message 50 times.

    $ ./esperf "http://localhost:9200"

Perform a match_all query 1000x3 times on all shards from 3 threads.

    $ echo '{"query": {"match_all": {}}}' | ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform range queries with randomly generated numbers (0 to 99).

    $ echo '{"query": {"range": {"my_length": {"gte": $RNUM(100)}}}}' |  ./esperf -r 1000 -t 3 "http://localhost:9200/_search"

Perform term queries with randomly selected strings from the dictionary.
    
    $ echo '{"query": {"term": {"first_name": {"value": "$RDICT"}}}}' | ./esperf -r 1000 -t 3 -d ./names.txt "http://localhost:9200/_search?size=1"

Perform bulk insert requests.

    $ ./esperf -X PUT -r 1000 -t 3 -d ./names.txt "http://localhost:9200/test-index/test-type/_bulk" < bulk.txt

Your may alo refer to [ibcurl error codes](https://curl.haxx.se/libcurl/c/libcurl-errors.html) for `curl_easy_perform()` related errors.

## Example output

```
$ .esperf -X GET -i 1 -w 2 -t 5 -r 3000 -d ./dict.txt localhost:9200/_search < ./body.txt
Timestamp                  Success      Fail HTTP>400   Upload Download Response
------------------------ --------- --------- -------- -------- -------- --------
2016-08-20T14:52:07+0900      3098         0        0      184      211   0.0015
2016-08-20T14:52:08+0900      4149         0        0      183      211   0.0006
2016-08-20T14:52:09+0900      2960         0        0      183      211   0.0005
2016-08-20T14:52:10+0900      4133         0        0      184      211   0.0003
2016-08-20T14:52:11+0900       658         0        0      183      211   0.0001
----------------------------------- Options ------------------------------------
                  Number of threads:               5
               Number of recurrence:            3000
                     Interval (sec):               1
                      Warm-up (sec):               2
                         Dictionary: ./dict.txt
                                URL: localhost:9200/_search
                        HTTP Method: GET
                               Body: {"query": {"term": {"first_name": {"value": "$RDICT"}}}}

----------------------------------- Results ------------------------------------
                   Total time (sec):         4.25686
                  Number of success:            7776
       Number of connection failure:               0
       Number of HTTP response >400:               0
    Average successful requests/sec:            1826
       Upload throughput (byte/sec):          336118
     Download throughput (byte/sec):          385444
        Average time transfer (sec):         0.00256
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
