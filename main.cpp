#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

/*
  Name: Helen Wang
  Date: May 2, 2023
  Program: Red Black Tree, able to correctly sort and balance tree
  -> users can insert numbers through console and file, and print the tree
 */

enum Color {red, black}; //Enumeration

//Struct for nodes
struct Node{
  int data;
  Color color;
  Node* left, *right, *parent;

  //Declaring new default node
  Node(int data) : data(data) {
    color = red;
    left = right = parent = nullptr;
  }
};

//Class for nodes used in binary tree
class rbtree {
  Node* root;

  //functions used within class
  void rotateLeft(Node* &x, Node* &root);
  void rotateRight(Node* &x, Node* &root);
  void fixViolation(Node *&, Node *&);

public:
  rbtree() {
    root = nullptr;
  }

  //functions to be called by others
  //void insert(int num);
  void insert(int &num);
  //Node* insertNode(Node* &root, Node* &newNode);
  Node* insertNode(Node* &root, int num);
  void print(Node* root, int space);
  void callPrint();
  void inorder();
};

//in order
void inorderHelper(Node * root) {
  if (root != nullptr) {
    inorderHelper(root->left);
    cout << root->data << " ";
    inorderHelper(root->right);
  }
}

//call the inorderHelper function
void rbtree::inorder() {
  inorderHelper(root);
}

