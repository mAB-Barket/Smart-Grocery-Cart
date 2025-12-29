#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include "Product.h"
using namespace std;

const int MAX_FREQUENT_ITEMS = 10;

struct FrequentItem {
    int id;
    string name;
    double price;
    string icon;
    int purchaseCount;
    bool isCustom;
    
    FrequentItem() {
        id = -1;
        name = "";
        price = 0.0;
        icon = "";
        purchaseCount = 0;
        isCustom = false;
    }
    
    FrequentItem(int itemId, string n, double p, string i, int count = 0, bool custom = false) {
        id = itemId;
        name = n;
        price = p;
        icon = i;
        purchaseCount = count;
        isCustom = custom;
    }
    
    bool operator>(const FrequentItem& other) const {
        return purchaseCount > other.purchaseCount;
    }
};

struct CustomItemNode {
    string name;
    double price;
    int purchaseCount;
    int uniqueId;
    CustomItemNode* next;
    
    CustomItemNode(string n, double p, int id) {
        name = n;
        price = p;
        purchaseCount = 0;
        uniqueId = id;
        next = nullptr;
    }
};

class CustomItemsList {
private:
    CustomItemNode* head;
    int itemCount;
    int nextId;

public:
    CustomItemsList() {
        head = nullptr;
        itemCount = 0;
        nextId = 1000;
    }
    
    ~CustomItemsList() { clear(); }
    
    void clear() {
        while (head != nullptr) {
            CustomItemNode* temp = head;
            head = head->next;
            delete temp;
        }
        itemCount = 0;
    }
    
    CustomItemNode* findByName(const string& name) {
        CustomItemNode* current = head;
        while (current != nullptr) {
            if (current->name == name) return current;
            current = current->next;
        }
        return nullptr;
    }
    
    int addOrUpdate(const string& name, double price, int quantity) {
        CustomItemNode* existing = findByName(name);
        
        if (existing != nullptr) {
            existing->purchaseCount += quantity;
            return existing->uniqueId;
        }
        
        CustomItemNode* newNode = new CustomItemNode(name, price, nextId++);
        newNode->purchaseCount = quantity;
        newNode->next = head;
        head = newNode;
        itemCount++;
        
        return newNode->uniqueId;
    }
    
    CustomItemNode* getHighestPurchaseItem() {
        if (head == nullptr) return nullptr;
        
        CustomItemNode* highest = head;
        CustomItemNode* current = head->next;
        
        while (current != nullptr) {
            if (current->purchaseCount > highest->purchaseCount) {
                highest = current;
            }
            current = current->next;
        }
        return highest;
    }
    
    bool remove(const string& name) {
        if (head == nullptr) return false;
        
        if (head->name == name) {
            CustomItemNode* temp = head;
            head = head->next;
            delete temp;
            itemCount--;
            return true;
        }
        
        CustomItemNode* current = head;
        while (current->next != nullptr) {
            if (current->next->name == name) {
                CustomItemNode* temp = current->next;
                current->next = temp->next;
                delete temp;
                itemCount--;
                return true;
            }
            current = current->next;
        }
        return false;
    }
    
    int size() const { return itemCount; }
    bool isEmpty() const { return head == nullptr; }
    CustomItemNode* getHead() const { return head; }
};

class FrequentItemsArray {
private:
    FrequentItem items[MAX_FREQUENT_ITEMS];
    int current_size;

public:
    FrequentItemsArray() : current_size(0) {
        addItem(0, "Milk (1 Liter)", 80, "🥛", 0);
        addItem(1, "Bread (Whole Wheat)", 60, "🍞", 0);
        addItem(2, "Eggs (Dozen)", 120, "🥚", 0);
        addItem(3, "Butter", 150, "🧈", 0);
        addItem(4, "Cheese (Cheddar)", 250, "🧀", 0);
        addItem(5, "Chicken Breast", 350, "🍗", 0);
        addItem(6, "Rice (5 kg bag)", 450, "🍚", 0);
        addItem(7, "Pasta", 90, "🍝", 0);
        addItem(8, "Tomato Sauce", 70, "🥫", 0);
        addItem(9, "Orange Juice", 180, "🍊", 0);
        sortByFrequency();
    }

