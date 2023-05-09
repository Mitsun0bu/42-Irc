# 42-Irc

The ft_IRC project is a 42 School project consisting in developping a simple IRC server implemented in C++98. It supports multiple clients and basic IRC commands, such as authentication, setting nicknames and usernames, joining channels, sending and receiving private messages, and handling operators. The server does not support server-to-server communication or a dedicated client.

Getting Started
These instructions will help you compile and run the ft_IRC server on your local machine.

Prerequisites
A C++98 compatible compiler (e.g., GCC or Clang)
GNU Make
Compiling the server
Clone the repository:
bash
Copy code
git clone https://github.com/Mitsun0bu/42-Irc.git
Navigate to the project directory:
bash
Copy code
cd 42-Irc
Compile the server using Make:
go
Copy code
make
Running the server
To start the ft_IRC server, use the following command:

php
Copy code
./ircserv <port> <password>
<port>: The port number on which the IRC server will listen for incoming IRC connections.
<password>: The connection password, which will be required by any IRC client trying to connect to the server.
For example:

bash
Copy code
./ircserv 6667 mypassword
Using the server
Choose an IRC client as a reference (e.g., HexChat, mIRC, or Irssi).

Configure your IRC client to connect to the ft_IRC server using the IP address or hostname of the machine running the server, along with the port number and password specified when starting the server.

Once connected, you can use your IRC client to authenticate, set a nickname and username, join channels, send and receive private messages, and manage operators.

License
This project is licensed under the MIT License. See the LICENSE file for details.
