
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <occi.h>


using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;


// Class definition
struct ShoppingCart 
{
    int product_id;
    double price;
    int quantity;
};

// Global Variable
const int MAX_NUM_ITEMS = 5;

// Menu interface
int mainMenu(void) {

    int option = 0;
    bool flag = true;
    bool wrongAnswer = false;

    // Display Main Menu and get the answer
    while (flag) {
        cout  << "********************* Main Menu *********************" << endl;
        cout << "1) Login" << endl;
        cout << "0) Exit" << endl;
        if (wrongAnswer)
            cout << "You entered a wrong value. ";;
        cout << "Enter an option (0-1): ";
        cin >> option;

        // If the user enters the wrong option set flag to false to ask again
        if (option == 0 || option == 1)
            flag = false;
        else 
            wrongAnswer = true;
    }

    return option;
}


int customerLogin(Connection* conn, int customerId) {

    Statement* stmt = nullptr;
    stmt = conn->createStatement();
    int found;

    try {
        // Define statement and arguments
        stmt->setSQL("BEGIN find_customer(:1, :2); END;");

        //Set parameters with their types
        stmt->setInt(1, customerId);
        stmt->registerOutParam(2, Type::OCCIINT, sizeof(found));

        //Execute Update
        stmt->executeUpdate();

        //Get the variable that will be returned and finish statement
        found = stmt->getInt(2);
        conn->terminateStatement(stmt);

        return found;


    } catch (SQLException& sqlExcp) 
        {
            cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
       }

    
}

double findProduct(Connection* conn, int product_id) {

    Statement* stmt = nullptr;
    stmt = conn->createStatement();
    double price;

    try {

        // Define statement and arguments
        stmt->setSQL("BEGIN find_product(:1, :2); END;");

        //Set parameters with their types
        stmt->setInt(1, product_id);
        stmt->registerOutParam(2, Type::OCCIDOUBLE, sizeof(price));

        //Execute Update
        stmt->executeUpdate();

        //Get the variable that will be returned and finish statement
        price = stmt->getDouble(2);
        conn->terminateStatement(stmt);

    }
    catch (SQLException& sqlExcp)
    {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }

    return price;

}


void displayProducts(struct ShoppingCart cart[], int productCount) {

    double total = 0.0;

    cout << "------- Ordered Products ---------" << endl;

    //Loop through every product in the cart to display
    for (int i = 0; i < productCount; i++) 
    {
        cout << "---Item " << i + 1 << endl;
        cout << "Product ID: " << cart[i].product_id << endl;
        cout << "Price: " << cart[i].price << endl;
        cout << "Quantity: " << cart[i].quantity << endl;  

        // Add to total
        total += (cart[i].price * cart[i].quantity);

    }

    cout << "----------------------------------" << endl;
    cout << "Total: "<< total << endl;

}

int addToCart(Connection* conn, struct ShoppingCart cart[]) {

    int prodID = 0;
    int prodQu = 0;
    double prodPrice = 0.0;
    int addMore = 1;
    int itemsAdded = 0;

    cout << "-------------- Add Products to Cart --------------" << endl;
   do {

        cout << "Enter the product ID: ";
        cin >> prodID;

        //Get the price of the product with that ID or 0 if is not in the DB
        prodPrice = findProduct(conn, prodID);

        if (prodPrice)
        {
            //Ask and assign variables to the local array of products
            cart[itemsAdded].product_id = prodID;
            cout << "Product Price: " << prodPrice << endl;
           
            cout << "Enter the product Quantity: ";
            cin >> prodQu;
            cart[itemsAdded].quantity = prodQu;

            cart[itemsAdded].price = prodPrice;

            cout << "Enter 1 to add more products or 0 to checkout: ";
            cin >> addMore;

            itemsAdded++;
        }
        else
        {
            cout << "The product does not exists. Try again..." <<endl;
        }
            
        // Keep adding items until the user hits 0 or the cart reaches the limit of 5 items
        } while (addMore && (itemsAdded < MAX_NUM_ITEMS));
    

    return itemsAdded;

}
 



int main(void)

{

    /* OCCI Variables */

    Environment* env = nullptr;
    Connection* conn = nullptr;


    // Used Variables

    string user = "dbs311_213za02"; //Enter your sql developer username/password in quotes 
    string pass = "30453575";
    string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";

    int option = 0;
    int custID = 0;
    int prodID = 0;
    int itemsAdded = 0;

    // Create connection
    env = Environment::createEnvironment(Environment::DEFAULT);
    conn = env->createConnection(user, pass, constr);

    printf("Connection created\n");
    Statement* stmt = nullptr;
    stmt = conn->createStatement();


    ShoppingCart cart[MAX_NUM_ITEMS] = {};
    
    do {
        // Keep taking the user to the main Menu 
        option = mainMenu();

        if (option) {
            cout << "Enter the customer ID: ";
            cin >> custID;

            // If the customer ID provided does exist in the DB, take it to the addToCart function
            if (customerLogin(conn, custID)) 
            {
               itemsAdded = addToCart(conn, cart);

               // Once the user is done adding items, display all the products in the cart
               displayProducts(cart, itemsAdded);

            }
            else {

                cout << "The customer does not exist." << endl;
                option = 1;

            }

        }
    
    // Don't exit until the user hits 0
    } while (option);

    cout << "Good bye!..." << endl;



    //Test connection
    /*  try {

        string statement = "SELECT First_Name FROM employees";
        ResultSet* rs = stmt->executeQuery(statement);

        while (rs->next())
        {

            string firstName = rs->getString(1);
            cout << firstName << endl;
        }
    }
    catch (SQLException& sqlExcp) {
        cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
    }*/

    
    // Terminate Connection
    env->terminateConnection(conn);
    Environment::terminateEnvironment(env);

    return 0;

}


