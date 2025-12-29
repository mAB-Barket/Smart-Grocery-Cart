#ifndef PRODUCT_H
#define PRODUCT_H

#include <iostream>
#include <string>
using namespace std;

class Product {
private:
    string name;
    double price;
    int quantity;
    int product_id;

public:
    Product() {
        name = "";
        price = 0.0;
        quantity = 1;
        product_id = 0;
    }
    
    Product(string n, double p, int q = 1, int id = 0) {
        name = n;
        price = p;
        quantity = q;
        product_id = id;
    }
    
    Product(const Product& other) {
        name = other.name;
        price = other.price;
        quantity = other.quantity;
        product_id = other.product_id;
    }

    ~Product() {}

    string getName() const { return name; }
    double getPrice() const { return price; }
    int getQuantity() const { return quantity; }
    int getProductId() const { return product_id; }

    void setName(string n) { name = n; }
    void setPrice(double p) { price = p; }
    void setQuantity(int q) { quantity = q; }
    void setProductId(int id) { product_id = id; }

    double total() const {
        return price * quantity;
    }

    bool equals(const Product& other) const {
        return name == other.name;
    }

    bool operator==(const Product& other) const {
        return name == other.name;
    }

    void display() const {
        cout << "Product: " << name 
             << " | Price: $" << price 
             << " | Qty: " << quantity 
             << " | Total: $" << total() << endl;
    }

    friend ostream& operator<<(ostream& os, const Product& p) {
        os << p.name << " ($" << p.price << " x " << p.quantity << ")";
        return os;
    }
};

#endif
