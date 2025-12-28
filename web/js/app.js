/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           SMART GROCERY CART
 *                    Data Structures Project - Air University
 *                              3rd Semester
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * FILE: app.js
 * PURPOSE: Web interface that connects to C++ data structures via Python API
 * 
 * ARCHITECTURE:
 *   Browser (JS) ──HTTP──▶ Flask (Python) ──ctypes──▶ C++ DLL
 * 
 * This JS file ONLY handles UI. All data structure logic runs in C++!
 * For VIVA preparation, study the C++ code in /src/core/
 */

// API Base URL
const API_BASE = '/api';

// LocalStorage Keys for persistence
const STORAGE_KEYS = {
    CART: 'smartCart_cartItems',
    LAST_SYNC: 'smartCart_lastSync'
};

// DOM Elements
let cartSidebar, cartOverlay, cartItems, cartCount, cartTotal;
let checkoutModal, receiptContent;
let visualPanel, toastContainer;

// Local cache of data (mirrors C++ state)
let frequentItemsCache = [];

// ═══════════════════════════════════════════════════════════════════════════════
//                         INITIALIZATION
// ═══════════════════════════════════════════════════════════════════════════════

document.addEventListener('DOMContentLoaded', function() {
    initializeElements();
    loadFrequentItems();  // Load from C++ via API
    setupEventListeners();
    restoreCartFromStorage();  // Restore saved cart data
    hideLoadingScreen();
});

function initializeElements() {
    cartSidebar = document.getElementById('cartSidebar');
    cartOverlay = document.getElementById('cartOverlay');
    cartItems = document.getElementById('cartItems');
    cartCount = document.getElementById('cartCount');
    cartTotal = document.getElementById('cartTotal');
    checkoutModal = document.getElementById('checkoutModal');
    receiptContent = document.getElementById('receiptContent');
    visualPanel = document.getElementById('visualPanel');
    toastContainer = document.getElementById('toastContainer');
}

function hideLoadingScreen() {
    setTimeout(() => {
        const loadingScreen = document.getElementById('loadingScreen');
        if (loadingScreen) {
            loadingScreen.classList.add('hidden');
        }
    }, 2000);
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         LOCAL STORAGE - Cart Persistence
//                    Saves cart data so it survives page refresh!
// ═══════════════════════════════════════════════════════════════════════════════

/**
 * Save cart items to localStorage
 * Called after every cart modification
 */
function saveCartToStorage(items) {
    try {
        const cartData = {
            items: items,
            savedAt: new Date().toISOString()
        };
        localStorage.setItem(STORAGE_KEYS.CART, JSON.stringify(cartData));
    } catch (error) {
        console.error('Failed to save cart to localStorage:', error);
    }
}

/**
 * Load cart items from localStorage
 */
function loadCartFromStorage() {
    try {
        const data = localStorage.getItem(STORAGE_KEYS.CART);
        if (data) {
            const cartData = JSON.parse(data);
            return cartData.items;
        }
    } catch (error) {
        console.error('Failed to load cart from localStorage:', error);
    }
    return [];
}

/**
 * Sync cart from C++ backend on page load
 * The C++ backend is the SINGLE SOURCE OF TRUTH
 * This ensures all browser windows stay in sync
 */
async function restoreCartFromStorage() {
    // Fetch from C++ backend - it's the authoritative source
    // This ensures all browser windows stay in sync
    await updateCartUI();
    await updateVisualization();
}

/**
 * Clear cart from localStorage
 * Called after successful checkout
 */
function clearCartStorage() {
    try {
        localStorage.removeItem(STORAGE_KEYS.CART);
    } catch (error) {
        console.error('Failed to clear cart storage:', error);
    }
}

function setupEventListeners() {
    // Navbar scroll effect
    window.addEventListener('scroll', function() {
        const navbar = document.querySelector('.navbar');
        if (navbar) {
            if (window.scrollY > 50) {
                navbar.classList.add('scrolled');
            } else {
                navbar.classList.remove('scrolled');
            }
        }
        updateActiveNavLink();
    });

    // Keyboard shortcuts
    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            closeCartPanel();
            closeCheckoutModal();
        }
        if (e.ctrlKey && e.key === 'z') {
            e.preventDefault();
            undoLastAction();
        }
    });
}

