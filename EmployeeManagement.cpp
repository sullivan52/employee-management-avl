//============================================================================
// Name        : EmployeeManagement.cpp
// Author      : Shayne Sullivan
// Version     : 2.0 (Enhanced)
// Description : Employee Management System using AVL Tree for guaranteed
//               O(log n) performance. Enhanced from original BST implementation
//               with improved memory management, error handling, modular design,
//               and self-balancing tree structure.
//============================================================================

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

struct Employee {
    string employeeId;
    string fullName;
    string department;
    string title;
    string managerId;
    vector<string> skills;
};

// Internal structure for the tree
struct Node {
    Employee employee;
    Node* left;
    Node* right;
    int height;  // Height of subtree rooted at this node

    // Default constructor
    Node() {
        left = nullptr;
        right = nullptr;
        height = 1;  // Leaf nodes have height 1
    }

    // Constructor that accepts an Employee object
    Node(Employee e) : employee(e), left(nullptr), right(nullptr), height(1) {}
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

class BinarySearchTree {

private:
    Node* root;

    Employee searchNode(Node* node, string employeeId);
    void printEmployeeList(Node* node);
    void destroyTree(Node* node);
    Node* copyTree(Node* node);  // Helper to deep copy a tree

    // AVL helper functions:
    int getHeight(Node* node);
    int getBalance(Node* node);
    void updateHeight(Node* node);
    Node* rotateRight(Node* y);
    Node* rotateLeft(Node* x);
    Node* insertNodeAVL(Node* node, Employee employee);  // AVL insert

public:
    BinarySearchTree();
    ~BinarySearchTree();
    BinarySearchTree(const BinarySearchTree& other);                    // Copy constructor
    BinarySearchTree& operator=(const BinarySearchTree& other);        // Assignment operator
    void displayEmployee(const Employee& employee);
    void addEmployee(Employee employee);
    void printEmployeeList();
    Employee findEmployeeById(string employeeId);
};

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    // Initialize empty tree
    root = nullptr;
}

/**
 * Display employee information
 *
 * @param employee The employee object containing the ID, name, department, title, manager, and skills
 */
void BinarySearchTree::displayEmployee(const Employee& employee) {
    cout << "Employee ID: " << employee.employeeId << endl;
    cout << "Full Name: " << employee.fullName << endl;
    cout << "Department: " << employee.department << endl;
    cout << "Title: " << employee.title << endl;
    cout << "Manager ID: ";

    // If there is no manager, output "None" (for executives)
    if (employee.managerId.empty()) {
        cout << "None (Executive Level)";
    }
    else {
        cout << employee.managerId;
    }
    cout << endl;

    cout << "Skills: ";
    // If there are no skills, output "None"
    if (employee.skills.empty()) {
        cout << "None";
    }
    // Else output all skills
    else {
        for (size_t i = 0; i < employee.skills.size(); ++i) {
            cout << employee.skills[i];
            if (i < employee.skills.size() - 1) {
                cout << ", ";  // Add comma between skills
            }
        }
    }
    cout << endl;
}

/**
 * Insert a new employee into the AVL tree
 *
 * @param employee The employee object to be added to the tree
 */
void BinarySearchTree::addEmployee(Employee employee) {
    root = insertNodeAVL(root, employee);  // Use AVL insertion and update root
}

/**
 * Searches the tree for a specific employee by their ID
 */
Employee BinarySearchTree::findEmployeeById(string employeeId) {
    // Begin the search from the root
    return searchNode(root, employeeId);
}

/**
 * Helper function to search for an employee by their ID
 *
 * @param node The current node in the tree
 * @param employeeId The ID of the employee to be found
 * @return The employee being searched, or an empty Employee object if not found
 */
Employee BinarySearchTree::searchNode(Node* node, string employeeId) {
    // Start at the root
    Node* cur = root;

    // Keep looping downwards until we find the matching employeeId or reach the bottom of the tree
    while (cur != nullptr) {
        // Check if we found the employee
        if (employeeId == cur->employee.employeeId) {
            return cur->employee;  // Return the found employee
        }
        // If the employeeId is smaller than the current node's employeeId, move left
        else if (employeeId < cur->employee.employeeId) {
            cur = cur->left;
        }
        // If the employeeId is larger than the current node's employeeId, move right
        else {
            cur = cur->right;
        }
    }

    // If no employee is found, return an empty Employee object
    return Employee();
}

