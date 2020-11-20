main: main.cpp
	g++ -o main -std=gnu++17 -Wall -g3 -I. main.cpp

run: main
	main



gen: gen.cpp
	g++ -o gen -Ofast -std=gnu++17 -I. gen.cpp





# end