function updateActiveNavLink() {
    const sections = document.querySelectorAll('section');
    const navLinks = document.querySelectorAll('.nav-links a');
    
    let current = '';
    sections.forEach(section => {
        const sectionTop = section.offsetTop;
        if (scrollY >= sectionTop - 200) {
            current = section.getAttribute('id');
        }
    });

    navLinks.forEach(link => {
        link.classList.remove('active');
        if (link.getAttribute('href') === '#' + current) {
            link.classList.add('active');
        }
    });
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    ARRAY OPERATIONS - O(1) ACCESS
//                    Data loaded from C++ FrequentItemsArray
// ═══════════════════════════════════════════════════════════════════════════════

async function loadFrequentItems() {
    try {
        const response = await fetch(`${API_BASE}/frequent-items`);
        const result = await response.json();
        
        if (result.success) {
            frequentItemsCache = result.data;
            renderProducts();
        }
    } catch (error) {
        console.error('Failed to load products from C++ API:', error);
        showToast('Failed to load products. Is the server running?', 'error');
    }
}

function renderProducts() {
    const productsGrid = document.getElementById('productsGrid');
    if (!productsGrid) return;
    
    productsGrid.innerHTML = '';

    // Display items from C++ Array - O(1) access per item!
    frequentItemsCache.forEach((item, index) => {
        const productCard = createProductCard(item, index);
        productsGrid.appendChild(productCard);
    });
}

function createProductCard(item, index) {
    const card = document.createElement('div');
    card.className = 'product-card';
    
    // Determine popularity badge based on purchaseCount
    let popularityBadge = '';
    if (item.purchaseCount >= 100) {
        popularityBadge = '<span class="popularity-badge hot">🔥 Best Seller</span>';
    } else if (item.purchaseCount >= 70) {
        popularityBadge = '<span class="popularity-badge popular">⭐ Popular</span>';
    } else if (item.purchaseCount >= 50) {
        popularityBadge = '<span class="popularity-badge trending">📈 Trending</span>';
    }
    
    card.innerHTML = `
        ${popularityBadge}
        <h4>${item.name}</h4>
        <div class="product-price">Rs. ${item.price.toFixed(2)}</div>
        <div class="purchase-info">
            <small><i class="fas fa-shopping-bag"></i> ${item.purchaseCount || 0} purchases</small>
        </div>
        <div class="product-actions">
            <div class="qty-control">
                <button onclick="adjustQty(${index}, -1)">-</button>
                <input type="number" id="qty-${index}" value="1" min="1" max="99">
                <button onclick="adjustQty(${index}, 1)">+</button>
            </div>
            <button class="add-to-cart-btn" onclick="addToCart(${index})">
                <i class="fas fa-plus"></i>
            </button>
        </div>
    `;
    return card;
}

function adjustQty(index, delta) {
    const input = document.getElementById(`qty-${index}`);
    let value = parseInt(input.value) + delta;
    if (value < 1) value = 1;
    if (value > 99) value = 99;
    input.value = value;
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    LINKED LIST OPERATIONS - Shopping Cart
//                    Calls C++ LinkedList via API
// ═══════════════════════════════════════════════════════════════════════════════

async function addToCart(index) {
    const item = frequentItemsCache[index];  // O(1) array access!
    const qtyInput = document.getElementById(`qty-${index}`);
    const quantity = parseInt(qtyInput.value) || 1;

    try {
        const response = await fetch(`${API_BASE}/cart/add`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                name: item.name,
                price: item.price,
                quantity: quantity,
                product_id: item.id  // Use unique item ID, not array index!
            })
        });
        
        const result = await response.json();
        
        if (result.success) {
            qtyInput.value = 1;
            await updateCartUI();
            await updateVisualization();
            showToast(`Added ${quantity}x ${item.name} to cart`, 'success');
        }
    } catch (error) {
        console.error('Failed to add to cart:', error);
        showToast('Failed to add item to cart', 'error');
    }
}

async function addCustomItem() {
    const nameInput = document.getElementById('customName');
    const priceInput = document.getElementById('customPrice');
    const qtyInput = document.getElementById('customQuantity');

    const name = nameInput.value.trim();
    const price = parseFloat(priceInput.value);
    const quantity = parseInt(qtyInput.value) || 1;

    if (!name) {
        showToast('Please enter an item name', 'error');
        return;
    }

    if (!price || price <= 0) {
        showToast('Please enter a valid price', 'error');
        return;
    }

    try {
        const response = await fetch(`${API_BASE}/cart/add`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                name: name,
                price: price,
                quantity: quantity,
                product_id: -1
            })
        });
        
        const result = await response.json();
        
        if (result.success) {
            nameInput.value = '';
            priceInput.value = '';
            qtyInput.value = '1';
            
            await updateCartUI();
            await updateVisualization();
            showToast(`Added ${quantity}x ${name} to cart`, 'success');
        }
    } catch (error) {
        console.error('Failed to add custom item:', error);
        showToast('Failed to add item to cart', 'error');
    }
}

