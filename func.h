#include <iostream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>
#include <ctime>
using namespace std;
#include "node.h"
#include "classes.h"
#include "project.cpp"

// ─────────────────────────────────────────
//  Add item
// ─────────────────────────────────────────
void addItem() {
    string name, donor;
    cout << "\n  Item name : "; getline(cin, name);
    if (name.empty()) {
        cout << "  Item name cannot be empty.\n";
        return;
    }

    cout << "  Your name : "; getline(cin, donor);
    if (donor.empty()) {
        cout << "  Donor name cannot be empty.\n";
        return;
    }

    string cond = pickCondition();

    cout << "\n  Category:" << endl;
    cout << "  1. Electronics" << endl;
    cout << "  2. Book" << endl;
    cout << "  3. Clothing" << endl;
    cout << "  4. Furniture" << endl;
    cout << "  5. Other" << endl;
    cout << "  > ";
    int cat;
    while (!(cin >> cat) || cat < 1 || cat > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Please enter 1-5: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    Item* item = nullptr;
    if (cat == 1) {
        string brand;
        cout << "  Brand     : "; getline(cin, brand);
        item = new Electronics(name, cond, donor, brand);
    } else if (cat == 2) {
        string author;
        cout << "  Author    : "; getline(cin, author);
        item = new Book(name, cond, donor, author);
    } else if (cat == 3) {
        string size;
        cout << "  Size (S/M/L/XL) : "; getline(cin, size);
        item = new Clothing(name, cond, donor, size);
    } else if (cat == 4) {
        item = new Furniture(name, cond, donor);
    } else {
        item = new Item(name, cond, donor);
    }

    bool inserted = false;
    root = insertBST(root, item, inserted);
    if (!inserted) {
        delete item;
        cout << "\n  An item with this name already exists.\n";
        return;
    }

    addToRecent(item);
    cout << "\n  ✓ Item added to ShareSpace!\n" << endl;
}

// ─────────────────────────────────────────
//  Browse all items
// ─────────────────────────────────────────
void browseAll() {
    if (!root) {
        cout << "\n  No items on the shelf yet.\n" << endl;
        return;
    }

    vector<Item*> items;
    inorderBST(root, items);

    cout << "\n========== ALL ITEMS (A-Z) ==========" << endl;
    int count = 0;
    for (size_t i = 0; i < items.size(); i++) {
        if (!items[i]->isClaimed) {
            cout << "\n[" << (i + 1) << "] Category: " << items[i]->getCategory() << endl;
            items[i]->displayInfo();
            count++;
        }
    }
    if (count == 0) {
        cout << "\n  No available items right now.\n";
    }
    cout << "====================================\n" << endl;
}

// ─────────────────────────────────────────
//  Browse by category
// ─────────────────────────────────────────
void browseByCategory() {
    cout << "\n  Filter by category:" << endl;
    cout << "  1. Electronics" << endl;
    cout << "  2. Book" << endl;
    cout << "  3. Clothing" << endl;
    cout << "  4. Furniture" << endl;
    cout << "  5. Other" << endl;
    cout << "  > ";

    int cat;
    while (!(cin >> cat) || cat < 1 || cat > 5) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "  Invalid input. Please enter 1-5: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string target;
    if (cat == 1) target = "Electronics";
    else if (cat == 2) target = "Book";
    else if (cat == 3) target = "Clothing";
    else if (cat == 4) target = "Furniture";
    else target = "General";

    vector<Item*> items;
    inorderBST(root, items);

    cout << "\n===== " << target << " =====" << endl;
    bool found = false;
    for (size_t i = 0; i < items.size(); i++) {
        if (!items[i]->isClaimed && items[i]->getCategory() == target) {
            cout << "\n[" << (i + 1) << "]" << endl;
            items[i]->displayInfo();
            found = true;
        }
    }
    if (!found) cout << "  No items in this category.\n";
    cout << endl;
}

// ─────────────────────────────────────────
//  Search by name (partial, case-insensitive)
// ─────────────────────────────────────────
void searchItem() {
    if (!root) {
        cout << "\n  No items in the database yet.\n" << endl;
        return;
    }

    string query;
    cout << "\n  Enter item name to search (partial match, case-insensitive): ";
    getline(cin, query);
    if (query.empty()) {
        cout << "  Item name cannot be empty.\n";
        return;
    }

    vector<Item*> items;
    inorderBST(root, items);

    vector<Item*> matched;
    string qlower = toLowerCase(query);
    for (Item* it : items) {
        string nameLower = toLowerCase(it->name);
        if (nameLower.find(qlower) != string::npos) {
            matched.push_back(it);
        }
    }

    if (matched.empty()) {
        cout << "\n  Item not found.\n" << endl;
        return;
    }

    cout << "\n===== Search results =====" << endl;
    for (size_t i = 0; i < matched.size(); i++) {
        cout << "\n[" << i + 1 << "] Category: " << matched[i]->getCategory() << endl;
        matched[i]->displayInfo();
    }

    while (true) {
        cout << "\n  Enter search item number to claim (0 to return): ";
        string input;
        getline(cin, input);
        if (input.empty()) continue;

        if (input == "0") break;

        try {
            int sel = stoi(input);
            if (sel < 1 || sel > (int)matched.size()) {
                cout << "  Invalid selection. Try again.\n";
                continue;
            }

            Item* target = matched[sel - 1];
            if (!target->isClaimed) {
                cout << "  Enter your name to claim '" << target->name << "': ";
                string claimer;
                getline(cin, claimer);
                if (!claimer.empty()) {
                    target->isClaimed = true;
                    claimLogs.push_back(ClaimLog(target->name, target->donorName, claimer));
                    sortedClaimHistory.insert(target->name, target->donorName, target->getCategory(), time(nullptr));
                    undoStack.push(target->name, claimer);
                    cout << "\n  ✓ You claimed: " << target->name << "! Enjoy it :)\n";
                    cout << "\n  (Donor: " << target->donorName << ", Claimer: " << claimer << ")\n";
                    break;
                }
            } else {
                cout << "  Sorry, this item is already claimed.\n";
            }
        } catch (...) {
            cout << "  Please enter a valid number.\n";
        }
    }

    cout << endl;
}

// ─────────────────────────────────────────
//  Claim item (paginated selection)
// ─────────────────────────────────────────
void claimItem() {
    if (!root) {
        cout << "\n  No items available to claim.\n" << endl;
        return;
    }

    vector<Item*> items;
    inorderBST(root, items);

    vector<Item*> unclaimed;
    for (Item* it : items) {
        if (!it->isClaimed) {
            unclaimed.push_back(it);
        }
    }

    if (unclaimed.empty()) {
        cout << "\n  No available unclaimed items.\n" << endl;
        return;
    }

    int page = 0;
    const int pageSize = 5;

    while (true) {
        int start = page * pageSize;
        int end = min(start + pageSize, (int)unclaimed.size());

        cout << "\n===== Claim Items (" << (page + 1) << "/" << ((int)(unclaimed.size() + pageSize - 1) / pageSize) << ") =====" << endl;
        for (int i = start; i < end; i++) {
            cout << "\n[" << (i - start + 1) << "] Category: " << unclaimed[i]->getCategory() << endl;
            unclaimed[i]->displayInfo();
        }

        cout << "\n  n. Next page" << endl;
        cout << "  p. Previous page" << endl;
        cout << "  0. Cancel" << endl;
        cout << "  Enter item number to claim: ";

        string input;
        getline(cin, input);
        if (input.empty()) continue;

        if (input == "0") break;
        if (input == "n" || input == "N") {
            if (end >= (int)unclaimed.size()) {
                cout << "  No more pages.\n";
            } else {
                page++;
            }
            continue;
        }
        if (input == "p" || input == "P") {
            if (page == 0) {
                cout << "  Already at first page.\n";
            } else {
                page--;
            }
            continue;
        }

        try {
            int sel = stoi(input);
            if (sel < 1 || sel > (end - start)) {
                cout << "  Invalid selection. Choose a visible item number.\n";
                continue;
            }
            Item* target = unclaimed[start + sel - 1];
            if (!target->isClaimed) {
                cout << "  Enter your name to claim '" << target->name << "': ";
                string claimer;
                getline(cin, claimer);
                if (!claimer.empty()) {
                    target->isClaimed = true;
                    claimLogs.push_back(ClaimLog(target->name, target->donorName, claimer));
                    sortedClaimHistory.insert(target->name, target->donorName, target->getCategory(), time(nullptr));
                    undoStack.push(target->name, claimer);
                    cout << "\n  ✓ You claimed: " << target->name << "! Enjoy it :)\n";
                    cout << "\n  (Donor: " << target->donorName << ", Claimer: " << claimer << ")\n";
                    break;
                }
            } else {
                cout << "  This item is already claimed.\n";
            }
        } catch (...) {
            cout << "  Please enter a valid option.\n";
        }
    }

    cout << endl;
}

// ─────────────────────────────────────────
//  Display recent items with claim option
// ─────────────────────────────────────────
void displayRecent() {
    if (recentList.getSize() == 0) {
        cout << "\n  No recent items yet.\n" << endl;
        return;
    }

    cout << "\n===== Recently Added Items =====" << endl;
    recentList.display();
    cout << "===============================\n" << endl;

    while (true) {
        cout << "  Enter recent item number to claim (0 to return): ";
        string input;
        getline(cin, input);
        if (input.empty()) continue;

        if (input == "0") break;

        try {
            int sel = stoi(input);
            int idx = 1;
            Node* cur = recentList.getHead();
            while (cur && idx < sel) {
                cur = cur->next;
                idx++;
            }

            if (!cur) {
                cout << "  Invalid selection.\n";
                continue;
            }

            Item* item = searchBST(root, cur->itemName);
            if (item && !item->isClaimed) {
                cout << "  Enter your name to claim '" << item->name << "': ";
                string claimer;
                getline(cin, claimer);
                if (!claimer.empty()) {
                    item->isClaimed = true;
                    claimLogs.push_back(ClaimLog(item->name, item->donorName, claimer));
                    sortedClaimHistory.insert(item->name, item->donorName, item->getCategory(), time(nullptr));
                    undoStack.push(item->name, claimer);
                    cout << "\n  ✓ You claimed: " << item->name << "! Enjoy it :)\n";
                    cout << "\n  (Donor: " << item->donorName << ", Claimer: " << claimer << ")\n";
                    break;
                }
            } else {
                cout << "  Item not available.\n";
            }
        } catch (...) {
            cout << "  Please enter a valid number.\n";
        }
    }
}

// ─────────────────────────────────────────
//  Show claim logs
// ─────────────────────────────────────────
void showClaimHistory() {
    if (claimLogs.empty()) {
        cout << "\n  No claim interactions logged yet.\n" << endl;
        return;
    }

    cout << "\n===== Claim History =====" << endl;
    for (size_t i = 0; i < claimLogs.size(); i++) {
        cout << "\n[" << i + 1 << "] Item: " << claimLogs[i].itemName << " | Donor: " << claimLogs[i].donorName << " | Claimer: " << claimLogs[i].claimerName << endl;
    }
    cout << "=========================\n" << endl;
}

// ─────────────────────────────────────────
//  Show sorted claim history (by item name)
// ─────────────────────────────────────────
void showSortedClaimHistory() {
    if (sortedClaimHistory.getSize() == 0) {
        cout << "\n  No sorted claim history yet.\n" << endl;
        return;
    }

    cout << "\n===== Sorted Claim History (A-Z by Item Name) =====" << endl;
    sortedClaimHistory.display();
    cout << "================================================\n" << endl;
}

// ─────────────────────────────────────────
//  Undo last claim
// ─────────────────────────────────────────
void undoLastClaim() {
    string itemName, claimerName;
    if (undoStack.pop(itemName, claimerName)) {
        Item* item = searchBST(root, itemName);
        if (item) {
            item->isClaimed = false;
            if (!claimLogs.empty()) {
                claimLogs.pop_back();
            }
            cout << "\n  ✓ Claim undone: " << itemName << " is now available again.\n";
            cout << "  (Unclaimed by: " << claimerName << ")\n\n";
        }
    } else {
        cout << "\n  No claims to undo.\n" << endl;
    }
}
