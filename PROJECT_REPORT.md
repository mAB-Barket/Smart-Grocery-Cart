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

**Smart Grocery Cart** is a comprehensive web-based application that demonstrates practical applications of fundamental data structures through an interactive grocery shopping simulation. The project showcases **Arrays, Linked Lists, Stacks, and Queues** implemented in C++ and accessed via a Python Flask backend with a modern web interface.

| **Attribute** | **Details** |
|---------------|-------------|
| **Project Name** | Smart Grocery Cart |
| **Project Type** | Data Structures Web Application |
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

- Demonstrates multiple data structures working together in a cohesive application
- Provides a practical, relatable use case (grocery shopping)
- Offers a modern web-based user interface
- Clearly shows the advantages and trade-offs of each data structure
- Implements data structures in C++ with a web frontend

---

## 4. Objectives

### Primary Objectives
1. ✅ Implement **Array** with O(1) constant-time access for frequent items
2. ✅ Implement **Singly Linked List** for dynamic shopping cart management
3. ✅ Implement **Stack (LIFO)** for undo functionality
4. ✅ Implement **Queue (FIFO)** for checkout process simulation
5. ✅ Create a web-based interface using HTML, CSS, and JavaScript
6. ✅ Develop a Python Flask backend to bridge web frontend with C++ data structures
7. ✅ Use ctypes for Python-C++ interoperability

### Secondary Objectives
1. ✅ Provide comprehensive documentation and code comments
2. ✅ Demonstrate time complexity differences between data structures
3. ✅ Implement responsive web design
4. ✅ Include data persistence and visualization features

---

## 5. Data Structures Implemented

### 5.1 Array (O(1) Access)

**File:** `src/core/Array.h`

**Concept:**
- Collection of elements stored in contiguous memory locations
- Direct access to any element using its index
- Fixed size allocation

**Use in Project:**
- **Frequent Items List** - Pre-loaded grocery items for instant selection
- Users can access any item by index in constant time
- Items are sorted by purchase frequency

**Visual Representation:**

```text
Index:   [0]      [1]      [2]      [3]      [4]    ...
       ┌──────┬──────┬──────┬──────┬──────┐
Data:  │ Milk │Bread │ Eggs │Butter│Cheese│ ...
       └──────┴──────┴──────┴──────┴──────┘
         ↑
    O(1) Direct Access by Index
```

### 5.2 Singly Linked List (Dynamic Cart)

**File:** `src/core/LinkedList.h`

**Concept:**
- Linear data structure where elements are not stored contiguously
- Each node contains data and a pointer to the next node
- Dynamic memory allocation allows flexible sizing

**Use in Project:**
- **Shopping Cart** - Add, remove, and manage items dynamically
- No pre-defined size limit
- Efficient insertion and deletion operations

**Visual Representation:**

```text
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

### 5.3 Stack (LIFO - Undo Operations)

**File:** `src/core/Stack.h`

**Concept:**
- Last In, First Out (LIFO) principle
- Elements added and removed from the same end (top)
- Like a stack of plates

**Use in Project:**
- **Undo Functionality** - Every action is pushed onto the stack
- Pressing "Undo" pops the last action
- Multiple undos work in reverse order of actions

**Visual Representation:**

```text
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

### 5.4 Queue (FIFO - Checkout Process)

**File:** `src/core/Queue.h`

**Concept:**
- First In, First Out (FIFO) principle
- Elements added at rear, removed from front
- Like a queue of people in line

**Use in Project:**
- **Checkout Process** - Items processed in order they were added
- First item added to checkout is processed first
- Simulates real billing process

**Visual Representation:**

