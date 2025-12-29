/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           SMART GROCERY CART
 *                    Data Structures Project - Air University
 *                              3rd Semester
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * FILE: grocery_api.cpp
 * PURPOSE: C++ API that implements all data structures (Array, LinkedList, Stack, Queue)
 * 
 * This is a shopping list reminder app - NO PRICES, just item names and quantities
 */

#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>
#include <cstring>
#include "core/Array.h"
#include "core/LinkedList.h"
#include "core/Stack.h"
#include "core/Queue.h"

using namespace std;

// Export functions for DLL
#ifdef _WIN32
    #define EXPORT extern "C" __declspec(dllexport)
#else
    #define EXPORT extern "C"
#endif

// ═══════════════════════════════════════════════════════════════════════════════
//                         GLOBAL DATA STRUCTURES
// ═══════════════════════════════════════════════════════════════════════════════

static FrequentItemsArray frequentItems;  // Array for O(1) access
static LinkedList cart;                    // Linked List for shopping cart
static Stack undoStack;                    // Stack for undo operations
static Queue checkoutQueue;                // Queue for checkout process
static CustomItemsList customItems;        // Linked List for tracking custom items

// ═══════════════════════════════════════════════════════════════════════════════
//                    HELPER: Convert C++ string to C string
// ═══════════════════════════════════════════════════════════════════════════════

