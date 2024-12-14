#include "head.h"
#include <iostream>
#include <chrono>
#include <fstream>
#include <stack>
#include <sstream> 
#include <assert.h>


event::event() : date{ 0, 0, 0 }, description("") {}

event::event(const event& another) : date(another.date), description(another.description) {}

event::~event() {}

Sdate event::getDate() const 
{
    return date;
}

string event::getDescription() const 
{
    return description;
}

void event::setDate(const Sdate input_date)
{
    if (input_date.year > 0) 
        date.year = input_date.year;
    if (input_date.month > 0 && input_date.month < 13) 
        date.month = input_date.month;
    if (isValidDate(input_date.year, input_date.month, input_date.day))     
        date.day = input_date.day;
}


void event::setDescription(const string input_description)
{
    description = input_description;
}
Node::Node() : data(nullptr), left(nullptr), right(nullptr) {}

Node::Node(event* e) : data(e), left(nullptr), right(nullptr) {}

Node::~Node()
{
    if (data != nullptr)
        delete data;
}
BinaryTree::BinaryTree() : root(){}
BinaryTree::BinaryTree(BinaryTree& another) : root(nullptr) {
    if (another.root != nullptr)
        root = copyTree(another.root);
}
BinaryTree::~BinaryTree() {clear(root);}

void BinaryTree::insert(event* e) {
    event* newEvent = new event(*e);
    Node* node = new Node(newEvent);
    Node* current = root;
    Node* parent = nullptr;

    if (current == nullptr) {
        root = node;
        return;
    }

    while (current != nullptr) {
        parent = current;
        if (compareDates(node->data->getDate(), current->data->getDate()) > 0)
            current = current->right;
        else
            current = current->left;
    }

    if (compareDates(node->data->getDate(), parent->data->getDate()) > 0)
        parent->right = node;
    else
        parent->left = node;
}




Node* BinaryTree::getRoot() const
{
    return root;
}
void BinaryTree::clear(Node* node) 
{
    if (node == nullptr)
        return;
    clear(node->left);    
    clear(node->right);   

    delete node;          
}
Node* BinaryTree::copyTree(Node* node) 
{
    if (node == nullptr)
        return nullptr;

    Node* newNode = new Node(new event(*node->data));

    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    return newNode;
}

event* BinaryTree::find(const Sdate& date) 
{
    return findHelper(root, date);
}

event* BinaryTree::findHelper(Node* node, const Sdate& date)
{
    if (node == nullptr)
        return nullptr;

    int comparisonResult = compareDates(node->data->getDate(), date);

    if (comparisonResult == 0) 
        return node->data;

    if (comparisonResult > 0) 
        return findHelper(node->left, date);
    else 
        return findHelper(node->right, date);
}

int BinaryTree::size() const 
{
    return sizeHelper(root);
}

int BinaryTree::sizeHelper(Node* node) const 
{
    if (node == nullptr)
        return 0; 
    return 1 + sizeHelper(node->left) + sizeHelper(node->right);
}

void BinaryTree::saveToFile(const string& filePath) const {
    ofstream file(filePath);
    if (!file.is_open())
        cout <<"file is not open." << endl;

    stack<Node*> nodes;
    nodes.push(root);

    while (!nodes.empty()) 
    {
        Node* current = nodes.top();
        nodes.pop();

        if (current == nullptr)
            file << "#\n";
        else 
        {

            const Sdate& date = current->data->getDate();
            file << date.day << " " << date.month << " " << date.year << " " << current->data->getDescription() << "\n";

            nodes.push(current->right);
            nodes.push(current->left);
        }
    }

    file.close();
}
void BinaryTree::loadFromFile(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open())
        cout << "file is not open." << endl;

    clear(root);
    root = nullptr;

    stack<Node**> nodes;
    nodes.push(&root);

    string line;
    while (getline(file, line)) {
        Node** current = nodes.top();
        nodes.pop();

        if (line == "#")
            *current = nullptr;
        else {
            istringstream iss(line);
            int day, month, year;
            string description;
            iss >> day >> month >> year;
            getline(iss, description);
            description = description.substr(1);

            event* e = new event();
            e->setDate({ day, month, year });
            e->setDescription(description);

            *current = new Node(e);

            nodes.push(&((*current)->right));
            nodes.push(&((*current)->left));
        }
    }

    file.close();
}



// ===========================================================================================

bool isValidDate(int year, int month, int day) 
{
    int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeapYear && month == 2) {
        daysInMonth[1] = 29;
    }

    return (day >= 1 && day <= daysInMonth[month - 1]);
}
int compareDates(const Sdate& date1, const Sdate& date2)
{
    if (date1.year < date2.year) {
        return -1;
    }
    else if (date1.year > date2.year) {
        return 1;
    }

    if (date1.month < date2.month) {
        return -1;
    }
    else if (date1.month > date2.month) {
        return 1;
    }

    if (date1.day < date2.day) {
        return -1;
    }
    else if (date1.day > date2.day) {
        return 1;
    }

    return 0;
}


void testBinaryTree() {
    Sdate date1; date1.year = 2023; date1.month = 6; date1.day = 13;
    Sdate date2; date2.year = 2023; date2.month = 1; date2.day = 5;
    Sdate date3; date3.year = 2021; date3.month = 8; date3.day = 23;
    event e1, e2, e3;

    e1.setDate(date1);
    e1.setDescription("Time to NEW YEAR");
    e2.setDate(date2);
    e2.setDescription("New Year's Party");
    e3.setDate(date3);
    e3.setDescription("Summer Holiday");

    BinaryTree tree;

    tree.insert(&e1);
    tree.insert(&e2);
    tree.insert(&e3);

    assert(tree.size() == 3 && "Test failed: Tree size not 3");

    event* foundEvent1 = tree.find(date1);
    event* foundEvent2 = tree.find(date2);
    event* foundEvent3 = tree.find(date3);

    assert(foundEvent1 != nullptr && "Test failed: Event with date1 not found");
    assert(foundEvent2 != nullptr && "Test failed: Event with date2 not found");
    assert(foundEvent3 != nullptr && "Test failed: Event with date3 not found");

    assert(foundEvent1->getDescription() == "Time to NEW YEAR" && "Test failed: err in event 1");
    assert(foundEvent2->getDescription() == "New Year's Party" && "Test failed: err in event 2");
    assert(foundEvent3->getDescription() == "Summer Holiday" && "Test failed: err in event 3");

    tree.saveToFile("tree_test.txt");

    BinaryTree newTree;
    newTree.loadFromFile("tree_test.txt");

    event* loadedEvent1 = newTree.find(date1);
    event* loadedEvent2 = newTree.find(date2);
    event* loadedEvent3 = newTree.find(date3);

    assert(loadedEvent1 != nullptr && "Test failed: err in with date1");
    assert(loadedEvent2 != nullptr && "Test failed: err in with date2 ");
    assert(loadedEvent3 != nullptr && "Test failed: err in with date3 ");

    assert(loadedEvent1->getDescription() == "Time to NEW YEAR" && "Test failed: err in event 1");
    assert(loadedEvent2->getDescription() == "New Year's Party" && "Test failed: err in event 2");
    assert(loadedEvent3->getDescription() == "Summer Holiday" && "Test failed: err in event 3");

    assert(newTree.size() == 3 && "Test failed: Tree size after loading should be 3");

    event* notFoundEvent = newTree.find({ 2020, 5, 1 });
    assert(notFoundEvent == nullptr && "Test failed: bad Event found");

    cout << "All tests passed =)!" << endl;
}
