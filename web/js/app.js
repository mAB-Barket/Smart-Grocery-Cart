/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           SMART GROCERY CART
 *                    Data Structures Project - Air University
 *                              3rd Semester
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * Shopping List Reminder App - helps users remember what to buy
 * NO PRICES - just item names and quantities
 */

const API_BASE = '/api';

let cartSidebar, cartOverlay, cartItems, cartCount;
let checkoutModal, receiptContent;
let visualPanel, toastContainer;
let resetModal;

let frequentItemsCache = [];

// ═══════════════════════════════════════════════════════════════════════════════
//                         INITIALIZATION
// ═══════════════════════════════════════════════════════════════════════════════

document.addEventListener('DOMContentLoaded', function() {
    initializeElements();
    loadFrequentItems();
    setupEventListeners();
    restoreCartFromStorage();
    hideLoadingScreen();
});

function initializeElements() {
    cartSidebar = document.getElementById('cartSidebar');
    cartOverlay = document.getElementById('cartOverlay');
    cartItems = document.getElementById('cartItems');
    cartCount = document.getElementById('cartCount');
    checkoutModal = document.getElementById('checkoutModal');
    receiptContent = document.getElementById('receiptContent');
    visualPanel = document.getElementById('visualPanel');
    toastContainer = document.getElementById('toastContainer');
    resetModal = document.getElementById('resetModal');
}

function hideLoadingScreen() {
    setTimeout(() => {
        const loadingScreen = document.getElementById('loadingScreen');
        if (loadingScreen) {
            loadingScreen.classList.add('hidden');
        }
    }, 2000);
}

async function restoreCartFromStorage() {
    await updateCartUI();
    await updateVisualization();
}

function setupEventListeners() {
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

    document.addEventListener('keydown', function(e) {
        if (e.key === 'Escape') {
            closeCartPanel();
            closeCheckoutModal();
            closeResetModal();
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
//                    ARRAY OPERATIONS - Frequent Items
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
        console.error('Failed to load products:', error);
        showToast('Failed to load products. Is the server running?', 'error');
    }
}

function renderProducts() {
    const productsGrid = document.getElementById('productsGrid');
    if (!productsGrid) return;
    
    productsGrid.innerHTML = '';

    frequentItemsCache.forEach((item, index) => {
        const productCard = createProductCard(item, index);
        productsGrid.appendChild(productCard);
    });
}

function createProductCard(item, index) {
    const card = document.createElement('div');
    card.className = 'product-card';
    
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
// ═══════════════════════════════════════════════════════════════════════════════

async function addToCart(index) {
    const item = frequentItemsCache[index];
    const qtyInput = document.getElementById(`qty-${index}`);
    const quantity = parseInt(qtyInput.value) || 1;

    try {
        const response = await fetch(`${API_BASE}/cart/add`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                name: item.name,
                quantity: quantity,
                product_id: item.id
            })
        });
        
        const result = await response.json();
        
        if (result.success) {
            qtyInput.value = 1;
            await updateCartUI();
            await updateVisualization();
            showToast(`Added ${quantity}x ${item.name} to list`, 'success');
        }
    } catch (error) {
        console.error('Failed to add to cart:', error);
        showToast('Failed to add item', 'error');
    }
}

async function addCustomItem() {
    const nameInput = document.getElementById('customName');
    const qtyInput = document.getElementById('customQuantity');

    const name = nameInput.value.trim();
    const quantity = parseInt(qtyInput.value) || 1;

    if (!name) {
        showToast('Please enter an item name', 'error');
        return;
    }

    try {
        const response = await fetch(`${API_BASE}/cart/add`, {
            method: 'POST',
            headers: { 'Content-Type': 'application/json' },
            body: JSON.stringify({
                name: name,
                quantity: quantity,
                product_id: -1
            })
        });
        
        const result = await response.json();
        
        if (result.success) {
            nameInput.value = '';
            qtyInput.value = '1';
            
            await updateCartUI();
            await updateVisualization();
            showToast(`Added ${quantity}x ${name} to list`, 'success');
        }
    } catch (error) {
        console.error('Failed to add custom item:', error);
        showToast('Failed to add item', 'error');
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
            showToast(`Removed ${result.removed.name} from list`, 'warning');
        }
    } catch (error) {
        console.error('Failed to remove from cart:', error);
        showToast('Failed to remove item', 'error');
    }
}

