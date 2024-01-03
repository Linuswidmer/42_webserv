# Webserv 🏄‍♂️

This 42 project is about writing your own **HyperText Transfer Protocol** server in C++.

## Summary

## 1. [Introduction](#Introduction)
## 2. [Usage](#Usage)
## 3. [Fundamentals](#Fundamentals)
  ### 3.1 [Core](#Server-Core)
  ### 3.2 [Configuration](#Server-setup/Configuration)
  ### 3.3 [CGI](#CGI)

## Introduction

The primary function of a web server is to store, process, and deliver web pages to clients. The communication between client and server takes place using HTTP requests, such as GET, POST or DELETE to name just a few.
A user agent, commonly a web browser, initiates communication by requesting a specific resource via HTTP and the server responds with the content of that resource or an error message if unable to do so (i.e. Error 404 for when the resource doesn't exist).

Here is an example of what happens after launching our webserv and entering `http://127.0.0.1:18000` in Firefox:

![webserv1](https://github.com/flo-12/webserv/assets/104844198/9c1f68dc-f9a8-4955-9091-52c420f6d52d)

In the console (`F12`), we can see the 4 GET requests sent by the browser to the server:

![webserv2](https://github.com/flo-12/webserv/assets/104844198/814d6989-2e4b-4a12-9464-1e3519c83e6a)

And the response from the server (HTML, CSS, and JS):

![webserv3](https://github.com/flo-12/webserv/assets/104844198/ed59562e-44a0-4e07-b209-092bb264ed82)

## Usage

Warning: `Makefile` is configured for `Linux` use only.

> - Compile with `make`
> - Then run with `./webserv` + (optional) `CONFIGFILE` | Example: `./webserv myserver.conf`
> - Running the program with no argument will load `default.conf` by default
> - Access the server through a browser at address `http://127.0.0.1:18000`
> - To shut the server down, use `Ctrl` + `C`

## Fundamentals

### Server Core
Building a web server involves two pivotal concepts: **robustness** and **efficiency**.

Firstly, robustness is paramount to prevent clients from unintentionally compromising the web server with erroneous requests. Protecting against such incidents is crucial to safeguard personal data and ensure consistent server availability. Throughout the project, we diligently integrated this concept, notably in the realm of HTTP connection management. As depicted here, we adopted the paradigm of short-lived connections, terminating the connection to the client after each request. While this incurs a minor performance cost, it fortifies security by preventing prolonged waits for client requests. This strategy demonstrated its efficacy, achieving an impressive 100% accuracy in benchmarks using siege.

Secondly, a web server must efficiently handle numerous concurrent requests from diverse clients, minimizing the time spent waiting for responses. This necessitates the incorporation of non-blocking commands and I/O Multiplexing, implemented through the utilization of the poll function. I/O Multiplexing enables the simultaneous monitoring of multiple file descriptors for reading data (client requests) and efficiently reading from each without waiting for one to finish before proceeding to the next. This crucial concept, exemplified by its application in modern server architectures, ensures optimal performance in today's dynamic web environments.

### Server Setup/Configuration
Upon launching the webserver reads in a specified configuration file or a default configration file, if none was provided. 
This configuration file lets the user control many important aspects of the servers runtime, such as its IP and port. 
But also the "locations" are an important part that allow us to specify specific operations for paths/files requested by the client.
The following two tables list and explain all configurations that can be made.

#### General configuration

|Key|Example| Description|Default value|
|-|-|-|-|
|listen| 8000 | Defines a port. Multiple ports have to be separated by spaces | Mandatory
|host|127.0.0.1|Defines the host IP|Mandatory|
|root|www/|Defines the default directory where to search for files|Mandatory|
|server_name|my_website|Gives an internal name to the server|server_{1}|
|client_max_body_size|1000000|sets the maximum size for the body of the client|300000|
|error_page|404 fun/404.html (405 fun2/405.html)|sets the default path for an error page|error_page/xxx.html|
|location|/ {OPTIONS}|One or more routes can be specified, that configure options for requests to that path|location for “/”|

#### Locations
Locations provide a way of setting secific options for request paths by the client such as redirections, allowing methods etc...
|Key|Example| Description|Default value|
|-|-|-|-|
|allow_methods|GET POST DELETE|Set the allowed method for a client request|GET POST DELETE|
|autoindex|on|autoindex directive processes requests ending with the slash character (‘/’) and produces a directory listing|off|
|return|/root|redirection of request|none|
|index|page.html|set a default file to answer if the request is a directory (mutually exclusive to autoindex)|none|

Generally the configuration logic is very close to NGINX's keywords and functionalities. An example of a configuration file can be found at /configs.

### CGI

CGI stands for **Common Gateway Interface**. Thanks to this interface, a web server can execute an external
program and return the result of that execution. For example, if you click on the CGI button of our web page,
the address will be `http://127.0.0.1:18000/index.php`. You guessed it, the client wouldn't know what to make of
`.php` files by default, and this is why we need a CGI here.

In order to execute a PHP file, we need the **PHP executable** usually located at `/usr/bin/php` on Linux.
The job of the CGI is to execute the PHP file and retrieve the result of that execution. 

In our website, we use PHP to execute a C++ program called **RPN** (Reverse Polish Notation, mathematical notation
in which operators follow their operands, ex: `4 4 5 + +`). After entering an operation in the submit field, a POST
request containing the string is sent to the server. The content of the string is passed as argument to the RPN
program, and the result of the execution gets embedded in an HTML body thanks to PHP. The resulting HTML response
can then be sent to the browser, interpreted, and displayed as a page:

![webserv4](https://github.com/flo-12/webserv/assets/104844198/cfabf106-b79e-4483-a1a4-f3342aba379c)

This project was done in collaboration by **[flo-12](https://github.com/flo-12)**, **[dubmix](https://github.com/dubmix)** and **[Linuswidmer](https://github.com/Linuswidmer)**.