```text
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

```text
DS Project/
│
├── 📄 README.md                 # Project documentation
├── 📄 PROJECT_REPORT.md         # This report
├── 📄 build.bat                 # Build script (Windows)
├── 📄 START.bat                 # Run script (Windows)
│
├── 📁 src/                      # Source code
│   │
│   ├── 📁 core/                 # C++ Data Structure Implementations
│   │   ├── Product.h            # Product class (OOP concepts)
│   │   ├── Node.h               # Node class (self-referential)
│   │   ├── Array.h              # Array with O(1) access
│   │   ├── LinkedList.h         # Singly Linked List (Cart)
│   │   ├── Stack.h              # Stack - LIFO (Undo)
│   │   └── Queue.h              # Queue - FIFO (Checkout)
│   │
│   ├── grocery_api.cpp          # C++ DLL source (exports functions)
│   ├── grocery_api.dll          # Compiled DLL (Windows)
│   ├── server.py                 # Flask server (Python bridge)
│   └── cart_data.json            # Initial data
│
├── 📁 web/                      # Web Interface
│   ├── index.html               # Main HTML file
│   ├── 📁 css/
│   │   └── styles.css           # Styling and animations
│   └── 📁 js/
│       └── app.js               # Frontend logic and API calls
│
└── 📁 docs/                     # Documentation
    ├── COURSE_CONCEPTS.md       # Code-to-concept mapping
    └── VIVA_NOTES.md            # Viva preparation guide
```

### 6.2 Component Interaction

```text
┌─────────────────────────────────────────────────────────┐
│                    WEB BROWSER                           │
│              (HTML/CSS/JavaScript)                       │
└─────────────────────┬───────────────────────────────────┘
                      │ HTTP Requests
                      ▼
┌─────────────────────────────────────────────────────────┐
│                FLASK WEB SERVER (Python)                 │
│              (server.py - API endpoints)                │
└─────────────────────┬───────────────────────────────────┘
                      │ ctypes calls
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

### 7.1 Product Class (`src/core/Product.h`)

The `Product` class serves as the data model for all items:

```cpp
class Product {
private:
    string name;       // Product name
    int quantity;      // Quantity in cart
    int product_id;    // Unique identifier

public:
    // Constructors
    Product();
    Product(string n, int q = 1, int id = 0);
    Product(const Product& other);  // Copy constructor

    // Accessors (Getters)
    string getName() const;
    int getQuantity() const;

    // Mutators (Setters)
    void setName(string n);
    void setQuantity(int q);

    // Utility Methods
    bool equals(const Product& other) const;
    void display() const;
};
```

### 7.2 C++ DLL Implementation (`src/grocery_api.cpp`)

The C++ code is compiled into a DLL that exports functions callable from Python:

```cpp
// Global data structures
static FrequentItemsArray frequentItems;  // Array for O(1) access
static LinkedList cart;                    // Linked List for shopping cart
static Stack undoStack;                    // Stack for undo operations
static Queue checkoutQueue;                // Queue for checkout process

// Export functions for Python ctypes
EXPORT const char* api_get_frequent_items_count() {
    // Return JSON string with count
}

EXPORT const char* api_add_to_cart(const char* item_json) {
    // Parse JSON, add to cart, push to undo stack
}
```

### 7.3 Python Flask Backend (`src/server.py`)

The Flask server loads the C++ DLL and provides REST API endpoints:

```python
from flask import Flask, jsonify, request
import ctypes

# Load C++ DLL
grocery_lib = ctypes.CDLL('grocery_api.dll')

# Define function signatures
grocery_lib.api_get_frequent_items_count.restype = ctypes.c_int

@app.route('/api/frequent-items')
def get_frequent_items():
    count = grocery_lib.api_get_frequent_items_count()
    # Return JSON response
```

### 7.4 Web Frontend (`web/js/app.js`)

JavaScript handles UI interactions and API calls:

