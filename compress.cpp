#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bitset>

#include "nodes.h"

using namespace std;

bool myComp(Node *l, Node *r) {return l->get_count() > r->get_count();}

void get_huffman_codes(Node * root, string str, vector<Node*> &nodesList)
{
    if(!root)
        return;

    if(root->get_char() > 0)
    {
        root->set_code(str);
        nodesList.push_back(root);
    }

    get_huffman_codes(root->get_left(), str + "0", nodesList);
    get_huffman_codes(root->get_right(), str + "1", nodesList);    
}

int current_bit = 0;
unsigned char bit_buffer;

FILE *write_file;

void write_byte(int bit)
{
    if(bit)
        bit_buffer |= (1<<current_bit);

    current_bit++;
    if(current_bit == 8)
    {
        fwrite(&bit_buffer, 1, 1, write_file);
        current_bit = 0;
        bit_buffer = 0;
    }

}

void flush_bits(void)
{
    while(current_bit){
        write_byte(0);
    }
}

int main(int argc, char *argv[])
{

    if(argc != 3)
    {
        cout << "./compress called with incorrect arguments." << endl;
        cout << "Usage: ./compress infile.txt outfile.joel" << endl;
        return 0;
    }

    ifstream inputFile;
    string file = argv[1];
    char ascii;
    int size = 129;

    vector<int> asciiArray(size);

    inputFile.open(file.c_str());

    if(inputFile.fail())
    {
        cout << "File could not be opened." << endl;
        return 1;
    }

    cout << "File is: " << file <<endl;

    //read all chars
    //get the counts of each character in the text file.
    while(inputFile >> noskipws >> ascii)
    {
        asciiArray[ascii]++;
    } 
    
    //make nodes of all ascii counts
    vector<Node*> node_vector;
    cout << "Count of each character in txt" << endl;
    for(int i = 0; i < 128; i++)
    {
        cout << (char) i << ": " << asciiArray[i] << endl;

        int count = asciiArray[i];
        Node *n = new Node(i, count, -2);
        n->set_left(NULL);
        n->set_right(NULL);

        node_vector.push_back(n);     
    }
    //end

    //EOF node
    asciiArray[128] = 1;
    Node *n = new Node(128, 1, -2);
    n->set_left(NULL);
    n->set_right(NULL);
    node_vector.push_back(n); 
    cout << (char) 128 << ": eof : " << 1 << endl;
    //end EAF node code

    cout << endl;

    //remove empty Nodes
    if(!node_vector.empty()) 
    {
        for(int i = node_vector.size() - 1; i >= 0 ; i--)
        {
            if(node_vector[i]->get_count() == 0)
                node_vector.erase(node_vector.begin() + i);
        }
    }


    //create Huffman tree
    vector<Node*> heap;

    heap = node_vector;

    while(heap.size() > 1)
    {
        //remove 1
        Node *min1 = heap.back();
        heap.pop_back();

        //remove 2
        Node *min2 = heap.back();
        heap.pop_back();

        //merge
        Node *new_node = new Node(-1 , (min1->get_count() + min2->get_count()), 1);
        new_node->set_left(min1);
        new_node->set_right(min2);

        //push back
        heap.push_back(new_node);
        sort(heap.begin(), heap.end(), myComp);


    }
    
    vector<Node*> nodesList;

    get_huffman_codes(heap[0], std::string(""), nodesList);


    sort(nodesList.begin(), nodesList.end(), myComp);

    cout << "Huffman codes of each character" << endl;
    for(int i = 0; i < nodesList.size(); i++)
    {   
        cout <<  (char) nodesList[i]->get_char() << ": " << nodesList[i]->get_count() << " : " << nodesList[i]->get_code() << endl;
    }    

    ifstream input;
    input.open(file.c_str());

    cout << endl;
    cout << "Compressed file is: " << argv[2] <<  endl;;
    cout << endl;
    string file1 = argv[2];
    write_file = fopen( file1.c_str() , "wb");

    //find out how many characters there are
    int char_count = 0; 
    for(int i = 0; i < 129; i++)
    {
        if(asciiArray[i] > 0)
            char_count++;
    }
    //write that number
    bitset<32> charcount (char_count);
    for(int i = 31; i >= 0; i--)
    {
        write_byte(charcount[i]);
    }

   // write character/count pairs
    for(int i = 0; i < 129; i++)
    {
        if(asciiArray[i] > 0)
        {
            //write char
            bitset<8> mychar (i);
            write_byte(mychar[7]);
            write_byte(mychar[6]);
            write_byte(mychar[5]);
            write_byte(mychar[4]);
            write_byte(mychar[3]);
            write_byte(mychar[2]);
            write_byte(mychar[1]);
            write_byte(mychar[0]);
            //write count
            bitset<32> mycount (asciiArray[i]);
            for(int i = 31; i >= 0; i--)
            {
                write_byte(mycount[i]);
            }
        }
    }
    
    //write data
    //loop through all characters
    while(input >> noskipws >> ascii)
    {   
        //get the code of the char
        string code;
        for(int i = 0; i < nodesList.size(); i++)
        {
            if(ascii == nodesList[i]->get_char())
            {
                code = nodesList[i]->get_code();
                break;
            }
        }
        //write the code of the char
        for(string::size_type i = 0; i < code.size(); i++)
        {
            if(code[i] == '1'){
                write_byte(1);
            }
            else{
                write_byte(0);
            }
            
        }
    } 

    //write EOF code to file
    string code;
    for(int i = 0; i < nodesList.size(); i++)
    {
        if(128 == nodesList[i]->get_char())
        {
            code = nodesList[i]->get_code();
            break;
        }
    }
    for(string::size_type i = 0; i < code.size(); i++)
    {
        if(code[i] == '1'){
            write_byte(1);
        }
        else{
            write_byte(0);
        }
        
    }

    flush_bits();

    fclose(write_file);

    double filesize = 0;
    ifstream in(file.c_str(), std::ios::binary | std::ios::ate); 
    filesize = in.tellg();

    cout << "Original file size: " << filesize << " bytes" << endl;

    double filesize2 = 0;
    ifstream in1(file1.c_str(), std::ios::binary | std::ios::ate); 
    filesize2 = in1.tellg();

    cout << "Compressed file size: " << filesize2 << " bytes" << endl;

    int percent = 0;
    percent = ((filesize - filesize2) / filesize) * 100;

    cout << "The file was compressed by " << percent << "%" << endl;
    cout << endl;


    
    inputFile.close();

    return 0;
}