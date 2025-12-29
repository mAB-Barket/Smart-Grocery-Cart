#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
using namespace std;

class Product {
private:
    string name;
    int quantity;
    int product_id;

public:
    Product() {
        name = "";
        quantity = 1;
        product_id = 0;
    }
    
    Product(string n, int q = 1, int id = 0) {
        name = n;
        quantity = q;
        product_id = id;
    }
    
    Product(const Product& other) {
        name = other.name;
        quantity = other.quantity;
        product_id = other.product_id;
    }

    ~Product() {}

    string getName() const { return name; }
    int getQuantity() const { return quantity; }
    int getProductId() const { return product_id; }

    void setName(string n) { name = n; }
    void setQuantity(int q) { quantity = q; }
    void setProductId(int id) { product_id = id; }

    bool equals(const Product& other) const {
        return name == other.name;
    }

    bool operator==(const Product& other) const {
        return name == other.name;
    }

    void display() const {
        cout << "Product: " << name 
             << " | Qty: " << quantity << endl;
    }

    friend ostream& operator<<(ostream& os, const Product& p) {
        os << p.name << " (x" << p.quantity << ")";
        return os;
    }
};

#endif
