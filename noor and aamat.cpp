#include <iostream>
#include <fstream> //file handling
#include <sstream>
#include <string>
#include <ctime> // time tracking
#include <iomanip> //setprecisions, setw
#include <thread> 
#include <chrono> //display styling
using namespace std;
//abstract class (has pure virtual function readMenuItems()
class MenuItems
{
//encapsulation by private and  protected members
private:
	string Item_Name;
    int Item_id;
    int Item_price;
    string Item_type;
    int stock;

protected:
    string menuItem_file;
    
public:
    MenuItems()
    {}
    MenuItems(string file = "Menu.csv") : menuItem_file(file), Item_id(0), Item_price(0), stock(0)
    {}
    //access private  data
    
    void setItemId(int id) { Item_id = id; }
    void setItemName(const string &name) { Item_Name = name; }
    void setItemPrice(int price) { Item_price = price; }
    void setItemType(const string &type) { Item_type = type; }
    void setStock(int stk) { stock = stk; }

    int getItemId() const { return Item_id; }
    string getItemName() const { return Item_Name; }
    int getItemPrice() const { return Item_price; }
    string getItemType() const { return Item_type; }
    int getStock() const { return stock; }

    
   
    void createMenuitem()
    {
        ofstream file(menuItem_file, ios::app | ios::in);
        if (!file.is_open())
        {
            cout << "menu.csv file not opening for writing\n ";
            return;
        }

        ifstream inFile(menuItem_file);
        if (!inFile.is_open())
        {
            cout << "menu.csv file not opening for reading\n ";
            return;
        }

        string headerLine;
        getline(inFile, headerLine);
        cout << "Enter the Item ID: \n";
        cin >> Item_id;
	while(Item_id<0)
	{	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>Item_id;	
		}
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, ',');

            if (stoi(idStr) == Item_id)
            {
                cout << "Item ID " << Item_id << " already exists! Please enter a different ID.\n";
                return;
            }
        }

        inFile.close();

        cin.ignore();
        cout << "Enter the Item name: \n";
        getline(cin, Item_Name);
        cout << "Enter the Item Type (non-veg/veg/gluten-free, others): \n";
        getline(cin, Item_type);
        cout << "Enter the price of the Item: \n";
        cin >> Item_price;
       	while(Item_price<0)
	   {	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>Item_price;	
		}
       cout << "Enter the stock available for the item: \n";
        cin >> stock;
        	while(stock<0)
	   {	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>stock;	
		}

        ifstream checkFile("Menu.csv");
        bool hasHeader = false;
        if (checkFile.is_open())
        {
            string firstLine;
            getline(checkFile, firstLine);
            if (firstLine.find("ID,Name,Type,Price,Stock") != string::npos)
            {
                hasHeader = true;
            }
            checkFile.close();
        }

        ofstream outFile(menuItem_file, ios::app);
        if (!outFile.is_open())
        {
            cout << "Error opening file for writing.\n";
            return;
        }
        if (!hasHeader)
        {
            outFile << "ID,Name,Type,Price,Stock\n";
        }

        outFile << Item_id << "," << Item_Name << "," << Item_type << "," << Item_price << "," << stock << "\n";
        outFile.close();

        cout << "Item added successfully!\n";
      
    }
       
};
//inheritance 
class Menu : public MenuItems
{
protected:
    MenuItems *items[100];
    int countItems = 0;

public:
    Menu(string file) : MenuItems(file) {}

    void addmenuItem(MenuItems *menu)
    {
        ofstream Outfile("Menu.csv", ios::app);
        if (!Outfile.is_open())
        {
            cout << "Menu file not open for writing \n";
            return;
        }

        if (countItems <= 100)
        {
            items[countItems++] = menu;
        }
        else
        {
            cout << "limit reached !!!!! If you want to add a new item, please delete a pre-existing item.\n";
        }
        Outfile.close();
    }

    void readMenuItems()
    {
        ifstream file(menuItem_file);
        if (!file.is_open())
        {
            cout << "Error opening file.\n";
            return;
        }
        string headerLine;
        getline(file, headerLine);
        string item;
        cout << "------------------- Welcome to Our Menu -------------------------:\n";
        while (getline(file, item))
        {   string id, name, price, type, stock;
            stringstream ss(item);
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');

            cout << "Id: " << id << ", Name: " << name << ", Item Type: " << type
                 << ", Price: " << price << ", Stocks: " << stock << endl;

            MenuItems *menuItem = new MenuItems("menu.csv");
            menuItem->setItemId(stoi(id));
            menuItem->setItemName(name);
            menuItem->setItemPrice(stoi(price));
            menuItem->setItemType(type);
            menuItem->setStock(stoi(stock));

            addmenuItem(menuItem);
        }
        file.close();
    }

	    MenuItems *getMenuItemById(int id)
	    {
	        for (int i = 0; i < countItems; ++i)
	        {
	            if (items[i]->getItemId() == id)
	            {
	                return items[i];
	            }
	        }
	        return nullptr;
	    }
	    void deleteMenuItem(int id)
	    {
	        ifstream file("menu.csv");
	        if (!file.is_open())
	        {
	            cout << "Error opening file for reading.\n";
	            return;
	        }
	
	        string headerLine;
	        getline(file, headerLine);
	
	        bool found = false;
	        ofstream tempFile("deleteitemfile.csv");
	
	        tempFile << headerLine << "\n";
	
	        string line;
	        while (getline(file, line))
	        {
	            stringstream ss(line);
	            string itemIdStr, itemName, itemType, itemPrice, itemStock;
	
	            getline(ss, itemIdStr, ',');
	            getline(ss, itemName, ',');
	            getline(ss, itemType, ',');
	            getline(ss, itemPrice, ',');
	            getline(ss, itemStock, ',');
	
	            if (stoi(itemIdStr) == id)
	            {
	                found = true;
	                cout << "Removing item: " << itemName << "\n";
	                continue;
	            }
	
	            tempFile << line << "\n";
	        }
	
	        file.close();
	        tempFile.close();
	
	        if (found)
	        {
	            remove("menu.csv");
	            rename("deleteitemfile.csv", "menu.csv");
	            cout << "Item with ID " << id << " has been deleted.\n";
	        }
	        else
	        {
	            remove("deleteitemfile.csv");
	            cout << "Item with ID " << id << " not found.\n";
	        }
	    }
	};

