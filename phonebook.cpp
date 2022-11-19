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
            bool userFound = result.fetchOne().isNull();
            if (!userFound) {
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
            break;
        }
        else {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
    }
}

bool emailIsValid(const std::string& email) {
    // Check if first character is a letter
    if ((tolower(email.at(0)) < 97 || tolower(email.at(0)) > 122)) {
        return false;
    }

    // Last character has to be a letter
    if (email.at(email.size()-1) < 97 || email.at(email.size()-1) > 122) {
        return false;
    }

    int atCount = 0;
    int dotIdx = 0;
    int charCount = 0;

    for (int i = 0; i < email.size(); i++) {
        if (email.at(i) == '@') {
            atCount++;
            if (atCount > 1) {
                return false;
            }
        }
        else if (email.at(i) == '.') {
            if (email.at(i - 1) == '.') {
                return false;
            }
            dotIdx = i;
        }
    }

    if ((email.size() - 1) - dotIdx < 2)
    {
        return false;
    }

    return true;
}

void setUserEmail(struct User& user) {
    std::string tempEmailAddr;
    while (true) {
        std::cout << "Enter email: ";
        std::getline(std::cin, tempEmailAddr);
        //Check if email length is too long
        if (tempEmailAddr.size() > 260) {
            std::cout << "Maximum characters exceeded. Try again." << std::endl;
        }
        else if (emailIsValid(tempEmailAddr)) {
            user.emailAddr = tempEmailAddr;
            break;
        }
        else {
            std::cout << "Invalid email address. Try again." << std::endl;
        }       
    }
}

void addUser(struct User& user) {

    setFirstNameFromUser(user);
    setLastNameFromUser(user);
    setUsernameFromUser(user);
    setUserPassword(user);
    setUserEmail(user);



    std::string insertUserQuery = "INSERT INTO user(first_name, last_name, username, user_password, email_address) VALUES ('";
    insertUserQuery.append(user.firstname + "', '");
    insertUserQuery.append(user.lastname + "', '");
    insertUserQuery.append(user.username + "', '");
    insertUserQuery.append(user.password + "', '");
    insertUserQuery.append(user.emailAddr + "')");
    mySession->sql(insertUserQuery).execute();

    std::cout << "Successfully added user: " + user.firstname + " " + user.lastname + " to the database." << std::endl;
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
