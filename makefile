run : kahancha runner.rb
	ruby runner.rb

kahancha : kahancha.cpp makefile
	g++-11 -Wall -O2 -std=c++17 kahancha.cpp -o kahancha

