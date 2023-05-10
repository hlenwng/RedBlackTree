#include <iostream>
#include <fstream>
#include <cstring>
using namespace std;

/*
  Name: Helen Wang
  Date: May 2, 2023
  Program: Red Black Tree, able to correctly sort and balance tree.
  The red or black node attribute is used to ensure balanced tree.

  Functions:
  -> insert values (console & file)
  -> print 2D model of tree
  -> search for value
  -> delete value (and correctly reformat tree)
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
  //void transplant(Node* x, Node* y);
  Node* minimum(Node* node);
  
  
public:
  rbtree() {
    root = nullptr;
  }

  //functions to be called by others
  void insert(int &num);
  Node* insertNode(Node* &root, int num);
  void print(Node* root, int space);
  void callPrint();
  int callSearch(int douexist);
  int search(int douexist, Node* root);
  void inorder();
  void deleteNode(int &n);
  void deleteHelper(Node* node);
  
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

Node* rbtree::minimum(Node* node) {
  while(node->left != NULL) {
    node = node->left;
  }
  return node;
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

  if(root->parent != nullptr) {
    //print node parent
    cout << "p:" << root->parent->data << ", ";
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

int rbtree::callSearch(int douexist) {
  return search(douexist, root);
}

int rbtree::search(int douexist, Node* root) {
  //if it equals the root
  if(!root || root->data == douexist) {
    return root->data;
  }

  //if number is bigger than root
  if(root->data < douexist) {
    //bottom of tree
    if(!root->right) {
      return root->data;
    }
    return search(douexist, root->right);
  }
  
  //if number is smaller than root
  else {
    //bootom of tree
    if(!root->left) {
      return root->data;
    }
    return search(douexist, root->left);
  }
}

//Resource for deletion: Programiz

//helper function to delete node
void rbtree::deleteNode(int& num) {
  //Case 1: Node has no children
  //Case 2: Node has 1 child
  //Case 3: Node has 2 children

  //search for node to delete
  Node* z = root;
  Node* x;
  Node* y;

  while(z != nullptr) {
    if(z->data == num) {
      break;
    }
    if(z->data < num) {
      z = z->right;
    } else {
      z= z->left;
    }
  }

  //determine node to splice out
  if(z->left == nullptr || z->right == nullptr) {
    //if node to delete has at most once child, splice
    y = z;
  } else {
    //or find node's successor and splice that out
    y = minimum(z->right);
  }

  //determine node to replace spliced-out node
  if(y->left != nullptr) {
    x = y->left;
  } else {
    x = y->right;
  }

  if(x != nullptr) {
    x->parent = y->parent;
  }

  if(y->parent == nullptr) {
    //if spliced out root, replace with new root
    root = x;
  } else if (y == y->parent->left) {
    y->parent->left = x;
  } else {
    y->parent->right = x;
  }

  if(y != z) {
    z->data = y->data;
  }

  if(y->color == black) {
    deleteHelper(x);
  }
  delete y;
}

void rbtree::deleteHelper(Node* node) {
  while (node != root && (node == nullptr || node->color == black)) {
    if (node == node->parent->left) {
      Node *sibling = node->parent->right;
      
      if (sibling->color == red) {
	//case 1: sibling is red
	sibling->color = black;
	node->parent->color = red;
	rotateLeft(node->parent, root);
	sibling = node->parent->right;
      }
      
      if ((sibling->left == nullptr || sibling->left->color == black) &&
	  (sibling->right == nullptr || sibling->right->color == black)) {
	//case 2: sibling is black & both of its children are black
	sibling->color = red;
	node = node->parent;
      } else {
	//case 3: sibling is black, left child is red & right child is black
	if (sibling->right == nullptr || sibling->right->color == black) {
	  sibling->left->color = black;
	  sibling->color = red;
	  rotateRight(sibling, root);
	  sibling = node->parent->right;
	}

	//case 4: sibling is black and right child is red
	sibling->color = node->parent->color;
	node->parent->color = black;
	sibling->right->color = black;
	rotateLeft(node->parent, root);
	node = root;
      }
    } else {
      Node *sibling = node->parent->left;
      
      if (sibling->color == red) {
	//case 1: sibling is red
	sibling->color = black;
	node->parent->color = red;
	rotateRight(node->parent, root);
	sibling = node->parent->left;
      }
      
      if ((sibling->left == nullptr || sibling->left->color == black) &&
	  (sibling->right == nullptr || sibling->right->color == black)) {
	//case 2: sibling is black & both its children are black
	sibling->color = red;
	node = node->parent;
      } else {
	if (sibling->left == nullptr || sibling->left->color == black) {
	  //case 3: sibling is black, right child is red, and left is black
	  sibling->right->color = black;
	  sibling->color = red;
	  rotateLeft(sibling, root);
	  sibling = node->parent->left;
	}
	//case 4: sibling is black and left child is red
	sibling->color = node->parent->color;
	node->parent->color = black;
	sibling->left->color = black;
	rotateRight(node->parent, root);
	node = root;
      }
    }
  }
  if (node!= nullptr) {
    node->color = black;
  }
}

int main() {
  //Intialize variables and tree
  rbtree tree;
  bool playing = true;
  char input[20];

  //Read in user input (different commands)
  while (playing == true) {
    cout << endl;
    cout << "[ADD] [FILE] [DELETE] [SEARCH] [PRINT] or [QUIT]" << endl;
    
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
    else if(strcmp(input, "FILE") == false) {
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

    //delete value from tree
    else if(strcmp(input, "DELETE") == false) {
      cout << "enter the # you wish to delete" << endl;
      int deleteNum = 0;
      cin >> deleteNum;
      cin.ignore();

      int found = tree.callSearch(deleteNum);

      
      if(found == deleteNum) {
	tree.deleteNode(deleteNum);
	cout << deleteNum << " has been deleted" << endl;
      }
      else if(found != deleteNum) {
	cout << "# isn't in tree" << endl;
      }
      
    }

    //search value in tree
    else if(strcmp(input, "SEARCH") == false) {
      cout << "enter the # you wish to search for" << endl;
      int searchNum = 0;
      cin >> searchNum;
      cin.ignore();

      int found = tree.callSearch(searchNum);

      if(found == searchNum) {
	cout << "# is found" << endl;
      }
      else if(found != searchNum) {
	cout << "# isn't in tree" << endl;
      }
    }
      
    //print 2d tree
    else if(strcmp(input, "PRINT") == false) {
      cout << "*p stands for parent" << endl;
      cout << endl;
      tree.callPrint();
    }

    //quit program
    else if(strcmp(input, "QUIT") == false) {
      playing = false;
    }
  }
}
