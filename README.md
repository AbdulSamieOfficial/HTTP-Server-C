# HTTP Server in C
This repository contains a simple multithreaded HTTP server implemented in C. The server supports basic HTTP methods (GET and POST) and provides endpoints for testing and statistics gathering.

# Features
Multithreaded handling of client connections
GET and POST methods supported

# Built-in endpoints:
  1. /ping: Responds with "pong"
  2. /echo: Responds with request headers
  3. /write: Writes POST request body to the server
  4. /read: Reads the data written to the server via /write endpoint
  5. /stats: Shows server statistics, including the number of requests, header bytes, body bytes, errors, and error bytes
  
# Prerequisites
  1. GCC (GNU Compiler Collection) or any other C compiler
  2. GNU Make (optional)  

# Compilation
To compile the server, run the following command in your terminal:
```
gcc -o server server.c -lpthread
```



