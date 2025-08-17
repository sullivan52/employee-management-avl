# Employee Management System

A high-performance employee data management application built in C++ featuring an AVL (self-balancing) tree for guaranteed O(log n) operations.

## Features

- **Self-Balancing AVL Tree**: Guarantees optimal performance for all operations
- **Fast Employee Search**: O(log n) lookup time regardless of data size
- **Comprehensive Data Management**: Store employee IDs, names, departments, titles, manager relationships, and skills
- **Robust CSV Parsing**: Handles quoted fields, special characters, and malformed data gracefully
- **Memory Safe**: Proper memory management with destructors and copy semantics
- **Error Handling**: Comprehensive validation and exception handling
- **User-Friendly Interface**: Clean console interface with input validation

## Performance

| Operation | Time Complexity | Example (1000 employees) |
|-----------|----------------|--------------------------|
| Search Employee | O(log n) | ~10 comparisons max |
| Add Employee | O(log n) | ~10 comparisons max |
| Display All | O(n) | Linear traversal |

## Installation and Setup

### Prerequisites
- C++ compiler with C++11 support
- Visual Studio 2019+ (recommended) or any modern C++ IDE

### Compilation
1. Open `EmployeeManagement.cpp` in Visual Studio
2. Build the project (`Ctrl + Shift + B`)
3. Run without debugging (`Ctrl + F5`)

### Alternative Compilation (Command Line)
```bash
g++ -std=c++11 EmployeeManagement.cpp -o EmployeeManagement
./EmployeeManagement
```

## Usage

### Running the Program
1. Ensure `employees.csv` is in the same directory as the executable
2. Run the program
3. Select from the menu options:
   - **1**: Load Employee Data from CSV
   - **2**: Print Employee Directory (alphabetical by ID)
   - **3**: Search for Specific Employee
   - **9**: Exit

### Sample Session
```
Welcome to the Employee Management System.

1. Load Employee Data.
2. Print Employee Directory.
3. Search for Employee.
9. Exit.

What would you like to do?
1

Attempting to load file: employees.csv
Successfully read 46 lines from employees.csv
Parsing employee data...
Data loading complete: 45 employees loaded
Employee data successfully loaded!
```

## CSV File Format

The system expects a CSV file with the following format:

```csv
EmployeeID,FullName,Department,Title,ManagerID,Skills
EMP001,John Smith,Engineering,Software Engineer,EMP010,"C++,Python,Agile"
EMP002,Jane Doe,Marketing,Marketing Manager,,"Leadership,Strategy,Analytics"
```

### Field Descriptions
- **EmployeeID**: Unique identifier (required, format: EMP###)
- **FullName**: Employee's full name (required)
- **Department**: Department name (optional)
- **Title**: Job title (optional)
- **ManagerID**: ID of direct manager (optional, empty for executives)
- **Skills**: Comma-separated skills list in quotes (optional)

### Data Requirements
- Employee IDs must start with "EMP"
- Names and IDs cannot be empty
- Skills should be enclosed in quotes if containing commas
- Maximum field lengths are validated

## Technical Implementation

### Core Technologies
- **Language**: C++11
- **Data Structure**: AVL (Adelson-Velsky-Landis) Tree
- **Memory Management**: RAII with custom destructors and copy constructors
- **File I/O**: Standard library with exception handling

### Architecture
- **Modular Design**: Separated concerns with dedicated functions for each operation
- **Error Handling**: Try-catch blocks around all file and data operations
- **Input Validation**: Comprehensive validation for both file data and user input
- **Memory Safety**: Proper cleanup and copy semantics prevent memory leaks

### Key Algorithms
- **AVL Rotations**: Four rotation types (LL, RR, LR, RL) maintain tree balance
- **Balance Factor Calculation**: Ensures tree height difference ≤ 1
- **In-Order Traversal**: Provides sorted output without additional sorting

## Project Structure

```
EmployeeManagement/
├── EmployeeManagement.cpp    # Main application source
├── employees.csv            # Sample employee data
└── README.md               # This documentation
```

## Sample Data

The project includes sample data for 45 employees across 6 departments:
- Engineering (8 employees)
- Sales (6 employees)  
- Marketing (5 employees)
- HR (4 employees)
- Finance (5 employees)
- Operations (6 employees)
- Customer Service (5 employees)

## Contributing

This is an academic project demonstrating advanced data structures and C++ programming concepts. 

## License

Educational/Academic use. Created as part of computer science coursework.

## Portfolio
This project is included in my ePortfolio: https://sullivan52.github.io

---

*This project demonstrates proficiency in advanced data structures, algorithm implementation, memory management, and software engineering best practices.*