```javascript
// Add item to cart
async function addToCart(itemId) {
    const response = await fetch('/api/add-to-cart', {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify({ id: itemId })
    });
    const result = await response.json();
    updateUI(result);
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
| **Frequent Items Display** | Array | Quick access to common products with O(1) indexing |
| **Shopping Cart Management** | Linked List | Add, remove, update items dynamically |
| **Undo Function** | Stack | Reverse last action (LIFO) with visual feedback |
| **Checkout Process** | Queue | Process items in order (FIFO) with receipt generation |
| **Visual Animations** | CSS/JS | Animated UI with modern design and transitions |
| **Responsive Design** | CSS | Works on all screen sizes with mobile support |
| **Real-time Updates** | AJAX | Live cart updates without page refresh |
| **Data Persistence** | Local Storage | Cart state saved in browser |

### 9.2 API Endpoints

| Endpoint | Method | Description |
|----------|--------|-------------|
| `/api/frequent-items` | GET | Get all frequent items from array |
| `/api/cart` | GET | Get current cart contents (linked list) |
| `/api/add-to-cart` | POST | Add item to cart and undo stack |
| `/api/remove-from-cart` | POST | Remove item from cart |
| `/api/undo` | POST | Undo last action (stack pop) |
| `/api/checkout` | POST | Process checkout (move to queue) |
| `/api/reset` | POST | Clear all data structures |

---

## 10. Technologies Used

### Frontend (Web Interface)
| Technology | Purpose |
|------------|---------|
| HTML5 | Structure and semantic markup |
| CSS3 | Styling, animations, and responsive design |
| JavaScript (ES6+) | Client-side logic and API interactions |
| PWA | Progressive Web App for Android installation |
| Font Awesome | Icons and visual elements |
| Google Fonts | Typography (Poppins, Inter) |

### Backend (Python)
| Technology | Purpose |
|------------|---------|
| Python 3.x | Server-side scripting |
| Flask | Web framework for API endpoints |
| Flask-CORS | Cross-origin resource sharing |
| ctypes | Python-C interface for C++ DLL |

### C++ Implementation
| Technology | Purpose |
|------------|---------|
| C++11/14 | Core programming language |
| STL (string, vector) | Standard library utilities |
| Header Files (.h) | Modular code organization |
| Clang++ | C++ compiler for DLL creation |

### Development Tools
| Tool | Purpose |
|------|---------|
| Visual Studio Code | IDE with C++ and Python support |
| Clang++ | C++ compilation to DLL |
| Python pip | Package management |
| Git | Version control |
| Chrome DevTools | Web debugging |

---

## 11. How to Run

### Prerequisites
1. **Python 3.x** - [Download here](https://www.python.org/downloads/)
   - ⚠️ Check "Add Python to PATH" during installation!
2. **C++ Compiler** - Clang++ (comes with VS Code or LLVM)
3. **Git** - For cloning (optional)

### Option 1: Automated Setup (Recommended)
```bash
# Double-click START.bat (Windows)
# or run in terminal:
START.bat
```
This will:
1. Compile the C++ DLL
2. Install Python dependencies (Flask, Flask-CORS)
3. Start the Flask server on <http://localhost:5000>
4. Open the web application in browser

### Option 2: Manual Setup
```bash
# 1. Compile C++ DLL
cd src
clang++ -shared -o grocery_api.dll grocery_api.cpp -std=c++17

# 2. Install Python dependencies
pip install flask flask-cors

# 3. Start the server
python server.py

# 4. Open <http://localhost:5000> in browser
```

### Option 3: Using Build Script
```bash
# Run build.bat for compilation only
build.bat
```

---

## 12. Screenshots & Demonstrations

### Web Interface Components

#### Hero Section
- Animated background with floating geometric shapes
- Interactive cart visualization with orbiting data structure labels
- Statistics display showing active data structures

#### Shopping Section
- Grid layout of frequent items with hover effects
- Add to cart functionality with quantity management
- Real-time cart counter updates

#### Cart Panel
- Sliding sidebar showing cart contents
- Item quantity adjustment controls
- Undo button with stack visualization
- Checkout button initiating queue processing

#### Checkout Modal
- Receipt generation with queue processing
- Item-by-item processing animation
- Final receipt display

### Data Structure Visualizations
```text
========== DATA STRUCTURE VISUALIZATION ==========

