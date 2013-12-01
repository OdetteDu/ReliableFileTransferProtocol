all:
	g++ -pthread -o sendfile sendfile.cc MD5.cc send/checkflag.cc send/readfile.cc send/send_small.cc send/send_big.cc
	g++ -o recvfile recvfile.cc MD5.cc recv/checkflag.cc recv/recv.cc recv/recv_big.cc
