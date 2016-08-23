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

- [esperf-0.2.1-linux-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.1/esperf-0.2.1-linux-x86_64.zip)
- [esperf-0.2.1-darwin-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.2.1/esperf-0.2.1-darwin-x86_64.zip)

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

Your may alo refer to [ibcurl error codes](https://curl.haxx.se/libcurl/c/libcurl-errors.html) for `curl_easy_perform()` related errors.

## Example output

```
$ esperf -X GET -i 1 -t 100 -r 1000 -d /tmp/dict.txt localhost:9200/_search
  Timestamp                  Success      Fail HTTP>400   Upload Download Response
  ------------------------ --------- --------- -------- -------- -------- --------
  2016-08-23T21:16:42+0900        91         0        0      128    16062   0.5925
  2016-08-23T21:16:43+0900        64         0        0      128    15899   0.4771
  2016-08-23T21:16:44+0900       116         0        0      128    15933   0.2279
  2016-08-23T21:16:45+0900       154         0        0      128    15973   0.3582
  2016-08-23T21:16:46+0900       134         0        2      128    16085   0.1973
  2016-08-23T21:16:47+0900       113         0        1      128    16066   0.0891
  2016-08-23T21:16:48+0900       137         0        0      128    16062   0.0603
  2016-08-23T21:16:49+0900       182         0        5      128    16154   0.1970
  ----------------------------------- Options ------------------------------------
                    Number of threads:             100
                 Number of recurrence:            1000
                       Interval (sec):               1
                        Warm-up (sec):               0
                        Timeout (sec):               0
                           Dictionary: /tmp/dict.txt
                                  URL: localhost:9200/_search
                          HTTP Method: GET
                                 Body: {"query": {"term": {"first_name": {"value": "$RDICT"}}}}
  ----------------------------------- Results ------------------------------------
                     Total time (sec):         7.90934
                    Number of success:             992
         Number of connection failure:               0
         Number of HTTP response >400:               8
      Average successful requests/sec:             125
         Upload throughput (byte/sec):           16053
       Download throughput (byte/sec):         2012180
          Average time transfer (sec):         0.76265
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
