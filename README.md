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
This will create an executable named server in the current directory. If you have GNU Make installed, you can alternatively run make to build the server.

# Usage
To start the server, run the following command in your terminal:
```
./server
```
The server will listen for incoming connections on port 8080. You can interact with the server using any HTTP client, such as curl, a web browser, or a custom client.

# Example Requests
  1. GET /ping
  
    curl -X GET http://localhost:8080/ping
  2. GET /echo
  ```
  curl -X GET -H "Custom-Header: MyValue" http://localhost:8080/echo
  ```
  3. POST /write
  ```
  curl -X POST -H "Content-Type: text/plain" -d "Hello, World!" http://localhost:8080/write
  ```
  4. GET /read
  ```
  curl -X GET http://localhost:8080/read
  ```
  5. GET /stats
  ```
  curl -X GET http://localhost:8080/stats
  ```
# Limitations
  1. This server is not designed for production use and may not be secure or efficient
  2. The server does not support HTTP/2 or any other protocol except for HTTP/1.1
  3. Error handling and logging are minimal
  4. The server does not support HTTPS (TLS/SSL)  
  
# Contributing
Contributions to this project are welcome! If you'd like to make improvements, fix bugs, or add new features, please follow these steps:

  1. Fork this repository on GitHub
  2. Create a new branch for your changes
  3. Make your changes, ensuring that your code follows the project's style and is well-documented
  4. Test your changes to ensure they work as expected and do not introduce new bugs
  5. Submit a pull request to the original repository with a clear and concise description of your changes


# License
This project is licensed under the MIT License. Please see the LICENSE file for more information.

# Support
If you encounter any issues or have questions about the project, feel free to open an issue on GitHub or submit a pull request with your proposed changes. Alternatively, you can contact the repository owner through their GitHub profile.

# Future Work
  1. Enhance error handling and logging for better debugging and monitoring
  2. Add support for additional HTTP methods, such as PUT and DELETE
  3. Implement HTTPS (TLS/SSL) support for secure communication
  4. Optimize server performance for better handling of large numbers of simultaneous connections
  5. Add support for HTTP/2 and other protocols to improve performance and compatibility with modern clients
  6. Create a more extensive test suite to ensure the server's reliability and correctness

