#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <iostream>
#include <cctype>
#include "Node.h"
using namespace std;

// Case-insensitive string comparison
inline bool strEqualsIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (tolower(a[i]) != tolower(b[i])) return false;
    }
    return true;
}

class LinkedList {
private:
    Node* list_head;
    int item_count;

public:
    LinkedList() {
        list_head = nullptr;
        item_count = 0;
    }

    ~LinkedList() { clear(); }

    bool empty() const { return list_head == nullptr; }
    Node* head() const { return list_head; }
    int size() const { return item_count; }

    Product front() const {
        if (empty()) return Product();
        return list_head->retrieve();
    }

    Product back() const {
        if (empty()) return Product();
        Node* ptr = list_head;
        while (ptr->next() != nullptr) {
            ptr = ptr->next();
        }
        return ptr->retrieve();
    }

    int total_quantity() const {
        int total = 0;
        for (Node* ptr = list_head; ptr != nullptr; ptr = ptr->next()) {
            total += ptr->retrieve().getQuantity();
        }
        return total;
    }

    Node* find(string productName) const {
        for (Node* ptr = list_head; ptr != nullptr; ptr = ptr->next()) {
            if (strEqualsIgnoreCase(ptr->retrieve().getName(), productName)) {
                return ptr;
            }
        }
        return nullptr;
    }

    Product get_at_position(int position) const {
        if (position < 1 || position > item_count) return Product();
        Node* ptr = list_head;
        for (int i = 1; i < position; i++) {
            ptr = ptr->next();
        }
        return ptr->retrieve();
    }

    void insert_at_head(Product val) {
        Node* new_node = new Node(val, list_head);
        list_head = new_node;
        item_count++;
    }

    void insert_at_tail(Product val) {
        if (empty()) {
            insert_at_head(val);
            return;
        }
        Node* ptr = list_head;
        while (ptr->next() != nullptr) {
            ptr = ptr->next();
        }
        ptr->set_next(new Node(val, nullptr));
        item_count++;
    }

    void insert_at_position(Product val, int position) {
        if (position < 1 || position > item_count + 1) return;
        if (position == 1) {
            insert_at_head(val);
            return;
        }
        Node* ptr = list_head;
        for (int i = 1; i < position - 1; i++) {
            ptr = ptr->next();
        }
        Node* new_node = new Node(val, ptr->next());
        ptr->set_next(new_node);
        item_count++;
    }

    void push_item(Product val) {
        Node* existing = find(val.getName());
        if (existing != nullptr) {
            Product updated = existing->retrieve();
            updated.setQuantity(updated.getQuantity() + val.getQuantity());
            existing->set_data(updated);
            return;
        }
        insert_at_tail(val);
    }

    Product delete_at_head() {
        if (empty()) return Product();
        Node* temp = list_head;
        Product deleted_item = temp->retrieve();
        list_head = list_head->next();
        delete temp;
        item_count--;
        return deleted_item;
    }

    Product delete_at_tail() {
        if (empty()) return Product();
        if (item_count == 1) return delete_at_head();
        
        Node* ptr = list_head;
        while (ptr->next()->next() != nullptr) {
            ptr = ptr->next();
        }
        Product deleted_item = ptr->next()->retrieve();
        delete ptr->next();
        ptr->set_next(nullptr);
        item_count--;
        return deleted_item;
    }

    Product delete_at_position(int position) {
        if (position < 1 || position > item_count) return Product();
        if (position == 1) return delete_at_head();
        
        Node* ptr = list_head;
        for (int i = 1; i < position - 1; i++) {
            ptr = ptr->next();
        }
        Node* to_delete = ptr->next();
        Product deleted_item = to_delete->retrieve();
        ptr->set_next(to_delete->next());
        delete to_delete;
        item_count--;
        return deleted_item;
    }

    bool delete_by_name(string productName) {
        if (empty()) return false;
        
        if (strEqualsIgnoreCase(list_head->retrieve().getName(), productName)) {
            delete_at_head();
            return true;
        }
        
        Node* ptr = list_head;
        while (ptr->next() != nullptr) {
            if (strEqualsIgnoreCase(ptr->next()->retrieve().getName(), productName)) {
                Node* to_delete = ptr->next();
                ptr->set_next(to_delete->next());
                delete to_delete;
                item_count--;
                return true;
            }
            ptr = ptr->next();
        }
        return false;
    }

    void clear() {
        while (!empty()) {
            delete_at_head();
        }
    }

    void traverse() const {
        cout << "\n=== SHOPPING CART ===" << endl;
        if (empty()) {
            cout << "Cart is empty" << endl;
            return;
        }
        int pos = 1;
        for (Node* ptr = list_head; ptr != nullptr; ptr = ptr->next()) {
            Product p = ptr->retrieve();
            cout << "[" << pos++ << "] " << p.getName() 
                 << " x " << p.getQuantity() << endl;
        }
        cout << "Total items: " << total_quantity() << endl;
    }

    void display_visual() const {
        cout << "head -> ";
        if (empty()) {
            cout << "NULL" << endl;
            return;
        }
        for (Node* ptr = list_head; ptr != nullptr; ptr = ptr->next()) {
            cout << "[" << ptr->retrieve().getName() << "]";
            if (ptr->next() != nullptr) cout << " -> ";
        }
        cout << " -> NULL" << endl;
    }
};

#endif
