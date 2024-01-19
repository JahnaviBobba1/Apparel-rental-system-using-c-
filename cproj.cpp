#include <iostream>
#include <fstream>
#include <vector>
#include <string>

using namespace std;

struct User {
    string username;
    string password;
};

struct Apparel {
    string name;
    string type;
    bool isRented;
};

const string USER_FILE = "user_data.txt";
const string FILE_NAME = "apparel_data.txt";

User currentUser; // To keep track of the current user

void displayMenu();
void displayInventory(const vector<Apparel>& apparelList);
void rentApparel(vector<Apparel>& apparelList);
void returnApparel(vector<Apparel>& apparelList);
void donateCloth(vector<Apparel>& apparelList);
void saveData(const vector<Apparel>& apparelList);
void loadData(vector<Apparel>& apparelList);
bool signup();
bool login();

int main() {
    int loginChoice;

    cout << "=== Welcome to the Apparel Rental System ===\n";
    cout << "1. Login\n";
    cout << "2. Sign Up\n";
    cout << "Enter your choice: ";
    cin >> loginChoice;

    bool loggedIn = false;

    if (loginChoice == 1) {
        // Login
        if (login()) {
            loggedIn = true;
        } else {
            cout << "Login failed. Would you like to sign up? (1 for Yes, 0 for No): ";
            int signUpChoice;
            cin >> signUpChoice;

            if (signUpChoice == 1) {
                if (signup()) {
                    loggedIn = true;
                } else {
                    cout << "Sign up failed. Exiting the program.\n";
                    return 0;
                }
            } else {
                cout << "Exiting the program.\n";
                return 0;
            }
        }
    } else if (loginChoice == 2) {
        // Sign Up
        if (signup()) {
            loggedIn = true;
        } else {
            cout << "Sign up failed. Exiting the program.\n";
            return 0;
        }
    } else {
        cout << "Invalid choice. Exiting the program.\n";
        return 0;
    }

    vector<Apparel> apparelList;
    loadData(apparelList);

    int choice;
    do {
        displayMenu();
        cin >> choice;

        switch (choice) {
            case 1:
                displayInventory(apparelList);
                break;
            case 2:
                rentApparel(apparelList);
                break;
            case 3:
                returnApparel(apparelList);
                break;
            case 4:
                donateCloth(apparelList);
                break;
            case 5:
                saveData(apparelList);
                cout << "Data saved successfully.\n";
                break;
            case 6:
                cout << "Exiting the program.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }

    } while (choice != 6);

    return 0;
}

void displayMenu() {
    cout << "=== Apparel Rental System ===\n";
    cout << "1. Display Inventory\n";
    cout << "2. Rent Apparel\n";
    cout << "3. Return Apparel\n";
    cout << "4. Donate Cloth\n";
    cout << "5. Save Data\n";
    cout << "6. Exit\n";
    cout << "Enter your choice: ";
}

void displayInventory(const vector<Apparel>& apparelList) {
    cout << "=== Inventory ===\n";
    cout << "Name\tType\tStatus\n";
    for (const auto& apparel : apparelList) {
        cout << apparel.name << "\t" << apparel.type << "\t"
             << (apparel.isRented ? "Rented" : "Available") << "\n";
    }
}

void rentApparel(vector<Apparel>& apparelList) {
    string appName;
    cout << "Enter the name of the apparel to rent: ";
    cin >> appName;

    for (auto& apparel : apparelList) {
        if (apparel.name == appName) {
            if (!apparel.isRented) {
                apparel.isRented = true;
                cout << "Apparel rented successfully.\n";
            } else {
                cout << "Apparel is already rented.\n";
            }
            return;
        }
    }

    cout << "Apparel not found in the inventory.\n";
}

void returnApparel(vector<Apparel>& apparelList) {
    string appName;
    cout << "Enter the name of the apparel to return: ";
    cin >> appName;

    for (auto& apparel : apparelList) {
        if (apparel.name == appName) {
            if (apparel.isRented) {
                apparel.isRented = false;
                cout << "Apparel returned successfully.\n";
            } else {
                cout << "Apparel is not rented.\n";
            }
            return;
        }
    }

    cout << "Apparel not found in the inventory.\n";
}

void donateCloth(vector<Apparel>& apparelList) {
    Apparel newApparel;
    cout << "Enter the name of the donated apparel: ";
    cin >> newApparel.name;

    cout << "Enter the type of the donated apparel: ";
    cin >> newApparel.type;

    newApparel.isRented = false; // Donated apparel is always available

    apparelList.push_back(newApparel);
    cout << "Apparel donated successfully.\n";
}

void saveData(const vector<Apparel>& apparelList) {
    ofstream outFile(FILE_NAME);
    if (outFile.is_open()) {
        for (const auto& apparel : apparelList) {
            outFile << apparel.name << " " << apparel.type << " " << apparel.isRented << "\n";
        }
        outFile.close();
    } else {
        cout << "Unable to open the file for saving data.\n";
    }
}

void loadData(vector<Apparel>& apparelList) {
    ifstream inFile(FILE_NAME);
    if (inFile.is_open()) {
        while (!inFile.eof()) {
            Apparel apparel;
            inFile >> apparel.name >> apparel.type >> apparel.isRented;
            if (!inFile.fail()) {
                apparelList.push_back(apparel);
            }
        }
        inFile.close();
    } else {
        cout << "File not found. Creating a new file.\n";
        ofstream outFile(FILE_NAME);
        outFile.close();
    }
}

bool signup() {
    string username, password;
    cout << "=== Sign Up ===\n";
    cout << "Enter your desired username: ";
    cin >> username;

    // Check if the username already exists
    ifstream userFile(USER_FILE);
    string existingUsername;
    while (userFile >> existingUsername) {
        if (existingUsername == username) {
            cout << "Username already exists. Please choose another username.\n";
            return false;
        }
    }
    userFile.close();

    // If the username is unique, proceed with password creation
    cout << "Enter your desired password: ";
    cin >> password;

    // Save the new user to the user data file
    ofstream outFile(USER_FILE, ios::app);
    if (outFile.is_open()) {
        outFile << username << " " << password << "\n";
        outFile.close();
        cout << "Sign up successful!\n";
        return true;
    } else {
        cout << "Unable to open the user data file for sign up.\n";
        return false;
    }
}

bool login() {
    string username, password;
    cout << "=== Login ===\n";
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    ifstream userFile(USER_FILE);
    string existingUsername, existingPassword;
    while (userFile >> existingUsername >> existingPassword) {
        if (existingUsername == username && existingPassword == password) {
            currentUser.username = username;
            currentUser.password = password;
            cout << "Login successful. Welcome, " << currentUser.username << "!\n";
            userFile.close();
            return true;
        }
    }

    userFile.close();
    cout << "Login failed. Please check your username and password.\n";
    return false;
}