async function removeFromCart(position) {
    try {
        const response = await fetch(`${API_BASE}/cart/remove/${position}`, {
            method: 'DELETE'
        });
        
        const result = await response.json();
        
        if (result.success) {
            await updateCartUI();
            await updateVisualization();
            showToast(`Removed ${result.removed.name} from cart`, 'warning');
        }
    } catch (error) {
        console.error('Failed to remove from cart:', error);
        showToast('Failed to remove item', 'error');
    }
}

/**
 * Clear entire cart - removes all items from C++ backend and localStorage
 */
async function clearCart() {
    try {
        const response = await fetch(`${API_BASE}/cart/clear`, {
            method: 'DELETE'
        });
        
        const result = await response.json();
        
        if (result.success) {
            clearCartStorage();  // Clear localStorage too
            await updateCartUI();
            await updateVisualization();
            showToast('Cart cleared!', 'warning');
        }
    } catch (error) {
        console.error('Failed to clear cart:', error);
        showToast('Failed to clear cart', 'error');
    }
}

/**
 * Factory Reset - Clear ALL data and start completely fresh
 * This resets purchase counts, cart, and all saved data
 */
async function factoryReset() {
    // Confirm with user
    const confirmed = confirm(
        '⚠️ FACTORY RESET ⚠️\n\n' +
        'This will:\n' +
        '• Clear your cart\n' +
        '• Reset ALL purchase counts to zero\n' +
        '• Remove all custom items\n' +
        '• Delete all saved data\n\n' +
        'Are you sure you want to start fresh?'
    );
    
    if (!confirmed) return;
    
    try {
        const response = await fetch(`${API_BASE}/factory-reset`, {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            clearCartStorage();
            await loadFrequentItems();
            await updateCartUI();
            await updateVisualization();
            showToast('🔄 Factory reset complete! Starting fresh.', 'success');
        } else {
            showToast(result.error || 'Reset failed', 'error');
        }
    } catch (error) {
        console.error('Failed to reset:', error);
        showToast('Failed to reset data', 'error');
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    STACK OPERATIONS - Undo (LIFO)
//                    Calls C++ Stack via API
// ═══════════════════════════════════════════════════════════════════════════════

async function undoLastAction() {
    try {
        const response = await fetch(`${API_BASE}/undo`, {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            await updateCartUI();
            await updateVisualization();
            showToast(`Undone: Removed ${result.undone.name}`, 'success');
        } else {
            showToast(result.error || 'No actions to undo', 'warning');
        }
    } catch (error) {
        console.error('Failed to undo:', error);
        showToast('Failed to undo action', 'error');
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    QUEUE OPERATIONS - Checkout (FIFO)
//                    Calls C++ Queue via API
// ═══════════════════════════════════════════════════════════════════════════════

async function proceedToCheckout() {
    try {
        // First check if cart has items
        const cartResponse = await fetch(`${API_BASE}/cart`);
        const cartData = await cartResponse.json();
        
        if (cartData.size === 0) {
            showToast('Your cart is empty!', 'error');
            return;
        }

        // Start checkout - moves items from Linked List to Queue
        // This also updates purchase counts and re-sorts frequent items
        await fetch(`${API_BASE}/checkout/start`, { method: 'POST' });
        
        // Process checkout - dequeues all items (FIFO)
        const response = await fetch(`${API_BASE}/checkout/process`, {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            generateReceipt(result.receipt);
            await updateCartUI();
            closeCartPanel();
            checkoutModal.classList.add('open');
            await updateVisualization();
            
            // Clear localStorage after successful checkout
            clearCartStorage();
            
            // Refresh frequent items
            await loadFrequentItems();
            showToast('Checkout complete! Thank you for shopping.', 'success');
        }
    } catch (error) {
        console.error('Failed to checkout:', error);
        showToast('Failed to process checkout', 'error');
    }
}

function generateReceipt(receipt) {
    const transactionId = Math.floor(Math.random() * 900000) + 100000;
    const date = new Date().toLocaleDateString('en-US', {
        year: 'numeric',
        month: 'long',
        day: 'numeric'
    });

    let itemsHTML = '';
    let total = 0;
    receipt.items.forEach(item => {
        const itemTotal = item.price * item.quantity;
        total += itemTotal;
        itemsHTML += `
            <div class="receipt-item">
                <span class="receipt-item-name">${item.name}</span>
                <span class="receipt-item-qty">x${item.quantity}</span>
                <span class="receipt-item-price">Rs. ${itemTotal.toFixed(2)}</span>
            </div>
        `;
    });

    receiptContent.innerHTML = `
        <div class="receipt-header">
            <h3>Smart Grocery Cart</h3>
            <p>Transaction #${transactionId}</p>
            <p>${date}</p>
        </div>
        <div class="receipt-items">
            ${itemsHTML}
        </div>
        <div class="receipt-totals">
            <div class="receipt-row grand-total">
                <span>Total:</span>
                <span>Rs. ${total.toFixed(2)}</span>
            </div>
        </div>
        <div class="receipt-footer">
            <p class="thanks">Thank you for shopping with us!</p>
            <p>Your order has been processed successfully</p>
            <p style="margin-top: 15px; font-size: 11px; color: #94a3b8;">
                Powered by Smart Grocery Cart System
            </p>
        </div>
    `;
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         UI UPDATE FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

async function updateCartUI() {
    try {
        const response = await fetch(`${API_BASE}/cart`);
        const result = await response.json();
        
        if (result.success) {
            if (cartCount) cartCount.textContent = result.size;
            if (cartTotal) cartTotal.textContent = `Rs. ${result.total.toFixed(2)}`;
            renderCartItems(result.data);
            
            // Save cart to localStorage for persistence
            saveCartToStorage(result.data);
        }
    } catch (error) {
        console.error('Failed to update cart UI:', error);
    }
}

function renderCartItems(items) {
    if (!cartItems) return;
    
    if (items.length === 0) {
        cartItems.innerHTML = `
            <div class="empty-cart">
                <i class="fas fa-shopping-cart"></i>
                <p>Your cart is empty</p>
                <p style="font-size: 12px; color: #94a3b8;">Cart = C++ Linked List</p>
            </div>
        `;
        return;
    }

    cartItems.innerHTML = '';
    items.forEach((item, index) => {
        const icon = getItemIcon(item.name);
        const cartItem = document.createElement('div');
        cartItem.className = 'cart-item';
        cartItem.innerHTML = `
            <div class="cart-item-icon">${icon}</div>
            <div class="cart-item-info">
                <div class="cart-item-name">${item.name}</div>
                <div class="cart-item-price">Rs. ${item.price.toFixed(2)} each</div>
                <div class="cart-item-qty">Qty: ${item.quantity}</div>
            </div>
            <div class="cart-item-total">Rs. ${item.total.toFixed(2)}</div>
            <button class="remove-item" onclick="removeFromCart(${index + 1})">
                <i class="fas fa-trash"></i>
            </button>
        `;
        cartItems.appendChild(cartItem);
    });
}

function getItemIcon(name) {
    const item = frequentItemsCache.find(f => f.name === name);
    return item ? item.icon : '📦';
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         PANEL TOGGLES
// ═══════════════════════════════════════════════════════════════════════════════

function toggleCartPanel() {
    if (cartSidebar) cartSidebar.classList.toggle('open');
    if (cartOverlay) cartOverlay.classList.toggle('open');
}

function closeCartPanel() {
    if (cartSidebar) cartSidebar.classList.remove('open');
    if (cartOverlay) cartOverlay.classList.remove('open');
}

function closeCheckoutModal() {
    if (checkoutModal) checkoutModal.classList.remove('open');
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         VISUALIZATION
//                    Shows actual C++ data structure states
// ═══════════════════════════════════════════════════════════════════════════════

function toggleVisualization() {
    if (visualPanel) {
        visualPanel.classList.toggle('open');
        updateVisualization();
    }
}

function showVisual(type) {
    document.querySelectorAll('.visual-tab').forEach(tab => tab.classList.remove('active'));
    event.target.classList.add('active');

    document.getElementById('linkedlistVisual').style.display = 'none';
    document.getElementById('stackVisual').style.display = 'none';
    document.getElementById('queueVisual').style.display = 'none';

    document.getElementById(`${type}Visual`).style.display = 'flex';
}

async function updateVisualization() {
    await updateLinkedListVisual();
    await updateStackVisual();
    await updateQueueVisual();
}

async function updateLinkedListVisual() {
    const container = document.getElementById('linkedlistVisual');
    if (!container) return;
    
    try {
        const response = await fetch(`${API_BASE}/cart`);
        const result = await response.json();
        const items = result.data || [];

        if (items.length === 0) {
            container.innerHTML = '<div class="visual-empty">head -> NULL (empty C++ linked list)</div>';
            return;
        }

        let html = '<div class="visual-node"><span class="node-label" style="color: #10b981; margin-right: 10px;">HEAD -></span></div>';
        
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box">
                        <div class="node-name">${item.name.substring(0, 10)}...</div>
                        <div class="node-price">Rs. ${item.price.toFixed(2)}</div>
                    </div>
                    <span class="node-arrow">-></span>
                </div>
            `;
        });

        html += '<div class="node-null">NULL</div>';
        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading linked list</div>';
    }
}

async function updateStackVisual() {
    const container = document.getElementById('stackVisual');
    if (!container) return;
    
    try {
        const response = await fetch(`${API_BASE}/stack`);
        const result = await response.json();
        const items = result.data || [];

        if (items.length === 0) {
            container.innerHTML = '<div class="visual-empty">C++ Stack is empty (LIFO)</div>';
            return;
        }

        let html = '';
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box" style="background: ${index === 0 ? 'var(--gradient-3)' : 'var(--gradient-1)'}">
                        ${index === 0 ? '<div class="stack-label"><- TOP (pop here)</div>' : ''}
                        <div class="node-name">${item.name.substring(0, 15)}</div>
                        <div class="node-price">Rs. ${item.price.toFixed(2)}</div>
                    </div>
                </div>
            `;
        });

        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading stack</div>';
    }
}

async function updateQueueVisual() {
    const container = document.getElementById('queueVisual');
    if (!container) return;
    
    try {
        const response = await fetch(`${API_BASE}/queue`);
        const result = await response.json();
        const items = result.data || [];

        if (items.length === 0) {
            container.innerHTML = '<div class="visual-empty">C++ Queue is empty (FIFO)</div>';
            return;
        }

        let html = '<div class="visual-node"><span class="node-label" style="color: #10b981; margin-right: 10px;">FRONT -></span></div>';
        
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box" style="background: ${index === items.length - 1 ? 'var(--gradient-2)' : 'var(--gradient-1)'}">
                        <div class="node-name">${item.name.substring(0, 10)}...</div>
                        <div class="node-price">Rs. ${item.price.toFixed(2)}</div>
                    </div>
                    <span class="node-arrow">-></span>
                </div>
            `;
        });

        html += '<div class="node-null"><- REAR</div>';
        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading queue</div>';
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         TOAST NOTIFICATIONS
// ═══════════════════════════════════════════════════════════════════════════════

function showToast(message, type = 'success') {
    if (!toastContainer) return;
    
    const toast = document.createElement('div');
    toast.className = `toast ${type}`;
    
    let icon = 'check-circle';
    if (type === 'error') icon = 'times-circle';
    if (type === 'warning') icon = 'exclamation-circle';

    toast.innerHTML = `
        <i class="fas fa-${icon}"></i>
        <span class="toast-message">${message}</span>
        <button class="toast-close" onclick="this.parentElement.remove()">
            <i class="fas fa-times"></i>
        </button>
    `;

    toastContainer.appendChild(toast);

    setTimeout(() => {
        toast.style.animation = 'toastSlide 0.3s ease reverse';
        setTimeout(() => toast.remove(), 300);
    }, 3000);
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         NAVIGATION
// ═══════════════════════════════════════════════════════════════════════════════

function scrollToShop() {
    document.getElementById('shop').scrollIntoView({ behavior: 'smooth' });
}

function scrollToFeatures() {
    document.getElementById('features').scrollIntoView({ behavior: 'smooth' });
}

// ═══════════════════════════════════════════════════════════════════════════════
//                         CONSOLE INFO
// ═══════════════════════════════════════════════════════════════════════════════

console.log('%c🛒 Smart Grocery Cart', 'font-size: 20px; font-weight: bold; color: #22C55E;');
