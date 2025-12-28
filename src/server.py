"""
═══════════════════════════════════════════════════════════════════════════════
                           SMART GROCERY CART
                    Data Structures Project - Air University
                              3rd Semester
═══════════════════════════════════════════════════════════════════════════════

FILE: server.py
PURPOSE: Flask web server that bridges HTML frontend with C++ data structures

ARCHITECTURE:
    ┌──────────────┐      HTTP      ┌──────────────┐     ctypes     ┌──────────────┐
    │   Browser    │ ────────────▶  │  Flask (Py)  │ ────────────▶  │  C++ DLL     │
    │   (HTML/JS)  │ ◀────────────  │   server.py  │ ◀────────────  │  grocery_api │
    └──────────────┘     JSON       └──────────────┘    Data        └──────────────┘

HOW TO RUN:
    1. Build the C++ DLL first (see build.bat or build.sh)
    2. pip install flask flask-cors
    3. python server.py
    4. Open http://localhost:5000 in browser
"""

from flask import Flask, jsonify, request, send_from_directory
from flask_cors import CORS
import ctypes
import os
import sys
import json
from datetime import datetime

# ═══════════════════════════════════════════════════════════════════════════════
#                              FLASK APP SETUP
# ═══════════════════════════════════════════════════════════════════════════════

app = Flask(__name__, static_folder='../web', static_url_path='')
CORS(app)  # Enable Cross-Origin Resource Sharing

# ═══════════════════════════════════════════════════════════════════════════════
#                           LOAD C++ DLL
# ═══════════════════════════════════════════════════════════════════════════════

# Determine the DLL/shared library path
if sys.platform == 'win32':
    dll_name = 'grocery_api.dll'
else:
    dll_name = 'libgrocery_api.so'

dll_path = os.path.join(os.path.dirname(__file__), dll_name)

try:
    # Load the C++ shared library
    grocery_lib = ctypes.CDLL(dll_path)
    
    # ═══════════════════════════════════════════════════════════════════════════
    #                    DEFINE C FUNCTION SIGNATURES
    # ═══════════════════════════════════════════════════════════════════════════
    
    # Array functions
    grocery_lib.api_get_frequent_items_count.restype = ctypes.c_int
    grocery_lib.api_get_frequent_item.argtypes = [ctypes.c_int]
    grocery_lib.api_get_frequent_item.restype = ctypes.c_char_p
    grocery_lib.api_get_all_frequent_items.restype = ctypes.c_char_p
    
    # Linked List (Cart) functions
    grocery_lib.api_add_to_cart.argtypes = [ctypes.c_char_p, ctypes.c_double, ctypes.c_int, ctypes.c_int]
    grocery_lib.api_add_to_cart.restype = None
    grocery_lib.api_remove_from_cart.argtypes = [ctypes.c_int]
    grocery_lib.api_remove_from_cart.restype = ctypes.c_char_p
    grocery_lib.api_get_cart_size.restype = ctypes.c_int
    grocery_lib.api_is_cart_empty.restype = ctypes.c_bool
    grocery_lib.api_get_cart_total.restype = ctypes.c_double
    grocery_lib.api_get_cart_items.restype = ctypes.c_char_p
    grocery_lib.api_clear_cart.restype = None
    
    # Stack (Undo) functions
    grocery_lib.api_undo_last_action.restype = ctypes.c_char_p
    grocery_lib.api_get_undo_stack_size.restype = ctypes.c_int
    grocery_lib.api_is_undo_stack_empty.restype = ctypes.c_bool
    grocery_lib.api_get_stack_items.restype = ctypes.c_char_p
    grocery_lib.api_clear_undo_stack.restype = None
    
    # Queue (Checkout) functions
    grocery_lib.api_start_checkout.restype = None
    grocery_lib.api_get_queue_size.restype = ctypes.c_int
    grocery_lib.api_process_checkout.restype = ctypes.c_char_p
    grocery_lib.api_get_queue_items.restype = ctypes.c_char_p
    
    # Purchase count update function
    grocery_lib.api_increment_purchase_count_by_id.argtypes = [ctypes.c_int]
    grocery_lib.api_increment_purchase_count_by_id.restype = None
    
    # Custom item restoration function (for data persistence)
    grocery_lib.api_restore_custom_item.argtypes = [ctypes.c_char_p, ctypes.c_double, ctypes.c_int, ctypes.c_int]
    grocery_lib.api_restore_custom_item.restype = None
    
    # Utility functions
    grocery_lib.api_reset_all.restype = None
    grocery_lib.api_factory_reset.restype = None
    grocery_lib.api_free_string.argtypes = [ctypes.c_char_p]
    grocery_lib.api_free_string.restype = None
    
    DLL_LOADED = True
    print(f"✅ C++ Library loaded successfully: {dll_path}")
    
