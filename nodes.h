#ifndef NODES_H
#define NODES_H

class Node{
private:
	int count;
	int character;
	Node *left;
	Node *right;
	std::string code;

public:
	void set_count(int c);
	int get_count();
	void set_char(int c);
	int get_char();	
    Node();
    ~Node();
    Node(int name, int the_count, int l);
    // Node(Node*left, Node*right);
    void set_left(Node *l);
    void set_right(Node *r);
    Node* get_left();
    Node* get_right();
    void set_code(std::string data);
    std::string get_code();
    int leaf;

};


#endif