async function clearCart() {
    try {
        const response = await fetch(`${API_BASE}/cart/clear`, {
            method: 'DELETE'
        });
        
        const result = await response.json();
        
        if (result.success) {
            await updateCartUI();
            await updateVisualization();
            showToast('List cleared!', 'warning');
        }
    } catch (error) {
        console.error('Failed to clear cart:', error);
        showToast('Failed to clear list', 'error');
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//                    FACTORY RESET - With Modal
// ═══════════════════════════════════════════════════════════════════════════════

function showResetModal() {
    if (resetModal) {
        resetModal.classList.add('open');
    }
}

function closeResetModal() {
    if (resetModal) {
        resetModal.classList.remove('open');
    }
}

async function confirmFactoryReset() {
    try {
        const response = await fetch(`${API_BASE}/factory-reset`, {
            method: 'POST'
        });
        
        const result = await response.json();
        
        if (result.success) {
            closeResetModal();
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
// ═══════════════════════════════════════════════════════════════════════════════

async function proceedToCheckout() {
    try {
        const cartResponse = await fetch(`${API_BASE}/cart`);
        const cartData = await cartResponse.json();
        
        if (cartData.size === 0) {
            showToast('Your list is empty!', 'error');
            return;
        }

        await fetch(`${API_BASE}/checkout/start`, { method: 'POST' });
        
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
            
            await loadFrequentItems();
            showToast('Shopping complete! Items tracked.', 'success');
        }
    } catch (error) {
        console.error('Failed to checkout:', error);
        showToast('Failed to complete shopping', 'error');
    }
}

function generateReceipt(receipt) {
    const date = new Date().toLocaleDateString('en-US', {
        year: 'numeric',
        month: 'long',
        day: 'numeric'
    });

    let itemsHTML = '';
    receipt.items.forEach(item => {
        itemsHTML += `
            <div class="receipt-item">
                <span class="receipt-item-name">${item.name}</span>
                <span class="receipt-item-qty">x${item.quantity}</span>
            </div>
        `;
    });

    receiptContent.innerHTML = `
        <div class="receipt-header">
            <h3>Shopping Complete!</h3>
            <p>${date}</p>
        </div>
        <div class="receipt-items">
            ${itemsHTML}
        </div>
        <div class="receipt-totals">
            <div class="receipt-row grand-total">
                <span>Total Items:</span>
                <span>${receipt.totalItems}</span>
            </div>
        </div>
        <div class="receipt-footer">
            <p class="thanks">Items have been tracked!</p>
            <p>Your purchase history has been updated</p>
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
            renderCartItems(result.data);
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
                <i class="fas fa-clipboard-list"></i>
                <p>Your shopping list is empty</p>
                <p style="font-size: 12px; color: #94a3b8;">Add items to remember what to buy</p>
            </div>
        `;
        return;
    }

    cartItems.innerHTML = '';
    items.forEach((item, index) => {
        const cartItem = document.createElement('div');
        cartItem.className = 'cart-item';
        cartItem.innerHTML = `
            <div class="cart-item-info">
                <div class="cart-item-name">${item.name}</div>
                <div class="cart-item-qty">Quantity: ${item.quantity}</div>
            </div>
            <button class="remove-item" onclick="removeFromCart(${index + 1})">
                <i class="fas fa-trash"></i>
            </button>
        `;
        cartItems.appendChild(cartItem);
    });
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
            container.innerHTML = '<div class="visual-empty">head -> NULL (empty list)</div>';
            return;
        }

        let html = '<div class="visual-node"><span class="node-label" style="color: #10b981; margin-right: 10px;">HEAD -></span></div>';
        
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box">
                        <div class="node-name">${item.name.substring(0, 12)}${item.name.length > 12 ? '...' : ''}</div>
                        <div class="node-qty">x${item.quantity}</div>
                    </div>
                    <span class="node-arrow">-></span>
                </div>
            `;
        });

        html += '<div class="node-null">NULL</div>';
        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading</div>';
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
            container.innerHTML = '<div class="visual-empty">Stack is empty (LIFO)</div>';
            return;
        }

        let html = '';
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box" style="background: ${index === 0 ? 'var(--gradient-3)' : 'var(--gradient-1)'}">
                        ${index === 0 ? '<div class="stack-label"><- TOP</div>' : ''}
                        <div class="node-name">${item.name.substring(0, 15)}</div>
                        <div class="node-qty">x${item.quantity}</div>
                    </div>
                </div>
            `;
        });

        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading</div>';
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
            container.innerHTML = '<div class="visual-empty">Queue is empty (FIFO)</div>';
            return;
        }

        let html = '<div class="visual-node"><span class="node-label" style="color: #10b981; margin-right: 10px;">FRONT -></span></div>';
        
        items.forEach((item, index) => {
            html += `
                <div class="visual-node">
                    <div class="node-box" style="background: ${index === items.length - 1 ? 'var(--gradient-2)' : 'var(--gradient-1)'}">
                        <div class="node-name">${item.name.substring(0, 12)}${item.name.length > 12 ? '...' : ''}</div>
                        <div class="node-qty">x${item.quantity}</div>
                    </div>
                    <span class="node-arrow">-></span>
                </div>
            `;
        });

        html += '<div class="node-null"><- REAR</div>';
        container.innerHTML = html;
    } catch (error) {
        container.innerHTML = '<div class="visual-empty">Error loading</div>';
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

console.log('%c🛒 Smart Grocery Cart', 'font-size: 20px; font-weight: bold; color: #22C55E;');
