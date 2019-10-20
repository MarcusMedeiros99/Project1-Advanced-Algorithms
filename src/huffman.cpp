#include <stdio.h>
#include <bits/stdc++.h>

#define LEFT 'l'
#define RIGHT 'r'
#define MAIN 'm'

using namespace std;

int idCounter = 0; //a global variable utilized to identify the characters


//struct to define a character with a frequency f and and id
//the id is utilized to help identify the new characters created due to the
//description of the algorithm
typedef struct character {
    //f is absolute frequency of the character c
    char c = (char) 0; 
    int f = 0;
    int id;

    //the operator is inverted so we can utilize the priorrity_queue template
    //as a min-heap
    bool operator<(const struct character &c) const {
        return f > c.f;
    }
} Char;

typedef struct node {
    char c;
    int id;
    
    struct node* left = NULL;
    struct node* right = NULL;

    bool operator==(const struct node &b) const {
        return id == b.id;
    }
    bool operator<(const struct node &b) const {
        return id < b.id;
    }
} Node;


//function utilized for encoding using the tree. Left is encoded as '0', and right
//is encoded as '1' The code is saved at  "char2code"
void printTree(Node* tree,  map<char, string>& char2code, string s = "") {  
    if (tree->left) printTree(tree->left, char2code, s + "0");
    if (tree->right) printTree(tree->right, char2code, s + "1");
    if (!tree->right && !tree->left) char2code[tree->c] = s;
}

void eraseTree(Node* tree) {
    if (tree) {
        eraseTree(tree->left);
        eraseTree(tree->right);
        free(tree);
    }
}

struct Comp{
    bool operator()(const Node* a,const  Node* b){
        return a->id < b->id;
    }
};

void huffman (priority_queue<Char> &input, Node** tree, set<Node*, Comp> &leaves) {
    if (input.size() == 2) { //if S has two letters
        //When we get to the end of recursive calls, we begin creating the tree
        //with two leaves representing the two characters in "input"
        Node* left = (Node*) malloc(sizeof(Node));
        Node* right = (Node*) malloc(sizeof(Node));
        Node* newTree = (Node*) malloc(sizeof(Node));

        if (left && right && newTree) {
            //creation of left node
            left->left = NULL;
            left->right = NULL;
            left->c = input.top().c; //left is encoded as '0'
            left->id = input.top().id;
            input.pop();
            leaves.insert(left);
            newTree->left = left;
            
            //creation of right node
            right->left = NULL;
            right->right = NULL;
            right->c = input.top().c; //right is encoded as '1'
            right->id = input.top().id;
            input.pop();
            leaves.insert(right);
            newTree->right = right;

            *tree = newTree;
            
            return;
        }
        if (left) free(left);
        if (right) free(right);
        if (newTree) free(newTree);
    }
    
    //newChar character will have the frequency of the two characters 
    //with the least frequencies removed
    Char newChar;
    newChar.f = 0;
    newChar.f += input.top().f; 
    char c1 = input.top().c; 
    int id1 = input.top().id;
    input.pop(); //removes one character c1 from S
    
    newChar.f += input.top().f;
    char c2 = input.top().c;
    int id2 = input.top().id;
    input.pop(); //removes one character c2 from S
    
    newChar.id = idCounter;
    Node* find = (Node*) malloc(sizeof(Node));//this node is created to help us find the character in the tree
    find->id = idCounter;
    idCounter++;
    input.push(newChar); //inserts new character w with frequency equals to the the sum of the characters removed

    huffman(input, tree, leaves); //recursive call with S
    Node* u = (Node*) malloc(sizeof(Node));
    Node* v = (Node*) malloc(sizeof(Node));
    
    u->c = c1;
    u->id = id1;
    v->c = c2;
    v->id = id2;
    
    //We look for the new character in the tree and add 'c1' and 'c2' as the two children
    //of it. Of course, it is not a leave anymore, just as 'c1' and 'c2' become leaves of
    //the tree
    Node* it = *(leaves.find(find) );
    free(find);
    it->left = u;
    it->right = v;
    leaves.erase(it);
    leaves.insert(u);
    leaves.insert(v);
    
    return;
}

int main(int argc, char const *argv[]) {
    int n;
    Char aux;
    Node* tree;
    set<Node*, Comp> leaves;//we keep control of the leaves so we don't need to search in the whole tree for the characters
    map<char, string> char2code;//the map is utilized for printing the codifications in order
    priority_queue<Char> input;
    FILE* in;
    
    in = fopen(argv[1], "r");
    if (in == NULL) {
        cout << "Error in file reading" << endl;
        exit(1);
    }
    
    fscanf(in, "%d ", &n);
    //vector<char> code(n, '\0');
    for (int i = 0; i < n; i++) {
        //cin >> aux.c >> aux.f;
        fscanf(in, "%c", &aux.c);
        fscanf(in, "%d ", &aux.f);
        aux.id = idCounter;
        idCounter++;

        input.push(aux);
    }
    huffman(input, &tree, leaves);
    
    printTree(tree, char2code);
    for (auto it : char2code) {
        cout << it.second << endl;
    }
    eraseTree(tree);
    fclose(in);
    return 0;
}