class Inventory
{
private:
    string menuFile;
    int itemStocks[100];
    float itemPrices[100];
    int itemCount;

public:
    Inventory(string file = "Menu.csv") : menuFile(file), itemCount(0) {}
    void loadInventory()
    {
        ifstream file(menuFile); // Open the inventory file (e.g., menu.csv)
        if (!file.is_open())
        {
            cout << "Error opening menu file.\n";
            return;
        }

        string line, headerline;

        getline(file, headerline);

        while (getline(file, line))
        {
            stringstream ss(line);
            string idStr, name, priceStr, stockStr, itemType;

            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, itemType, ',');
            getline(ss, priceStr, ',');
            getline(ss, stockStr, ',');

            int id = stoi(idStr);
            float price = stoi(priceStr);
            int stock = stoi(stockStr);

            if (id >= 0 && id < 100)
            {
                itemStocks[id] = stock;
                itemPrices[id] = price;
                itemCount++;
            }
        }
file.close(); 
    }
    void updateInventory(int id, const string &field, int &newValue)
    {
        ifstream inFile(menuFile);
        if (!inFile.is_open())
        {
            cout << "Error: Could not open inventory file.\n";
            return;
        }

        ofstream tempFile("TempMenu.csv");
        if (!tempFile.is_open())
        {
            cout << "Error: Could not create temporary file.\n";
            return;
        }

        string line, header;
        getline(inFile, header);
        tempFile << header << "\n";

        bool found = false;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string idStr, name, type, price, stock;
            getline(ss, idStr, ',');
            getline(ss, name, ',');
            getline(ss, type, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');

            if (stoi(idStr) == id)
            {
                found = true;
                if (field == "stock")
                {
                    tempFile << idStr << "," << name << "," << type << "," << price << "," << newValue << "\n";
                    cout << "Updated stock for item: " << name << " to " << newValue << "\n";
                }
                else if (field == "price")
                {
                    tempFile << idStr << "," << name << "," << type << "," << newValue << "," << stock << "\n";
                    cout << "Updated price for item: " << name << " to " << newValue << "\n";
                }
            }
            else
            {
                tempFile << line << "\n";
            }
        }

        inFile.close();
        tempFile.close();

        if (found)
        {
            remove(menuFile.c_str());
            rename("TempMenu.csv", menuFile.c_str());
        }
        else
        {
            remove("TempMenu.csv");
            cout << "Item not found in inventory.\n";
        }

        loadInventory();
    }
    void updateStock(int id, int newStock)
    {
        updateInventory(id, "stock", newStock);
    }

    void updatePrice(int id, int newPrice)
    {
        updateInventory(id, "price", newPrice);
    }

    int getItemStock(int itemID)
    {
        if (itemID >= 0 && itemID < 100)
        {
            return itemStocks[itemID];
        }
        return 0;
    }

    int getItemPrice(int itemID)
    {
        if (itemID >= 0 && itemID < 100)
        {
            return itemPrices[itemID];
        }
        return 0;
    }
    void upgradeinventory(int itemID, int quantity)
    {
        ifstream File("menu.csv");
        if (!File.is_open())
        {
            cout << "error opening menu file for readin \n";
        }
        string line, headerline;

	        // Read and write the header
	        getline(File, headerline);
        ofstream tempFile("temp_inventory.csv");

        tempFile << headerline << "\n";

        bool itemFound = false;

        while (getline(File, line))
        {
            stringstream ss(line);
            string id, name, stock, diet, price;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, diet, ',');
            getline(ss, price, ',');
            getline(ss, stock, ',');

            if (stoi(id) == itemID)
            {
                int currentStock = stoi(stock);
                int updatedStock = currentStock - quantity;
                if (updatedStock < 0)
                {
                    cout << "Error: Insufficient stock for item \"" << name << "\" (ID: " << itemID << ").\n";
                    cout << "Current stock: " << currentStock << ". Enter  stock to restock: ";

                    int restockAmount;
                    cin >> restockAmount;

                    if (restockAmount < 0)
                    {
                        cout << "Invalid restock amount.\n";
                        File.close();
                        tempFile.close();
                        remove("temp_inventory.csv"); // Remove temporary file
                        return;
                    }

                    currentStock += restockAmount;
                    updatedStock = currentStock - quantity;

                    if (updatedStock < 0)
                    {
                        cout << "Error: Still insufficient stock after restocking..\n";
                        File.close();
                        tempFile.close();
                        remove("temp_inventory.csv"); // Remove temporary file
                        return;
                    }
                }
                stock = to_string(updatedStock);
                itemFound = true;
            }

            tempFile << id << "," << name << "," << diet << "," << price << "," << stock << "\n";
        }

        File.close();
        tempFile.close();
        if (itemFound)
        {
            remove("menu.csv");
            rename("temp_inventory.csv", "menu.csv");
            cout << "stocks of ID " << itemID << " has been update.\n";
        }
        else
        {
            remove("temp_inventory.csv");
            cout << "Item of ID " << itemID << " not found.\n";
        }
    }
};
//inheritance
class Customer
{

protected:
    string filename;
    int id;
    string name;
    string contact;
    string email;
    string dietaryPreference;

public:
    Customer(string file = "customer.csv") : filename(file) {}

    void setId(int customerId) { id = customerId; }
    void setName(const string &customerName) { name = customerName; }
    void setContact(const string &customerContact) { contact = customerContact; }
    void setEmail(const string &customerEmail) { email = customerEmail; }
    void setDietaryPreference(const string &preference) { dietaryPreference = preference; }