/**
 * Prints the list of employees in the tree in alphanumeric order
 */
void BinarySearchTree::printEmployeeList() {
    // In order traversal starting from the root
    printEmployeeList(root);
}

/**
 * Helper function that prints the employees in alphanumeric order using in-order traversal
 *
 * @param node The current node in the tree
 */
void BinarySearchTree::printEmployeeList(Node* node) {
    if (node != nullptr) {
        printEmployeeList(node->left);  // Traverse the left subtree
        displayEmployee(node->employee);  // Output employee details
        cout << endl; // Add a new line between employees
        printEmployeeList(node->right);  // Traverse the right subtree
    }
}

/**
 * Destructor - prevents memory leaks by cleaning up all nodes
 */
BinarySearchTree::~BinarySearchTree() {
    destroyTree(root);
}

/**
 * Helper function to recursively delete all nodes in the tree
 *
 * @param node The current node to delete (and its subtrees)
 */
void BinarySearchTree::destroyTree(Node* node) {
    if (node != nullptr) {
        destroyTree(node->left);   // Delete left subtree
        destroyTree(node->right);  // Delete right subtree
        delete node;               // Delete current node
    }
}

/**
 * Copy constructor - creates a deep copy of another tree
 *
 * @param other The tree to copy from
 */
BinarySearchTree::BinarySearchTree(const BinarySearchTree& other) {
    root = copyTree(other.root);
}

/**
 * Assignment operator - assigns one tree to another with deep copy
 *
 * @param other The tree to copy from
 * @return Reference to this tree
 */
BinarySearchTree& BinarySearchTree::operator=(const BinarySearchTree& other) {
    if (this != &other) {  // Avoid self-assignment
        // Clean up existing tree
        destroyTree(root);
        // Copy the other tree
        root = copyTree(other.root);
    }
    return *this;
}

/**
 * Helper function to recursively copy a tree
 *
 * @param node The root of the subtree to copy
 * @return Pointer to the root of the new copied subtree
 */
Node* BinarySearchTree::copyTree(Node* node) {
    if (node == nullptr) {
        return nullptr;
    }

    // Create new node with same employee data
    Node* newNode = new Node(node->employee);
    newNode->height = node->height;  // Copy height for AVL

    // Recursively copy left and right subtrees
    newNode->left = copyTree(node->left);
    newNode->right = copyTree(node->right);

    return newNode;
}

/**
 * Get height of a node (0 for null nodes)
 *
 * @param node The node to get height of
 * @return Height of the node
 */
int BinarySearchTree::getHeight(Node* node) {
    return node ? node->height : 0;
}

/**
 * Get balance factor of a node (right height - left height)
 *
 * @param node The node to get balance factor of
 * @return Balance factor (-1, 0, 1 for balanced; -2, +2 for unbalanced)
 */
int BinarySearchTree::getBalance(Node* node) {
    return node ? getHeight(node->right) - getHeight(node->left) : 0;
}

/**
 * Update height of a node based on its children's heights
 *
 * @param node The node to update height for
 */
void BinarySearchTree::updateHeight(Node* node) {
    if (node) {
        node->height = 1 + max(getHeight(node->left), getHeight(node->right));
    }
}

/**
 * Right rotation (for left-heavy trees)
 *
 *     y                    x
 *    / \                  / \
 *   x   T3      -->      T1  y
 *  / \                      / \
 * T1  T2                   T2  T3
 *
 * @param y The unbalanced node
 * @return New root of the subtree
 */
Node* BinarySearchTree::rotateRight(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    // Perform rotation
    x->right = y;
    y->left = T2;

    // Update heights (order matters: y first, then x)
    updateHeight(y);
    updateHeight(x);

    // Return new root
    return x;
}

/**
 * Left rotation (for right-heavy trees)
 *
 *   x                      y
 *  / \                    / \
 * T1  y        -->       x   T3
 *    / \                / \
 *   T2  T3             T1  T2
 *
 * @param x The unbalanced node
 * @return New root of the subtree
 */
Node* BinarySearchTree::rotateLeft(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    // Perform rotation
    y->left = x;
    x->right = T2;

    // Update heights (order matters: x first, then y)
    updateHeight(x);
    updateHeight(y);

    // Return new root
    return y;
}

