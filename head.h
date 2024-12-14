#ifndef head
#define head
#include <string>
#include <chrono>
using namespace std;

struct Sdate
{
	int day;
	int month;
	int year;
};

class event
{
public:
	event();
	event(const event& another);
	~event();


	void setDate(const Sdate input_date);
	void setDescription(const string text);

	Sdate getDate() const;
	string getDescription() const;
private:
	Sdate date;
	string description;
};

class Node 
{
public:
	Node();
	Node(event* e);   
	~Node();          
    
	event* data;        
	Node* left;         
	Node* right;        
};

class BinaryTree 
{
public:
	BinaryTree();
	BinaryTree(BinaryTree& another);
	~BinaryTree();      

	void insert(event* e);     
	event* find(const Sdate& date);  
	Node* getRoot() const;
	void saveToFile(const string& filePath) const;
	void loadFromFile(const string& filePath);
	int size() const;

private:
	void clear(Node* node);
	Node* copyTree(Node* node);
	event* findHelper(Node* node, const Sdate& date);
	int sizeHelper(Node* node) const;
	Node* root;            
};

bool isValidDate(int year, int month, int day);
int compareDates(const Sdate& date1, const Sdate& date2);
void testBinaryTree();
#endif // !head
