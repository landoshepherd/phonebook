// phonebook.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <mysqlx/xdevapi.h>
#include <vector>

using namespace mysqlx;

std::string username = "root";
//Only for learning purposes! THIS IS DANGEROUS PRACTICE!
std::string password = "Xp0Sls6KjpyY7AvW#Zx1";
Session* mySession = nullptr;

struct userData {
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


void addUser() {
    while (true) {
        std::cout << "Enter First Name: ";
        std::getline(std::cin, firstname);
        std::cout << "Your first name is " << firstname << std::endl;

        std::cout << "Enter last Name: ";
        std::getline(std::cin, lastname);
        std::cout << "Your last name is " << lastname << std::endl;

        //TODO: Add username validation logic. Must be unique.
        std::cout << "Enter username: ";
        std::getline(std::cin, username);
        std::cout << "Your username is " << username << std::endl;

        std::cout << "Enter password: ";
        std::getline(std::cin, password);
        std::cout << "Your password is " << password << std::endl;

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


    while (true) {
        std::cout << "Welcome to the Phonebook" << std::endl;
        std::cout << "[1] Add To Contacts" << std::endl;
        std::cout << "[2] Search Contacts" << std::endl;
        std::getline(std::cin, mainInput);

        if (mainInput != "exit") {
            if (mainInput == "1") {
                addUser();
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