except OSError as e:
    DLL_LOADED = False
    print(f"⚠️  Warning: Could not load C++ library: {e}")
    print("   Running in fallback mode with Python implementations")

# ═══════════════════════════════════════════════════════════════════════════════
#                           HELPER FUNCTIONS
# ═══════════════════════════════════════════════════════════════════════════════

def parse_json_response(c_string):
    """Parse C string response to Python dict"""
    if c_string:
        return json.loads(c_string.decode('utf-8'))
    return {}

# ═══════════════════════════════════════════════════════════════════════════════
#                    DATA PERSISTENCE (JSON File Storage)
# ═══════════════════════════════════════════════════════════════════════════════

DATA_FILE = os.path.join(os.path.dirname(__file__), 'cart_data.json')

def save_all_data():
    """
    Save current state (frequent items + cart items) to JSON file.
    Called after cart changes and checkout to persist data.
    """
    if not DLL_LOADED:
        return False
    
    try:
        # Get frequent items
        result = grocery_lib.api_get_all_frequent_items()
        frequent_items = parse_json_response(result)
        
        # Get cart items
        cart_result = grocery_lib.api_get_cart_items()
        cart_items = parse_json_response(cart_result)
        
        data = {
            'frequent_items': frequent_items,
            'cart_items': cart_items,
            'last_updated': datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        }
        
        with open(DATA_FILE, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        
        print(f"💾 Data saved to {DATA_FILE}")
        return True
    except Exception as e:
        print(f"❌ Failed to save data: {e}")
        return False

def load_all_data():
    """
    Load all data (purchase counts + cart items) from JSON file and update C++ backend.
    Called when server starts.
    """
    if not DLL_LOADED:
        return False
    
    if not os.path.exists(DATA_FILE):
        print("📂 No saved data found, starting fresh")
        return False
    
    try:
        with open(DATA_FILE, 'r', encoding='utf-8') as f:
            data = json.load(f)
        
        # Restore purchase counts
        items = data.get('frequent_items', [])
        for item in items:
            item_id = item.get('id', -1)
            purchase_count = item.get('purchaseCount', 0)
            is_custom = item.get('isCustom', False)
            
            if item_id >= 0 and purchase_count > 0:
                if is_custom or item_id >= 1000:
                    # This is a custom item - use the restore function
                    name = item.get('name', '')
                    price = item.get('price', 0)
                    grocery_lib.api_restore_custom_item(
                        name.encode('utf-8'),
                        ctypes.c_double(price),
                        ctypes.c_int(purchase_count),
                        ctypes.c_int(item_id)
                    )
                else:
                    # Regular frequent item - increment purchase count
                    for _ in range(purchase_count):
                        grocery_lib.api_increment_purchase_count_by_id(item_id)
        
        # Restore cart items
        cart_items = data.get('cart_items', [])
        for cart_item in cart_items:
            name = cart_item.get('name', '')
            price = cart_item.get('price', 0)
            quantity = cart_item.get('quantity', 1)
            product_id = cart_item.get('product_id', -1)
            
            if name:
                grocery_lib.api_add_to_cart(
                    name.encode('utf-8'),
                    ctypes.c_double(price),
                    ctypes.c_int(quantity),
                    ctypes.c_int(product_id)
                )
        
        cart_count = len(cart_items)
        print(f"✅ Data loaded: {len(items)} frequent items, {cart_count} cart items")
        return True
    except Exception as e:
        print(f"❌ Failed to load data: {e}")
        return False

# ═══════════════════════════════════════════════════════════════════════════════
#                           STATIC FILES ROUTES
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/')
def serve_index():
    """Serve the main HTML page"""
    return send_from_directory(app.static_folder, 'index.html')

@app.route('/<path:path>')
def serve_static(path):
    """Serve static files (CSS, JS, etc.)"""
    return send_from_directory(app.static_folder, path)

# ═══════════════════════════════════════════════════════════════════════════════
#                    API ROUTES - ARRAY (Frequent Items)
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/api/frequent-items', methods=['GET'])
def get_frequent_items():
    """
    Get all frequent items - O(n) iteration but each access is O(1)!
    
    📚 COURSE CONCEPT: Array provides O(1) random access
    """
    if DLL_LOADED:
        result = grocery_lib.api_get_all_frequent_items()
        items = parse_json_response(result)
        return jsonify({
            'success': True,
            'data': items,
            'dataStructure': 'Array',
            'timeComplexity': 'O(1) per access'
        })
    else:
        # Fallback - return static data
        return jsonify({
            'success': True,
            'data': [
                {"name": "Milk (1 Liter)", "price": 80, "icon": "🥛"},
                {"name": "Bread (Whole Wheat)", "price": 60, "icon": "🍞"},
                {"name": "Eggs (Dozen)", "price": 120, "icon": "🥚"},
                {"name": "Butter", "price": 150, "icon": "🧈"},
                {"name": "Cheese (Cheddar)", "price": 250, "icon": "🧀"},
                {"name": "Chicken Breast", "price": 350, "icon": "🍗"},
                {"name": "Rice (5 kg bag)", "price": 450, "icon": "🍚"},
                {"name": "Pasta", "price": 90, "icon": "🍝"},
                {"name": "Tomato Sauce", "price": 70, "icon": "🥫"},
                {"name": "Orange Juice", "price": 180, "icon": "🍊"}
            ],
            'dataStructure': 'Array (Fallback)',
            'timeComplexity': 'O(1) per access'
        })

@app.route('/api/frequent-items/<int:index>', methods=['GET'])
def get_frequent_item(index):
    """
    Get single frequent item by index - O(1) direct access!
    
    📚 COURSE CONCEPT: Array[index] calculates address directly
    Address = Base + (index × sizeof(element))
    """
    if DLL_LOADED:
        result = grocery_lib.api_get_frequent_item(index)
        item = parse_json_response(result)
        return jsonify({
            'success': True,
            'data': item,
            'index': index,
            'timeComplexity': 'O(1) - Direct array access!'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

# ═══════════════════════════════════════════════════════════════════════════════
#                    API ROUTES - LINKED LIST (Cart)
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/api/cart', methods=['GET'])
def get_cart():
    """
    Get all cart items - O(n) traversal
    
    📚 COURSE CONCEPT: Linked List requires traversal from head
    """
    if DLL_LOADED:
        result = grocery_lib.api_get_cart_items()
        items = parse_json_response(result)
        total = grocery_lib.api_get_cart_total()
        size = grocery_lib.api_get_cart_size()
        
        return jsonify({
            'success': True,
            'data': items,
            'total': total,
            'size': size,
            'dataStructure': 'Linked List',
            'timeComplexity': 'O(n) traversal'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/cart/add', methods=['POST'])
def add_to_cart():
    """
    Add item to cart - O(1) insertion at head
    
    📚 COURSE CONCEPT: Linked List head insertion is O(1)
    Also pushes to undo Stack (LIFO)
    """
    data = request.json
    name = data.get('name', '')
    price = data.get('price', 0)
    quantity = data.get('quantity', 1)
    product_id = data.get('product_id', -1)
    
    if DLL_LOADED:
        grocery_lib.api_add_to_cart(
            name.encode('utf-8'),
            ctypes.c_double(price),
            ctypes.c_int(quantity),
            ctypes.c_int(product_id)
        )
        
        # Save cart data for persistence
        save_all_data()
        
        return jsonify({
            'success': True,
            'message': f'Added {quantity}x {name} to cart',
            'dataStructure': 'Linked List (push_item)',
            'undoStack': 'Also pushed to Stack for undo (LIFO)'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/cart/remove/<int:position>', methods=['DELETE'])
def remove_from_cart(position):
    """
    Remove item from cart at position - O(n) to traverse to position
    
    📚 COURSE CONCEPT: Linked List deletion requires traversal
    """
    if DLL_LOADED:
        result = grocery_lib.api_remove_from_cart(position)
        removed = parse_json_response(result)
        
        # Save cart data for persistence
        save_all_data()
        
        return jsonify({
            'success': True,
            'removed': removed,
            'dataStructure': 'Linked List (delete_at_position)',
            'timeComplexity': 'O(n) - traverse to position'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/cart/clear', methods=['DELETE'])
def clear_cart():
    """
    Clear entire cart - O(n) to delete all nodes
    
    📚 COURSE CONCEPT: Linked List clear traverses and deletes all nodes
    """
    if DLL_LOADED:
        grocery_lib.api_clear_cart()
        
        # Save cart data for persistence
        save_all_data()
        
        return jsonify({
            'success': True,
            'message': 'Cart cleared',
            'dataStructure': 'Linked List (clear)',
            'timeComplexity': 'O(n) - delete all nodes'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

# ═══════════════════════════════════════════════════════════════════════════════
#                    API ROUTES - STACK (Undo)
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/api/undo', methods=['POST'])
def undo_action():
    """
    Undo last action - Stack POP (LIFO) - O(1)
    
    📚 COURSE CONCEPT: Stack follows Last-In-First-Out
    The most recent action is undone first
    """
    if DLL_LOADED:
        result = grocery_lib.api_undo_last_action()
        undone = parse_json_response(result)
        
        if 'error' in undone:
            return jsonify({
                'success': False,
                'error': undone['error'],
                'dataStructure': 'Stack (LIFO)'
            })
        
        # Save cart data after undo (cart was modified)
        save_all_data()
        
        return jsonify({
            'success': True,
            'undone': undone,
            'dataStructure': 'Stack (pop)',
            'principle': 'LIFO - Last In First Out',
            'timeComplexity': 'O(1)'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/stack', methods=['GET'])
def get_stack():
    """Get stack items for visualization"""
    if DLL_LOADED:
        result = grocery_lib.api_get_stack_items()
        items = parse_json_response(result)
        
        return jsonify({
            'success': True,
            'data': items,
            'size': grocery_lib.api_get_undo_stack_size(),
            'dataStructure': 'Stack (LIFO)'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

# ═══════════════════════════════════════════════════════════════════════════════
#                    API ROUTES - QUEUE (Checkout)
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/api/checkout/start', methods=['POST'])
def start_checkout():
    """
    Move cart items to checkout queue - Multiple enqueue operations
    
    📚 COURSE CONCEPT: Queue follows First-In-First-Out
    Items are enqueued in order they were added to cart
    """
    if DLL_LOADED:
        grocery_lib.api_start_checkout()
        
        return jsonify({
            'success': True,
            'message': 'Items moved to checkout queue',
            'dataStructure': 'Queue (enqueue)',
            'principle': 'FIFO - First In First Out'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/checkout/process', methods=['POST'])
def process_checkout():
    """
    Process checkout - Dequeue all items (FIFO) - O(1) per dequeue
    
    📚 COURSE CONCEPT: Queue dequeue removes from front
    First item added is processed first
    """
    if DLL_LOADED:
        result = grocery_lib.api_process_checkout()
        receipt = parse_json_response(result)
        
        # Save all data to file for persistence
        save_all_data()
        
        return jsonify({
            'success': True,
            'receipt': receipt,
            'dataStructure': 'Queue (dequeue)',
            'principle': 'FIFO - First item added is billed first',
            'timeComplexity': 'O(1) per dequeue'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/queue', methods=['GET'])
def get_queue():
    """Get queue items for visualization"""
    if DLL_LOADED:
        result = grocery_lib.api_get_queue_items()
        items = parse_json_response(result)
        
        return jsonify({
            'success': True,
            'data': items,
            'size': grocery_lib.api_get_queue_size(),
            'dataStructure': 'Queue (FIFO)'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

# ═══════════════════════════════════════════════════════════════════════════════
#                    API ROUTES - UTILITY
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/api/reset', methods=['POST'])
def reset_all():
    """Reset all data structures"""
    if DLL_LOADED:
        grocery_lib.api_reset_all()
        return jsonify({
            'success': True,
            'message': 'All data structures reset'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/factory-reset', methods=['POST'])
def factory_reset():
    """
    Factory reset - Clear EVERYTHING and start fresh
    This resets all purchase counts to zero and clears saved data
    """
    if DLL_LOADED:
        # Reset C++ data structures and purchase counts
        grocery_lib.api_factory_reset()
        
        # Delete the saved data file
        if os.path.exists(DATA_FILE):
            os.remove(DATA_FILE)
            print("🗑️ Saved data file deleted")
        
        print("🔄 Factory reset complete - all data cleared!")
        
        return jsonify({
            'success': True,
            'message': 'Factory reset complete! All purchase counts reset to zero.'
        })
    else:
        return jsonify({'success': False, 'error': 'DLL not loaded'})

@app.route('/api/status', methods=['GET'])
def get_status():
    """Get system status"""
    return jsonify({
        'success': True,
        'dll_loaded': DLL_LOADED,
        'dll_path': dll_path if DLL_LOADED else None,
        'data_structures': {
            'array': 'Frequent Items (O(1) access)',
            'linked_list': 'Shopping Cart (dynamic)',
            'stack': 'Undo Operations (LIFO)',
            'queue': 'Checkout Process (FIFO)'
        }
    })

# ═══════════════════════════════════════════════════════════════════════════════
#                              MAIN
# ═══════════════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    print("\n" + "═" * 60)
    print("       🛒 SMART GROCERY CART")
    print("═" * 60)
    print(f"\n📦 C++ Backend: {'✅ Loaded' if DLL_LOADED else '❌ Not loaded'}")
    
    # Load saved data (purchase counts + cart items) from file
    if DLL_LOADED:
        if load_all_data():
            print("📊 Previous data restored (cart + purchase counts)!")
        else:
            print("📊 Starting with fresh data")
    
    print("\n🌐 Server starting...")
    print("   Open http://localhost:5000 in your browser\n")
    print("═" * 60 + "\n")
    
    app.run(debug=True, host='0.0.0.0', port=5000)
