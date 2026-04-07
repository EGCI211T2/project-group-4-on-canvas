#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
#include "classes.h"
#include "project.cpp"
#include "func.h"
using namespace std;
// ─────────────────────────────────────────
//  NODE CLASS (3+ data members + link)
// ─────────────────────────────────────────
class Node {
public:
    string itemName;
    string donorName;
    string category;
    long timestamp;
    Node* next;

    Node(const string& item, const string& donor, const string& cat, long ts)
        : itemName(item), donorName(donor), category(cat), timestamp(ts), next(nullptr) {}

    ~Node() {}
};
// ─────────────────────────────────────────
//  Binary Search Tree Storage
// ─────────────────────────────────────────
struct BSTNode {
    Item* item;
    BSTNode* left;
    BSTNode* right;

    BSTNode(Item* i) : item(i), left(nullptr), right(nullptr) {}
    ~BSTNode() {}
};

BSTNode* root = nullptr;
int totalItems = 0;

struct ClaimLog {
    string itemName;
    string donorName;
    string claimerName;

    ClaimLog(const string& item, const string& donor, const string& claimer)
        : itemName(item), donorName(donor), claimerName(claimer) {}
};

vector<ClaimLog> claimLogs;
Stack undoStack;

static string toLowerCase(const string& input) {
    string value = input;
    transform(value.begin(), value.end(), value.begin(), ::tolower);
    return value;
}

BSTNode* insertBST(BSTNode* node, Item* item, bool& inserted) {
    if (!node) {
        inserted = true;
        totalItems++;
        return new BSTNode(item);
    }

    if (item->name < node->item->name) {
        node->left = insertBST(node->left, item, inserted);
    } else if (item->name > node->item->name) {
        node->right = insertBST(node->right, item, inserted);
    } else {
        inserted = false;
    }
    return node;
}

Item* searchBST(BSTNode* node, const string& name) {
    if (!node) return nullptr;
    if (name == node->item->name) return node->item;
    if (name < node->item->name) return searchBST(node->left, name);
    return searchBST(node->right, name);
}

void inorderBST(BSTNode* node, vector<Item*>& items) {
    if (!node) return;
    inorderBST(node->left, items);
    items.push_back(node->item);
    inorderBST(node->right, items);
}

void deleteBST(BSTNode* node) {
    if (!node) return;
    deleteBST(node->left);
    deleteBST(node->right);
    delete node->item;
    delete node;
}


