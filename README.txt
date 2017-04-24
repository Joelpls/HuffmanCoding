This is a program that will compress and uncompress text files in ascii.

It works by using Huffman coding to save a compressed version of the text file you feed it. 

To run it:
*run the make file
*compress usage:
	./compress infile.txt outfile.joel
*uncompress usage:
	./uncompress infile.joel outfile.txt
*The file must be in the same directory as the program

It doesn't always work on every .txt file. Long files won't uncompress correctly. I would like to make it work for all txt files in the future, but getting this project to where it is now was tough enough.

I have included some text files for you to compress and uncompress.

Warning: Trying to compress small text files (like a file with only 1 paragraph) will result in compressed files larger than their originals.


Testing:

 Try it with the .txt files I've included in the file. 
 run: ./compress TaleOfTwoCities.txt TwoCities.joel
 (you can put any name for the output file in these programs.)
  This should compress it from 5777 bytes to 3434 bytes

 Uncompress it now.
 run: ./uncompress TwoCities.joel TC.txt
  TC.txt and TaleOfTwoCities.txt should be exactly the same

You can also try it on the README or copy and paste a paper you have written into Notepad and save it as a .txt file.



The code:

compress.cpp: 

 It takes two arguements. The file to compress and the file name for the final compressed file.
 First, all the ascii in the txt file are read and a count of each character is made in an array called asciiArray.

 Then nodes for the Huffman Tree are made out of these counts of chars. 
 I created a Node class that holds information of a Node (see below). The code loops through the asciiArray creating Nodes and puts them into a Node vector.
 A psuedo end of file (EOF) node character is also created. This psuedo-EOF is written to the end of the compressed file so the uncompressor knows when it's finished. 
 
 After the empty nodes are taken out of the vector, the Huffman tree is created.
  The Huffman tree is created by popping the two back nodes of the vector, adding their count together, and putting the count into a new node, and setting the new node's left and right pointers to the popped nodes. 
  Then the heap is sorted.
  This goes until there's only 1 node in the heap, the root node.

 Next, the Huffman codes are generated.
  This is done by recursivly traversing the heap and figuring out if it went right (1) or left (0) to reach a leaf. When it reaches a leaf the final string of 1s and 0s is written as that leaf's Huffman code.

 Next, a new file is opened to be written to. This is the compressed file.
  The first thing it does is find out how many characters are in the Huffman Tree. The number of characters is the first thing written to the compressed file. It's a 32 bit int.
  Then the characters and their count are written to the compressed file. 8 bits are the char then 32 bits are its count. This repeats until all characters are written.
  Then the ascii characters are written as their binary Huffman codes to the compressed file. 
  	The program reads the ascii characters one by one from the input file and writes its Huffman code.
  Finally, the psuedo-EOF character is written to the file.


uncompress.cpp

 First, it gets the file size of the compressed file in bytes and then makes a char array that is length (file size in bits). This will hold all the 1s and 0s from the compressed file. 

 Next, there's a while loop to read the binary and add the 1s and 0s to the char array. 

 After that it recreates the Huffman tree. It does this the same way as from compress.cpp where it takes counts and creates nodes of the char.

 Then it uses the Huffman tree to write to the file. The code loops through the char array with the 1s and 0s. If there's a 1, it goes right in the Huffman tree, 0 vice-versa. When a leaf node is reached, it writes that leaf's char to the output string. This string is then written to the file. 


nodes.cpp
 Information held in a node:
  The char name (as an int)
  The count of how many times that char appears in the text file.
  A pointer the the left and right sub-node.
  The Huffman Code of the character.
  An int to tell if it's a leaf (-2) or not (1)
 There are also getters and setters for each of these. 
