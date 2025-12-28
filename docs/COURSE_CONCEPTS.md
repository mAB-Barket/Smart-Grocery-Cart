# 📋 Course Concepts Mapping

## Smart Grocery Cart - Data Structures Project
### Air University - 3rd Semester

This document maps each code file to the specific **course concepts** covered.

---

## 📁 File: `/src/core/Product.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Classes & Objects** | 47-150 | Product class definition |
| **Access Specifiers** | 48-52, 54-95 | private/public keywords |
| **Default Constructor** | 60-61 | `Product()` with default values |
| **Parameterized Constructor** | 63-73 | `Product(name, price, qty, id)` |
| **Copy Constructor** | 75-83 | `Product(const Product& other)` |
| **Destructor** | 85-92 | `~Product()` |
| **Initializer List** | 61, 71-72, 80-81 | `name(n), price(p)` syntax |
| **Accessor Methods** | 97-100 | `getName()`, `getPrice()` etc. |
| **Mutator Methods** | 105-108 | `setName()`, `setPrice()` etc. |
| **const Methods** | 97-100, 110-116 | Methods that don't modify object |
| **Operator Overloading** | 122-131 | `operator==` and `operator<<` |
| **Friend Functions** | 133-137 | `friend ostream& operator<<` |

---

## 📁 File: `/src/core/Node.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Self-Referential Structure** | 47-49 | Node contains `Node* next_node` |
| **Pointers** | 48 | `Node* next_node` pointer member |
| **nullptr vs NULL** | 61 | Modern C++ null pointer |
| **Friend Classes** | 120-122 | `friend class LinkedList` |
| **Data Part & Link Part** | 47-48 | Core node structure |

---

## 📁 File: `/src/core/Array.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Static Array** | 87 | `FrequentItem items[MAX_FREQUENT_ITEMS]` |
| **O(1) Constant Time Access** | 107-118 | Direct index access |
| **Address Calculation** | Header comments | `address = base + (index × size)` |
| **Operator [] Overload** | 127-129 | Array subscript operator |
| **Linear Search O(n)** | 157-164 | Search through all elements |
| **Const Qualifier** | 89-90, 133-138 | Constants for array size |

---

## 📁 File: `/src/core/LinkedList.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Singly Linked List** | 60-350 | Complete implementation |
| **Head Pointer** | 63 | `Node* list_head` |
| **Dynamic Memory** | 140, 180 | `new Node()` allocation |
| **Memory Deallocation** | 78-80, 220-224 | `delete` to prevent leaks |
| **Traversal** | 105-108, 335-360 | Using `ptr = ptr->next()` |
| **Insert at Head O(1)** | 135-149 | No traversal needed |
| **Insert at Tail O(n)** | 165-182 | Must traverse to end |
| **Delete at Head O(1)** | 220-238 | Just update head |
| **Delete at Tail O(n)** | 253-275 | Find second-last node |
| **Search O(n)** | 117-124 | Linear search |

---

## 📁 File: `/src/core/Stack.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Stack Data Structure** | 70-200 | Complete implementation |
| **LIFO Principle** | Header comments | Last In, First Out |
| **Push Operation O(1)** | 115-132 | Add to top |
| **Pop Operation O(1)** | 150-175 | Remove from top |
| **Top/Peek O(1)** | 100-108 | View without removing |
| **Stack Underflow** | 100, 155 | Pop from empty stack |
| **Stack Applications** | Header comments | Undo, function calls, etc. |

---

## 📁 File: `/src/core/Queue.h`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Queue Data Structure** | 75-240 | Complete implementation |
| **FIFO Principle** | Header comments | First In, First Out |
| **Front & Rear Pointers** | 78-79 | Two pointers for O(1) |
| **Enqueue O(1)** | 130-155 | Add at rear |
| **Dequeue O(1)** | 175-205 | Remove from front |
| **Queue Underflow** | 110, 180 | Dequeue from empty queue |
| **Empty Queue Handling** | 190-192 | Update rear to NULL |
| **Queue Applications** | Header comments | Checkout, BFS, scheduling |

---

## 📁 File: `/src/main.cpp`

### Course Concepts Covered:

| Concept | Line Numbers | Description |
|---------|--------------|-------------|
| **Header File Inclusion** | 30-35 | `#include` preprocessor |
| **Using Namespace** | 37 | `using namespace std` |
| **Global Variables** | 43-47 | Data structure instances |
| **Function Prototypes** | 53-62 | Forward declarations |
| **Menu-Driven Program** | 68-105 | Switch-case structure |
| **Input/Output** | Throughout | `cin`, `cout` usage |

---

## 🎯 Summary of Course Concepts by Topic

### Object-Oriented Programming
- Classes and Objects
- Constructors (Default, Parameterized, Copy)
- Destructor
- Access Specifiers (private, public)
- Encapsulation (Getters/Setters)
- Operator Overloading
- Friend Functions/Classes

### Pointers and Memory Management
- Pointer Declaration and Usage
- Dynamic Memory Allocation (`new`)
- Memory Deallocation (`delete`)
- nullptr vs NULL
- Self-Referential Structures

### Data Structures
- Array (O(1) access)
- Singly Linked List
- Stack (LIFO)
- Queue (FIFO)
- Node Structure

### Time Complexity
- O(1) Constant Time
- O(n) Linear Time
- Comparing operations across structures

### Algorithms
- Traversal
- Insertion (at head, tail, position)
- Deletion (at head, tail, position)
- Linear Search
- Push/Pop (Stack)
- Enqueue/Dequeue (Queue)

---

## 📚 Study Priority (Most Important First)

1. **Node Structure** - Foundation of all linked structures
2. **Time Complexity** - Know WHY each operation is O(1) or O(n)
3. **Stack vs Queue** - LIFO vs FIFO difference
4. **Array vs Linked List** - Trade-offs
5. **Memory Management** - new/delete, memory leaks
6. **Traversal Pattern** - `while (ptr != nullptr)`

---

*Use this guide to prepare for your viva. Focus on understanding the WHY, not just the WHAT!*
