# Non blocking soket programing
* Raw socket
* udp
* libev

## build
* Please make sure you have libev installed.

```sh
# build server by make file
$ make

#build client by command line
$ gcc -o rawudp rawudpclient.c
```

## Run
```sh
# input port number
$ sudo ./server 8888 

# input server [IP] [port] [data byte] [time to test] [spoofed IP]
$ sudo ./rawudp you.server.ip.here port 1500 10 8.8.8.8
```


## clean
```sh
#delete server
$ make clean

#delete client
rm rawudp
```


