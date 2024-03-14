ft_irc: An IRC Server in C++

This project implements a complete IRC server named ircserv written in C++. It adheres to the IRC protocol defined in RFC 2812, allowing users to connect, chat, and create channels for communication.
Building and Running

This project utilizes a Makefile for building and running the server. The Makefile defines various targets for different functionalities.

Prerequisites:

    A C++ compiler (e.g., g++)

Building:

    Clone the repository:
    git clone https://github.com/lucaslion/irc.git
    cd irc
    make

This will create the ircserv executable in the current directory.

Running:

    ./ircserv <port> <password>
    Replace <port> with the desired port number (usually: 6667).
    Replace <password> with a password for server access.

Additional Targets:

The Makefile provides several additional targets:

    make clean: Removes object files and the build directory.
    make fclean: Removes the executable, object files, and the build directory.
    make re: Rebuilds the project from scratch (equivalent to make fclean all).
    make debug: Builds the executable with AddressSanitizer for memory error detection.
    make leaks: Builds the executable with Valgrind memory leak detection.
    make run: Builds the executable and runs it with the default command.

For a more detailed explanation of the Makefile syntax, refer to the extensive documentation available online.
Usage

Clients compatible with the IRC protocol (RFC 2812) can connect to your server. Use the following command in your IRC client to connect:

This project is licensed under the GPLv3 license. Refer to the LICENSE file for details.
Contributing

We welcome contributions to this project! Feel free to fork the repository, make changes, and submit pull requests.