/**
 * AVL insertion - maintains tree balance automatically
 *
 * @param node Current node (subtree root)
 * @param employee Employee to insert
 * @return New root of the subtree after insertion and balancing
 */
Node* BinarySearchTree::insertNodeAVL(Node* node, Employee employee) {
    // 1. Normal BST insertion
    if (node == nullptr) {
        return new Node(employee);
    }

    if (employee.employeeId < node->employee.employeeId) {
        node->left = insertNodeAVL(node->left, employee);
    }
    else if (employee.employeeId > node->employee.employeeId) {
        node->right = insertNodeAVL(node->right, employee);
    }
    else {
        // Duplicate keys not allowed, return unchanged
        return node;
    }

    // 2. Update height of current node
    updateHeight(node);

    // 3. Get balance factor
    int balance = getBalance(node);

    // 4. If unbalanced, there are 4 rotation cases:

    // Left-Left Case (balance < -1 and new node went to left of left child)
    if (balance < -1 && employee.employeeId < node->left->employee.employeeId) {
        return rotateRight(node);
    }

    // Right-Right Case (balance > 1 and new node went to right of right child)
    if (balance > 1 && employee.employeeId > node->right->employee.employeeId) {
        return rotateLeft(node);
    }

    // Left-Right Case (balance < -1 and new node went to right of left child)
    if (balance < -1 && employee.employeeId > node->left->employee.employeeId) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Right-Left Case (balance > 1 and new node went to left of right child)
    if (balance > 1 && employee.employeeId < node->right->employee.employeeId) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    // Return unchanged node if balanced
    return node;
}

//============================================================================
// Function declarations for main() helpers
//============================================================================
void displayMenu();
int getUserChoice();
bool loadEmployeeData(BinarySearchTree& tree, const string& fileName);
void printEmployeeDirectory(BinarySearchTree& tree, bool dataLoaded);
void searchForEmployee(BinarySearchTree& tree, bool dataLoaded);
bool processMenuChoice(int choice, BinarySearchTree& tree, bool& dataLoaded, const string& fileName);

// New helper function declarations
vector<string> parseCSVLine(const string& line);
vector<string> parseSkills(const string& skillsString);
bool validateEmployeeData(const Employee& employee);

//============================================================================
// Utility Functions for file reading and employee creation
//============================================================================

/**
 * Enhanced CSV parsing function that properly handles quoted fields
 *
 * @param line The CSV line to parse
 * @return Vector of parsed tokens
 */
vector<string> parseCSVLine(const string& line) {
    vector<string> tokens;
    string currentToken = "";
    bool inQuotes = false;

    for (size_t i = 0; i < line.length(); ++i) {
        char c = line[i];

        if (c == '"') {
            // Toggle quote state
            inQuotes = !inQuotes;
        }
        else if (c == ',' && !inQuotes) {
            // End of token - trim whitespace and add to vector
            string trimmed = currentToken;
            // Remove leading/trailing whitespace
            size_t start = trimmed.find_first_not_of(" \t\r\n");
            if (start != string::npos) {
                size_t end = trimmed.find_last_not_of(" \t\r\n");
                trimmed = trimmed.substr(start, end - start + 1);
            }
            else {
                trimmed = "";
            }
            tokens.push_back(trimmed);
            currentToken = "";
        }
        else {
            currentToken += c;
        }
    }

    // Don't forget the last token
    if (!currentToken.empty() || !tokens.empty()) {
        string trimmed = currentToken;
        // Remove leading/trailing whitespace
        size_t start = trimmed.find_first_not_of(" \t\r\n");
        if (start != string::npos) {
            size_t end = trimmed.find_last_not_of(" \t\r\n");
            trimmed = trimmed.substr(start, end - start + 1);
        }
        else {
            trimmed = "";
        }
        tokens.push_back(trimmed);
    }

    return tokens;
}

/**
 * Parse individual skills from a comma-separated skills string
 *
 * @param skillsString The skills string to parse
 * @return Vector of individual skills
 */
