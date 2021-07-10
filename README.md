# Sockets C

Two simple programs to demonstrate how socket work by creating and connecting server and client

## Requirements

* Unix Based System
* GCC C Compiler

## Simple Example

The first example is just to demonstrate how to create and connect sockets, first we create our server socket, and then our client socket. Our server socket keeps listening to income connection from clients

### How to run the example

1 - After cloning the repository go to the `simple_example` folder

2 - Compile the server and the client files

**server.c**

```shell
gcc server.c -o ./build/server
```

**client.c**

```shell
gcc client.c -o ./build/client
```

3 - Run the compiled files, but remember, the client needs a server, so always run first the server, otherwise a error will occur

**server**

```shell
./build/server
```

**client**

```shell
./build/server
```

When connected a message will be sent from the server to the client, and when the client recives a message, will send a response to the server

## One To One Chat

This example is a chat-like application, where is possible to exchange messages between the client and the server. We use the select function to do a async request, this makes possible to send a message and at the same time print if a message is recived

### How to run the example

1 - After clonning the repository go to the `one_to_one_chat` folder

2 - Compile the server and the client files

**server.c**

```shell
gcc server.c -o ./build/server
```

**client.c**

```shell
gcc client.c -o ./build/client
```

3 - Run the compiled files, but remember, the client needs a server, so always run first the server, otherwise a error will occur

**server**

```shell
./build/server
```

**client**

```shell
./build/server
```

when connected will be possible to exchange messages between the server and the client