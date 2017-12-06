// C++ implemntation of search() and traverse() methods
#include<iostream>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <locale>
#include <sstream>
#include <string>
using namespace std;

// A BTree node
class BTreeNode
{
    int *keys;  // An array of keys
    int t;      // Minimum degree (defines the range for number of keys)
    long int *C; // An array of child pointers
    int n;     // Current number of keys
    bool leaf; // Is true when node is leaf. Otherwise false
    bool root; // Verdadeiro quando o nodo for a raiz
public:
    BTreeNode(int _t, bool _leaf, bool _root);   // Constructor
    BTreeNode(string _name);   // Constructor
 
    // A function to traverse all nodes in a subtree rooted with this node
    void traverse();
 
    // A function to search a key in subtree rooted with this node.    
    BTreeNode *search(int k);   // returns NULL if k is not present.

    void save_node();
    void save_node(string name);

    BTreeNode read_node(string name);
 
// Make BTree friend of this so that we can access private members of this
// class in BTree functions
friend class BTree;
};
 
// A BTree
class BTree
{
    BTreeNode *root; // Pointer to root node
    int t;  // Minimum degree
public:
    // Constructor (Initializes tree as empty)
    BTree(int _t)
    {  root = NULL;  t = _t; }
 
    // function to traverse the tree
    void traverse()
    {  if (root != NULL) root->traverse(); }
 
    // function to search a key in this tree
    BTreeNode* search(int k)
    {  return (root == NULL)? NULL : root->search(k); }
};
 
// Constructor for BTreeNode class
BTreeNode::BTreeNode(int _t, bool _leaf, bool _root)
{
    // Copy the given minimum degree and leaf property
    t = _t;
    leaf = _leaf;
    root = _root;
 
    // Allocate memory for maximum number of possible keys
    // and child pointers
    keys = new int[2*t-1];
    C = new long int [2*t];
 
    // Initialize the number of keys as 0
    n = 0;
    save_node();
}

BTreeNode::BTreeNode(string name)
{ 
    // Allocate memory for maximum number of possible keys
    // and child pointer
 
    // Initialize the number of keys as 0
    n = 0;
    read_node(name);
}

void BTreeNode::save_node(){
    // Salva o nodo em arquivo
    // printf("ADSASDASd");
    FILE *arq;
    if (root == true){
        arq = fopen("root", "wb");
    }
    else{
        long int ptr_node = time(0);       
        string Result;        
        ostringstream convert;   
        convert << ptr_node;     
        Result = convert.str(); 
        const char * name = Result.c_str();
        arq = fopen(name, "wb");
    }
    // Salva no formato [ordem, no folha, numero de chaves no no, chaves e ponteiros]
    fwrite(&t, sizeof(t), 1, arq);
    if (leaf == true) {
        int temp = 1;
        fwrite(&temp, sizeof(int), 1, arq);
    }
    else {
        int temp = 0;
        fwrite(&temp, sizeof(int), 1, arq);
    }
    fwrite(&n, sizeof(n), 1, arq);
    fclose(arq);
}

void BTreeNode::save_node(string name){
    // Salva o nodo em arquivo
    // printf("ADSASDASd");
    FILE *arq;
    arq = fopen(name.c_str(), "wb");
    // Salva no formato [ordem, no folha, numero de chaves no no, chaves e ponteiros]
    fwrite(&t, sizeof(t), 1, arq);
    if (leaf == true) {
        int temp = 1;
        fwrite(&temp, sizeof(int), 1, arq);
    }
    else {
        int temp = 0;
        fwrite(&temp, sizeof(int), 1, arq);
    }
    fwrite(&n, sizeof(n), 1, arq);
    fclose(arq);
}

BTreeNode BTreeNode::read_node(string name){
    FILE *arq;
    arq = fopen(name.c_str(), "rb");
    int leitura[3];
    
    fread(leitura, sizeof(int), 3, arq);

    // Inicializa as variaveis
    t = leitura[0];
    leaf = leitura[1];
    n = leitura[2];
    keys = new int[2*t-1];
    C = new long int [2*t];

    // Le as informacoes do nodo
    if (n > 0){
        long int aux;
        fread(&aux, sizeof(long int), 1, arq);
        C[0] = aux;
        for (int i = 0; i < n; i++)
        {
            fread(&aux, sizeof(long int), 1, arq);
            C[i + 1] = aux;
            int temp;
            fread(&temp, sizeof(int), 1, arq);
            keys[i] = temp;
        }
    }

    printf("%d %d %d", t, leaf, n);
    fclose(arq);
}

// Function to traverse all nodes in a subtree rooted with this node
void BTreeNode::traverse()
{
    // There are n keys and n+1 children, travers through n keys
    // and first n children
    int i;
    for (i = 0; i < n; i++)
    {
        // If this is not leaf, then before printing key[i],
        // traverse the subtree rooted with child C[i].
        if (leaf == false){
            string Result;        
            ostringstream convert;   
            convert << C[i];     
            Result = convert.str(); 
            BTreeNode a(Result);
            a.traverse();
        }
        cout << " " << keys[i];
    }
 
    // Print the subtree rooted with last child
    if (leaf == false){
        string Result;        
        ostringstream convert;   
        convert << C[i];     
        Result = convert.str(); 
        BTreeNode a(Result);
        a.traverse();
    }
}
 
// Function to search key k in subtree rooted with this node
BTreeNode *BTreeNode::search(int k)
{
    // Find the first key greater than or equal to k
    int i = 0;
    while (i < n && k > keys[i])
        i++;
 
    // If the found key is equal to k, return this node
    if (keys[i] == k)
        return this;
 
    // If key is not found here and this is a leaf node
    if (leaf == true)
        return NULL;
 
    // Go to the appropriate child
    string Result;        
    ostringstream convert;   
    convert << C[i];     
    Result = convert.str(); 
    BTreeNode a(Result);
    a.traverse();
    return a.search(k);
}


int main(){
    BTreeNode a(144, true, true);
    // a.read_node();

}