    int getId() const { return id; }
    string getName() const { return name; }
    string getContact() const { return contact; }
    string getEmail() const { return email; }
    string getDietaryPreference() const { return dietaryPreference; } 
    // overriding
    virtual void takeInput() = 0;
    virtual void displayDetails() = 0;
};
//inheritance
class LoyalCustomer : public Customer
{//encapsulation
private:
    int loyaltyPoints;
    int totalSpent;
    Menu *menu;
public:
    LoyalCustomer(string file = "customer.csv", int points = 0, int spent = 0)
        : Customer(file), loyaltyPoints(points), totalSpent(spent) {}

    int getLoyaltyPoints() const { return loyaltyPoints; }
    int getTotalSpent() const { return totalSpent; }

    void addLoyaltyPoints(int amountSpent = 0)
    {
        int pointsEarned = (amountSpent / 100);
        loyaltyPoints += pointsEarned;
        cout << "You have earned " << pointsEarned << " loyalty points this purchase.\n";
    }

    void takeInput() override
    {
        ofstream file("customer.csv", ios::app | ios::in);
        if (!file.is_open())
        {
            cout << "Error opening file for writing.\n";
            return;
        }

        ifstream inFile(filename);
        if (!inFile.is_open())
        {
            cout << "Error opening customer file \n";
            return;
        }
        string headerLine;
        getline(inFile, headerLine);

        cout << "Enter Customer ID: ";
        cin >> id;
        while(id<0){
        	cout<<"Alert!!!!! Please eneter positive value \n";
        	cin>>id;
		}    
        string line;
        while (getline(inFile, line))
        {
            stringstream ss(line);
            string idStr;
            getline(ss, idStr, ',');

            if (stoi(idStr) == id)
            {
                cout << "CustomerID " << id << " already exists! Please enter a different ID.\n";
                inFile.close();
                return;
            }
        }
        inFile.close();

        cin.ignore();
        cout << "Enter Customer Name: ";
        getline(cin, name);
 
 
          bool isValid = true;
        while (true)
        {
            cout << "Enter Contact Information (must be 11 digit number): ";
            getline(cin, contact);

            if (contact.length() == 11 )
        {
		    bool isValid = true;
           for (char c : contact) {
            if (!isdigit(c)) { 
                isValid = false;
                break;
            }}
        if (isValid)
		 { 
            break;
          }  } 
    cout << "Invalid input. Please enter exactly an 11-digit number with no alphabets or special characters.\n";
}
  while (true)
        {
            cout << "Enter your email address: ";
            getline(cin, email);
            if ((email.find("@gmail.com") != string::npos || email.find("@hotmail.com") != string::npos || email.find("@icloud.com") != string::npos))
            {
                break;
            }
            else
            {
                cout << "Invalid email address. Please enter in format (xyz@gmail.com)  .\n";
            }
        }

        cout << "Enter Dietary Preference: ";
        getline(cin, dietaryPreference);
        ifstream checkFile("customer.csv");
        bool hasHeader = false;
        if (checkFile.is_open())
        {
            string firstLine;
            getline(checkFile, firstLine);
            if (firstLine.find("ID,Name,Phone no.,Gmail,Diet,Points,Total") != string::npos)
            {
                hasHeader = true;
            }
            checkFile.close();
        }

        ofstream outFile(filename, ios::app);
        if (!outFile.is_open())
        {
            cout << "Error opening file for writing.\n";
            return;
        }
        if (!hasHeader)
        {
            outFile << "ID,Name,Phone no.,Gmail,Diet,Points,Total\n";
        }

        outFile << id << "," << name << "," << contact << "," << email << "," << dietaryPreference << "," << loyaltyPoints << "," << totalSpent << "\n";
        outFile.close();

        cout << "Customer added successfully!\n";
    }

