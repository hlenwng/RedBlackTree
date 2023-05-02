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
  void rotateLeft(Node *&);
  void rotateRight(Node *&);
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
void rbtree::rotateLeft(Node * &x) {
  Node* y = x->right;
  x->right = y->left;

  if(x->right != nullptr) {
    x->right->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == nullptr) {
    root = y;
  }
  else if(x == x->parent->left) {
    x->parent->left = y;
  }
  else {
    x->parent->right = y;
  }

  x->left = x;
  x->parent = y;
}

//arrangement of nodes on left side transformed to right side
void rbtree::rotateRight(Node* &x) {
  Node* y = x->left;
  x->left = y->right;

  if(x->left != nullptr) {
    x->left->parent = x;
  }

  y->parent = x->parent;

  if(x->parent == nullptr) {
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

  //loop until inserted node 'n' becomes root or until parent's color becomes black
  
  while((n != root) && (n->color != black) && (n->parent->color == red)) {
    parent = n->parent;
    grandparent = n->parent->parent;

    if(parent == grandparent->left) {
      Node* uncle = grandparent->right;

      if(uncle != nullptr && uncle->color == red) {
	grandparent->color = red;
	parent->color = black;
	uncle->color = black;
	n = grandparent;
      }

      else {
	if (n == parent->right) {
	  rotateLeft(parent);
	  n = parent;
	  parent = n->parent;
	}

	rotateRight(grandparent);
	swap(parent->color, grandparent->color);
	n = parent;
      }
    }

    else {
      Node* uncle = grandparent->left;

      if((uncle != nullptr) && (uncle->color == red)) {
	grandparent->color = red;
	parent->color = black;
	uncle->color = black;
	n = grandparent;
      }

      else {
	if (n == parent->left) {
	  rotateRight(parent);
	  n = parent;
	  parent = n->parent;
	}

	rotateLeft(grandparent);
	swap(parent->color, grandparent->color);
	n = parent;
      }
    }
  }
  root->color = black;
}


//call the insertNode function
void rbtree::insert(int &num) {
  insertNode(root, num);
  /*
  Node* newNode = new Node(num);
  root = insertNode(root, newNode);

  fixViolation(root, newNode);
  */
}

//insert a node with # given by user into tree
Node* rbtree::insertNode(Node* &root, int num) {
  if (root == nullptr) {
    Node* newRoot = new Node(num);
    //return newNode;
    root = newRoot;
    return root;
  }

  else {
    //if new node is smaller than root, added to left side of tree 
    //if to the left, check if it is at the base of tree
    if(num < root->data && root->left) {
      Node* temp = root->left;
      return insertNode(temp, num);
    }
    else if(num < root->data && !root->left) {
      Node* newNode = new Node(num);
      root->left = newNode;
      newNode->parent = root;
      return newNode;
    }

    if(num > root->data && root->right) {
      Node* temp = root->right;
      return insertNode(temp, num);
    }
    else if(num > root->data && !root->right) {
      Node* newNode = new Node(num);
      root->right = newNode;
      newNode->parent = root;
      return newNode;
    }

    /*
    if(newNode->data < root->data) {
      root->left = insertNode(root->left, newNode);
      root->left->parent = root;
    }
    
    //if new node is bigger than root, added to right side of tree
    else if(newNode->data > root->data) {
      root->right = insertNode(root->right, newNode);
      root->right->parent = root;
    }
    */
  }
  return NULL;
  //return root;
}


/*
void rbtree::insert(int num) {
  Node* newNode = new Node(num);

  Node* y = nullptr;
  Node* x = this->root;

  while (x != nullptr) {
    y = x;
    if (newNode->data < x->data) {
      x = x->left;
    } else{
      x = x->right;
    }
  }

  newNode->parent = y;
  if(y == nullptr) {
    root = newNode;
  } else if (newNode->data < y->data) {
    y->left = newNode;
  } else {
    y->right = newNode;
  }

  if (newNode->parent == nullptr) {
    newNode->color = black;
    return;
  }

  if(newNode->parent->parent == nullptr) {
    return;
  }

  fixViolation(root, newNode);
}
*/

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