vector<string> parseSkills(const string& skillsString) {
    vector<string> skills;
    if (skillsString.empty()) {
        return skills;
    }

    stringstream ss(skillsString);
    string skill;

    while (getline(ss, skill, ',')) {
        // Trim whitespace from each skill
        size_t start = skill.find_first_not_of(" \t\r\n");
        if (start != string::npos) {
            size_t end = skill.find_last_not_of(" \t\r\n");
            skill = skill.substr(start, end - start + 1);
            if (!skill.empty()) {
                skills.push_back(skill);
            }
        }
    }

    return skills;
}

/**
 * Validate employee data for basic integrity
 *
 * @param employee The employee object to validate
 * @return True if employee data is valid, false otherwise
 */
bool validateEmployeeData(const Employee& employee) {
    // Check required fields
    if (employee.employeeId.empty() || employee.fullName.empty()) {
        return false;
    }

    // Check for reasonable field lengths
    if (employee.employeeId.length() > 20 ||
        employee.fullName.length() > 100 ||
        employee.department.length() > 50 ||
        employee.title.length() > 100 ||
        employee.managerId.length() > 20) {
        return false;
    }

    // Check employee ID format (should start with EMP)
    if (employee.employeeId.substr(0, 3) != "EMP") {
        return false;
    }

    return true;
}

/**
 * Enhanced file reading with better error handling
 *
 * @param fileName The name of the file to be read
 * @return A vector containing each line of the file as a string
 */
vector<string> readFile(const string& fileName) {
    vector<string> lines;

    try {
        ifstream file(fileName);

        // Check if the file was opened successfully
        if (!file.is_open()) {
            throw runtime_error("Could not open file: " + fileName);
        }

        string line;
        int lineCount = 0;

        // Read each line and add it to the vector
        while (getline(file, line)) {
            lines.push_back(line);
            lineCount++;
        }

        file.close();

        if (lineCount == 0) {
            throw runtime_error("File is empty: " + fileName);
        }

        cout << "Successfully read " << lineCount << " lines from " << fileName << endl;

    }
    catch (const exception& e) {
        cout << "File reading error: " << e.what() << endl;
        return vector<string>(); // Return empty vector
    }

    return lines;
}

/**
 * Enhanced function to parse the input file with better error handling
 *
 * @param lines The vector of strings created from the input file
 * @return The populated binary search tree with employee objects
 */
BinarySearchTree createEmployee(const vector<string> lines) {
    BinarySearchTree tree;
    int successCount = 0;
    int errorCount = 0;

    cout << "Parsing employee data..." << endl;

    // Skip the first line (header row) by starting at index 1
    for (size_t lineIndex = 1; lineIndex < lines.size(); ++lineIndex) {
        const string& line = lines[lineIndex];

        // Skip empty lines
        if (line.empty()) {
            continue;
        }

        try {
            // Use enhanced CSV parsing
            vector<string> tokens = parseCSVLine(line);

            // Validate minimum required fields
            if (tokens.size() < 2) {
                cout << "Warning: Skipping line " << lineIndex + 1 << " - insufficient data" << endl;
                errorCount++;
                continue;
            }

            // Create employee object safely
            Employee employee;

            // Required fields
            employee.employeeId = tokens[0];
            employee.fullName = tokens[1];

            // Optional fields with bounds checking
            employee.department = (tokens.size() > 2) ? tokens[2] : "";
            employee.title = (tokens.size() > 3) ? tokens[3] : "";
            employee.managerId = (tokens.size() > 4) ? tokens[4] : "";

            // Handle skills field (6th field) with proper parsing
            if (tokens.size() > 5) {
                employee.skills = parseSkills(tokens[5]);
            }

            // Validate the employee data
            if (!validateEmployeeData(employee)) {
                cout << "Warning: Skipping invalid employee data: " << employee.employeeId << endl;
                errorCount++;
                continue;
            }

            // Add the employee to the tree
            tree.addEmployee(employee);
            successCount++;

        }
        catch (const exception& e) {
            cout << "Error processing line " << lineIndex + 1 << ": " << e.what() << endl;
            errorCount++;
            continue;
        }
    }

    cout << "Data loading complete: " << successCount << " employees loaded";
    if (errorCount > 0) {
        cout << " (" << errorCount << " errors)";
    }
    cout << endl;

    return tree;
}

//============================================================================
// Main program functions
//============================================================================

/**
 * Display the main menu options to the user
 */