    bool getCustomerById(int customerId)
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file for reading.\n";
            return false;
        }
        string headerline;
        getline(file, headerline);
        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string idStr, nameStr, contactStr, emailStr, dietaryPreferenceStr, loyaltyPointsStr, totalSpentStr;

            getline(ss, idStr, ',');
            getline(ss, nameStr, ',');
            getline(ss, contactStr, ',');
            getline(ss, emailStr, ',');
            getline(ss, dietaryPreferenceStr, ',');
            getline(ss, loyaltyPointsStr, ',');
            getline(ss, totalSpentStr, ',');

            if (stoi(idStr) == customerId)
            {
                id = stoi(idStr);
                name = nameStr;
                contact = contactStr;
                email = emailStr;
                dietaryPreference = dietaryPreferenceStr;
                loyaltyPoints = stoi(loyaltyPointsStr);
                totalSpent = stoi(totalSpentStr);
                file.close();
                return true;
            }
        }

        file.close();
        cout << "Customer with ID " << customerId << " not found.\n";
        return false;
    }


    void updateCustomerField(const string &filename, int customerId, const string &fieldName, const string &newValue)
    {
        ifstream inFile(filename);
        ofstream tempFile("temp.csv");

         bool found = false;

        if (!inFile.is_open() || !tempFile.is_open())
        {
            cout << "Error opening file for reading or writing.\n";
            return;
        }

        string line, headerLine;
        getline(inFile, headerLine);
        tempFile << headerLine << "\n";

        while (getline(inFile, line))
        {
            stringstream ss(line);
            string idStr, nameStr, contactStr, emailStr, dietaryPreferenceStr, loyaltyPointsStr, totalSpentStr;
            getline(ss, idStr, ',');
            getline(ss, nameStr, ',');
            getline(ss, contactStr, ',');
            getline(ss, emailStr, ',');
            getline(ss, dietaryPreferenceStr, ',');
            getline(ss, loyaltyPointsStr, ',');
            getline(ss, totalSpentStr, ',');

            if (stoi(idStr) == customerId)
            {
                found = true;

                // Update the correct field based on fieldName
                if (fieldName == "contact")
                {
                    contactStr = newValue;
                }
                else if (fieldName == "email")
                {
                    emailStr = newValue;
                }
                else if (fieldName == "loyaltyPoints")
                {
                    loyaltyPointsStr = newValue;
                }
                else if (fieldName == "dietaryPreference")
                {
                    dietaryPreferenceStr = newValue;
                }

                cout << "Updated " << fieldName << " for customer: " << nameStr << " to " << newValue << "\n";
            }

            // Write the updated data to the temporary file
            tempFile << idStr << "," << nameStr << "," << contactStr << "," << emailStr << "," << dietaryPreferenceStr
                     << "," << loyaltyPointsStr << "," << totalSpentStr << "\n";
        }

        inFile.close();
        tempFile.close();

        if (found)
        {
            remove(filename.c_str());
            rename("temp.csv", filename.c_str());
        }
        else
        {
            remove("temp.csv");
            cout << "Customer not found in file.\n";
        }
    }

    void deleteCustomerData(int Customerid)
    {
        ifstream file("customer.csv");
        if (!file.is_open())
        {
            cout << "Error opening customer file for reading.\n";
            return;
        }

        string headerLine;
        getline(file, headerLine);

        bool found = false;
        ofstream tempFile("deleteCustomerfile.csv");

        tempFile << headerLine << "\n";

        string line;
        while (getline(file, line))
        {
            stringstream ss(line);
            string idStr, nameStr, contactStr, emailStr, dietaryPreferenceStr, loyaltyPointsStr, totalSpentStr;
            getline(ss, idStr, ',');
            getline(ss, nameStr, ',');
            getline(ss, contactStr, ',');
            getline(ss, emailStr, ',');
            getline(ss, dietaryPreferenceStr, ',');
            getline(ss, loyaltyPointsStr, ',');
            getline(ss, totalSpentStr, ',');

            if (stoi(idStr) == Customerid)
            {
                found = true;
                cout << "Removing Customer History: " << nameStr << "\n";
                continue;
            }

            tempFile << line << "\n";
        }

        file.close();
        tempFile.close();

        if (found)
        {
            remove("customer.csv");
            rename("deleteCustomerfile.csv", "customer.csv");
            cout << "Customer with ID " << Customerid << " has been deleted.\n";
        }
        else
        {
            remove("deleteCustomerfile.csv");
            cout << "Customer with ID " << Customerid << " not found.\n";
        }
    }
    // virtual fucntion to display info
    void displayDetails() override
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file.\n";
            return;
        }
        string headerLine;
        getline(file, headerLine);
        string line;
        cout << "\nCustomer Details:\n";
        while (getline(file, line))
        {
            stringstream ss(line);
            string id, name, contact, email, dietaryPreference, loyaltyPointsStr, totalSpentStr;
            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, contact, ',');
            getline(ss, email, ',');
            getline(ss, dietaryPreference, ',');
            getline(ss, loyaltyPointsStr, ',');
            getline(ss, totalSpentStr, ',');

            cout << "ID:" << id << " ,  Name: " << name << " , Phone:" << contact
                 << " ,  Email:" << email << "  ,  Dietary Preference: " << dietaryPreference
                 << " ,  Loyalty Points:" << loyaltyPointsStr << "  , AmountSpent: Rs" <<fixed<<setprecision(2)<<totalSpentStr << endl;
        }
        file.close();
    }//customer management system display
    void customerMenu()
    {
        int choice;
        do
        {
            cout << "\n--- Customer Management System ---\n";
            cout << "1. Add Customer\n";
            cout << "2. Display Customer Details\n";
            cout << "3. Delete Customer History\n";
            cout << "4. Update Customer Data \n"; 
		    cout << "5. Exit\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice)
            {
            case 1:
                takeInput();
                break;
            case 2:
                displayDetails();
                break;
            case 3:
            {
                int CustomerIdToDelete;
                displayDetails();
                cout << "Enter the ID of the Customer  you want to delete: ";
                cin >> CustomerIdToDelete;
                deleteCustomerData(CustomerIdToDelete);
                break;
            }
            case 4:
            {
                while (true)
                {
                    int choose;

                    cout << "-----Update Contact information-----:\n";
                    cout << "1. Update phone number  \n";
                    cout << "2. Update email \n";

                    cout << "3. Update Diet preference \n";
                    cout << "4. exit \n";

                    cout << "Enter the choice: \n";
                    cin >> choose;

                    if (choose == 1)
                    {
                        int id;
                        string phoneno;
                        displayDetails();
                        cout << "enetr the Id of the customer please:\n";
                        cin >> id;
                        cout << "Update the new phone number:\n";
                        cin >> phoneno;

                        updateCustomerField("customer.csv", id, "contact", phoneno);

                        continue;
                    }
                    else if (choose == 2)
                    {
                        int id;
                        string emailaddress;
                        displayDetails();

                        cout << "enetr the Id of the customer please:\n";
                        cin >> id;
                        cout << "Update the new email address:\n";
                        cin >> emailaddress;
                        updateCustomerField("customer.csv", id, "email", emailaddress);
                        continue;
                    }
                    else if (choose == 3)
                    {
                        int id;
                        string diet;
                        displayDetails();

                        cout << "enetr the Id of the customer please:\n";
                        cin >> id;
                        cin.ignore();
                        cout << "Update the prefered Diet :\n";
                        getline(cin, diet);
                        updateCustomerField("customer.csv", id, "dietaryPreference", diet);
                        continue;
                    }
                    else if (choose == 4)
                    {
                        cout << "Exiting update contact info.\n";
                        break;
                    }
                    else
                    {
                        cout << "Invalid option. Please try again.\n";
                    }
                }

                break;
            }
            
			 case 5:
                cout << "Exiting Customer Menu...\n";
                break;
            default:
                cout << "Invalid option! Please try again.\n";
                break;
            }
        } while (choice != 5);
    }
};
class Order
{// encapsulation
private:
    string filename; // encapsulation
    LoyalCustomer *customerData;//composition
    Menu *menuData;
    Inventory *inventory;
    string orderStatus;
    int ordercount = 0;
    int orderid;

public:
    Order(string file, Menu *menuinfo, LoyalCustomer *customerinfo, Inventory *Inv) : menuData(menuinfo), customerData(customerinfo), inventory(Inv)
    {

        filename = file;

        orderStatus = "pending";//initally
    }

