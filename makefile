run : kahancha runner.rb
	ruby runner.rb

kahancha : kahancha.cpp
	g++-11 -O2 -std=c++17 kahancha.cpp -o kahancha

