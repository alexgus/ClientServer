# ClientServer
Allows to handle commands sended by a client through network thanks to TCP protocol.

You can add more commands from C++ (`CMDLine` class for parsing the commands and options and `ServerComdHanlder` for executing your commands) and Python thanks to the module `server.py`

## Configuration
You can configure the program by editing the `Server.conf` and `Client.conf` files.

## Dependencies
`Python3` for running python scripts
`SQLite3`
This programme used too the `inih` parser (http://code.google.com/p/inih/) under new BSD License, already in the project in `res` folder.