    void displayMenuItems()
    {
        menuData->readMenuItems();
    }
    void placeOrder()
    {
        inventory->loadInventory();
        ofstream file(filename, ios::app);
        ifstream customerfile("customer.csv");
        if (!file.is_open() || !customerfile.is_open())
        {
            cout << "Error opening file for writing order or reading customer file.\n";
            return;
        }

        string headerline;
        getline(customerfile, headerline);

        int orderID, customerID, itemID;
        ifstream inFile(filename);
        if (!inFile.is_open())
        {
            cout << "Error: Unable to open file " << filename << endl;
            return;
        }

        string line;
        bool hasHeader = false;
        bool duplicateFound = false;
        cout << "Order Id:\n";
        cin >> orderID;
        	while(orderID<0)
	{	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>orderID;
       	}
        while (getline(inFile, line))
        {
            stringstream ss(line);

            if (!hasHeader)
            {
                if (line.find("OrdrID,CustomerID,ItemName,Quantity,Total,Customization,Status,OrderDate") != string::npos)
                {
                    hasHeader = true;
                    continue;
                }
            }

            string fileOrderID, customerID, itemName, quantity, total, customization, orderStatus, orderDate;

            getline(ss, fileOrderID, ',');
            getline(ss, customerID, ',');
            getline(ss, itemName, ',');
            getline(ss, quantity, ',');
            getline(ss, total, ',');
            getline(ss, customization, ',');
            getline(ss, orderStatus, ',');
            getline(ss, orderDate, ',');

            // Compare OrderID
            if (stoi(fileOrderID) == orderID) {
            
				
            cout << "Order ID " << orderID << " already exists! Please enter a different ID.\n";
            duplicateFound=true;
		
            inFile.close();
            return; 
        }
        }

        if (!duplicateFound)
        {
            cout << "Order ID " << orderID << " is unique. Proceeding..." << endl;
        }

        inFile.close();

        cout << "Enter Customer ID:\n ";
        cin >> customerID;
        while(customerID<0)
	{	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>customerID;
       	}
        if (!customerData->getCustomerById(customerID))
        {
            cout << "Order cannot proceed without a valid customer.\nRegister ID to place an order.\n";
            customerData->takeInput();
        }
     displayMenuItems();
        
        // ordering

        cout << "Enter Item ID: ";
        cin >> itemID;
          while(itemID<0)
	{	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>itemID;
       	}
        MenuItems *ChosenItem = menuData->getMenuItemById(itemID);
        if (!ChosenItem)
        {
            cout << "Item not found. Please try again.\n";
            return;
        }

        int quantity;
        cout << "Enter Quantity: ";
        cin >> quantity;
        while(quantity<0)
	{	cout<<"ALert!!!!!Please enter positive value \n";
        	cin>>quantity;
       	}
        int stock = inventory->getItemStock(itemID);
        if (stock < quantity)
        {
            cout << "Insufficient stock available. Only " << stock << " units in stock.\n";
            return;
        }

        inventory->upgradeinventory(itemID, quantity);
        char choice;
        cin.ignore();
        // customization
        cout << "Any customization? \n It will cost an additional (100 Rs).\n Enter (Y/N):" << endl;
        cin >> choice;
        string customization;
        float customizationCost = 0.0;

        if (choice == 'Y' || choice == 'y')
        {
            cout << "Instructions:\n";
            cin.ignore();
            getline(cin, customization);
            customizationCost = 100.0;
        }

        float total = 0.0;
        float itemPrice = inventory->getItemPrice(itemID);
        total = (itemPrice * quantity) + customizationCost;
        // discount
        float discount = 0.0;

        int loyaltyPoints = customerData->getLoyaltyPoints();
        if (loyaltyPoints >= 100)
        {
            float loyaltyDiscount = (loyaltyPoints / 10) * 0.05 * total;
            discount += loyaltyDiscount;
            cout << loyaltyPoints / 10 << " x 5% loyalty discount applied.\n";
        }

        total -= discount;

        cout << "Total after discount: Rs " << total << "\n";

        customerData->addLoyaltyPoints(total);
        cout << "Updated Loyalty Points: " << customerData->getLoyaltyPoints() << "\n";

        time_t now = time(0);
        tm *ltm = localtime(&now);
        string orderDate = to_string(1900 + ltm->tm_year) + "-" + (ltm->tm_mon + 1 < 10 ? "0" : "") + to_string(1 + ltm->tm_mon) + "-" + (ltm->tm_mday < 10 ? "0" : "") + to_string(ltm->tm_mday);

        string itemName = ChosenItem->getItemName();
        file << orderID << "," << customerID << "," << itemName << "," << quantity << "," << total << ","
             << customization << "," << orderStatus << "," << orderDate << "\n";
        file.close();

        customerfile.close();

        updateOrderStatus(customerID);
        cout << "Order placed successfully!\n";

        cout << "Genertaing Bill \n";
        generateBill(orderID, customerID, orderDate, itemName,
                     quantity, itemPrice, customizationCost, discount, total, *customerData);

        ordercount++;
        cout << "Order number placed today: " << ordercount << endl;
        updateCustomerFile(customerID, total);
    }

    void updateCustomerFile(int customerID, int orderTotal)
    {
        string filename = "customer.csv";
        ifstream customerfile(filename);
        ofstream tempFile("temp.csv");

        if (!customerfile.is_open() || !tempFile.is_open())
        {
            cout << "Error: Unable to open customer.csv or temp.csv.\n";
            return;
        }

        string line, headerline;

        getline(customerfile, headerline);
        tempFile << headerline << "\n";

        bool customerFound = false;

        while (getline(customerfile, line))
        {
            stringstream ss(line);
            string id, name, contact, email, diet, points, totalSpent;

            getline(ss, id, ',');
            getline(ss, name, ',');
            getline(ss, contact, ',');
            getline(ss, email, ',');
            getline(ss, diet, ',');
            getline(ss, points, ',');
            getline(ss, totalSpent, ',');

            if (stoi(id) == customerID)
            {
                int updatedTotal = stoi(totalSpent) + orderTotal;
                totalSpent = to_string(updatedTotal);

                int updatedPoints = stoi(points) + orderTotal / 100;
                points = to_string(updatedPoints);

                customerFound = true;
            }

            tempFile << id << "," << name << "," << contact << "," << email << ","
                     << diet << "," << points << "," << totalSpent << "\n";
        }

        customerfile.close();
        tempFile.close();

        if (customerFound)
        {
            remove(filename.c_str());
            rename("temp.csv", filename.c_str());
            cout << "Customer file updated successfully \n";
        }
        else
        {

            remove("temp.csv");
            cout << "Customer Id is not found \n";
        }
    }

