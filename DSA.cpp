 #include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>

using namespace std;

// ===== 1. Linked List Node (O(1) Access & O(N) Iteration) =====
struct Node {
    string line;
    Node* next;
};

// ===== Global Variables & O(1) Access Structure (Vector of Pointers) =====
Node* head = NULL;
Node* tail = NULL;
int lineCount = 0;


vector<Node*> linePointers;

// Undo Stack
struct UndoAction {
    enum Type { DELETE, UPDATE, INSERT };
    Type type;
    int lineNum;
    string oldText;
};
stack<UndoAction> undoStack;

// ===== Function Prototypes =====
void showMenu();
void insertLine();
void insertAtLine();
void getLine();
void deleteLine();
void updateLine();
void findString();
void findAndReplace();
void displayText();
void undoLastAction();  // Undo
void displayGUI();      // GUI
void updatePointers();

//  Main
int main() {
    int choice;
    do {
        showMenu();
        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = 0;
        }
        cin.ignore();


        switch (choice) {
        case 1: insertLine(); break;
        case 2: insertAtLine(); break;
        case 3: getLine(); break;
        case 4: deleteLine(); break;
        case 5: updateLine(); break;
        case 6: findString(); break;
        case 7: findAndReplace(); break;
        case 8: displayText(); break;
        case 9: undoLastAction(); break;
        case 10: displayGUI(); break; // (GUI Representation)
        case 11: cout << "Exiting program...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 11);

    Node* current = head;
    while (current) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    return 0;
}

//Menu
void showMenu() {
    cout << "\n=========== MINI TEXT EDITOR ===========\n";
    cout << "1. Insert line at end\n";
    cout << "2. Insert at specific line\n";
    cout << "3. Get line text (O(1))\n";
    cout << "4. Delete line\n";
    cout << "5. Update line (O(1))\n";
    cout << "6. Find string\n";
    cout << "7. Find & Replace\n";
    cout << "8. Display all lines\n";
    cout << "--- Bonus Features ---\n";
    cout << "9. Undo last action\n";
    cout << "10. Display GUI representation\n";
    cout << "11. Exit\n";
    cout << "Choose option: ";
}


void updatePointers() {
    linePointers.clear();
    Node* temp = head;
    while (temp) {
        linePointers.push_back(temp);
        temp = temp->next;
    }
}

// ===== Get line (O(1)) =====
void getLine() {
    int pos;
    cout << "Enter line number: ";
    cin >> pos;


    if (pos < 1 || pos > (int)linePointers.size()) {
        cout << "Invalid line number!\n";
        return;
    }


    Node* targetNode = linePointers[pos - 1];
    cout << "Line " << pos << ": " << targetNode->line << endl;
}


void updateLine() {
    int pos;
    cout << "Enter line number to update: ";
    cin >> pos;
    cin.ignore();

    if (pos < 1 || pos > (int)linePointers.size()) {
        cout << "Invalid line number!\n";
        return;
    }
       //(O(1))
    Node* targetNode = linePointers[pos - 1];

    undoStack.push({ UndoAction::UPDATE, pos, targetNode->line });

    cout << "Enter new text: ";
    getline(cin, targetNode->line);
}



// ===== Insert at end (O(1)) =====
void insertLine() {
    string text;
    cout << "Enter text: ";
    getline(cin, text);

    Node* newNode = new Node{ text, NULL };

    if (!head) {
        head = tail = newNode;
    } else {
        tail->next = newNode;
        tail = newNode;
    }
    lineCount++;


    linePointers.push_back(newNode);


    undoStack.push({ UndoAction::INSERT, lineCount, "" });
}


void insertAtLine() {
    int pos;
    string text;
    cout << "Enter line number: ";
    cin >> pos;
    cin.ignore();

    if (pos < 1 || pos > lineCount + 1) {
        cout << "Invalid line number!\n";
        return;
    }

    cout << "Enter text: ";
    getline(cin, text);
    Node* newNode = new Node{ text, NULL };

    if (pos == 1) {
        newNode->next = head;
        head = newNode;
        if (!tail) tail = newNode;
    } else {

        Node* prev = linePointers[pos - 2];
        newNode->next = prev->next;
        prev->next = newNode;
        if (newNode->next == NULL) tail = newNode;
    }
    lineCount++;


    updatePointers();
    undoStack.push({ UndoAction::INSERT, pos, "" });
}

void deleteLine() {
    int pos;
    cout << "Enter line number to delete: ";
    cin >> pos;

    if (pos < 1 || pos > lineCount) {
        cout << "Invalid line number!\n";
        return;
    }

    Node* temp = NULL;
    string deletedText = "";

    if (pos == 1) {
        temp = head;
        deletedText = head->line;
        head = head->next;
        if (!head) tail = NULL;
    } else {

        Node* prev = linePointers[pos - 2];
        temp = prev->next;
        deletedText = temp->line;
        prev->next = temp->next;
        if (temp == tail) tail = prev;
    }

    undoStack.push({ UndoAction::DELETE, pos, deletedText });

    delete temp;
    lineCount--;


    updatePointers(); // O(N)
}


void undoLastAction() {
    if (undoStack.empty()) {
        cout << "Nothing to undo.\n";
        return;
    }

    UndoAction action = undoStack.top();
    undoStack.pop();

    cout << "Attempting to undo last action (Type: " << action.type << ")... ";

    switch (action.type) {
        case UndoAction::UPDATE: {

            if (action.lineNum >= 1 && action.lineNum <= (int)linePointers.size()) {
                 Node* targetNode = linePointers[action.lineNum - 1];
                 targetNode->line = action.oldText;
                 cout << "Update undone for line " << action.lineNum << ".\n";
            } else {
                 cout << "Error: Target line for update undo not found!\n";
            }
            break;
        }
        case UndoAction::DELETE: {

            int pos = action.lineNum;
            Node* newNode = new Node{ action.oldText, NULL };

            if (pos == 1) {
                     newNode->next = head;
                head = newNode;
                if (!tail) tail = newNode;
            } else {

                Node* prev = head;
                for (int i = 1; i < pos - 1; i++) prev = prev->next;

                newNode->next = prev->next;
                prev->next = newNode;
                if (newNode->next == NULL) tail = newNode;
            }
            lineCount++;
            updatePointers(); // O(N)
            cout << "Deletion undone. Line " << action.lineNum << " restored.\n";
            break;
        }
        case UndoAction::INSERT: {

            int pos = action.lineNum;
            Node* temp = NULL;

            if (pos == 1) {
                temp = head;
                head = head->next;
                if (!head) tail = NULL;
            } else {

                Node* prev = head;
                for (int i = 1; i < pos - 1; i++) prev = prev->next;

                temp = prev->next;
                prev->next = temp->next;
                if (temp == tail) tail = prev;
            }
            delete temp;
            lineCount--;
            updatePointers(); // O(N)
            cout << "Insertion undone. Line " << action.lineNum << " removed.\n";
            break;
        }
    }
}

void displayGUI() {
    cout << "\n\n**************************************************\n";
    cout << "* 🚀 Mini Text Editor (GUI Simulation) 🚀       *\n";
    cout << "**************************************************\n";
    cout << "| File | Edit | View | Help |\n";
    cout << "--------------------------------------------------\n";
    cout << ">>> Console Application - C++\n";
    cout << ">>> Using Linked List + O(1) Access Vector\n";
    cout << "--------------------------------------------------\n";


    displayText();

    cout << "--------------------------------------------------\n";
    cout << "Status: " << lineCount << " line(s) loaded. Ready.\n";
    cout << "**************************************************\n";
}



void displayText() {
    if (linePointers.empty()) {
        cout << "No text to display.\n";
        return;
    }


    Node* temp = head;
    int i = 1;
    while (temp) {
        cout << i << ": " << temp->line << endl;
        temp = temp->next;
        i++;
    }
}

void findString() {
    string key;
    cout << "Enter string to search: ";
    getline(cin, key);

    bool found = false;
    for (size_t i = 0; i < linePointers.size(); ++i) {
        if (linePointers[i]->line.find(key) != string::npos) {
            cout << "Found in line " << i + 1 << endl;
            found = true;
        }
    }
    if (!found)
        cout << "String not found!\n";
}

void findAndReplace() {
    string oldStr, newStr;
    cout << "Enter string to find: ";
    getline(cin, oldStr);
    cout << "Enter string to replace with: ";
    getline(cin, newStr);

    for (size_t i = 0; i < linePointers.size(); ++i) {
        Node* temp = linePointers[i];

        size_t pos = 0;
        while ((pos = temp->line.find(oldStr, pos)) != string::npos) {
            temp->line.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
    }
    cout << "Replacement completed.\n";
}

