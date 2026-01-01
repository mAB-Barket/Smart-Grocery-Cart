/**
 * ═══════════════════════════════════════════════════════════════════════════════
 *                           SMART GROCERY CART
 *                         Service Worker for PWA
 *                    Data Structures Project - Air University
 * ═══════════════════════════════════════════════════════════════════════════════
 * 
 * This service worker enables:
 * - App installation on Android devices
 * - Caching of static assets for faster loading
 * - Offline fallback page
 */

const CACHE_NAME = 'smart-grocery-cart-v1';
const STATIC_CACHE = 'static-v1';
const DYNAMIC_CACHE = 'dynamic-v1';

// Assets to cache immediately on install
const STATIC_ASSETS = [
    '/',
    '/index.html',
    '/css/styles.css',
    '/js/app.js',
    '/manifest.json',
    '/icons/icon.svg'
];

// External resources to cache
const EXTERNAL_ASSETS = [
    'https://fonts.googleapis.com/css2?family=Poppins:wght@300;400;500;600;700;800&family=Inter:wght@300;400;500;600;700&display=swap',
    'https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/css/all.min.css'
];

// ═══════════════════════════════════════════════════════════════════════════════
//                              INSTALL EVENT
// ═══════════════════════════════════════════════════════════════════════════════

self.addEventListener('install', (event) => {
    console.log('[Service Worker] Installing...');
    
    event.waitUntil(
        caches.open(STATIC_CACHE)
            .then((cache) => {
                console.log('[Service Worker] Caching static assets');
                return cache.addAll(STATIC_ASSETS);
            })
            .then(() => {
                console.log('[Service Worker] Static assets cached');
                return self.skipWaiting();
            })
            .catch((error) => {
                console.log('[Service Worker] Cache failed:', error);
            })
    );
});

// ═══════════════════════════════════════════════════════════════════════════════
//                              ACTIVATE EVENT
// ═══════════════════════════════════════════════════════════════════════════════

self.addEventListener('activate', (event) => {
    console.log('[Service Worker] Activating...');
    
    event.waitUntil(
        caches.keys()
            .then((cacheNames) => {
                return Promise.all(
                    cacheNames
                        .filter((name) => name !== STATIC_CACHE && name !== DYNAMIC_CACHE)
                        .map((name) => {
                            console.log('[Service Worker] Deleting old cache:', name);
                            return caches.delete(name);
                        })
                );
            })
            .then(() => {
                console.log('[Service Worker] Activated');
                return self.clients.claim();
            })
    );
});

// ═══════════════════════════════════════════════════════════════════════════════
//                              FETCH EVENT
// ═══════════════════════════════════════════════════════════════════════════════

self.addEventListener('fetch', (event) => {
    const { request } = event;
    const url = new URL(request.url);
    
    // Skip non-GET requests
    if (request.method !== 'GET') {
        return;
    }
    
    // API calls - Network first, no cache (we need fresh data)
    if (url.pathname.startsWith('/api/')) {
        event.respondWith(
            fetch(request)
                .catch(() => {
                    return new Response(
                        JSON.stringify({ error: 'You are offline. Please connect to use the app.' }),
                        { 
                            status: 503,
                            headers: { 'Content-Type': 'application/json' }
                        }
                    );
                })
        );
        return;
    }
    
    // Static assets - Cache first, then network
    event.respondWith(
        caches.match(request)
            .then((cachedResponse) => {
                if (cachedResponse) {
                    // Return cached version and update cache in background
                    fetchAndCache(request);
                    return cachedResponse;
                }
                
                // Not in cache, fetch from network
                return fetchAndCache(request);
            })
            .catch(() => {
                // Offline fallback for HTML pages
                if (request.headers.get('accept').includes('text/html')) {
                    return caches.match('/index.html');
                }
            })
    );
});

// ═══════════════════════════════════════════════════════════════════════════════
//                              HELPER FUNCTIONS
// ═══════════════════════════════════════════════════════════════════════════════

async function fetchAndCache(request) {
    try {
        const response = await fetch(request);
        
        // Only cache successful responses
        if (response.ok) {
            const cache = await caches.open(DYNAMIC_CACHE);
            cache.put(request, response.clone());
        }
        
        return response;
    } catch (error) {
        console.log('[Service Worker] Fetch failed:', error);
        throw error;
    }
}

// ═══════════════════════════════════════════════════════════════════════════════
//                              PUSH NOTIFICATIONS (Future)
// ═══════════════════════════════════════════════════════════════════════════════

self.addEventListener('push', (event) => {
    const options = {
        body: event.data ? event.data.text() : 'New notification from Smart Grocery Cart',
        icon: '/icons/icon.svg',
        badge: '/icons/icon.svg',
        vibrate: [100, 50, 100],
        data: {
            dateOfArrival: Date.now(),
            primaryKey: 1
        }
    };
    
    event.waitUntil(
        self.registration.showNotification('Smart Grocery Cart', options)
    );
});

// Handle notification clicks
self.addEventListener('notificationclick', (event) => {
    event.notification.close();
    event.waitUntil(
        clients.openWindow('/')
    );
});

console.log('[Service Worker] Loaded');
