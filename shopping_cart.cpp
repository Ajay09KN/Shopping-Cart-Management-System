
#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <algorithm>
#include <string>
#include <limits>
using namespace std;

// ---------------- PRODUCT CLASS ----------------

class Product {
public:
    int id;
    string name;
    double price;
    int stock;

    Product(int i, string n, double p, int s) {
        id = i;
        name = n;
        price = p;
        stock = s;
    }
};

// ---------------- CART ITEM CLASS ----------------

class CartItem {
public:
    Product product;
    int quantity;

    CartItem(Product p, int q)
        : product(p), quantity(q) {}
};

// ---------------- SHOPPING CART CLASS ----------------

class ShoppingCart {
private:
    vector<CartItem> cart;

public:

    // Add product to cart
    void addToCart(Product p, int quantity) {

        if (quantity <= 0) {
            cout << "Invalid quantity!\n";
            return;
        }

        if (quantity > p.stock) {
            cout << "Not enough stock available!\n";
            return;
        }

        for (auto &item : cart) {
            if (item.product.id == p.id) {

                if (item.quantity + quantity > p.stock) {
                    cout << "Maximum stock exceeded!\n";
                    return;
                }

                item.quantity += quantity;
                cout << "Quantity updated successfully!\n";
                return;
            }
        }

        cart.push_back(CartItem(p, quantity));

        cout << "Product added to cart successfully!\n";
    }

    // Display cart
    void displayCart() {

        if (cart.empty()) {
            cout << "\nYour cart is empty!\n";
            return;
        }

        cout << "\n========== YOUR SHOPPING CART ==========\n";

        cout << left << setw(8) << "ID"
             << setw(20) << "Product"
             << setw(12) << "Price"
             << setw(10) << "Quantity"
             << "Total\n";

        cout << "--------------------------------------------------\n";

        for (auto &item : cart) {

            cout << left << setw(8) << item.product.id
                 << setw(20) << item.product.name
                 << setw(12) << item.product.price
                 << setw(10) << item.quantity
                 << item.product.price * item.quantity
                 << endl;
        }

        cout << "--------------------------------------------------\n";

        cout << "Total Amount: Rs. "
             << fixed << setprecision(2)
             << calculateTotal() << endl;
    }

    // Calculate total
    double calculateTotal() {

        double total = 0;

        for (auto &item : cart) {
            total += item.product.price * item.quantity;
        }

        return total;
    }

    // Remove product
    void removeProduct(int id) {

        for (auto it = cart.begin(); it != cart.end(); it++) {

            if (it->product.id == id) {

                cart.erase(it);

                cout << "Product removed successfully!\n";
                return;
            }
        }

        cout << "Product not found in cart!\n";
    }

    // Update quantity
    void updateQuantity(int id, int quantity) {

        if (quantity <= 0) {
            cout << "Invalid quantity!\n";
            return;
        }

        for (auto &item : cart) {

            if (item.product.id == id) {

                if (quantity > item.product.stock) {
                    cout << "Not enough stock!\n";
                    return;
                }

                item.quantity = quantity;

                cout << "Quantity updated successfully!\n";
                return;
            }
        }

        cout << "Product not found!\n";
    }

    // Search product in cart
    void searchProduct(string name) {

        bool found = false;

        for (auto &item : cart) {

            if (item.product.name.find(name) != string::npos) {

                cout << item.product.name
                     << " - Rs. " << item.product.price
                     << " - Quantity: " << item.quantity << endl;

                found = true;
            }
        }

        if (!found) {
            cout << "Product not found in cart!\n";
        }
    }

    // Save cart to file
    void saveCart() {

        ofstream file("cart.txt");

        if (!file) {
            cout << "Unable to save cart!\n";
            return;
        }

        for (auto &item : cart) {

            file << item.product.id << " "
                 << quoted(item.product.name) << " "
                 << item.product.price << " "
                 << item.product.stock << " "
                 << item.quantity << endl;
        }

        file.close();

        cout << "Cart saved successfully!\n";
    }

