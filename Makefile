default: myProgram myProgram2

myProgram: compress.cpp
	g++ nodes.cpp compress.cpp -o compress -std=c++0x -g

myProgram2: uncompress.cpp
	g++ nodes.cpp uncompress.cpp -o uncompress -std=c++0x -g

clean:
	rm -f compress *~
	rm -f uncompress *~