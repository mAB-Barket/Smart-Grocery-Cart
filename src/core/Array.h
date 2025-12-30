#ifndef ARRAY_H
#define ARRAY_H

#include <iostream>
#include <algorithm>
#include <cctype>
#include "Product.h"
using namespace std;

// Maximum items to display as "frequent items"
const int MAX_DISPLAY_ITEMS = 10;
// Maximum total items we can store
const int MAX_TOTAL_ITEMS = 1000;

// Case-insensitive string comparison helper
inline bool equalsIgnoreCase(const string& a, const string& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); i++) {
        if (tolower(a[i]) != tolower(b[i])) return false;
    }
    return true;
}

/**
 * FrequentItem - Represents any item (default or custom) with purchase tracking
 */
struct FrequentItem {
    int id;
    string name;
    int purchaseCount;
    bool isCustom;  // true if user-added, false if default item
    
    FrequentItem() {
        id = -1;
        name = "";
        purchaseCount = 0;
        isCustom = false;
    }
    
    FrequentItem(int itemId, string n, int count = 0, bool custom = false) {
        id = itemId;
        name = n;
        purchaseCount = count;
        isCustom = custom;
    }
    
    bool operator>(const FrequentItem& other) const {
        return purchaseCount > other.purchaseCount;
    }
};

/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                    UNIFIED ITEMS ARRAY (Single Storage)
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * This class stores ALL items (both default and custom) in a single array.
 * Items are sorted by purchase frequency - top 10 are displayed as "frequent items"
 * 
 * KEY FEATURES:
 * - Single unified storage for all items
 * - Automatic sorting by purchase count
 * - Top 10 items shown as frequent items
 * - Custom items automatically get promoted when purchased frequently
 */
class FrequentItemsArray {
private:
    FrequentItem items[MAX_TOTAL_ITEMS];
    int current_size;
    int nextCustomId;  // ID generator for custom items (starts at 1000)

public:
    FrequentItemsArray() : current_size(0), nextCustomId(1000) {
        // Add default items (id 0-9, isCustom = false)
        addDefaultItem(0, "Milk");
        addDefaultItem(1, "Bread");
        addDefaultItem(2, "Eggs");
        addDefaultItem(3, "Butter");
        addDefaultItem(4, "Cheese");
        addDefaultItem(5, "Chicken");
        addDefaultItem(6, "Rice");
        addDefaultItem(7, "Pasta");
        addDefaultItem(8, "Tomato Sauce");
        addDefaultItem(9, "Orange Juice");
    }

    // Add a default (non-custom) item
    void addDefaultItem(int id, const string& name) {
        if (current_size >= MAX_TOTAL_ITEMS) return;
        items[current_size] = FrequentItem(id, name, 0, false);
        current_size++;
    }

    // Get item at index (O(1) access)
    FrequentItem getItem(int index) const {
        if (index < 0 || index >= current_size) {
            return FrequentItem();
        }
        return items[index];
    }

    FrequentItem operator[](int index) const {
        return getItem(index);
    }

    // Total number of items stored
    int totalSize() const { return current_size; }
    
    // Number of items to display (max 10)
    int size() const { 
        return (current_size < MAX_DISPLAY_ITEMS) ? current_size : MAX_DISPLAY_ITEMS; 
    }
    
    bool isFull() const { return current_size >= MAX_TOTAL_ITEMS; }
    bool isEmpty() const { return current_size == 0; }

    // Case-insensitive search by name - searches ALL items
    int findByName(const string& name) const {
        for (int i = 0; i < current_size; i++) {
            if (equalsIgnoreCase(items[i].name, name)) return i;
        }
        return -1;
    }
    
    // Find item by ID
    int findById(int itemId) const {
        for (int i = 0; i < current_size; i++) {
            if (items[i].id == itemId) return i;
        }
        return -1;
    }

    /**
     * Add or update an item with purchase count
     * - If item exists: increment purchase count
     * - If new item: add to the array
     * Returns the item's ID
     */
    int addOrUpdateItem(const string& name, int quantity = 1, int forceId = -1) {
        // Check if item already exists (case-insensitive)
        int existingIndex = findByName(name);
        
        if (existingIndex != -1) {
            // Item exists - increment purchase count
            items[existingIndex].purchaseCount += quantity;
            sortByFrequency();
            return items[existingIndex].id;
        }
        
        // New item - add it
        if (isFull()) {
            // Array full - can't add more items
            return -1;
        }
        
        // Assign ID: use forceId if provided, otherwise generate new custom ID
        int newId = (forceId >= 0) ? forceId : nextCustomId++;
        
        // Ensure nextCustomId stays ahead of manually assigned IDs
        if (newId >= nextCustomId) {
            nextCustomId = newId + 1;
        }
        
        items[current_size] = FrequentItem(newId, name, quantity, true);
        current_size++;
        sortByFrequency();
        
        return newId;
    }

    // Sort items by purchase count (descending) - Bubble Sort
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

    // Increment purchase count for item at index
    void incrementPurchaseCount(int index) {
        if (index >= 0 && index < current_size) {
            items[index].purchaseCount++;
            sortByFrequency();
        }
    }

    // Get purchase count for item at index
    int getPurchaseCount(int index) const {
        if (index >= 0 && index < current_size) {
            return items[index].purchaseCount;
        }
        return 0;
    }

    // Increment purchase count by item ID
    bool incrementPurchaseCountById(int itemId) {
        int index = findById(itemId);
        if (index != -1) {
            items[index].purchaseCount++;
            sortByFrequency();
            return true;
        }
        return false;
    }

    // Search by name (returns index)
    int search(const string& name) const {
        return findByName(name);
    }

    // Get last item (lowest frequency in display range)
    FrequentItem getLastItem() const {
        int displaySize = size();
        if (displaySize == 0) return FrequentItem();
        return items[displaySize - 1];
    }
    
    // Reset to default state (10 default items with 0 purchase count)
    void resetToDefaults() {
        current_size = 0;
        nextCustomId = 1000;
        addDefaultItem(0, "Milk");
        addDefaultItem(1, "Bread");
        addDefaultItem(2, "Eggs");
        addDefaultItem(3, "Butter");
        addDefaultItem(4, "Cheese");
        addDefaultItem(5, "Chicken");
        addDefaultItem(6, "Rice");
        addDefaultItem(7, "Pasta");
        addDefaultItem(8, "Tomato Sauce");
        addDefaultItem(9, "Orange Juice");
    }

    // Get next available custom ID
    int getNextId() const {
        return nextCustomId;
    }
    
    // Set next custom ID (used for data restoration)
    void setNextId(int id) {
        if (id >= nextCustomId) {
            nextCustomId = id;
        }
    }

    // Display items (for debugging)
    void display() const {
        cout << "\n=== ALL ITEMS (Top " << size() << " shown as frequent) ===" << endl;
        for (int i = 0; i < current_size; i++) {
            string marker = (i < MAX_DISPLAY_ITEMS) ? "[FREQ] " : "[    ] ";
            cout << marker << "[" << i << "] " << items[i].name 
                 << " (ID: " << items[i].id 
                 << ", Purchases: " << items[i].purchaseCount 
                 << ", Custom: " << (items[i].isCustom ? "Yes" : "No") << ")" << endl;
        }
    }
    
    // Get all items as a pointer (for iteration)
    const FrequentItem* getAllItems() const {
        return items;
    }
};

#endif
