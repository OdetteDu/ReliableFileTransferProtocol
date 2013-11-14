all:
	g++ -o sendfile sendfile.cc MD5.cc send/checkflag.cc send/readfile.cc send/send_small.cc send/send_big.cc
