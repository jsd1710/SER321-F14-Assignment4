# README #

This project is to demonstrate the understanding of jsonrpc and jsoncpp in C++ and Java. More details found at [Professor Lindquist's Assignment Page](http://pooh.poly.asu.edu/Cst420/Assigns/Assign4/assign4.html).


### Dependencies (FEDORA) ###
* sudo yum install curlpp-devel.x86_64
* sudo ln -s /usr/local/lib/libjsonrpc.so. /lib64/libjsonrpc.so.      //This is to make the jsonrpc file from the MAKEFILE visible to the Fedora Linker

### Dependencies (OTHER) ###
* [antlibs.jar](http://pooh.poly.asu.edu/Cst420/Resources/antlibs.jar) extracted to home dir
* g++
* gcc
* libcurl4-openssl-dev
* cmake
* ant-contrib
* ant-contrib-cpptasks
* Make jsonrpc files from this [tutorial]
(http://pooh.poly.asu.edu/Cst420/Resources/setupJsonJavaNJsonRPCcpp.html).
* libjsoncpp-dev