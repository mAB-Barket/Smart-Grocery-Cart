# 📋 PROJECT REPORT

## Smart Grocery Cart
### Data Structures Project - Air University

---

## 📑 Table of Contents
1. [Project Overview](#1-project-overview)
2. [Team & Institution Information](#2-team--institution-information)
3. [Problem Statement](#3-problem-statement)
4. [Objectives](#4-objectives)
5. [Data Structures Implemented](#5-data-structures-implemented)
6. [System Architecture](#6-system-architecture)
7. [Implementation Details](#7-implementation-details)
8. [Time Complexity Analysis](#8-time-complexity-analysis)
9. [Features & Functionality](#9-features--functionality)
10. [Technologies Used](#10-technologies-used)
11. [How to Run](#11-how-to-run)
12. [Screenshots & Demonstrations](#12-screenshots--demonstrations)
13. [Future Enhancements](#13-future-enhancements)
14. [Conclusion](#14-conclusion)
15. [References](#15-references)

---

## 1. Project Overview

**Smart Grocery Cart** is a comprehensive implementation of fundamental data structures through an interactive grocery shopping simulation. The project demonstrates practical, real-world applications of **Arrays, Linked Lists, Stacks, and Queues** in both a web-based interface and a C++ console application.

| **Attribute** | **Details** |
|---------------|-------------|
| **Project Name** | Smart Grocery Cart |
| **Project Type** | Data Structures Implementation |
| **Semester** | 3rd Semester |
| **Subject** | Data Structures |
| **Submission Date** | December 2025 |

---

## 2. Team & Institution Information

| **Field** | **Information** |
|-----------|-----------------|
| **Institution** | Air University |
| **Department** | Computer Science |
| **Semester** | 3rd Semester |
| **Course** | Data Structures |
| **Academic Year** | 2024-2025 |

---

## 3. Problem Statement

Traditional data structure education often lacks practical, real-world context. Students learn theoretical concepts but struggle to see how these structures apply to everyday problems. 

**Challenge:** Design and implement a system that:
- Demonstrates multiple data structures working together
- Provides a practical, relatable use case (grocery shopping)
- Offers both visual (web) and console (C++) implementations
- Clearly shows the advantages and trade-offs of each data structure

---

## 4. Objectives

### Primary Objectives
1. ✅ Implement **Array** with O(1) constant-time access for frequent items
2. ✅ Implement **Singly Linked List** for dynamic shopping cart management
3. ✅ Implement **Stack (LIFO)** for undo functionality
4. ✅ Implement **Queue (FIFO)** for checkout process simulation

### Secondary Objectives
1. ✅ Create an interactive web-based user interface
2. ✅ Develop a C++ console application for viva demonstration
3. ✅ Provide comprehensive documentation and code comments
4. ✅ Demonstrate time complexity differences between data structures

---

## 5. Data Structures Implemented

### 5.1 Array (O(1) Access)

**File:** `cpp/Array.h` | `js/Array.js`

**Concept:**
- Collection of elements stored in contiguous memory locations
- Direct access to any element using its index
- Fixed size allocation

**Use in Project:**
- **Frequent Items List** - Pre-loaded grocery items for instant selection
- Users can access any item by index in constant time

**Visual Representation:**
```
Index:   [0]      [1]      [2]      [3]      [4]    ...
       ┌──────┬──────┬──────┬──────┬──────┐
Data:  │ Milk │Bread │ Eggs │Butter│Cheese│ ...
       └──────┴──────┴──────┴──────┴──────┘
         ↑
    O(1) Direct Access by Index
```

---

### 5.2 Singly Linked List (Dynamic Cart)

**File:** `cpp/LinkedList.h` | `js/LinkedList.js`

**Concept:**
- Linear data structure where elements are not stored contiguously
- Each node contains data and a pointer to the next node
- Dynamic memory allocation allows flexible sizing

**Use in Project:**
- **Shopping Cart** - Add, remove, and manage items dynamically
- No pre-defined size limit
- Efficient insertion and deletion at head

**Visual Representation:**
```
head → [Milk|•]→ [Bread|•]→ [Eggs|•]→ [Cheese|•]→ NULL
         ↑
       Node Structure: [Data | Next Pointer]
```

**Node Structure (C++):**
```cpp
class Node {
private:
    Product data;       // Data stored in the node
    Node* next_node;    // Pointer to next node
public:
    Product retrieve() const;
    Node* next() const;
    void set_data(Product val);
    void set_next(Node* next);
};
```

---

### 5.3 Stack (LIFO - Undo Operations)

**File:** `cpp/Stack.h` | `js/Stack.js`

**Concept:**
- Last In, First Out (LIFO) principle
- Elements added and removed from the same end (top)
- Like a stack of plates

**Use in Project:**
- **Undo Functionality** - Every action is pushed onto the stack
- Pressing "Undo" pops the last action
- Multiple undos work in reverse order of actions

**Visual Representation:**
```
        ┌─────────┐
  TOP → │ Action3 │  ← Last In, First Out
        ├─────────┤
        │ Action2 │
        ├─────────┤
        │ Action1 │
        └─────────┘
```

**Operations:**
```cpp
push(item);    // Add to top      - O(1)
pop();         // Remove from top - O(1)
top();         // View top        - O(1)
```

---

### 5.4 Queue (FIFO - Checkout Process)

**File:** `cpp/Queue.h` | `js/Queue.js`

**Concept:**
- First In, First Out (FIFO) principle
- Elements added at rear, removed from front
- Like a queue of people in line

**Use in Project:**
- **Checkout Process** - Items processed in order they were added
- First item added to checkout is processed first
- Simulates real billing process

**Visual Representation:**
```
FRONT                                    REAR
  ↓                                        ↓
┌──────┬──────┬──────┬──────┬──────┐
│ Milk │Bread │ Eggs │Cheese│ Rice │  ← Enqueue here
└──────┴──────┴──────┴──────┴──────┘
  ↑
Dequeue here (process first)
```

**Operations:**
```cpp
enqueue(item);  // Add to rear     - O(1)
dequeue();      // Remove from front - O(1)
front();        // View front      - O(1)
rear();         // View rear       - O(1)
```

---

## 6. System Architecture

### 6.1 Project Structure

```
DS Project/
│
├── 📄 index.html              # Main HTML file (web interface)
├── 📄 styles.css              # Complete styling (CSS)
├── 📄 datastructures.js       # Original combined JS (legacy)
├── 📄 app.js                  # Original app logic (legacy)
├── 📄 README.md               # Project documentation
├── 📄 PROJECT_REPORT.md       # This report
│
├── 📁 js/                     # JavaScript modules
│   ├── Array.js               # Array implementation
│   ├── LinkedList.js          # Linked List (Shopping Cart)
│   ├── Stack.js               # Stack (Undo operations)
│   ├── Queue.js               # Queue (Checkout process)
│   └── app.js                 # Main application logic
│
└── 📁 cpp/                    # C++ implementations
    ├── Product.h              # Product class definition
    ├── Array.h                # Array implementation
    ├── LinkedList.h           # Linked List implementation
    ├── Stack.h                # Stack implementation
    ├── Queue.h                # Queue implementation
    ├── main.cpp               # Main console application
    ├── DS_QuickReference.cpp  # Viva quick reference
    └── README.md              # C++ documentation
```

### 6.2 Component Interaction

```
┌─────────────────────────────────────────────────────────┐
│                    USER INTERFACE                        │
│         (Web Browser / Console Application)              │
└─────────────────────┬───────────────────────────────────┘
                      │
                      ▼
┌─────────────────────────────────────────────────────────┐
│                APPLICATION LAYER                         │
│              (app.js / main.cpp)                        │
└─────────────────────┬───────────────────────────────────┘
                      │
    ┌─────────────────┼─────────────────┐
    │                 │                 │
    ▼                 ▼                 ▼
┌────────┐      ┌───────────┐     ┌─────────────┐
│ ARRAY  │      │   STACK   │     │    QUEUE    │
│ O(1)   │      │   LIFO    │     │    FIFO     │
│ Access │      │   Undo    │     │  Checkout   │
└────────┘      └───────────┘     └─────────────┘
    │                 │                 │
    └─────────────────┼─────────────────┘
                      │
                      ▼
          ┌───────────────────────┐
          │     LINKED LIST       │
          │   (Shopping Cart)     │
          │   Dynamic Storage     │
          └───────────────────────┘
```

---

## 7. Implementation Details

### 7.1 Product Class (`Product.h`)

The `Product` class serves as the data model for all items:

```cpp
class Product {
private:
    string name;       // Product name
    double price;      // Price per unit
    int quantity;      // Quantity in cart
    int product_id;    // Unique identifier

public:
    // Constructors
    Product();
    Product(string n, double p, int q = 1, int id = 0);
    Product(const Product& other);  // Copy constructor
    
    // Accessors (Getters)
    string getName() const;
    double getPrice() const;
    int getQuantity() const;
    
    // Mutators (Setters)
    void setName(string n);
    void setPrice(double p);
    void setQuantity(int q);
    
    // Utility Methods
    double total() const;           // price * quantity
    bool equals(const Product& other) const;
    void display() const;
};
```

### 7.2 Key Methods Implementation

#### Linked List - Insert at Head O(1)
```cpp
void insertAtHead(Product val) {
    Node* new_node = new Node(val, list_head);
    list_head = new_node;
    item_count++;
}
```

#### Stack - Push Operation O(1)
```cpp
void push(Product val) {
    StackNode* new_node = new StackNode(val, stack_top);
    stack_top = new_node;
    stack_size++;
}
```

#### Queue - Enqueue Operation O(1)
```cpp
void enqueue(Product val) {
    QueueNode* new_node = new QueueNode(val);
    if (empty()) {
        queue_front = queue_rear = new_node;
    } else {
        queue_rear->set_next(new_node);
        queue_rear = new_node;
    }
    queue_size++;
}
```

#### Array - Direct Access O(1)
```cpp
FrequentItem getItem(int index) const {
    if (index < 0 || index >= current_size) {
        return FrequentItem();  // Invalid index
    }
    return items[index];  // O(1) direct access!
}
```

---

## 8. Time Complexity Analysis

### Complete Comparison Table

| Operation | Array | Linked List | Stack | Queue |
|-----------|-------|-------------|-------|-------|
| **Access by Index** | **O(1)** ✅ | O(n) | - | - |
| **Search** | O(n) | O(n) | - | - |
| **Insert at Beginning** | O(n) | **O(1)** ✅ | - | - |
| **Insert at End** | O(1)* | O(n)** | - | - |
| **Delete at Beginning** | O(n) | **O(1)** ✅ | - | - |
| **Delete at End** | O(1) | O(n) | - | - |
| **Push (Top)** | - | - | **O(1)** ✅ | - |
| **Pop (Top)** | - | - | **O(1)** ✅ | - |
| **Peek Top** | - | - | **O(1)** ✅ | - |
| **Enqueue (Rear)** | - | - | - | **O(1)** ✅ |
| **Dequeue (Front)** | - | - | - | **O(1)** ✅ |
| **Peek Front/Rear** | - | - | - | **O(1)** ✅ |

*O(1) if space available
**O(1) with tail pointer

### Space Complexity

| Data Structure | Space Complexity |
|----------------|------------------|
| Array | O(n) - contiguous |
| Linked List | O(n) + pointer overhead |
| Stack | O(n) |
| Queue | O(n) |

---

## 9. Features & Functionality

### 9.1 Web Application Features

| Feature | Data Structure Used | Description |
|---------|---------------------|-------------|
| **Frequent Items** | Array | Quick access to common products |
| **Shopping Cart** | Linked List | Add, remove, update items dynamically |
| **Undo Function** | Stack | Reverse last action (LIFO) |
| **Checkout** | Queue | Process items in order (FIFO) |
| **Visual Animations** | CSS/JS | Animated UI with modern design |
| **Responsive Design** | CSS | Works on all screen sizes |

### 9.2 C++ Console Application Features

| Feature | Menu Option | Description |
|---------|-------------|-------------|
| View Frequent Items | 1 | Display array with O(1) access demo |
| Add Item to Cart | 2 | Select from frequent items |
| Add Custom Item | 3 | Enter custom product details |
| View Cart | 4 | Display linked list contents |
| Remove Item | 5 | Delete from cart |
| Undo Last Action | 6 | Pop from stack |
| Checkout | 7 | Process queue |
| Visualize DS | 8 | ASCII visualization of structures |

---

## 10. Technologies Used

### Web Application
| Technology | Purpose |
|------------|---------|
| HTML5 | Structure and content |
| CSS3 | Styling and animations |
| JavaScript (ES6+) | Logic and interactivity |
| Font Awesome | Icons |
| Google Fonts | Typography (Poppins, Playfair) |

### C++ Application
| Technology | Purpose |
|------------|---------|
| C++11/14 | Core programming language |
| STL (iostream, string) | Input/output and strings |
| Header Files (.h) | Modular code organization |
| Clang++/G++ | Compilation |

### Development Tools
| Tool | Purpose |
|------|---------|
| Visual Studio Code | IDE / Code Editor |
| Clang++ | C++ Compiler |
| Git | Version Control |
| Live Server | Local web server for testing |

---

## 11. How to Run

### 11.1 Web Application

1. Navigate to the project folder
2. Open `index.html` in any modern web browser
3. **No server required** - runs entirely in the browser
4. Interact with the shopping cart interface

### 11.2 C++ Console Application

**Using Command Line:**
```bash
cd cpp
clang++ -g -o grocery_cart.exe main.cpp
./grocery_cart.exe
```

**Using VS Code Tasks:**
1. Open any `.cpp` file in VS Code
2. Press `Ctrl+Shift+B` to build
3. Run the compiled executable

**Sample Console Output:**
```
=============================================
   SMART GROCERY CART
   Data Structures Project
   Air University - 3rd Semester
=============================================

Data Structures Used:
  1. Array        - O(1) access for frequent items
  2. Linked List  - Dynamic shopping cart
  3. Stack (LIFO) - Undo operations
  4. Queue (FIFO) - Checkout process
=============================================

============== MAIN MENU ==============
  1. View Frequent Items (Array)
  2. Add Item to Cart
  3. Add Custom Item
  4. View Cart (Linked List)
  5. Remove Item from Cart
  6. Undo Last Action (Stack)
  7. Checkout (Queue)
  8. Visualize Data Structures
  0. Exit
========================================
```

---

## 12. Screenshots & Demonstrations

### Web Interface Components

#### Hero Section
- Animated background with floating shapes
- Interactive cart visualization with orbiting data structure labels
- Statistics display (4 Data Structures, 10+ Products, ∞ Undo Actions)

#### Shopping Section
- Grid layout of products with hover effects
- Add to cart functionality with quantity selection
- Real-time cart updates

#### Cart Panel
- Sliding panel showing cart contents (Linked List visualization)
- Item quantity adjustment
- Undo button (Stack demonstration)
- Checkout button (Queue processing)

### C++ Console Visualization
```
========== DATA STRUCTURE VISUALIZATION ==========

----- ARRAY (Frequent Items) -----
[0] Milk ($3.99)
[1] Bread ($2.49)
[2] Eggs ($4.99)
...
^ O(1) access by index!

----- LINKED LIST (Shopping Cart) -----
HEAD -> [Milk] -> [Bread] -> [Eggs] -> NULL
Total Items: 3

----- STACK (Undo History) -----
  TOP
   |
   v
[Added: Eggs]
[Added: Bread]
[Added: Milk]
  BOTTOM
Last action can be undone first (LIFO)

----- QUEUE (Checkout) -----
FRONT -> [Milk] -> [Bread] -> [Eggs] <- REAR
First item added will be processed first (FIFO)
```

---

## 13. Future Enhancements

### Short-term Improvements
- [ ] Add search functionality using Binary Search Tree
- [ ] Implement sorting algorithms for cart items
- [ ] Add price range filtering using Heap
- [ ] User authentication and session management

### Long-term Enhancements
- [ ] Database integration (SQL/NoSQL)
- [ ] RESTful API backend
- [ ] Mobile application (React Native)
- [ ] Machine learning for product recommendations
- [ ] Inventory management system

### Additional Data Structures to Implement
- [ ] **Binary Search Tree** - Efficient searching
- [ ] **Hash Table** - O(1) average lookup
- [ ] **Heap/Priority Queue** - Price-based sorting
- [ ] **Graph** - Product relationships/recommendations

---

## 14. Conclusion

The **Smart Grocery Cart** project successfully demonstrates the practical application of four fundamental data structures:

1. **Array** - Proves O(1) constant-time access advantage for frequent items
2. **Linked List** - Shows dynamic memory management for flexible cart operations
3. **Stack** - Implements LIFO principle for intuitive undo functionality
4. **Queue** - Demonstrates FIFO principle for realistic checkout simulation

### Key Learnings
- Each data structure has specific strengths and use cases
- Time complexity analysis is crucial for choosing the right structure
- Real-world applications make theoretical concepts tangible
- Clean code organization (header files, modules) improves maintainability

### Project Outcomes
- ✅ Dual implementation (Web + Console) increases understanding
- ✅ Comprehensive documentation supports viva preparation
- ✅ Modern UI demonstrates real-world application potential
- ✅ Modular code structure allows easy extension

---

## 15. References

### Books
1. Cormen, T. H., et al. *Introduction to Algorithms* (3rd Edition)
2. Weiss, Mark Allen. *Data Structures and Algorithm Analysis in C++*
3. Sedgewick, Robert. *Algorithms in C++*

### Online Resources
1. GeeksforGeeks - Data Structures (www.geeksforgeeks.org)
2. Visualgo - Data Structure Visualizations (www.visualgo.net)
3. MDN Web Docs - JavaScript (developer.mozilla.org)
4. cppreference.com - C++ Reference

### Course Materials
- Air University Data Structures Course Slides
- Lab Manual and Practice Problems

---

## Appendix A: Glossary

| Term | Definition |
|------|------------|
| **Node** | A basic unit of a linked structure containing data and pointers |
| **Pointer** | A variable that stores the memory address of another variable |
| **LIFO** | Last In, First Out - stack ordering principle |
| **FIFO** | First In, First Out - queue ordering principle |
| **Time Complexity** | Measure of algorithm efficiency as input grows |
| **O(1)** | Constant time - operation takes same time regardless of input size |
| **O(n)** | Linear time - operation time grows linearly with input size |

---

## Appendix B: Code Statistics

| Metric | Value |
|--------|-------|
| Total Lines of Code (C++) | ~1,500+ |
| Total Lines of Code (JS) | ~1,000+ |
| Number of Classes (C++) | 8 |
| Number of Header Files | 5 |
| Number of JS Modules | 5 |

---

**Document Prepared:** December 2025  
**Project Status:** ✅ Complete  
**Version:** 1.0

---

*This project report is submitted as part of the Data Structures course requirements at Air University.*