    FrequentItem getItem(int index) const {
        if (index < 0 || index >= current_size) {
            return FrequentItem();
        }
        return items[index];
    }

    FrequentItem operator[](int index) const {
        return getItem(index);
    }

    int size() const { return current_size; }
    bool isFull() const { return current_size >= MAX_FREQUENT_ITEMS; }
    bool isEmpty() const { return current_size == 0; }

    bool addItem(int id, string name, double price, string icon, int purchaseCount = 0) {
        if (isFull()) return false;
        items[current_size] = FrequentItem(id, name, price, icon, purchaseCount);
        current_size++;
        return true;
    }

    void sortByFrequency() {
        for (int i = 0; i < current_size - 1; i++) {
            for (int j = 0; j < current_size - i - 1; j++) {
                if (items[j].purchaseCount < items[j + 1].purchaseCount) {
                    FrequentItem temp = items[j];
                    items[j] = items[j + 1];
                    items[j + 1] = temp;
                }
            }
        }
    }

    void incrementPurchaseCount(int index) {
        if (index >= 0 && index < current_size) {
            items[index].purchaseCount++;
        }
    }

    int getPurchaseCount(int index) const {
        if (index >= 0 && index < current_size) {
            return items[index].purchaseCount;
        }
        return 0;
    }

    bool incrementPurchaseCountById(int itemId) {
        for (int i = 0; i < current_size; i++) {
            if (items[i].id == itemId) {
                items[i].purchaseCount++;
                return true;
            }
        }
        return false;
    }

    int search(string name) const {
        for (int i = 0; i < current_size; i++) {
            if (items[i].name == name) return i;
        }
        return -1;
    }

    int getMinPurchaseIndex() const {
        if (current_size == 0) return -1;
        int minIndex = 0;
        for (int i = 1; i < current_size; i++) {
            if (items[i].purchaseCount < items[minIndex].purchaseCount) {
                minIndex = i;
            }
        }
        return minIndex;
    }
    
    int getMinPurchaseCount() const {
        int minIndex = getMinPurchaseIndex();
        if (minIndex == -1) return 0;
        return items[minIndex].purchaseCount;
    }
    
    FrequentItem getLastItem() const {
        if (current_size == 0) return FrequentItem();
        return items[current_size - 1];
    }
    
    void resetToDefaults() {
        current_size = 0;
        addItem(0, "Milk (1 Liter)", 80, "🥛", 0);
        addItem(1, "Bread (Whole Wheat)", 60, "🍞", 0);
        addItem(2, "Eggs (Dozen)", 120, "🥚", 0);
        addItem(3, "Butter", 150, "🧈", 0);
        addItem(4, "Cheese (Cheddar)", 250, "🧀", 0);
        addItem(5, "Chicken Breast", 350, "🍗", 0);
        addItem(6, "Rice (5 kg bag)", 450, "🍚", 0);
        addItem(7, "Pasta", 90, "🍝", 0);
        addItem(8, "Tomato Sauce", 70, "🥫", 0);
        addItem(9, "Orange Juice", 180, "🍊", 0);
        sortByFrequency();
    }
    
    bool replaceItem(int index, int newId, string name, double price, string icon, int purchaseCount) {
        if (index < 0 || index >= current_size) return false;
        items[index] = FrequentItem(newId, name, price, icon, purchaseCount, true);
        return true;
    }
    
    int getNextId() const {
        int maxId = 0;
        for (int i = 0; i < current_size; i++) {
            if (items[i].id > maxId) maxId = items[i].id;
        }
        return maxId + 1;
    }

    void display() const {
        cout << "\n=== FREQUENT ITEMS ===" << endl;
        for (int i = 0; i < current_size; i++) {
            cout << "[" << i << "] " << items[i].icon << " " 
                 << items[i].name << " - $" << items[i].price 
                 << " (Purchases: " << items[i].purchaseCount << ")" << endl;
        }
    }
};

#endif
