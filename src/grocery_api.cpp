/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           SMART GROCERY CART
 *                    Data Structures Project - Air University
 *                              3rd Semester
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * FILE: grocery_api.cpp
 * PURPOSE: C API wrapper for Python integration via ctypes
 * 
 * This file creates a shared library (DLL on Windows) that exposes C functions
 * which Python can call using the ctypes module.
 * 
 * COMPILATION (Windows):
 *   clang++ -shared -o grocery_api.dll grocery_api.cpp -DBUILD_DLL
 * 
 * COMPILATION (Linux/Mac):
 *   g++ -shared -fPIC -o libgrocery_api.so grocery_api.cpp
 */

#include <cstring>
#include <cstdlib>
#include <sstream>

// Include all data structure headers
#include "core/Product.h"
#include "core/Node.h"
#include "core/Array.h"
#include "core/LinkedList.h"
#include "core/Stack.h"
#include "core/Queue.h"

// ═══════════════════════════════════════════════════════════════════════════════
//                         DLL EXPORT MACROS
// ═══════════════════════════════════════════════════════════════════════════════

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
 * Returns JSON string with item's unique ID
 */
EXPORT const char* api_get_frequent_item(int index) {
    FrequentItem item = frequentItems[index];  // O(1) array access!
    
    ostringstream json;
    json << "{\"id\":" << item.id << ","
         << "\"name\":\"" << item.name << "\","
         << "\"price\":" << item.price << ","
         << "\"icon\":\"" << item.icon << "\","
         << "\"purchaseCount\":" << item.purchaseCount << "}";
    
    return string_to_cstr(json.str());
}

/**
 * Get all frequent items as JSON array
 * Items are sorted by purchaseCount (most frequent first)
 * Each item has a unique ID that doesn't change when sorted
 * Includes isCustom flag to identify promoted items
 */
EXPORT const char* api_get_all_frequent_items() {
    ostringstream json;
    json << "[";
    
    for (int i = 0; i < frequentItems.size(); i++) {
        FrequentItem item = frequentItems[i];
        if (i > 0) json << ",";
        json << "{\"id\":" << item.id << ","
             << "\"name\":\"" << item.name << "\","
             << "\"price\":" << item.price << ","
             << "\"icon\":\"" << item.icon << "\","
             << "\"purchaseCount\":" << item.purchaseCount << ","
             << "\"isCustom\":" << (item.isCustom ? "true" : "false") << "}";
    }
    
    json << "]";
    return string_to_cstr(json.str());
}

/**
 * Increment purchase count for item by ID
 * Uses linear search O(n) to find item by ID (not position)
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
EXPORT void api_add_to_cart(const char* name, double price, int quantity, int product_id) {
    Product product(name, price, quantity, product_id);
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
         << "\"price\":" << removed.getPrice() << ","
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
 * Get cart total
 */
