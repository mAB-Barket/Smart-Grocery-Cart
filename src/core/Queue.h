/**
 * Smart Grocery Cart - Data Structures Project
 * Air University, 3rd Semester
 * 
 * Queue.h - Queue implementation (FIFO) for Checkout process
 * 
 * Time Complexity (All O(1)):
 *   - enqueue(): Add to rear
 *   - dequeue(): Remove from front
 *   - front(): View front element
 *   - rear(): View rear element
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <iostream>
#include "Node.h"
using namespace std;

/**
 * Queue Class (FIFO - First In First Out)
 * Used for checkout process - first item added is processed first.
 * Structure: FRONT -> [node] -> [node] -> ... -> REAR
 */
class Queue {
private:
    Node* queue_front;
    Node* queue_rear;
    int queue_size;

public:
    Queue() : queue_front(nullptr), queue_rear(nullptr), queue_size(0) {}
    ~Queue() { clear(); }

    // Accessors
    bool empty() const { return queue_front == nullptr; }
    int size() const { return queue_size; }
    Node* front_node() const { return queue_front; }

    // View front element - O(1)
    Product front() const {
        if (empty()) return Product();
        return queue_front->retrieve();
    }

    // View rear element - O(1)
    Product rear() const {
        if (empty()) return Product();
        return queue_rear->retrieve();
    }

    // Enqueue - Add to rear - O(1)
    void enqueue(Product val) {
        Node* new_node = new Node(val, nullptr);
        
        if (empty()) {
            queue_front = new_node;
            queue_rear = new_node;
        } else {
            queue_rear->set_next(new_node);
            queue_rear = new_node;
        }
        queue_size++;
    }

    // Dequeue - Remove and return front element - O(1)
    Product dequeue() {
        if (empty()) return Product();
        
        Node* temp = queue_front;
        Product dequeued_item = temp->retrieve();
        queue_front = queue_front->next();
        
        // If queue becomes empty, update rear
        if (queue_front == nullptr) {
            queue_rear = nullptr;
        }
        
        delete temp;
        queue_size--;
        return dequeued_item;
    }

    // Clear all elements - O(n)
    void clear() {
        while (!empty()) {
            dequeue();
        }
    }

    double calculate_total() const {
        double total = 0.0;
        for (Node* ptr = queue_front; ptr != nullptr; ptr = ptr->next()) {
            total += ptr->retrieve().total();
        }
        return total;
    }

    // Display
    void traverse() const {
        cout << "\n=== CHECKOUT QUEUE (FIFO) ===" << endl;
        if (empty()) {
            cout << "Queue is empty" << endl;
            return;
        }
        cout << "FRONT -> ";
        for (Node* ptr = queue_front; ptr != nullptr; ptr = ptr->next()) {
            cout << "[" << ptr->retrieve().getName() << "]";
            if (ptr->next() != nullptr) cout << " -> ";
        }
        cout << " <- REAR" << endl;
        cout << "Total: $" << calculate_total() << endl;
    }

    void display_visual() const {
        cout << "FRONT -> ";
        if (empty()) {
            cout << "NULL" << endl;
            return;
        }
        for (Node* ptr = queue_front; ptr != nullptr; ptr = ptr->next()) {
            cout << "[" << ptr->retrieve().getName() << "]";
            if (ptr->next() != nullptr) cout << " -> ";
        }
        cout << " <- REAR" << endl;
    }
};

#endif // QUEUE_H
