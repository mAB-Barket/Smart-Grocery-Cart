# 📚 VIVA QUICK REFERENCE GUIDE

## Smart Grocery Cart - Data Structures Project
### Air University - 3rd Semester

---

## 🎯 Time Complexity Table (MUST MEMORIZE!)

| Operation | Array | Linked List | Stack | Queue |
|-----------|-------|-------------|-------|-------|
| **Access by index** | **O(1)** ✓ | O(n) | N/A | N/A |
| **Insert at head** | O(n) | **O(1)** ✓ | N/A | N/A |
| **Insert at tail** | O(1)* | O(n) | N/A | N/A |
| **Delete at head** | O(n) | **O(1)** ✓ | N/A | N/A |
| **Delete at tail** | O(1) | O(n) | N/A | N/A |
| **Search** | O(n) | O(n) | N/A | N/A |
| **Push** | N/A | N/A | **O(1)** ✓ | N/A |
| **Pop** | N/A | N/A | **O(1)** ✓ | N/A |
| **Top/Peek** | N/A | N/A | **O(1)** ✓ | N/A |
| **Enqueue** | N/A | N/A | N/A | **O(1)** ✓ |
| **Dequeue** | N/A | N/A | N/A | **O(1)** ✓ |
| **Front/Rear** | N/A | N/A | N/A | **O(1)** ✓ |

*O(1) if space available

---

## 📖 Key Viva Questions & Answers

### ARRAY

**Q: Why is array access O(1)?**
```
A: Address = Base_Address + (Index × Size_of_Element)
   Direct calculation - no traversal needed!
```

**Q: What's the main advantage of arrays?**
```
A: Constant time O(1) access to any element using index.
```

**Q: Why can't we change array size dynamically?**
```
A: Static arrays are allocated contiguous memory at compile time.
   Dynamic arrays (using new[]) can resize but require reallocation.
```

---

### LINKED LIST

**Q: What is a self-referential structure?**
```cpp
A: A structure containing a pointer to the same type.
   
class Node {
    Product data;      // DATA part
    Node* next_node;   // LINK part (pointer to same type)
};
```

**Q: Why is insertion at head O(1)?**
```cpp
A: Only two pointer updates needed:
   1. new_node->next = head
   2. head = new_node
   No traversal required!
```

**Q: Why is deletion at tail O(n)?**
```
A: Must find the second-last node to update its next pointer.
   This requires traversing from head to second-last node.
```

**Q: Linked List vs Array - when to use which?**
```
Array: When you need fast random access (O(1))
Linked List: When you need frequent insertions/deletions at head
```

---

### STACK (LIFO)

**Q: What does LIFO mean?**
```
A: Last In, First Out
   The last element added is the first one removed.
   Like a stack of plates!
```

**Q: What are push and pop operations?**
```cpp
push(x): Add element x to TOP of stack - O(1)
pop():   Remove and return element from TOP - O(1)
top():   View TOP element without removing - O(1)
```

**Q: What is Stack Underflow?**
```
A: Trying to pop from an empty stack.
   Must check empty() before pop()!
```

**Q: What is Stack Overflow?**
```
A: Trying to push to a full stack (fixed-size array implementation).
   Not applicable in linked list implementation.
```

**Q: Real-world applications of Stack?**
```
1. Undo operations (this project!)
2. Browser back button
3. Function call stack
4. Expression evaluation (postfix)
5. Parenthesis matching
```

---

### QUEUE (FIFO)

**Q: What does FIFO mean?**
```
A: First In, First Out
   The first element added is the first one removed.
   Like a queue of people!
```

**Q: What are enqueue and dequeue?**
```cpp
enqueue(x): Add element x at REAR - O(1)
dequeue():  Remove and return element from FRONT - O(1)
```

**Q: Why do we need both front AND rear pointers?**
```
A: - front: For O(1) dequeue (remove from front)
   - rear:  For O(1) enqueue (add at rear)
   
   Without rear pointer, enqueue would be O(n)!
```

**Q: What is Queue Underflow?**
```
A: Trying to dequeue from an empty queue.
```

**Q: Why update rear to NULL when queue becomes empty?**
```cpp
if (front == NULL) {
    rear = NULL;  // IMPORTANT!
}
// Prevents dangling pointer
```

**Q: Real-world applications of Queue?**
```
1. Checkout process (this project!)
2. Print queue
3. CPU scheduling
4. BFS algorithm
5. Call center systems
```

---

## 🔑 Key Concepts to Explain

### 1. Node Structure
```cpp
class Node {
    Product data;      // DATA: actual information
    Node* next_node;   // LINK: pointer to next node
};
```

### 2. Traversal Pattern
```cpp
Node* ptr = head;
while (ptr != nullptr) {
    // Process ptr->retrieve()
    ptr = ptr->next();
}
```

### 3. Insert at Head (O(1))
```cpp
void insert_at_head(Product val) {
    Node* new_node = new Node(val, head);  // Point to current head
    head = new_node;                        // Update head
}
```

### 4. Delete at Head (O(1))
```cpp
Product delete_at_head() {
    Node* temp = head;           // Store head
    Product val = temp->data;    // Get data
    head = head->next;           // Move head
    delete temp;                 // Free memory
    return val;
}
```

### 5. Stack Push (O(1))
```cpp
void push(Product val) {
    Node* new_node = new Node(val, top);  // Point to current top
    top = new_node;                        // Update top
}
```

### 6. Queue Enqueue (O(1))
```cpp
void enqueue(Product val) {
    Node* new_node = new Node(val, nullptr);
    if (empty()) {
        front = rear = new_node;
    } else {
        rear->next = new_node;  // Link at rear
        rear = new_node;        // Update rear
    }
}
```

---

## ⚠️ Common Mistakes to Avoid

1. **Forgetting to check empty()** before pop/dequeue
2. **Not updating rear to NULL** when queue becomes empty
3. **Memory leak** - always delete removed nodes
4. **Confusing LIFO vs FIFO**
5. **Not handling head == nullptr** case
6. **Forgetting to update item_count**

---

## 📁 File Locations for Code Reference

| Concept | File |
|---------|------|
| Product class | `/src/core/Product.h` |
| Node class | `/src/core/Node.h` |
| Array O(1) access | `/src/core/Array.h` |
| Linked List | `/src/core/LinkedList.h` |
| Stack (LIFO) | `/src/core/Stack.h` |
| Queue (FIFO) | `/src/core/Queue.h` |
| Main program | `/src/main.cpp` |

---

## 🎓 Tips for Viva

1. **Be ready to trace through code** - draw diagrams
2. **Know the WHY** - not just what, but why O(1) or O(n)
3. **Connect to real-world** - explain practical applications
4. **Compare structures** - when to use which
5. **Explain trade-offs** - memory vs time
6. **Run the C++ program** to demonstrate

**Good luck! 🍀**
