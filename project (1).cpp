#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
#include "node.h"
#include "classes.h"
#include "func.h"


// ─────────────────────────────────────────
//  SORTED LINKED LIST (by item name, A-Z)
// ─────────────────────────────────────────
class SortedLinkedList {
private:
    Node* head;
    int size;

public:
    SortedLinkedList() : head(nullptr), size(0) {}

    ~SortedLinkedList() {
        clear();
    }

    void insert(const string& item, const string& donor, const string& cat, long ts) {
        Node* newNode = new Node(item, donor, cat, ts);

        if (!head || head->itemName > item) {
            newNode->next = head;
            head = newNode;
            size++;
            return;
        }

        Node* cur = head;
        while (cur->next && cur->next->itemName < item) {
            cur = cur->next;
        }

        newNode->next = cur->next;
        cur->next = newNode;
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
//  Linked List for Recent Items
// ─────────────────────────────────────────
LinkedList recentList;

void addToRecent(Item* item) {
    long ts = time(nullptr);
    recentList.addFront(item->name, item->donorName, item->getCategory(), ts);

    if (recentList.getSize() > 10) {
        Node* removed = recentList.removeOldest();
        delete removed;
    }
}

// ─────────────────────────────────────────
//  Sorted Claim History
// ─────────────────────────────────────────
SortedLinkedList sortedClaimHistory;

// ─────────────────────────────────────────
//  Helper: pick condition
// ─────────────────────────────────────────
string pickCondition() {
    cout << "  Condition:" << endl;
    cout << "  1. New" << endl;
    cout << "  2. Good" << endl;
    cout << "  3. Fair" << endl;
    cout << "  > ";
    int c;
    while (!(cin >> c) || c < 1 || c > 3) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Please enter 1, 2, or 3: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    if (c == 1) return "New";
    if (c == 2) return "Good";
    return "Fair";
}

// ─────────────────────────────────────────
//  Main menu
// ─────────────────────────────────────────
int main() {
    cout << "╔══════════════════════════════╗" << endl;
    cout << "║   Welcome to ShareSpace!     ║" << endl;
    cout << "║  Share what you don't need   ║" << endl;
    cout << "╚══════════════════════════════╝" << endl;

    int choice;
    do {
        cout << "─────────────────────────────" << endl;
        cout << " 1. Donate an item" << endl;
        cout << " 2. Browse all items" << endl;
        cout << " 3. Browse by category" << endl;
        cout << " 4. Claim an item" << endl;
        cout << " 5. Search item by name" << endl;
        cout << " 6. Show recently added items" << endl;
        cout << " 7. Show claim history" << endl;
        cout << " 8. Show sorted claim history" << endl;
        cout << " 9. Undo last claim" << endl;
        cout << " 0. Exit" << endl;
        cout << "─────────────────────────────" << endl;
        cout << " > ";

        while (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "  Invalid input. Please enter a number: ";
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if      (choice == 1) addItem();
        else if (choice == 2) browseAll();
        else if (choice == 3) browseByCategory();
        else if (choice == 4) claimItem();
        else if (choice == 5) searchItem();
        else if (choice == 6) displayRecent();
        else if (choice == 7) showClaimHistory();
        else if (choice == 8) showSortedClaimHistory();
        else if (choice == 9) undoLastClaim();
        else if (choice != 0) cout << "  Invalid option.\n";

    } while (choice != 0);

    deleteBST(root);

    cout << "\n  Thanks for using ShareSpace! Bye :)\n" << endl;
    return 0;
}