    void updateOrderStatus(int customerID)
    {
        ifstream orderFile("order.csv");
        if (!orderFile.is_open())
        {
            cout << "Error opening order file.\n";
            return;
        }

        ofstream tempFile("TempOrder.csv");
        if (!tempFile.is_open())
        {
            cout << "Error creating temporary file.\n";
            return;
        }

        string line, headerLine;
        getline(orderFile, headerLine); // Skip the header line
        tempFile << headerLine << "\n"; // Write the header to temp file

        bool orderFound = false;
        string newStatus;
        cout << "Enter the new status (e.g., Pending, Completed, Shipped): ";
        cin >> newStatus;

        // Process each line in the original file
        while (getline(orderFile, line))
        {
            stringstream ss(line);
            string orderID, customerIDStr, itemName, quantity, total, customization, status, orderDate;

            getline(ss, orderID, ',');
            getline(ss, customerIDStr, ',');
            getline(ss, itemName, ',');
            getline(ss, quantity, ',');
            getline(ss, total, ',');
            getline(ss, customization, ',');
            getline(ss, status, ',');
            getline(ss, orderDate, ',');

            if (stoi(customerIDStr) == customerID)
            {
                orderFound = true;
                status = newStatus;
            }

            tempFile << orderID << "," << customerIDStr << "," << itemName << ","
                     << quantity << "," << total << "," << customization << ","
                     << status << "," << orderDate << "\n";
        }

        orderFile.close();
        tempFile.close();

        if (orderFound)
        {
            remove("order.csv");
            rename("TempOrder.csv", "order.csv");
            cout << "Order status updated successfully.\n";
        }
        else
        {
            remove("TempOrder.csv");
            cout << "Order with customer ID " << customerID << " not found.\n";
        }
    }

    void generateBill(int orderID, int customerID, const string &orderDate, const string &itemName,
                      int quantity, float itemPrice, float customizationCost, float discount, float totalAmount, LoyalCustomer &customer)
    {

        // Print the bill header
        cout << "                          *CAMPPUS BITES*                            \n";

        cout << "--------------------------- BILL RECEIPT ---------------------------\n";
        cout << "Order ID: " << orderID << endl;
        cout << "Customer ID: " << customerID << endl;
        cout << "Customer Name: " << customer.getName() << endl;
        cout << "Loyalty Points: " << customer.getLoyaltyPoints() << endl;
        cout << "Order Date: " << orderDate << endl;
        cout << "-------------------------------------------------------------------\n";

        cout << "Item Name    \t   Quantity   \t         Price  \tTotal\n";
        cout << "--------------------------------------------------------------------\n";

        float itemTotal = (itemPrice * quantity) + customizationCost;

        cout << left << setw(20) << itemName << setw(20) << quantity << setw(15) << fixed << setprecision(2) << itemPrice << setw(15) << fixed << setprecision(2) << itemTotal << endl;

        cout << "--------------------------------------------------------------------\n";
        cout << "Total Before Discount: Rs " << itemTotal << endl;
        cout << "Discount Applied: Rs " << discount << endl;

        cout << "Total After Discount: Rs " << totalAmount << endl;
        cout << "-------------------------------------------------------------------\n";

        customer.addLoyaltyPoints(totalAmount);

        cout << "---------------Enjoy your meal with Campus Bites!-------------------\n";
    }

    void displayOrders()
    {
        ifstream file(filename);
        if (!file.is_open())
        {
            cout << "Error opening file.\n";
            return;
        }
        string line;
        cout << "Order Details:\n";
        while (getline(file, line))
        {
            stringstream ss(line);
            string customerID, itemName, quantity, orderid, total, customization, orderStatus, orderDate;

            getline(ss, orderid, ',');
            getline(ss, customerID, ',');
            getline(ss, itemName, ',');
            getline(ss, quantity, ',');
            getline(ss, total, ',');
            getline(ss, customization, ',');
            getline(ss, orderStatus, ',');
            getline(ss, orderDate, ',');
            cout << "Order ID:" << orderid << ", Customer ID: " << customerID << ", Item Name: " << itemName << ", Quantity: " << quantity << ", Total:" << total << ", Customization:" << customization << ", Order Status:" << orderStatus << ", Date" << orderDate << endl;
        }
        file.close();
    }
};
class SalesReport
{
private:
    string filename = "order.csv";

public:
    SalesReport(string file)
    {
        filename = file;
    }

    const int MAX_ITEMS = 100;
    int itemCount = 0;
    float totalSales = 0.0f;
    float revenueGenerated = 0.0f; // Track revenue generated
    int ordersPlaced = 0;
    struct ItemSales
    {
        string itemName;
        int quantitySold;
    };

    void addOrUpdateItem(ItemSales items[], int &itemCount, const string &itemName, int quantity)
    {
        for (int i = 0; i < itemCount; i++)
        {
            if (items[i].itemName == itemName)
            {
                items[i].quantitySold += quantity;
                return;
            }
        }
        // If item not found, add new entry
        if (itemCount < MAX_ITEMS)
        {
            items[itemCount].itemName = itemName;
            items[itemCount].quantitySold = quantity;
            itemCount++;
        }
        else
        {
            cout << "Error: Maximum item limit reached!\n";
        }
    }