//arrangement of nodes on right side transformed to left side
void rbtree::rotateLeft(Node * &x, Node* &root) {
  Node* y = x->right;
  x->right = y->left;

  if(x->right) {
    x->right->parent = x;
  }
  y->parent = x->parent;
  if(!x->parent) {
    root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}

//arrangement of nodes on left side transformed to right side
void rbtree::rotateRight(Node* &x, Node* &root) {
  Node* y = x->left;
  x->left = y->right;

  if(x->left) {
    x->left->parent = x;
  }
  y->parent = x->parent;
  if(!x->parent) {
    root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }
  y->right = x;
  x->parent = y;
  
}

/*
void rbtree::fixViolation(Node* &root, Node* &n) {
  Node* parent = nullptr;
  Node* grandparent = nullptr;
  Node* u;
  
  while(n->parent->color == red) {
    parent = n->parent;
    grandparent = n->parent->parent;

    if(parent == grandparent->right) {
      u = grandparent->left;
      if(u->color == red) {
	u->color = black;
	parent->color = black;
	grandparent->color = red;
	n = grandparent;
      } else {
	if(n == parent->left) {
	  n = parent;
	  rotateRight(n);
	}
	parent->color = black;
	grandparent->color = red;
	rotateLeft(grandparent);
      }
    } else {
      u = grandparent->right;

      if(u->color == red) {
	u->color = black;
	parent->color = black;
	grandparent->color = red;
	n = grandparent;
      } else {
	if(n == parent->right) {
	  n = parent;
	  rotateLeft(n);
	}
	parent->color = black;
	grandparent->color = red;
	rotateRight(grandparent);
      }
    }
    if(n == root) {

      break;
    }
  }
  root->color = black;
}
*/


//balance the tree after insertion
void rbtree::fixViolation(Node* &root, Node* &n) {
  Node* parent = nullptr;
  Node* grandparent = nullptr;
  Node* uncle = nullptr;

  //loop until inserted node 'n' becomes root or until parent's color becomes black
  
  while((n != root) && (n->color != black) && (n->parent->color == red)) {
    parent = n->parent;
    grandparent = n->parent->parent;

    if(parent == grandparent->left) { //if parent is left child
      uncle = grandparent->right;

      if(uncle && uncle->color == red) {
	grandparent->color = red;
	parent->color = black;
	uncle->color = black;
	n = grandparent;
      }

      else {
	if (n == parent->right) {
	  rotateLeft(parent, root);
	  n = parent;
	  parent = n->parent;
	}

	rotateRight(grandparent, root);
	//swap(parent->color, grandparent->color);
	parent->color = black;
	grandparent->color = red;
	n = parent;
      }
    }

    else {
      uncle = grandparent->left;

      if((uncle != nullptr) && (uncle->color == red)) {
	grandparent->color = red;
	parent->color = black;
	uncle->color = black;
	n = grandparent;
      }

      else {
	if (n == parent->left) {
	  rotateRight(parent, root);
	  n = parent;
	  parent = n->parent;
	}

	rotateLeft(grandparent, root);
	//swap(parent->color, grandparent->color);
	parent->color = black;
	grandparent->color = red;
	n = parent;
      }
    }
  }
  root->color = black;
}


//call the insertNode function
void rbtree::insert(int &num) {
  Node* current = new Node(num);
  current = insertNode(root, num);
  fixViolation(root, current);
}

//insert a node with # given by user into tree
Node* rbtree::insertNode(Node* &root, int num) {
  if (root == nullptr) {
    Node* newRoot = new Node(num);
    root = newRoot;
    return root;
  }

  else {
    //if new node is smaller than root, added to left side of tree 
    //if to the left, check if it is at the base of tree
    if(num < root->data && root->left) {
      Node* temp = root->left;
      //newNode = root->left;
      //return insertNode(newNode, num, newNode);
      //fixViolation(root, temp);
      return insertNode(temp, num);
    }
    else if(num < root->data && !root->left) {
      Node* newNode = new Node(num);
      root->left = newNode;
      newNode->parent = root;
      //fixViolation(root, newNode);
      return newNode;
    }

    if(num > root->data && root->right) {
      Node* temp = root->right;
      //fixViolation(root, temp);
      return insertNode(temp, num);
      //return insertNode(newNode, num, newNode);
    }
    else if(num > root->data && !root->right) {
      Node* newNode = new Node(num);
      root->right = newNode;
      newNode->parent = root;
      //fixViolation(root, newNode);
      return newNode;
    }
  }
  //fixViolation(root, newNode);
  return NULL;
  //return root;
}

//print 2d tree with node color
void rbtree::print(Node* root, int space) {
  if (root == nullptr) {
    //cout << "tree is empty" << endl;
    return;
  }

  int count = 5;
  space += count;

  //right child first
  print(root->right, space);

  //print current node
  cout << endl;
  for(int i = count; i < space; i++) {
    cout << " ";
  }

  //if node color is red or black,
  //print color correspondingly in parenthesis next to number
  cout << root->data << "(" << (root->color == red ? "R" : "B") << ")" << endl;

  //print left child
  print(root->left, space);
}

//function to call print function
void rbtree::callPrint() {
  print(root, 0);
}

int main() {
  //Intialize variables and tree
  rbtree tree;
  bool playing = true;
  char input[20];

  //Read in user input (different commands)
  while (playing == true) {
    cout << endl;
    cout << "[ADD] [READ] [PRINT] or [QUIT]" << endl;
    
    cin.get(input, 20, '\n');
    cin.ignore();
    
    if(strcmp(input, "ADD") == false) {

      //Read in values from user
      cout << "Please enter number (1-999): ";

      int num = 0;
      
      cin >> num;
      cin.ignore();
      
      //tnode* temp = new tnode(num);
      //temp->setColor(0);
      //cout << temp->getColor() << endl;
      tree.insert(num);
      //tree.fixViolation(root,  
      cout << "# has been added" << endl;
    }
  
    //Read in values from a file
    else if(strcmp(input, "READ") == false) {
      fstream file;
      file.open("number.txt");
      //file.open(number);
      
      int input = 0;
      
      while(file >> input) {
	cout << input << endl;
	tree.insert(input);
      }
      file.close();
    }

    //print 2d tree
    else if(strcmp(input, "PRINT") == false) {
      tree.callPrint();
    }

    //quit program
    else if(strcmp(input, "QUIT") == false) {
      playing = false;
    }
  }
}
