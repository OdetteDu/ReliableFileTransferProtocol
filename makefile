all:
	g++ -o sendfile MD5.cc sendfile.cc
	g++ -o recvfile MD5.cc recvfile.cc
