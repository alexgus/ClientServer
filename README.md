# ClientServer
Allows to handle commands sended by a client through network thanks to TCP protocol.

You can add more commands from C++ (`CMDLine` class for parsing the commands and options and `ServerComdHanlder` for executing your commands) and Python thanks to the module `server.py`

## Dependencies
`Python3` for running python scripts
`SQLite3`
This programme used too the `inih` parser (http://code.google.com/p/inih/) under new BSD License, already in the project in `res` folder.

## Compilation
Just type `make all` in the Debug folder.

## Configuration
You can configure the program by editing the `SCP.conf`
(soon two files `server.conf` and `client.conf`)

## Run for test
Type `ClientServer` in a terminal in Debug folder once you have compiled the program

## Using the Client and/or Server in your own program

### Include
    include "iniparseur/INIReader.h"
    include "Server.h"
    include "Client.h"
### Global Variable
You have to declare `INIReader g_conf(<TheConfFile>);` in the global scope.
### Client
    client = new Client(<IP>,<PORT>);
    client->run();
The client will continue until the `Bye !` of the server
### Server
    serv = new Server(<PORT>);
    serv->acceptConnection();
For stopping the server :
    serv->stopServer();
