# 🛒 Smart Grocery Cart

A **unified** implementation of fundamental data structures through an interactive grocery shopping simulation. This project demonstrates practical applications of **Arrays, Linked Lists, Stacks, and Queues** using **C++ with a Python Flask bridge** to connect with a web interface.

---

## 🚀 Quick Start (Easy!)

### Just Double-Click to Run:
1. **Double-click `START.bat`** (or `build.bat`)
2. Open http://localhost:5000 in your browser
3. That's it! 🎉

### Requirements:
- ✅ **Python 3.x** - [Download here](https://www.python.org/downloads/) 
  - ⚠️ Check "Add Python to PATH" during installation!
- ✅ **DLL is pre-compiled** - No C++ compiler needed!

---

## 🏗️ Architecture

```
┌──────────────┐      HTTP/JSON      ┌──────────────┐     ctypes     ┌──────────────┐
│   Browser    │ ──────────────────▶ │  Flask (Py)  │ ─────────────▶ │  C++ DLL     │
│   (HTML/JS)  │ ◀────────────────── │   server.py  │ ◀───────────── │  grocery_api │
└──────────────┘                     └──────────────┘                └──────────────┘
     UI Only                         Web Server                    Data Structures
                                                                  (Array, LinkedList,
                                                                   Stack, Queue)
```

**Key Point:** All data structure logic runs in C++. The JavaScript only handles UI.

---

## 📁 Project Structure

```
Smart_Grocery_Cart/
│
├── 📄 README.md                 # This file
├── 📄 PROJECT_REPORT.md         # Detailed project report
├── 📄 build.bat                 # Build & Run script (Windows)
│
├── 📁 src/                      # 🎯 SOURCE CODE (VIVA FOCUS)
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
│   └── server.py                # Flask server (Python bridge)
│
├── 📁 web/                      # Web Interface (UI Only)
│   ├── index.html               # Main HTML file
│   ├── 📁 css/
│   │   └── styles.css           # Styling
│   └── 📁 js/
│       └── app.js               # UI logic (calls API)
│
├── 📁 docs/                     # 📚 Documentation
│   ├── VIVA_NOTES.md            # Quick reference for viva
│   └── COURSE_CONCEPTS.md       # Code-to-concept mapping
│
└── 📁 .vscode/                  # VS Code configuration
    └── tasks.json               # Build tasks
```

---

## 🚀 How to Run

### Prerequisites
1. **C++ Compiler**: clang++ (LLVM) or g++ (MinGW)
2. **Python 3.x** with pip
3. **Flask**: `pip install flask flask-cors`

### Option 1: Using Build Script (Recommended)
```bash
# Windows
build.bat
```
This will:
1. Compile the C++ DLL
2. Install Python dependencies
3. Start the Flask server
4. Open http://localhost:5000 in browser

### Option 2: Manual Steps
```bash
# 1. Compile C++ DLL
cd src
clang++ -shared -o grocery_api.dll grocery_api.cpp -DBUILD_DLL -static

# 2. Install Flask
pip install flask flask-cors

# 3. Run server
python server.py

# 4. Open browser
# Navigate to http://localhost:5000
```

---

## 📊 Data Structures Used

| Data Structure | Purpose | Time Complexity | C++ File |
|----------------|---------|-----------------|----------|
| **Array** | Frequent Items (O(1) access) | Access: O(1) | `core/Array.h` |
| **Linked List** | Shopping Cart | Insert: O(1), Delete: O(n) | `core/LinkedList.h` |
| **Stack (LIFO)** | Undo Operations | Push/Pop: O(1) | `core/Stack.h` |
| **Queue (FIFO)** | Checkout Process | Enqueue/Dequeue: O(1) | `core/Queue.h` |

---

## 🔌 API Endpoints

| Endpoint | Method | Description | Data Structure |
|----------|--------|-------------|----------------|
| `/api/frequent-items` | GET | Get all products | Array O(1) |
| `/api/cart` | GET | Get cart items | Linked List |
| `/api/cart/add` | POST | Add to cart | Linked List + Stack |
| `/api/cart/remove/:pos` | DELETE | Remove from cart | Linked List |
| `/api/undo` | POST | Undo last action | Stack (LIFO) |
| `/api/checkout/start` | POST | Move to queue | Queue (FIFO) |
| `/api/checkout/process` | POST | Process checkout | Queue dequeue |

---

## 📚 For VIVA Preparation

**Study these files in order:**
1. `src/core/Product.h` - OOP basics (classes, constructors)
2. `src/core/Node.h` - Self-referential structures
3. `src/core/Array.h` - O(1) array access
4. `src/core/LinkedList.h` - Singly linked list operations
5. `src/core/Stack.h` - LIFO principle
6. `src/core/Queue.h` - FIFO principle

Each file contains detailed comments explaining:
- Course concepts used
- Time complexity analysis
- Viva questions and answers

---

## 💡 Key Concepts Demonstrated

### 1. Array - O(1) Random Access
```cpp
// Address = Base + (Index × Size)
FrequentItem item = frequentItems[5];  // Direct access, no traversal!
```

### 2. Linked List - Dynamic Memory
```cpp
// Insert at head: O(1)
Node* new_node = new Node(data, head);
head = new_node;
```

### 3. Stack - LIFO (Last In First Out)
```cpp
// Push: O(1), Pop: O(1)
undoStack.push(action);  // Add to top
undoStack.pop();         // Remove from top (most recent first)
```

### 4. Queue - FIFO (First In First Out)
```cpp
// Enqueue: O(1), Dequeue: O(1)
checkoutQueue.enqueue(item);  // Add at rear
checkoutQueue.dequeue();       // Remove from front (oldest first)
```

---

## 🛠️ Technologies

- **C++** - Data structures implementation
- **Python Flask** - Web server bridge
- **ctypes** - Python-C++ interop
- **HTML/CSS/JS** - Web interface
- **Font Awesome** - Icons

---

## 👨‍💻 Project Info

- **Course**: Data Structures
- **Semester**: 3rd
- **University**: Air University
- **Date**: December 2025

---

## 📝 License

This project is for educational purposes as part of the Data Structures course at Air University.
