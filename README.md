# ping_pong_server
Building a simple server, which responds with 2 'pong' for every 'ping' command

Usage:

Open 2 terminal windows, do 'make' in one of them to compile the ./server executable
In the other one do 'gcc client.c -o client' to comiple the client part

In sever window start server with './server *address*'

In client window start client with './client localhost *the same address*'

Send messages from client to host, receive 2 'pong' for every 'ping' command. Any other command except 'ping' will be repeated.