    void generateDailySalesReport()
    {
        string date;
        cout << "Enter the date (MM/DD/YYYY): ";
        cin >> date;

        stringstream ss(date);
        string month, day, year;
        char separator;

        getline(ss, month, '/'); // Month part
        getline(ss, day, '/');   // Day part
        getline(ss, year);       // Year part

        string formattedUserDate = year + "-" + (month.length() == 1 ? "0" + month : month) + "-" + (day.length() == 1 ? "0" + day : day);

        ifstream orderFile("order.csv");
        if (!orderFile.is_open())
        {
            cout << "Error opening order file for reading.\n";
            return;
        }

        string line, headerLine;
        getline(orderFile, headerLine); // Skip header line

        float totalSales = 0.0f;
        ItemSales itemSales[MAX_ITEMS];
        int itemCount = 0;
        cout << "--------------------------------*Item Sales*----------------------------------------\n";

        cout << "Item \t \t\tQuantity\t\tAmount\t\t Date" << endl;
        while (getline(orderFile, line))
        {
            stringstream ss(line);
            string orderID, customerID, itemName, quantityStr, totalStr, customization, status, orderDate;

            getline(ss, orderID, ',');
            getline(ss, customerID, ',');
            getline(ss, itemName, ',');
            getline(ss, quantityStr, ',');
            getline(ss, totalStr, ',');
            getline(ss, customization, ',');
            getline(ss, status, ',');
            getline(ss, orderDate, ',');

            int orderYear = stoi(orderDate.substr(0, 4));
            int orderMonth = stoi(orderDate.substr(5, 2));
            int orderDay = stoi(orderDate.substr(8, 2));

            string formattedOrderDate = to_string(orderYear) + "-" + (orderMonth < 10 ? "0" + to_string(orderMonth) : to_string(orderMonth)) + "-" + (orderDay < 10 ? "0" + to_string(orderDay) : to_string(orderDay));

            if (formattedOrderDate == formattedUserDate)
            {
                ordersPlaced++;
                revenueGenerated += stof(totalStr);
                totalSales += stof(totalStr);
                addOrUpdateItem(itemSales, itemCount, itemName, stoi(quantityStr));
                cout << left << setw(25) << itemName << setw(20) << quantityStr << setw(15) << totalStr << setw(15) << formattedOrderDate << endl;
            }
        }
        cout << "-------------------------------------------------------------------------\n";
        for (int i = 0; i < itemCount; i++)
        {
            cout << itemSales[i].itemName << ": " << itemSales[i].quantitySold << " units sold\n";
        }

        cout << "--------------------------- DAILY SALES REPORT ---------------------------\n";
        cout << "Date: " << formattedUserDate << endl;
        cout << "Total Sales: Rs " << totalSales << endl;
        cout << "Revenue Generated: Rs " << revenueGenerated << endl; // Display revenue generated
        cout << "Orders Placed: " << ordersPlaced << endl;            // Display total orders placed

        
        cout << "--------------------------------------------------------------------------\n";

        orderFile.close();
    }

    void generateMonthlySalesReport(int month, int year)
    {
        ifstream orderFile("order.csv");
        if (!orderFile.is_open())
        {
            cout << "Error opening order file for reading.\n";
            return;
        }

        string line, headerLine;
        getline(orderFile, headerLine); // Skip header line

        float totalSales = 0.0f;
        ItemSales itemSales[MAX_ITEMS];
        int itemCount = 0;
        cout << "--------------------------------*Item Sales*-------------------------------------------\n";

        cout << "Item \t\t\t Quantity\t\tAmount\t\tDate \n ";
        while (getline(orderFile, line))
        {
            stringstream ss(line);
            string orderID, customerID, itemName, quantityStr, totalStr, customization, status, orderDate;

            getline(ss, orderID, ',');
            getline(ss, customerID, ',');
            getline(ss, itemName, ',');
            getline(ss, quantityStr, ',');
            getline(ss, totalStr, ',');
            getline(ss, customization, ',');
            getline(ss, status, ',');
            getline(ss, orderDate, ',');

            int orderYear = stoi(orderDate.substr(0, 4));
            int orderMonth = stoi(orderDate.substr(5, 2));

            if (orderYear == year && orderMonth == month)
            {
                ordersPlaced++;                     // Increment order count for the day
                revenueGenerated += stof(totalStr); // Add to total revenue

                totalSales += stof(totalStr);

                addOrUpdateItem(itemSales, itemCount, itemName, stoi(quantityStr));
            }
            cout << left << setw(25) << itemName << setw(20) << quantityStr << setw(15) << totalStr << setw(15) << orderDate << endl;
        }
        cout << "----------------------------------------------------------------------------\n";
        for (int i = 0; i < itemCount; i++)
        {
            cout << itemSales[i].itemName << ": " << itemSales[i].quantitySold << " units sold\n";
        }
        cout << "--------------------------- MONTHLY SALES REPORT ---------------------------\n";
        cout << "Date:" << month << "/" << year << endl;
        cout << "Orders Placed this month: " << ordersPlaced << endl;
        cout << "Revenue Generated: Rs " << revenueGenerated << endl;
        cout << "Total Sales: Rs " << totalSales << endl;

        generateMostSoldItem();

        cout << "----------------------------------------------------------------------------\n";

        orderFile.close();
    }

    void generateMostSoldItem()
    {
        ifstream orderFile("order.csv");
        if (!orderFile.is_open())
        {
            cout << "Error opening order file for reading.\n";
            return;
        }

        string line, headerLine;
        getline(orderFile, headerLine); // Skip header line

        ItemSales itemSales[MAX_ITEMS];
        int itemCount = 0;

        while (getline(orderFile, line))
        {
            stringstream ss(line);
            string orderID, customerID, itemName, quantityStr, totalStr, customization, status, orderDate;

            getline(ss, orderID, ',');
            getline(ss, customerID, ',');
            getline(ss, itemName, ',');
            getline(ss, quantityStr, ',');
            getline(ss, totalStr, ',');
            getline(ss, customization, ',');
            getline(ss, status, ',');
            getline(ss, orderDate, ',');

            addOrUpdateItem(itemSales, itemCount, itemName, stoi(quantityStr));
        }

      
        string mostSoldItem;
        int maxQuantity = 0;

        for (int i = 0; i < itemCount; i++)
        {
            if (itemSales[i].quantitySold > maxQuantity)
            {
                mostSoldItem = itemSales[i].itemName;
                maxQuantity = itemSales[i].quantitySold;
            }
        }
        cout << "                                               *MOST SOLD ITEM*                             \n";

        cout << "Most Sold Item: " << mostSoldItem << " (" << maxQuantity << " units sold)\n";

        orderFile.close();
    }
};

