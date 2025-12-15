#include <iostream>
#include <string>
#include <stack>

using namespace std;

// ===== Linked List Node =====
struct Node {
    string line;
    Node* next;
};

Node* head = NULL;
Node* tail = NULL;
int lineCount = 0;

// Stack for Undo (stores deleted/updated lines)
stack<string> undoStack;

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

// ===== Main =====
int main() {
    int choice;

    do {
        showMenu();
        cin >> choice;
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
        case 9: cout << "Exiting program...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 9);

    return 0;
}

// ===== Menu =====
void showMenu() {
    cout << "\n=========== MINI TEXT EDITOR ===========\n";
    cout << "1. Insert line\n";
    cout << "2. Insert at specific line\n";
    cout << "3. Get line text\n";
    cout << "4. Delete line\n";
    cout << "5. Update line\n";
    cout << "6. Find string\n";
    cout << "7. Find & Replace\n";
    cout << "8. Display all lines\n";
    cout << "9. Exit\n";
    cout << "Choose option: ";
}

// ===== Insert at end =====
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
}

// ===== Insert at specific line =====
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
        Node* temp = head;
        for (int i = 1; i < pos - 1; i++)
            temp = temp->next;

        newNode->next = temp->next;
        temp->next = newNode;
        if (newNode->next == NULL) tail = newNode;
    }

    lineCount++;
}

// ===== Get line =====
void getLine() {
    int pos;
    cout << "Enter line number: ";
    cin >> pos;

    if (pos < 1 || pos > lineCount) {
        cout << "Invalid line number!\n";
        return;
    }

    Node* temp = head;
    for (int i = 1; i < pos; i++)
        temp = temp->next;

    cout << "Line " << pos << ": " << temp->line << endl;
}

// ===== Delete line =====
void deleteLine() {
    int pos;
    cout << "Enter line number to delete: ";
    cin >> pos;

    if (pos < 1 || pos > lineCount) {
        cout << "Invalid line number!\n";
        return;
    }

    Node* temp = head;

    if (pos == 1) {
        undoStack.push(head->line);
        head = head->next;
        delete temp;
        if (!head) tail = NULL;
    } else {
        Node* prev = NULL;
        for (int i = 1; i < pos; i++) {
            prev = temp;
            temp = temp->next;
        }
        undoStack.push(temp->line);
        prev->next = temp->next;
        if (temp == tail) tail = prev;
        delete temp;
    }

    lineCount--;
}

// ===== Update line =====
void updateLine() {
    int pos;
    cout << "Enter line number to update: ";
    cin >> pos;
    cin.ignore();

    if (pos < 1 || pos > lineCount) {
        cout << "Invalid line number!\n";
        return;
    }

    Node* temp = head;
    for (int i = 1; i < pos; i++)
        temp = temp->next;

    undoStack.push(temp->line);

    cout << "Enter new text: ";
    getline(cin, temp->line);
}

// ===== Find string =====
void findString(

    ) {
    string key;
    cout << "Enter string to search: ";
    getline(cin, key);

    Node* temp = head;
    int lineNum = 1;
    bool found = false;

    while (temp) {
        if (temp->line.find(key) != string::npos) {
            cout << "Found in line " << lineNum << endl;
            found = true;
        }
        temp = temp->next;
        lineNum++;
    }

    if (!found)
        cout << "String not found!\n";
}

// ===== Find and Replace =====
void findAndReplace() {
    string oldStr, newStr;
    cout << "Enter string to find: ";
    getline(cin, oldStr);
    cout << "Enter string to replace with: ";
    getline(cin, newStr);

    Node* temp = head;

    while (temp) {
        size_t pos = 0;
        while ((pos = temp->line.find(oldStr, pos)) != string::npos) {
            temp->line.replace(pos, oldStr.length(), newStr);
            pos += newStr.length();
        }
        temp = temp->next;
    }

    cout << "Replacement completed.\n";
}

// ===== Display =====
void displayText() {
    Node* temp = head;
    int i = 1;

    if (!temp) {
        cout << "No text to display.\n";
        return;
    }

    while (temp) {
        cout << i << ": " << temp->line << endl;
        temp = temp->next;
        i++;
    }
}
