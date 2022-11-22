all: parser

parser: parser.cpp
	g++ -g -no-pie -fstack-protector -o parser parser.cpp 

clean:
	rm parser