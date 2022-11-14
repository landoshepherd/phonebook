// phonebook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <stack>
#include <mysqlx/xdevapi.h>
#include <vector>

using namespace mysqlx;

std::string username = "root";
//Only for learning purposes! THIS IS DANGEROUS PRACTICE!
std::string password = "Xp0Sls6KjpyY7AvW#Zx1";
Session* mySession = nullptr;

struct User{
    std::string firstname;
    std::string lastname;
    std::string username;
    std::string password;
    std::string emailAddr;
    std::string phone;
    std::string streetNum;
    std::string streetName;
    std::string suite_num; //Suite or apartment. This may be left empty.
    std::string city;
    std::string state; //State should be abbreviated
};

void connectToDatabase() {
    try {
        mySession = new Session(SessionOption::HOST, "localhost",
            SessionOption::PORT, 33060,
            SessionOption::USER, username,
            SessionOption::PWD, password,
            SessionOption::DB, "car_rental_system");
    }
    catch (std::exception e) {
        std::cout << "ERROR: Unable to connect to database." << std::endl;
        std::cerr << e.what() << std::endl;
    }
}

void setFirstNameFromUser(struct User& user) {
    std::string tempFirstName;
    while (true) {
        std::cout << "Enter First Name: ";
        std::getline(std::cin, tempFirstName);

        if (tempFirstName.size() > 50) {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
        else {
            user.firstname = tempFirstName;
            break;
        }
    }    
}

void setLastNameFromUser(struct User& user) {
    std::string tempLastName;
    while (true) {
        std::cout << "Enter last Name: ";
        std::getline(std::cin, tempLastName);

        if (tempLastName.size() > 50) {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
        else {
            user.lastname = tempLastName;
            break;
        }
    }
}

void setUsernameFromUser(struct User& user) {
    std::string tempUsername;
    while (true) {
        std::cout << "Enter username: ";
        std::getline(std::cin, tempUsername);

        if (tempUsername.size() <= 50) {
            auto result = mySession->sql("SELECT * FROM user WHERE username = '" + tempUsername + "'").execute();
            if (result.hasData()) {
                std::string errorMessage = "'" + tempUsername + "' is already in use. Pick another user name.";
                std::cout << errorMessage << std::endl;
            }
            else {
                user.username = tempUsername;
                break;
            }
        }
        else {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }      
    }
}

void setUserPassword(struct User& user) {
    std::string tempPassword;

    while (true) {
        std::cout << "Enter password: ";
        std::getline(std::cin, tempPassword);

        if (tempPassword.size() <= 20) {
            user.password = tempPassword;
        }
        else {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
    }
}

bool emailIsValid(std::string& email) {
    if (!(email.at(0) >= 65 && email.at(0) <= 90) ||
        !(email.at(0) >= 97 && email.at(0) <= 122)) {
        return false;
    }

    std::stack<char> validEmailPattern;
    validEmailPattern.push('.');
    validEmailPattern.push('@');
    int numChars = 0;

    for (int i = 0; i < email.size(); i++) {
        if (email.at(i) == '@') {
            if (numChars > 0) {
                validEmailPattern.pop();
            }
        }

        if (dots >= 2) {
            return false;
        }
    }
}

void setUserEmail(struct User& user) {
    std::string tempEmailAddr;
    while (true) {
        //Check if email length is too long
        if (tempEmailAddr.size() > 50) {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
        
        for(int i = temp)
        // The email address must start with a letter (no numbers or symbols). (65 - 90 && 97 - 122)
        // There must be an @ somewhere in the string that is located before the dot.
        // There must be text after the @ symbol but before the dot.
        // There must be a dota nd text after the dot.
    }
}

void addUser(struct User& user) {
    while (true) {
        setFirstNameFromUser(user);
        setLastNameFromUser(user);
        setUsernameFromUser(user);
        setUserPassword(user);

        //TODO: Add username validation logic. Must be unique.
        

        

        break;
    }

    std::string insertUserQuery = "INSERT INTO users(first_name, last_name, username, password) VALUES ('" + firstname + "', '" + lastname + "', '" + username + "', '" + password + "')";
    mySession->sql(insertUserQuery).execute();
}

void searchDatabaseForUser() {
    std::string username;
    std::cout << "Enter username: ";
    //Validate!
    std::getline(std::cin, username);
    try {
        auto myResult = mySession->sql("SELECT first_name, last_name FROM users WHERE username = '" + username + "'").execute();
        if (myResult.hasData()) {
            Row user = myResult.fetchOne();
            std::cout << "Row Count: " << user.colCount() << std::endl;
            std::cout << "First Name: " << user.get(0) << std::endl;
            std::cout << "Last Name: " << user.get(1) << std::endl;
        }
        else {
            std::cout << "User not found" << std::endl;
        }
    }
    catch (std::exception e) {
        //do nothing
        std::cerr << "ERROR: Invalid SQL query. " << e.what() << std::endl;
    }
}

int main()
{
    connectToDatabase();

    std::list<Schema> schemas = mySession->getSchemas();
    std::cout << schemas.front().getName() << std::endl;

    std::string mainInput;
    std::string username;
    struct User user;


    while (true) {
        std::cout << "Welcome to the Phonebook" << std::endl;
        std::cout << "[1] Add To Contacts" << std::endl;
        std::cout << "[2] Search Contacts" << std::endl;
        std::getline(std::cin, mainInput);

        if (mainInput != "exit") {
            if (mainInput == "1") {
                addUser(user);
            }
            else if (mainInput == "2") {
                searchDatabaseForUser();
            }
            else {
                std::cout << "Invalid Entry." << std::endl;
            }
        }
        else {
            break;
        }
    }

    return 0;
}
