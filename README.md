#Course: AR305 - Distributed Systems of Automation


> Building a Server which whill host a webpage: index.html
> on the IP_ADDRESS:PORT
> allow clients to acces the webpage through the ip address and port


compiling and building the server:
```sh
$ gcc -o server server.c
```
running the server:
```sh
$ ./server index.html
```

compiling and building the client (open another terminal):
```sh
$ gcc -o client client.c
```
running the client with the provided IP_ADDRESS:PORT from server:
```sh
$ ./client IP_ADDRESS:PORT
```
example:
```sh
$ ./client 192.168.1.5:8080
``
