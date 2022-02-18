# Ibmi_shutdown_econnreset_demo

Created by: Xu Meng <mengxumx@cn.ibm.com>

`gcc -o server server.c && gcc -o client client.c`

In one terminal, call `./server`

In another terminal, call `./client`

Expected Result:

server--
```
$ ./server
send(103) total sent (103)
send(103) total sent (206)
send(103) total sent (309)
send(103) total sent (412)
send(103) total sent (515)
send(103) total sent (618)
send(103) total sent (721)
send(103) total sent (824)
send(103) total sent (927)
send(97) total sent (1024)
recv(74) total recv (74)
recv(74) total recv (148)
recv(74) total recv (222)
recv(74) total recv (296)
recv(74) total recv (370)
recv(74) total recv (444)
recv(74) total recv (518)
recv(74) total recv (592)
recv(74) total recv (666)
recv(74) total recv (740)
recv(74) total recv (814)
recv(74) total recv (888)
recv(74) total recv (962)
recv(62) total recv (1024)
```
client --
```
$ ./client
send(103) total sent (103)
send(103) total sent (206)
send(103) total sent (309)
send(103) total sent (412)
send(103) total sent (515)
send(103) total sent (618)
send(103) total sent (721)
send(103) total sent (824)
send(103) total sent (927)
send(97) total sent (1024)
recv(74) total recv (74)
recv(74) total recv (148)
recv(74) total recv (222)
recv(74) total recv (296)
recv(74) total recv (370)
recv(74) total recv (444)
recv(74) total recv (518)
recv(74) total recv (592)
recv(74) total recv (666)
recv(74) total recv (740)
recv(74) total recv (814)
recv(74) total recv (888)
recv(74) total recv (962)
recv(62) total recv (1024)
```

For those platforms on which the socket API shutdown() work differently, below error happens --
```
$ ./client
send(103) total sent (103)
send(103) total sent (206)
send(103) total sent (309)
send(103) total sent (412)
send(103) total sent (515)
send(103) total sent (618)
send(103) total sent (721)
send(103) total sent (824)
send(103) total sent (927)
send(97) total sent (1024)
recv() failed: Connection reset by peer
```
