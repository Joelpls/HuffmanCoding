#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <bitset>
#include <sstream>

using namespace std;

#include "nodes.h"

bool myComp(Node *l, Node *r) {return l->get_count() > r->get_count();}

int char_to_string(const char *s, int a, int b)
{
    int val = 0;
    while (a < b)
    {
        val = val * 10 + s[a++] - '0';
    }
    return val;
}

void get_huffman_codes(Node * root, string str, vector<Node*> &nodesList)
{
    if(!root)// || root->leaf != -2 && root->leaf != 1)
        return;

    if(root->get_char() > 0)
    {
        root->set_code(str);
        nodesList.push_back(root);
    }

    get_huffman_codes(root->get_left(), str + "0", nodesList);
    get_huffman_codes(root->get_right(), str + "1", nodesList);
    
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        cout << "./uncompress called with incorrect arguments." << endl;
        cout << "Usage: ./uncompress infile.joel outfile.txt" << endl;
        return 0;
    }

    cout << endl;
    string compressed_file = argv[1];
    string write_to_file = argv[2];
    cout << "uncompressing: " << compressed_file << endl;
    cout << "uncompressed to: " << argv[2] << endl;

    ifstream compressed(compressed_file.c_str(), ios::binary | ios::in );
    char c;

    vector<Node*> node_vector;    

    int charcount=0;

    //file size in bytes
    int filesize = 0;
    ifstream in(compressed_file.c_str(), std::ios::binary | std::ios::ate); 
    filesize = in.tellg();


    int filesize_bits = filesize*8;
    //create char array that is length file size in bits to hold binary
    char the_binary [filesize_bits];

    ifstream infile(compressed_file.c_str());
    //get length of file
    infile.seekg(0, infile.end);
    size_t length = infile.tellg();
    infile.seekg(0, infile.beg);


    ifstream compressed2(compressed_file.c_str(), ios::binary | ios::in );

    int index = 0;
    bitset<32> char_count(0);

    //read binary from compressed file
    //and place it in the char array
    while(compressed2.get(c))
    {
        for(int i = 0; i < 8; i++)
        {
            if(index < 32)
            {
                if(((c >> i) & 1) == 1 )
                {
                    char_count.set(31-index);
                    charcount = (int) char_count.to_ulong();
                    the_binary[index] = '1';
                }
                else
                    the_binary[index] = '0';
            }
            if(index >= 32 )
            {
                if(((c >> i) & 1) == 1 )
                    the_binary[index] = '1';
                else
                    the_binary[index] = '0';
            }
            index++;
        }     
    }

    //make tree
    for(int i = 0; i < charcount ; i++)
    {
        //get the char that this binary represents
        int thechar = char_to_string(the_binary,  40*i +32,  8 + 40*i+32);
        string s = to_string(thechar);
        char * pEnd;
        int b1 = strtol(s.c_str(), &pEnd,2);

        //get the count of that char
        int thecount = char_to_string(the_binary, 40*i + 32 + 8, 40*i + 32 + 8 + 32);
        s = to_string(thecount);
        int b2 = strtol(s.c_str(), &pEnd,2);

        //make a node of those
        Node *n = new Node(b1,b2,-2);
        n->set_left(NULL);
        n->set_right(NULL);
        node_vector.push_back(n);
    }

    //recreate tree
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
    //end creating huffman tree


    vector<Node*> nodesList;

    get_huffman_codes(heap[0], std::string(""), nodesList);


    Node * no = heap[0];

    ofstream outfile (write_to_file.c_str(), ofstream::binary);
    string str;

    //loop through binary chars
    //use the 1s and 0s to go right or left
    //in the tree and when it reaches
    //a leaf, print out the character to the file
    for(int i = charcount*40 + 32; i < filesize_bits; i++)
    {
        if(the_binary[i] == '1')
            no = no->get_right();
        else
            no = no->get_left();
        if(no->leaf == -2)
        {
            if(no->get_char() == 128)
                break;
            str += (char) no->get_char();
            no = heap[0];
        }
    }
    outfile << str;
    outfile.close();
    cout << endl;


    
    return 0;
}