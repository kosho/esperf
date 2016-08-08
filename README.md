# esperf - Elasticsearch Stress Testing Tool 

`esperf` is a multi-thread program designed for measuring Elasticsearch cluster's search/aggregation capability.
It reads the query DSL from the standard input and performs HTTP requests as the request body to the specified URL.
It is also able to modify the query string with random numbers and random strings in each request.

Usage: `esperf [-d dictionary_file] [-r max_reoccurrence] [-t num_threads] [-X method] [-u user:password] url`
Options:
- `-d dictionary_file`: Newline delimited strings dictionary file 
- `-h`: Show this help
- `-r max_recurrence`: Number of recurrence HTTP requests per thread
- `-t num_threads`: Number of threads to generate, not always a big number gives more pressure
- `-u user:password`: Username and password for HTTP authentication 
- `-v`: Verbose outputs for debugging purpose
- `-X`: HTTP method to perform

## Output format

    <ISO 8601 formatted timestamp> <number of successful requests/sec> <number of failures/sec>

## Example usages

    $ ./esperf "http://localhost:9200"

    $ echo '{"query": {"match_all": {}}}' | ./esperf -r 1000 -t 3 "http://localhost:9200/_search"
    
    $ echo '{"query": {"range": {"my_length": {"gte": $RNUM}}}}' |  ./esperf -r 1000 -t 3 "http://localhost:9200/_search"
    
    $ echo '{"query": {"term": {"first_name": {"value": "$RDICT"}}}}' | ./esperf -r 1000 -t 3 -d ./names.txt "http://localhost:9200/_search?size=1"

Your may alo refer to [ibcurl error codes](https://curl.haxx.se/libcurl/c/libcurl-errors.html) for `curl_easy_perform()` related errors.

## Downloads

- [esperf-0.1.1-linux-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.1.0/esperf-0.1.1-darwin-x86_64.zip)
- [esperf-0.1.1-darwin-x86_64.zip](https://github.com/kosho/esperf/releases/download/0.1.0/esperf-0.1.1-linux-x86_64.zip)

## How to build

### Platforms

The program is confirmed built and run on the following operating systems.

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