char* string_to_cstr(const string& str) {
    size_t len = str.length() + 1;
    char* result = (char*)malloc(len);
    if (result != nullptr) {
#ifdef _WIN32
        strcpy_s(result, len, str.c_str());
#else
        strcpy(result, str.c_str());
#endif
    }
    return result;
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    ARRAY OPERATIONS - Frequent Items
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Get number of frequent items
 */
EXPORT int api_get_frequent_items_count() {
    return frequentItems.size();
}

/**
 * Get frequent item at index (O(1) access!)
 */
EXPORT const char* api_get_frequent_item(int index) {
    FrequentItem item = frequentItems[index];
    
    ostringstream json;
    json << "{\"id\":" << item.id << ","
         << "\"name\":\"" << item.name << "\","
         << "\"purchaseCount\":" << item.purchaseCount << "}";
    
    return string_to_cstr(json.str());
}

/**
 * Get all frequent items as JSON array
 */
EXPORT const char* api_get_all_frequent_items() {
    ostringstream json;
    json << "[";
    
    for (int i = 0; i < frequentItems.size(); i++) {
        FrequentItem item = frequentItems[i];
        if (i > 0) json << ",";
        json << "{\"id\":" << item.id << ","
             << "\"name\":\"" << item.name << "\","
             << "\"purchaseCount\":" << item.purchaseCount << ","
             << "\"isCustom\":" << (item.isCustom ? "true" : "false") << "}";
    }
    
    json << "]";
    return string_to_cstr(json.str());
}

/**
 * Increment purchase count for item by ID
 */
EXPORT void api_increment_purchase_count_by_id(int itemId) {
    frequentItems.incrementPurchaseCountById(itemId);
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    LINKED LIST OPERATIONS - Shopping Cart
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Add item to cart (Linked List insertion)
 */
EXPORT void api_add_to_cart(const char* name, int quantity, int product_id) {
    Product product(name, quantity, product_id);
    cart.push_item(product);
    
    // Also push to undo stack (LIFO)
    undoStack.push(product);
}

/**
 * Remove item from cart at position (1-indexed)
 */
EXPORT const char* api_remove_from_cart(int position) {
    Product removed = cart.delete_at_position(position);
    
    ostringstream json;
    json << "{\"name\":\"" << removed.getName() << "\","
         << "\"quantity\":" << removed.getQuantity() << "}";
    
    return string_to_cstr(json.str());
}

/**
 * Get cart size
 */
EXPORT int api_get_cart_size() {
    return cart.size();
}

/**
 * Check if cart is empty
 */
EXPORT bool api_is_cart_empty() {
    return cart.empty();
}

/**
 * Get total quantity in cart
 */
EXPORT int api_get_cart_total_quantity() {
    return cart.total_quantity();
}

/**
 * Get all cart items as JSON array
 */
EXPORT const char* api_get_cart_items() {
    ostringstream json;
    json << "[";
    
    Node* current = cart.head();
    bool first = true;
    
    while (current != nullptr) {
        Product item = current->retrieve();
        if (!first) json << ",";
        first = false;
        
        json << "{\"name\":\"" << item.getName() << "\","
             << "\"quantity\":" << item.getQuantity() << ","
             << "\"product_id\":" << item.getProductId() << "}";
        
        current = current->next();
    }
    
    json << "]";
    return string_to_cstr(json.str());
}

/**
 * Clear the cart
 */
EXPORT void api_clear_cart() {
    cart.clear();
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    STACK OPERATIONS - Undo (LIFO)
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Undo last action (Stack pop - LIFO)
 */
EXPORT const char* api_undo_last_action() {
    if (undoStack.empty()) {
        return string_to_cstr("{\"error\":\"No actions to undo\"}");
    }
    
    Product lastAction = undoStack.pop();
    cart.delete_by_name(lastAction.getName());
    
    ostringstream json;
    json << "{\"name\":\"" << lastAction.getName() << "\","
         << "\"quantity\":" << lastAction.getQuantity() << "}";
    
    return string_to_cstr(json.str());
}

/**
 * Get undo stack size
 */
EXPORT int api_get_undo_stack_size() {
    return undoStack.size();
}

/**
 * Check if undo stack is empty
 */
EXPORT bool api_is_undo_stack_empty() {
    return undoStack.empty();
}

/**
 * Get all stack items (for visualization)
 */
EXPORT const char* api_get_stack_items() {
    ostringstream json;
    json << "[";
    
    Node* current = undoStack.top_node();
    bool first = true;
    
    while (current != nullptr) {
        Product item = current->retrieve();
        if (!first) json << ",";
        first = false;
        
        json << "{\"name\":\"" << item.getName() << "\","
             << "\"quantity\":" << item.getQuantity() << "}";
        
        current = current->next();
    }
    
    json << "]";
    return string_to_cstr(json.str());
}

/**
 * Clear the undo stack
 */
EXPORT void api_clear_undo_stack() {
    undoStack.clear();
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    QUEUE OPERATIONS - Checkout (FIFO)
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Check if a custom item should be promoted to frequent items
 */
void checkAndPromoteCustomItems() {
    CustomItemNode* topCustom = customItems.getHighestPurchaseItem();
    if (topCustom == nullptr) {
        return;
    }
    
    FrequentItem lastFrequent = frequentItems.getLastItem();
    
    if (topCustom->purchaseCount > lastFrequent.purchaseCount) {
        int lastIndex = frequentItems.size() - 1;
        
        frequentItems.replaceItem(
            lastIndex,
            topCustom->uniqueId,
            topCustom->name,
            topCustom->purchaseCount
        );
        
        customItems.remove(topCustom->name);
        frequentItems.sortByFrequency();
    }
}

/**
 * Move all cart items to checkout queue (FIFO)
 * Also updates purchase counts
 */
EXPORT void api_start_checkout() {
    Node* current = cart.head();
    
    while (current != nullptr) {
        Product item = current->retrieve();
        checkoutQueue.enqueue(item);
        
        int productId = item.getProductId();
        
        if (productId == -1 || productId >= 1000) {
            customItems.addOrUpdate(item.getName(), item.getQuantity());
        } else {
            for (int i = 0; i < item.getQuantity(); i++) {
                frequentItems.incrementPurchaseCountById(productId);
            }
        }
        
        current = current->next();
    }
    
    frequentItems.sortByFrequency();
    checkAndPromoteCustomItems();
    
    cart.clear();
    undoStack.clear();
}

/**
 * Get checkout queue size
 */
EXPORT int api_get_queue_size() {
    return checkoutQueue.size();
}

/**
 * Process checkout - dequeue all items (FIFO) and return receipt
 */
EXPORT const char* api_process_checkout() {
    ostringstream json;
    json << "{\"items\":[";
    
    int totalItems = 0;
    bool first = true;
    
    while (!checkoutQueue.empty()) {
        Product item = checkoutQueue.dequeue();
        totalItems += item.getQuantity();
        
        if (!first) json << ",";
        first = false;
        
        json << "{\"name\":\"" << item.getName() << "\","
             << "\"quantity\":" << item.getQuantity() << "}";
    }
    
    json << "],\"totalItems\":" << totalItems << "}";
    
    return string_to_cstr(json.str());
}

/**
 * Get all queue items (for visualization)
 */
EXPORT const char* api_get_queue_items() {
    ostringstream json;
    json << "[";
    
    Node* current = checkoutQueue.front_node();
    bool first = true;
    
    while (current != nullptr) {
        Product item = current->retrieve();
        if (!first) json << ",";
        first = false;
        
        json << "{\"name\":\"" << item.getName() << "\","
             << "\"quantity\":" << item.getQuantity() << "}";
        
        current = current->next();
    }
    
    json << "]";
    return string_to_cstr(json.str());
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    DATA RESTORATION FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Restore a custom item with its purchase count (for data persistence)
 */
EXPORT void api_restore_custom_item(const char* name, int purchaseCount, int itemId) {
    bool foundInFrequent = false;
    for (int i = 0; i < frequentItems.size(); i++) {
        FrequentItem item = frequentItems[i];
        if (item.id == itemId) {
            for (int j = 0; j < purchaseCount; j++) {
                frequentItems.incrementPurchaseCountById(itemId);
            }
            foundInFrequent = true;
            break;
        }
    }
    
    if (!foundInFrequent) {
        for (int i = 0; i < purchaseCount; i++) {
            customItems.addOrUpdate(name, 1);
        }
        checkAndPromoteCustomItems();
    }
    
    frequentItems.sortByFrequency();
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    UTILITY FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Reset all data structures
 */
EXPORT void api_reset_all() {
    cart.clear();
    undoStack.clear();
    checkoutQueue.clear();
    customItems.clear();
}

/**
 * Factory reset - clear everything and reset purchase counts to zero
 */
EXPORT void api_factory_reset() {
    cart.clear();
    undoStack.clear();
    checkoutQueue.clear();
    customItems.clear();
    frequentItems.resetToDefaults();
}

/**
 * Free allocated memory
 */
EXPORT void api_free_string(char* str) {
    if (str) free(str);
}
