#include <iostream>
#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
#include "node.h"
#include "project.cpp"
#include "func.h"

// ─────────────────────────────────────────
//  BASE CLASS
// ─────────────────────────────────────────
class Item {
public:
    string name;
    string condition;
    string donorName;
    bool isClaimed;

    Item(string n, string cond, string donor)
        : name(n), condition(cond), donorName(donor), isClaimed(false) {}

    virtual ~Item() {}

    bool operator==(const Item& other) const {
        return name == other.name;
    }

    virtual void displayInfo() {
        cout << "  Name      : " << name << endl;
        cout << "  Condition : " << condition << endl;
        cout << "  Donor     : " << donorName << endl;
        cout << "  Status    : " << (isClaimed ? "Claimed" : "Available") << endl;
    }

    virtual string getCategory() {
        return "General";
    }
};

// ─────────────────────────────────────────
//  SUBCLASSES
// ─────────────────────────────────────────
class Electronics : public Item {
public:
    string brand;

    Electronics(string n, string cond, string donor, string b)
        : Item(n, cond, donor), brand(b) {}

    ~Electronics() {}

    void displayInfo() override {
        Item::displayInfo();
        cout << "  Brand     : " << brand << endl;
    }

    string getCategory() override { return "Electronics"; }
};

class Book : public Item {
public:
    string author;

    Book(string n, string cond, string donor, string a)
        : Item(n, cond, donor), author(a) {}

    ~Book() {}

    void displayInfo() override {
        Item::displayInfo();
        cout << "  Author    : " << author << endl;
    }

    string getCategory() override { return "Book"; }
};

class Clothing : public Item {
public:
    string size;

    Clothing(string n, string cond, string donor, string s)
        : Item(n, cond, donor), size(s) {}

    ~Clothing() {}

    void displayInfo() override {
        Item::displayInfo();
        cout << "  Size      : " << size << endl;
    }

    string getCategory() override { return "Clothing"; }
};

class Furniture : public Item {
public:
    Furniture(string n, string cond, string donor)
        : Item(n, cond, donor) {}

    ~Furniture() {}

    string getCategory() override { return "Furniture"; }
};
// ─────────────────────────────────────────
//  LINKED LIST CLASS
// ─────────────────────────────────────────
class LinkedList {
private:
    Node* head;
    int size;

public:
    LinkedList() : head(nullptr), size(0) {}

    ~LinkedList() {
        clear();
    }

    void addFront(const string& item, const string& donor, const string& cat, long ts) {
        Node* newNode = new Node(item, donor, cat, ts);
        newNode->next = head;
        head = newNode;
        size++;
    }

    Node* getHead() const { return head; }
    int getSize() const { return size; }

    void clear() {
        Node* cur = head;
        while (cur) {
            Node* next = cur->next;
            delete cur;
            cur = next;
        }
        head = nullptr;
        size = 0;
    }

    Node* removeOldest() {
        if (!head) return nullptr;
        if (size == 1) {
            Node* temp = head;
            head = nullptr;
            size = 0;
            return temp;
        }

        Node* cur = head;
        while (cur->next->next) cur = cur->next;
        Node* removed = cur->next;
        cur->next = nullptr;
        size--;
        return removed;
    }

    void display() {
        Node* cur = head;
        int idx = 1;
        while (cur) {
            cout << "\n[" << idx++ << "] Item: " << cur->itemName
                 << " | Donor: " << cur->donorName
                 << " | Category: " << cur->category << endl;
            cur = cur->next;
        }
    }
};
// ─────────────────────────────────────────
//  STACK CLASS (for undo operations)
// ─────────────────────────────────────────
class Stack {
private:
    struct StackNode {
        string itemName;
        string claimerName;
        StackNode* next;

        StackNode(const string& item, const string& claimer)
            : itemName(item), claimerName(claimer), next(nullptr) {}
    };

    StackNode* top;
    int size;

public:
    Stack() : top(nullptr), size(0) {}

    ~Stack() {
        clear();
    }

    void push(const string& itemName, const string& claimerName) {
        StackNode* newNode = new StackNode(itemName, claimerName);
        newNode->next = top;
        top = newNode;
        size++;
    }

    bool pop(string& itemName, string& claimerName) {
        if (!top) return false;
        StackNode* temp = top;
        itemName = temp->itemName;
        claimerName = temp->claimerName;
        top = temp->next;
        delete temp;
        size--;
        return true;
    }

    bool peek(string& itemName, string& claimerName) const {
        if (!top) return false;
        itemName = top->itemName;
        claimerName = top->claimerName;
        return true;
    }

    int getSize() const { return size; }
    bool isEmpty() const { return size == 0; }

    void clear() {
        while (top) {
            StackNode* temp = top;
            top = top->next;
            delete temp;
        }
        size = 0;
    }
};