void displayMenu() {
    cout << "Welcome to the Employee Management System.\n" << endl;
    cout << "1. Load Employee Data." << endl;
    cout << "2. Print Employee Directory." << endl;
    cout << "3. Search for Employee." << endl;
    cout << "9. Exit.\n" << endl;
    cout << "What would you like to do?" << endl;
}

/**
 * Enhanced user choice input with better validation
 *
 * @return Valid menu choice (1, 2, 3, or 9)
 */
int getUserChoice() {
    int choice;
    string input;

    while (true) {
        try {
            getline(cin, input);

            // Check for empty input
            if (input.empty()) {
                cout << "Please enter a choice: ";
                continue;
            }

            // Try to convert to integer
            choice = stoi(input);

            // Validate range (accept any integer, let processMenuChoice handle invalid options)
            cout << endl;
            return choice;

        }
        catch (const invalid_argument&) {
            cout << "Invalid input. Please enter a number: ";
        }
        catch (const out_of_range&) {
            cout << "Number too large. Please enter a valid choice: ";
        }
    }
}

/**
 * Load employee data from CSV file
 *
 * @param tree Reference to the tree to populate
 * @param fileName Name of the CSV file to load
 * @return True if loading was successful, false otherwise
 */
bool loadEmployeeData(BinarySearchTree& tree, const string& fileName) {
    cout << "Attempting to load file: " << fileName << endl;

    vector<string> lines = readFile(fileName);
    if (lines.empty()) {
        cout << "Unable to open file." << endl;
        return false;
    }

    tree = createEmployee(lines);
    cout << "Employee data successfully loaded!" << endl;
    return true;
}

/**
 * Print the complete employee directory
 *
 * @param tree The tree containing employee data
 * @param dataLoaded Whether data has been loaded
 */
void printEmployeeDirectory(BinarySearchTree& tree, bool dataLoaded) {
    if (!dataLoaded) {
        cout << "Please load the employee data first." << endl;
        return;
    }

    cout << "Here is the employee directory:\n" << endl;
    tree.printEmployeeList();
}

/**
 * Search for and display a specific employee
 *
 * @param tree The tree containing employee data
 * @param dataLoaded Whether data has been loaded
 */
void searchForEmployee(BinarySearchTree& tree, bool dataLoaded) {
    if (!dataLoaded) {
        cout << "Please load the employee data first." << endl;
        return;
    }

    string employeeId;
    cout << "Please enter the Employee ID you're looking for:" << endl;
    cin >> employeeId;
    cout << endl;

    // Transform input to accept both upper and lowercase letters
    transform(employeeId.begin(), employeeId.end(), employeeId.begin(), ::toupper);

    Employee employee = tree.findEmployeeById(employeeId);

    // Validate the employee was found
    if (!employee.employeeId.empty()) {
        cout << employeeId << " Information:" << endl;
        tree.displayEmployee(employee);
    }
    else {
        cout << "We're sorry. No employee matching the ID " << employeeId << " was found." << endl;
    }
}

/**
 * Process user menu choice and execute appropriate action
 *
 * @param choice User's menu selection
 * @param tree Reference to the employee tree
 * @param dataLoaded Reference to data loaded flag
 * @param fileName CSV file name
 * @return True to continue program, false to exit
 */
bool processMenuChoice(int choice, BinarySearchTree& tree, bool& dataLoaded, const string& fileName) {
    switch (choice) {
    case 1: {
        dataLoaded = loadEmployeeData(tree, fileName);
        break;
    }
    case 2: {
        printEmployeeDirectory(tree, dataLoaded);
        break;
    }
    case 3: {
        searchForEmployee(tree, dataLoaded);
        break;
    }
    case 9: {
        cout << "Goodbye!" << endl;
        return false; // Signal to exit
    }
    default: {
        cout << choice << " is not a valid option." << endl;
        break;
    }
    }
    return true; // Continue program
}

/**
 * Main program entry point - now clean and focused
 */
int main() {
    BinarySearchTree tree;
    string fileName = "employees.csv";
    bool dataLoaded = false;
    bool continueProgram = true;

    // Main program loop
    while (continueProgram) {
        displayMenu();
        int choice = getUserChoice();
        continueProgram = processMenuChoice(choice, tree, dataLoaded, fileName);
        cout << endl; // Newline for clarity
    }

    return 0;
}