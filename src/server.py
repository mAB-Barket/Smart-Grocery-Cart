"""
═══════════════════════════════════════════════════════════════════════════════
                           SMART GROCERY CART
                    Data Structures Project - Air University
                              3rd Semester
═══════════════════════════════════════════════════════════════════════════════

FILE: server.py
PURPOSE: Flask web server that bridges HTML frontend with C++ data structures

This is a shopping list reminder app - helps users remember what to buy
NO PRICES - just item names and quantities
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
CORS(app)

# ═══════════════════════════════════════════════════════════════════════════════
#                           LOAD C++ DLL
# ═══════════════════════════════════════════════════════════════════════════════

if sys.platform == 'win32':
    dll_name = 'grocery_api.dll'
else:
    dll_name = 'libgrocery_api.so'

dll_path = os.path.join(os.path.dirname(__file__), dll_name)

try:
    grocery_lib = ctypes.CDLL(dll_path)
    
    # Array functions
    grocery_lib.api_get_frequent_items_count.restype = ctypes.c_int
    grocery_lib.api_get_frequent_item.argtypes = [ctypes.c_int]
    grocery_lib.api_get_frequent_item.restype = ctypes.c_char_p
    grocery_lib.api_get_all_frequent_items.restype = ctypes.c_char_p
    
    # Linked List (Cart) functions - NO PRICE
    grocery_lib.api_add_to_cart.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
    grocery_lib.api_add_to_cart.restype = None
    grocery_lib.api_remove_from_cart.argtypes = [ctypes.c_int]
    grocery_lib.api_remove_from_cart.restype = ctypes.c_char_p
    grocery_lib.api_get_cart_size.restype = ctypes.c_int
    grocery_lib.api_is_cart_empty.restype = ctypes.c_bool
    grocery_lib.api_get_cart_total_quantity.restype = ctypes.c_int
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
    
    # Custom item restoration function - NO PRICE
    grocery_lib.api_restore_custom_item.argtypes = [ctypes.c_char_p, ctypes.c_int, ctypes.c_int]
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

# ═══════════════════════════════════════════════════════════════════════════════
#                           HELPER FUNCTIONS
# ═══════════════════════════════════════════════════════════════════════════════

def parse_json_response(c_string):
    if c_string:
        return json.loads(c_string.decode('utf-8'))
    return {}

# ═══════════════════════════════════════════════════════════════════════════════
#                    DATA PERSISTENCE (JSON File Storage)
# ═══════════════════════════════════════════════════════════════════════════════

DATA_FILE = os.path.join(os.path.dirname(__file__), 'cart_data.json')

def save_all_data():
    if not DLL_LOADED:
        return False
    
    try:
        result = grocery_lib.api_get_all_frequent_items()
        frequent_items = parse_json_response(result)
        
        cart_result = grocery_lib.api_get_cart_items()
        cart_items = parse_json_response(cart_result)
        
        data = {
            'frequent_items': frequent_items,
            'cart_items': cart_items,
            'last_updated': datetime.now().strftime('%Y-%m-%d %H:%M:%S')
        }
        
        with open(DATA_FILE, 'w', encoding='utf-8') as f:
            json.dump(data, f, indent=2, ensure_ascii=False)
        
        return True
    except Exception as e:
        print(f"❌ Failed to save data: {e}")
        return False

def load_all_data():
    """
    Load all data from JSON file and restore to unified C++ storage.
    All items (default and custom) are stored in ONE array - top 10 shown as frequent.
    """
    if not DLL_LOADED:
        return False
    
    if not os.path.exists(DATA_FILE):
        print("📂 No saved data found, starting fresh")
        return False
    
    try:
        with open(DATA_FILE, 'r', encoding='utf-8') as f:
            data = json.load(f)
        
        # Load all items (unified storage - both default and custom)
        items = data.get('frequent_items', [])
        for item in items:
            item_id = item.get('id', -1)
            purchase_count = item.get('purchaseCount', 0)
            name = item.get('name', '')
            
            if item_id >= 0 and purchase_count > 0 and name:
                # Use api_restore_custom_item for ALL items
                # It handles both default (id 0-9) and custom (id >= 1000) items
                grocery_lib.api_restore_custom_item(
                    name.encode('utf-8'),
                    ctypes.c_int(purchase_count),
                    ctypes.c_int(item_id)
                )
        
        # Restore cart items
        cart_items = data.get('cart_items', [])
        for cart_item in cart_items:
            name = cart_item.get('name', '')
            quantity = cart_item.get('quantity', 1)
            product_id = cart_item.get('product_id', -1)
            
            if name:
                grocery_lib.api_add_to_cart(
                    name.encode('utf-8'),
                    ctypes.c_int(quantity),
                    ctypes.c_int(product_id)
                )
        
        cart_count = len(cart_items)
        print(f"✅ Data loaded: {len(items)} items, {cart_count} cart items")
        print("📊 Previous data restored (top 10 shown as frequent items)!")
        return True
    except Exception as e:
        print(f"❌ Failed to load data: {e}")
        return False

# ═══════════════════════════════════════════════════════════════════════════════
#                           API ROUTES
# ═══════════════════════════════════════════════════════════════════════════════

@app.route('/')
def index():
    return send_from_directory(app.static_folder, 'index.html')

@app.route('/api/frequent-items', methods=['GET'])
def get_frequent_items():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_get_all_frequent_items()
    items = parse_json_response(result)
    
    return jsonify({
        'success': True,
        'data': items,
        'count': len(items)
    })

@app.route('/api/cart/add', methods=['POST'])
def add_to_cart():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    data = request.get_json()
    name = data.get('name', '')
    quantity = data.get('quantity', 1)
    product_id = data.get('product_id', -1)
    
    grocery_lib.api_add_to_cart(
        name.encode('utf-8'),
        ctypes.c_int(quantity),
        ctypes.c_int(product_id)
    )
    
    save_all_data()
    
    return jsonify({
        'success': True,
        'message': f'Added {quantity}x {name} to cart'
    })

@app.route('/api/cart/remove/<int:position>', methods=['DELETE'])
def remove_from_cart(position):
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_remove_from_cart(position)
    removed = parse_json_response(result)
    
    save_all_data()
    
    return jsonify({
        'success': True,
        'removed': removed
    })

@app.route('/api/cart', methods=['GET'])
def get_cart():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_get_cart_items()
    items = parse_json_response(result)
    size = grocery_lib.api_get_cart_size()
    total_qty = grocery_lib.api_get_cart_total_quantity()
    
    return jsonify({
        'success': True,
        'data': items,
        'size': size,
        'totalQuantity': total_qty
    })

@app.route('/api/cart/clear', methods=['DELETE'])
def clear_cart():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    grocery_lib.api_clear_cart()
    grocery_lib.api_clear_undo_stack()
    save_all_data()
    
    return jsonify({'success': True, 'message': 'Cart cleared'})

@app.route('/api/undo', methods=['POST'])
def undo_action():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_undo_last_action()
    undone = parse_json_response(result)
    
    if 'error' in undone:
        return jsonify({'success': False, 'error': undone['error']})
    
    save_all_data()
    
    return jsonify({
        'success': True,
        'undone': undone
    })

@app.route('/api/stack', methods=['GET'])
def get_stack():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_get_stack_items()
    items = parse_json_response(result)
    
    return jsonify({
        'success': True,
        'data': items,
        'size': grocery_lib.api_get_undo_stack_size()
    })

@app.route('/api/checkout/start', methods=['POST'])
def start_checkout():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    grocery_lib.api_start_checkout()
    save_all_data()
    
    return jsonify({'success': True, 'message': 'Checkout started'})

@app.route('/api/checkout/process', methods=['POST'])
def process_checkout():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_process_checkout()
    receipt = parse_json_response(result)
    
    save_all_data()
    
    return jsonify({
        'success': True,
        'receipt': receipt
    })

@app.route('/api/queue', methods=['GET'])
def get_queue():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    result = grocery_lib.api_get_queue_items()
    items = parse_json_response(result)
    
    return jsonify({
        'success': True,
        'data': items,
        'size': grocery_lib.api_get_queue_size()
    })

@app.route('/api/factory-reset', methods=['POST'])
def factory_reset():
    if not DLL_LOADED:
        return jsonify({'success': False, 'error': 'C++ library not loaded'}), 500
    
    grocery_lib.api_factory_reset()
    
    if os.path.exists(DATA_FILE):
        os.remove(DATA_FILE)
    
    return jsonify({
        'success': True,
        'message': 'Factory reset complete'
    })

# ═══════════════════════════════════════════════════════════════════════════════
#                           SERVER STARTUP
# ═══════════════════════════════════════════════════════════════════════════════

if __name__ == '__main__':
    print("\n" + "═" * 60)
    print("       🛒 SMART GROCERY CART")
    print("═" * 60)
    
    if DLL_LOADED:
        print("\n📦 C++ Backend: ✅ Loaded")
        if load_all_data():
            pass
        else:
            print("📂 Starting with default items")
    else:
        print("\n📦 C++ Backend: ❌ Not loaded")
    
    print("\n🌐 Server starting...")
    print("   Open http://localhost:5000 in your browser")
    print("\n" + "═" * 60 + "\n")
    
    app.run(host='0.0.0.0', port=5000, debug=True)