    // Load cart from file
    void loadCart(vector<Product> products) {

        ifstream file("cart.txt");

        if (!file) {
            return;
        }

        cart.clear();

        int id, stock, quantity;
        string name;
        double price;

        while (file >> id >> quoted(name)
                    >> price >> stock >> quantity) {

            for (auto &p : products) {

                if (p.id == id && quantity > 0
                    && quantity <= p.stock) {

                    cart.push_back(CartItem(p, quantity));
                    break;
                }
            }
        }

        file.close();

        cout << "Previous cart loaded successfully!\n";
    }

    // Checkout
    void checkout() {

        if (cart.empty()) {
            cout << "Your cart is empty!\n";
            return;
        }

        double total = calculateTotal();
        double discount = 0;

        if (total >= 5000) {
            discount = total * 0.10;
        }

        double finalAmount = total - discount;

        cout << "\n========== FINAL BILL ==========\n";

        displayCart();

        cout << "\nOriginal Amount: Rs. " << total;
        cout << "\nDiscount: Rs. " << discount;
        cout << "\nFinal Amount: Rs. " << finalAmount;

        cout << "\n\nThank you for shopping!\n";

        ofstream history("order_history.txt", ios::app);

        if (history) {
            history << "Order Total: Rs. "
                    << finalAmount << endl;
        }

        history.close();

        cart.clear();

        cout << "Checkout completed successfully!\n";
    }

    bool isEmpty() {
        return cart.empty();
    }
};

// ---------------- MAIN CLASS ----------------

int main() {

    vector<Product> products = {

        Product(1, "Laptop", 50000, 10),
        Product(2, "Mouse", 500, 50),
        Product(3, "Keyboard", 1000, 30),
        Product(4, "Headphones", 2000, 20),
        Product(5, "Monitor", 12000, 15),
        Product(6, "Smartphone", 25000, 12),
        Product(7, "USB Drive", 600, 40)
    };

    ShoppingCart cart;

    cart.loadCart(products);

    int choice;

    do {

        cout << "\n\n========== SHOPPING CART SYSTEM ==========\n";

        cout << "1. Display Products\n";
        cout << "2. Add Product to Cart\n";
        cout << "3. View Shopping Cart\n";
        cout << "4. Remove Product\n";
        cout << "5. Update Quantity\n";
        cout << "6. Search Product in Cart\n";
        cout << "7. Calculate Total Bill\n";
        cout << "8. Checkout\n";
        cout << "9. Save Cart\n";
        cout << "10. Exit\n";

        cout << "\nEnter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        int id, quantity;
        string name;

        switch (choice) {

        case 1:

            cout << "\n========== AVAILABLE PRODUCTS ==========\n";

            cout << left << setw(8) << "ID"
                 << setw(20) << "Name"
                 << setw(12) << "Price"
                 << "Stock\n";

            for (auto &p : products) {

                cout << left << setw(8) << p.id
                     << setw(20) << p.name
                     << setw(12) << p.price
                     << p.stock << endl;
            }

            break;

        case 2:

            cout << "Enter Product ID: ";
            cin >> id;

            cout << "Enter Quantity: ";
            cin >> quantity;

            {
                bool found = false;

                for (auto &p : products) {

                    if (p.id == id) {

                        cart.addToCart(p, quantity);
                        found = true;
                        break;
                    }
                }

                if (!found) {
                    cout << "Invalid Product ID!\n";
                }
            }

            break;

        case 3:

            cart.displayCart();
            break;

        case 4:

            cout << "Enter Product ID to remove: ";
            cin >> id;

            cart.removeProduct(id);
            break;

        case 5:

            cout << "Enter Product ID: ";
            cin >> id;

            cout << "Enter New Quantity: ";
            cin >> quantity;

            cart.updateQuantity(id, quantity);
            break;

        case 6:

            cout << "Enter Product Name to search: ";

            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, name);

            cart.searchProduct(name);
            break;

        case 7:

            cout << "Total Bill: Rs. "
                 << cart.calculateTotal() << endl;

            break;

        case 8:

            cart.checkout();
            break;

        case 9:

            cart.saveCart();
            break;

        case 10:

            cart.saveCart();

            cout << "Thank you! Exiting program...\n";
            break;

        default:

            cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 10);

    return 0;
}
