# Webserv 🏄‍♂️

This 42 project is about writing your own **HyperText Transfer Protocol** server in C++.

## Summary

## 1. [Introduction](#Introduction)
## 2. [Usage](#Usage)
## 3. [Fundamentals](#Fundamentals)
  ### 3.1 [Core](#Server Core)
  ### 3.2 [Configuration](#Server Setup/Configuration)
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

### Server Setup/Configuration
Our webserver was required to run multiple (virtual) webservers on different IP Adresses and Ports. To run multipple virtual servers, include multiple server{} sections.
Additionally the user can apply many customizations to the webserver via the config file. It's format is close to NGINX config file.
The following configurations can be made:

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

#### CGI
If the user wants to include a CGI, this has to be specified as a location. For example:
```
 location .php {
		allow_methods GET POST;
		root ./cgi-bin;
		cgi_path usr/bin/php;
	}
```
```
	location .py {
		allow_methods GET POST;
		root ./cgi-bin;
		cgi_path usr/bin/php;
	}
```
where the user can specify the path of the cgi-program (cgi-path), as well as from which folder a script will be executed (root).

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