void printCentered(const string &text)
{
    int width = 100;
    int padding = (width - text.size()) / 2;
    cout << setw(padding + text.size()) << text << endl;
}

void displayHeader()
{
    cout << "**************************************************************************************************\n";
    cout << "*                                                                                                *\n";
    printCentered("                    WELCOME TO MY CAMPUS BITES           ");
    printCentered("                       *Project by Aamat&Noor*              ");
    cout << "*                                                                                                *\n";

    cout << "**************************************************************************************************\n";
    string header = "Your Favorite Spot for Delicious Meals and Snacks!\n";

    for (char c : header)
    {
        cout << c << flush;
        this_thread::sleep_for(chrono::milliseconds(80));
    }
    cout << endl;
}

void setConsoleColor()
{
    system("color e6	");
}

int main()
{
    setConsoleColor();
    Menu menu("Menu.csv");
    LoyalCustomer customer("customer.csv");
    Inventory inventory;
    Order order("order.csv", &menu, &customer, &inventory);

    SalesReport report("order.csv");
    int choice;

    do
    {
        displayHeader();
        cout << "\n------------- Main Menu -----------\n";
        cout << "1. Menu management System\n";
        cout << "2. Customer Management System \n";
        cout << "3. Order Management System\n";
        cout << "4.Sales Report Management System\n";
        cout << "5. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
        {
            int choice;
            do
            {
                cout << "-----------Menu Management System---------------\n";
                cout << "1. Add Menu Item\n";
                cout << "2. View Menu\n";
                cout << "3. Delete Menu Item\n";
                cout << "4. Update stocks\n";

                cout << "5. Update  Prices\n";
                cout << "6. Exit Menu System \n";
                cout << "Enter choice \n";
                cin >> choice;
                switch (choice)
                {
                case 1:
                {

                    MenuItems *menuItem = new MenuItems("Menu.csv");
                    menuItem->createMenuitem();
                    menu.addmenuItem(menuItem);
                    break;
                }
                case 2:
                    menu.readMenuItems();
                    break;
                case 3:
                {
                    int itemIdToDelete;
                    menu.readMenuItems();
                    cout << "Enter the ID of the menu item to delete: ";
                    cin >> itemIdToDelete;
                    menu.deleteMenuItem(itemIdToDelete);
                    break;
                }
                case 4:
                {	string password;
   bool encrypted = false;

   while (!encrypted) {
    cout << "Enter password to See Sales:\n ";
     cin>>password;
     if (password != "70148752" && password != "70148753") {
        cout << " Only the manager can update stock.\n";
    } 
	else {
        encrypted = true;
    }
}
                    menu.readMenuItems();
                    int id, newStock;
                    cout << "Enter Item ID to update stock: ";
                    cin >> id;
                    cout << "Enter new stock quantity: ";
                    cin >> newStock;

                    inventory.updateStock(id, newStock);
                    break;
                }
                case 5:
                {	string password;
              bool encrypted = false;

                  while (!encrypted) {
      			 cout << "Enter password to See Sales: ";
   				  cin>>password;
   				 if (password != "70148752" && password != "70148753") {
   		     cout << " Only the manager can update stock.\n";
    			} else {
       			 encrypted = true;
  			  }
			}
                    menu.readMenuItems();
                    int id;
                    float newPrice;
                    cout << "Enter Item ID to update price: ";
                    cin >> id;
                    cout << "Enter new price: ";
                    cin >> newPrice;
                    inventory.updatePrice(id, newPrice);
                    break;
                }
                case 6:
                    cout << "Exiting menu system.\n";
                    break;
                default:
                    cout << "Invalid option! Please try again.\n";
                }
            } while (choice != 6);
            break;
        }

        case 2:
            customer.customerMenu();
            break;
        case 3:
        {

            int itemChoice;
            do
            {
                cout << "\n--- Menu ---\n";
                cout << "1. place  Order\n";
                cout << "2. Display Order\n";
               cout << "3. Exit Order Menu\n";
                cout << "Enter your choice: ";
                cin >> itemChoice;

                switch (itemChoice)
                {
                case 1:
                {
                    order.placeOrder();
                    break;
                }
                case 2:
                {
                    order.displayOrders();

                    break;
                }
                case 3:
                    cout << "Exiting order menu.\n";
                    break;
                default:
                    cout << "Invalid option! Please try again.\n";
                }
            } while (itemChoice != 3);

            break;
        }

        case 4:
        {
        	string password;
bool encrypted = false;

while (!encrypted) {
    cout << "Enter password to See Sales: ";
    cin>>password;
    if (password != "70148752" && password != "70148753") {
        cout << " Only the manager can update stock.\n";
    } else {
        encrypted = true;
    }
}
            int choice;
            do
            {
                cout << "--------------------Welcome to Sales Report------------------\n";
                cout << "1.Daily Report  \n";
                cout << "2.Monthly Report \n";
                cout << "3.Most and least sold  item \n";
                cout << "4.Exit Report Menu\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice)
                {

                case 1:

                    report.generateDailySalesReport();
                    break;
                case 2:
                {
                    int year, month;
                    cout << "Enter month (MM): ";
                    cin >> month;
                    cout << "enetr year \n";
                    cin >> year;
                    report.generateMonthlySalesReport(month, year);
                    break;
                }

                case 3:
                    report.generateMostSoldItem();
                    break;
                case 4:
                    cout << "Exiting order menu.\n";
                    break;

                default:
                    cout << "Invalid option! Please try again.\n";
                }
            } while (choice != 4);
            break;
        }

        case 5:
            cout << "\nThanks for Ordering From Campus Bites...\n";
            break;
        default:
            cout << "Invalid choice! Try again.\n";
        }
    } while (choice != 5);

    return 0;
}