----- ARRAY (Frequent Items) -----
[0] Milk (2 purchases)
[1] Bread (2 purchases)
[2] Eggs (2 purchases)
...
^ O(1) access by index!

----- LINKED LIST (Shopping Cart) -----
HEAD → [Milk] → [Bread] → [Eggs] → NULL
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
FRONT → [Milk] → [Bread] → [Eggs] ← REAR
First item added will be processed first (FIFO)
```

---

## 13. Future Enhancements

### Short-term Improvements
- [ ] Add search functionality across frequent items
- [ ] Implement item categories and filtering
- [ ] Add price tracking and budget calculations
- [ ] User session management and cart persistence
- [ ] Export cart as shopping list

### Long-term Enhancements
- [ ] Database integration (SQLite/PostgreSQL)
- [ ] User authentication and multi-user support
- [x] ~~Mobile application (React Native/PWA)~~ ✅ **IMPLEMENTED** - PWA support added!
- [ ] Machine learning for purchase predictions
- [ ] Inventory management and stock tracking

### Additional Data Structures to Implement
- [ ] **Binary Search Tree** - Efficient searching and sorting
- [ ] **Hash Table** - O(1) average lookup for item search
- [ ] **Priority Queue** - Price-based or category-based ordering
- [ ] **Graph** - Product relationships and recommendations

---

## 14. Conclusion

The **Smart Grocery Cart** project successfully demonstrates the practical application of four fundamental data structures in a modern web application:

1. **Array** - Proves O(1) constant-time access advantage for frequent items
2. **Linked List** - Shows dynamic memory management for flexible cart operations
3. **Stack** - Implements LIFO principle for intuitive undo functionality
4. **Queue** - Demonstrates FIFO principle for realistic checkout simulation

### Key Learnings
- Each data structure has specific strengths and use cases
- Time complexity analysis is crucial for choosing the right structure
- Real-world applications make theoretical concepts tangible
- Interoperability between languages (C++/Python/JS) enables powerful applications
- Clean code organization and modular design improves maintainability

### Project Outcomes
- ✅ Web-based implementation with modern UI/UX
- ✅ C++ data structures with Python bridge using ctypes
- ✅ Comprehensive documentation and viva preparation materials
- ✅ Responsive design working across devices
- ✅ Real-time interactivity and visual feedback
- ✅ **PWA support** - Installable on Android devices (Android 5.0+)

---

## 15. References

### Books
1. Cormen, T. H., et al. *Introduction to Algorithms* (3rd Edition)
2. Weiss, Mark Allen. *Data Structures and Algorithm Analysis in C++*
3. Sedgewick, Robert. *Algorithms in C++*
4. Lutz, Mark. *Programming Python* (Python reference)

### Online Resources
1. GeeksforGeeks - Data Structures (www.geeksforgeeks.org)
2. Visualgo - Data Structure Visualizations (www.visualgo.net)
3. MDN Web Docs - JavaScript (developer.mozilla.org)
4. cppreference.com - C++ Reference
5. Flask Documentation (flask.palletsprojects.com)

### Course Materials
- Air University Data Structures Course Slides
- Lab Manual and Practice Problems
- C++ Programming Reference Materials

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
| **DLL** | Dynamic Link Library - compiled C++ code callable from other languages |
| **ctypes** | Python library for calling C functions from DLLs |
| **Flask** | Python web framework for building APIs |

---

## Appendix B: Code Statistics

| Metric | Value |
|--------|-------|
| Total Lines of Code (C++) | ~2,000+ |
| Total Lines of Code (Python) | ~400+ |
| Total Lines of Code (JavaScript) | ~700+ |
| Number of C++ Classes | 6 |
| Number of C++ Header Files | 5 |
| Number of API Endpoints | 8 |
| Number of Data Structures | 4 |

---

**Document Prepared:** December 2025  
**Project Status:** ✅ Complete  
**Version:** 2.0

---

*This project report is submitted as part of the Data Structures course requirements at Air University.*