EXPORT double api_get_cart_total() {
    return cart.calculate_total();
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
             << "\"price\":" << item.getPrice() << ","
             << "\"quantity\":" << item.getQuantity() << ","
             << "\"total\":" << item.total() << ","
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
 * Returns the undone item as JSON
 */
EXPORT const char* api_undo_last_action() {
    if (undoStack.empty()) {
        return string_to_cstr("{\"error\":\"No actions to undo\"}");
    }
    
    // Pop from stack (LIFO - last action first)
    Product lastAction = undoStack.pop();
    
    // Remove from cart
    cart.delete_by_name(lastAction.getName());
    
    ostringstream json;
    json << "{\"name\":\"" << lastAction.getName() << "\","
         << "\"price\":" << lastAction.getPrice() << ","
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
             << "\"price\":" << item.getPrice() << ","
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
 * 
 * 📚 COURSE CONCEPT: Comparing data between Linked List and Array
 * 
 * ALGORITHM:
 * 1. Find the custom item with highest purchase count (O(n) in linked list)
 * 2. Get the frequent item with lowest purchase count (O(1) - last after sort)
 * 3. If custom > frequent minimum, swap them!
 */
void checkAndPromoteCustomItems() {
    // Get the most purchased custom item
    CustomItemNode* topCustom = customItems.getHighestPurchaseItem();
    if (topCustom == nullptr) {
        return;
    }
    
    // Get the least purchased frequent item (after sorting, it's at the end)
    FrequentItem lastFrequent = frequentItems.getLastItem();
    
    // If custom item has MORE purchases than the lowest frequent item → PROMOTE!
    if (topCustom->purchaseCount > lastFrequent.purchaseCount) {
        // Find the index of the last item (lowest purchase count)
        int lastIndex = frequentItems.size() - 1;
        
        // Replace the lowest frequent item with the top custom item
        frequentItems.replaceItem(
            lastIndex,
            topCustom->uniqueId,
            topCustom->name,
            topCustom->price,
            "🆕",  // Custom items get a "new" icon
            topCustom->purchaseCount
        );
        
        // Remove the promoted item from custom items list
        customItems.remove(topCustom->name);
        
        // Re-sort so the promoted item moves to its correct position
        frequentItems.sortByFrequency();
    }
}

/**
 * Move all cart items to checkout queue (FIFO)
 * Also updates purchase counts for frequent items AND custom items
 */
EXPORT void api_start_checkout() {
    // Move all items from cart (Linked List) to checkout queue (FIFO)
    Node* current = cart.head();
    
    while (current != nullptr) {
        Product item = current->retrieve();
        checkoutQueue.enqueue(item);
        
        // Update purchase count for frequent items using unique ID (not array index!)
        int productId = item.getProductId();
        
        // Check if this is a custom item (ID >= 1000 or ID == -1)
        if (productId == -1 || productId >= 1000) {
            // This is a CUSTOM ITEM - track it in the custom items linked list
            customItems.addOrUpdate(item.getName(), item.getPrice(), item.getQuantity());
        } else {
            // This is a FREQUENT ITEM - update in the array
            for (int i = 0; i < item.getQuantity(); i++) {
                frequentItems.incrementPurchaseCountById(productId);
            }
        }
        
        current = current->next();
    }
    
    // Re-sort frequent items by popularity (Bubble Sort - O(n²))
    frequentItems.sortByFrequency();
    
    // CHECK FOR PROMOTION: Can any custom item replace the lowest frequent item?
    checkAndPromoteCustomItems();
    
    // Clear cart and undo stack
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
    
    double subtotal = 0;
    bool first = true;
    
    // Dequeue each item (FIFO - first item added is processed first)
    while (!checkoutQueue.empty()) {
        Product item = checkoutQueue.dequeue();
        subtotal += item.total();
        
        if (!first) json << ",";
        first = false;
        
        json << "{\"name\":\"" << item.getName() << "\","
             << "\"price\":" << item.getPrice() << ","
             << "\"quantity\":" << item.getQuantity() << ","
             << "\"total\":" << item.total() << "}";
    }
    
    double tax = subtotal * 0.08;
    double discount = (subtotal > 500) ? subtotal * 0.05 : 0;  // 5% discount over ₹500
    double grandTotal = subtotal + tax - discount;
    
    json << "],\"subtotal\":" << subtotal
         << ",\"tax\":" << tax
         << ",\"discount\":" << discount
         << ",\"grandTotal\":" << grandTotal << "}";
    
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
             << "\"price\":" << item.getPrice() << ","
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
 * This adds the item to the customItems linked list AND checks if it should be promoted
 * 
 * @param name - Item name
 * @param price - Item price
 * @param purchaseCount - Number of times purchased (from saved data)
 * @param itemId - The saved unique ID to preserve
 */
EXPORT void api_restore_custom_item(const char* name, double price, int purchaseCount, int itemId) {
    // First check if this item was already promoted to frequent items
    // (happens if ID >= 1000 and item exists in frequentItems array)
    bool foundInFrequent = false;
    for (int i = 0; i < frequentItems.size(); i++) {
        FrequentItem item = frequentItems[i];
        if (item.id == itemId) {
            // Item was previously promoted - update its purchase count
            for (int j = 0; j < purchaseCount; j++) {
                frequentItems.incrementPurchaseCountById(itemId);
            }
            foundInFrequent = true;
            break;
        }
    }
    
    // If not in frequent items, add to custom items list
    if (!foundInFrequent) {
        // Add to custom items with the full purchase count
        for (int i = 0; i < purchaseCount; i++) {
            customItems.addOrUpdate(name, price, 1);
        }
        
        // Check if this custom item should be promoted
        checkAndPromoteCustomItems();
    }
    
    // Re-sort frequent items
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
 * This restores the application to its initial state
 */
EXPORT void api_factory_reset() {
    cart.clear();
    undoStack.clear();
    checkoutQueue.clear();
    customItems.clear();
    frequentItems.resetToDefaults();  // Reset all purchase counts to zero
}

/**
 * Free allocated memory (call from Python when done with string)
 */
EXPORT void api_free_string(char* str) {
    if (str) free(str);
}
