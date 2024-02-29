# UDP Echo Server and Client

This project consists of an UDP Echo Server and Client implemented in C language.

## Overview

The UDP Echo Server is configured to listen on a specific IPv4 address and port number, allowing it to handle incoming UDP messages from clients and echo them back. 
Clients have the option to manually set the IPv4 address, port number, and maximum retry time. After sending a message to the server, the client waits for the echoed response. If a timeout occurs, the message is resent.

### Prerequisites

- Docker
- [Dev Containers](https://marketplace.visualstudio.com/items?itemName=ms-vscode-remote.remote-containers)

### Usage

First, open Visual Studio Code, then click on the bottom-left button and select _Reopen in container_.
Next, Compile the server and client code using the following command:

```bash
gcc -o server server.c udp.c
gcc -o client client.c udp.c
```

Run the executable file, and then follow the instructions provided.
