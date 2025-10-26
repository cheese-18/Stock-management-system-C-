#include <iostream>
#include <map>
#include <set>
#include <queue>
#include <stack>
#include <vector>
#include <algorithm>
#include <string>
using namespace std;

// ------------------------- PRODUCT STRUCT -------------------------
struct Product {
    int productID;
    string name;
    double price;
    int quantity;
    string supplier;
    int salesCount;
};

// ------------------------- GLOBAL DATA STRUCTURES -------------------------
map<int, Product> productMap;        // Map: productID → Product details
set<int> lowStockIDs;                // Set: productIDs with low stock
set<string> supplierIDs;             // Set: unique supplier names
queue<string> purchaseOrderQueue;    // Queue: pending purchase orders
queue<string> deliveryQueue;         // Queue: pending deliveries
stack<string> transactionStack;      // Stack: recent transactions (LIFO)
vector<Product> inventory;           // Vector: dynamic list of products

// ------------------------- FUNCTION DECLARATIONS -------------------------
void addProduct();
void viewProducts();
void recordSale();
void restockProduct();
void generateLowStockReport();
void sortProductsMenu();
void displayMenu();

// ------------------------- MAIN FUNCTION -------------------------
int main() {
    int choice;
    do {
        displayMenu();
        cout << "Enter choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1: addProduct(); break;
            case 2: viewProducts(); break;
            case 3: recordSale(); break;
            case 4: restockProduct(); break;
            case 5: generateLowStockReport(); break;
            case 6: sortProductsMenu(); break;
            case 7: cout << "Exiting system...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 7);
    return 0;
}

// ------------------------- MENU -------------------------
void displayMenu() {
    cout << "\n========== INVENTORY MANAGEMENT SYSTEM ==========\n";
    cout << "1. Add Product\n";
    cout << "2. View Products\n";
    cout << "3. Record Sale\n";
    cout << "4. Restock Product\n";
    cout << "5. Generate Low-Stock Report\n";
    cout << "6. Sort Products\n";
    cout << "7. Exit\n";
    cout << "===============================================\n";
}

// ------------------------- ADD PRODUCT -------------------------
void addProduct() {
    Product p;
    cout << "Enter Product ID: "; cin >> p.productID;
    cin.ignore();
    cout << "Enter Name: "; getline(cin, p.name);
    cout << "Enter Price: "; cin >> p.price;
    cout << "Enter Quantity: "; cin >> p.quantity;
    cin.ignore();
    cout << "Enter Supplier: "; getline(cin, p.supplier);
    p.salesCount = 0;

    productMap[p.productID] = p;
    supplierIDs.insert(p.supplier);
    inventory.push_back(p);

    if (p.quantity < 5) lowStockIDs.insert(p.productID);

    transactionStack.push("Added product: " + p.name);
    cout << "✅ Product added successfully!\n";
}

// ------------------------- VIEW PRODUCTS -------------------------
void viewProducts() {
    cout << "\n--- Current Inventory ---\n";
    for (const auto &item : productMap) {
        const Product &p = item.second;
        cout << "ID: " << p.productID << " | " << p.name
             << " | Price: " << p.price
             << " | Qty: " << p.quantity
             << " | Supplier: " << p.supplier << "\n";
    }
}

// ------------------------- RECORD SALE -------------------------
void recordSale() {
    int id, qty;
    cout << "Enter Product ID: ";
    cin >> id;
    cout << "Enter Quantity Sold: ";
    cin >> qty;

    if (productMap.find(id) == productMap.end()) {
        cout << "❌ Product not found!\n";
        return;
    }

    Product &p = productMap[id];
    if (p.quantity < qty) {
        cout << "⚠️ Not enough stock!\n";
        return;
    }

    p.quantity -= qty;
    p.salesCount += qty;

    transactionStack.push("Sold " + to_string(qty) + " of " + p.name);

    if (p.quantity < 5) lowStockIDs.insert(p.productID);

    cout << "✅ Sale recorded successfully!\n";
}

// ------------------------- RESTOCK PRODUCT -------------------------
void restockProduct() {
    int id, qty;
    cout << "Enter Product ID: ";
    cin >> id;
    cout << "Enter Quantity to Add: ";
    cin >> qty;

    if (productMap.find(id) == productMap.end()) {
        cout << "❌ Product not found!\n";
        return;
    }

    Product &p = productMap[id];
    p.quantity += qty;
    lowStockIDs.erase(id);

    purchaseOrderQueue.push(p.name);
    deliveryQueue.push(p.name);
    transactionStack.push("Restocked " + to_string(qty) + " of " + p.name);

    cout << "✅ Product restocked successfully!\n";
}

// ------------------------- LOW-STOCK REPORT -------------------------
void generateLowStockReport() {
    cout << "\n--- Low Stock Products (Qty < 5) ---\n";
    if (lowStockIDs.empty()) {
        cout << "All products are sufficiently stocked.\n";
        return;
    }

    for (int id : lowStockIDs) {
        Product &p = productMap[id];
        cout << "ID: " << p.productID << " | " << p.name
             << " | Qty: " << p.quantity << "\n";
    }
}

// ------------------------- SORT PRODUCTS -------------------------
bool sortByStock(const Product &a, const Product &b) { return a.quantity > b.quantity; }
bool sortByPrice(const Product &a, const Product &b) { return a.price < b.price; }
bool sortBySales(const Product &a, const Product &b) { return a.salesCount > b.salesCount; }

void sortProductsMenu() {
    int choice;
    cout << "\nSort by:\n1. Stock Quantity\n2. Price\n3. Sales Frequency\nEnter choice: ";
    cin >> choice;

    switch (choice) {
        case 1: sort(inventory.begin(), inventory.end(), sortByStock); break;
        case 2: sort(inventory.begin(), inventory.end(), sortByPrice); break;
        case 3: sort(inventory.begin(), inventory.end(), sortBySales); break;
        default: cout << "Invalid choice.\n"; return;
    }

    cout << "\n--- Sorted Products ---\n";
    for (auto &p : inventory) {
        cout << "ID: " << p.productID << " | " << p.name
             << " | Qty: " << p.quantity
             << " | Price: " << p.price
             << " | Sales: " << p.salesCount << "\n";
    }
}
