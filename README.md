
<h3 align="center" style="color:red;">!🤖 SERVER CURRENTLY STILL IN DEVELOPMENT - WORK IN PROGRESS 🤖!</h3>

<h1 align="center">Internet Relay Chat Server (IRC)</h1>

> Hive Helsinki (School 42) 13th project (Rank05/06) of core curriculum.

<h2 align="center">
	<a href="#about">About</a>
	<span> · </span>
	<a href="#program">Program</a>
	<span> · </span>
	<a href="#requirements">Requirements</a>
	<span> · </span>
	<a href="#instructions">Instructions</a>
</h2>

## About
The repository contains a custom made `Internet Relay Chat server (IRC)` to which connection can be established via the `IRSSI` client. Programming is done in collaboration with:  
- Timo Saari (https://github.com/LandSurroundedByWater)  
- Matti Rinkinen (https://github.com/MRinkinen)

## Program
The server utelises C++ socket programming in order to create a TCP socket which operates over IP4. The server binds to the socket and furhter listens to incoming connection requests of clients. 
The following user commands are handled from the server:
 - JOIN
 - INVITE
 - KICK
 - TOPIC
 - MODE
   - +t (only channel operators can set channel topic)
   - +i (join a channel only possible with an invitation)
   - +k (sets password for the channel)
   - +l (sets user limit for the channel)
   - +o (assigns operator role to a channel member)

## Requirements
- `c++` compiler
- `macOS` or `linux based OS` with `irrsi` chat client

## Instructions

### 1. Compiling the program

To compile the files run the following command in the repository folder in the terminal:

```
$ make 
```

### 2. Launch the server

The server can be launched by passing a `port_number` and `password` to the executable, e.g.
```
$ ./ircserv 9090 1234
```
For connecting a client, open another terminal window and type in the following:
```
$ irssi
```
Establish a connection to the server within irssi with the following command (port number used from example above):
```
$ /connect localhost 9090
```

### 3. Navigation within the IRC
`JOIN` a channel:
```
/join <made_up_channel>
```
set `MODE` (if channel operator)
```
/mode +t
/mode +i
/mode +k <channel_password>
/mode +l <channel_user_limit>
/mode +o <channel_member>
```
set `TOPIC` of a channel
```
/topic <channel_topic>
```

`KICK` users from channels (if channel operator)
```
/kick <channel_member>
```
