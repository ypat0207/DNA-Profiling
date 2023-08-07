
/*

  Project 1 - DNA Profiling
  Name: Yash Patel
  CS 251 Fall 2022

  In the Program the user will have several options to enter for example, load_db, load_dna, 
  process,search and display. This app is designed to find correct person depending on the DNA strands 
  using the database and DNA files.

  Custom Command = "add_db"
  When user enters this command("add_db") it will ask user to input the custom database
  that the user wants to enter using commas after name and numbers in the data
  the code will automatically uppercase the first character of person name.
  After user inputs the data it will get stored in a file called "custom.txt"
  and the file will be created in the program folder and you will be able to use
  that text file for load database, load DNA, process the DNA and then search
  it.

 */

#include "ourvector.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

struct DNAProcessing {
  ourvector<char> personName; // Stores persons name from the database files
  ourvector<int> STRnumbers;  // stores STR numbers from the database files
};

// This is a helper function for load_db, it will help read data and will help
// load it.
void load_db_helper(ourvector<DNAProcessing> &database, ourvector<char> &str,
                    string s) {
  DNAProcessing storingData;
  string number;
  int num;
  // using this to filter off the first line containing
  // the str's by check if the first char is lower case
  if (islower(s[0])) {
    for (size_t i = 5; i < s.size(); i++) {
      str.push_back(s[i]);
    }
  } 
  else {
    for (size_t i = 0; i < s.size(); i++) {
      // if its an alphabet it would save in the name member of the struct
      if (isalpha(s[i])) {
        storingData.personName.push_back(s[i]);
      } 
      else if (isdigit(s[i])) { // filters the number off to save in its
                                  // proper member
        number += s[i];
        if (number.length() == 0) {
          continue;
        } 
        /* This condition will check if there is a number in string after ',' if there is it pushes into STRnumbers vector */
      else if ((s[i + 1] == ',') || (s[i + 1] == '\0')) {
          num = stoi(number);
          number = "";
          storingData.STRnumbers.push_back(num);
        }
      }
    }
    // pushes the struct into the vector of structs
    database.push_back(storingData);
  }
}

// Function for user input "load_db"
void loadDB(ourvector<DNAProcessing> &database, string filename,
            ourvector<char> &str) {
  ifstream inputFile;
  inputFile.open(filename);
  string file_data;
  /* opens the file and passed the data to load_db_helper where database is stored in personName and STRNumbers vectors. */
  if (inputFile.is_open()) {
    while (inputFile >> file_data) {
      load_db_helper(database, str, file_data);
    }
  } 
  else {
    cout << "Error: unable to open "
         << "'" << filename << "'" << endl;
  }
  inputFile.close();
  str.push_back(',');
}

// Function for user input "load_dna"
void loadDNA(string filename, ourvector<char> &DNA) {
  ifstream inputFile;
  inputFile.open(filename);
  string file_data;
  // opens the DNA file and passes the data into DNA character vector
  if (inputFile.is_open()) {
    while (inputFile >> file_data) {
      for (size_t i = 0; i < file_data.size(); i++) {
        DNA.push_back(file_data[i]);
      }
    }
  } 
  else {
    cout << "Error: unable to open "
         << "'" << filename << "'" << endl;
  }
  inputFile.close();
}

// This is a helper function for process to find the longest str sequence
bool longSequence(ourvector<char> singleSTR, ourvector<char> DNA, int &dnaAtI,
                  int &index, int &matchingNumber, int &bigSequence, int &i,
                  int &j) {
// this will check if the single str is same as the DNA single str if it is then it will return true
  if (DNA.at(dnaAtI) == singleSTR[j]) {
    index++;
    if (index == singleSTR.size()) {
      i += index;
      matchingNumber++;
      return true;
    } 
    else {
      dnaAtI++;
    }
  } 
  else {
    if (bigSequence < matchingNumber) {
      bigSequence = matchingNumber;
    }
    matchingNumber = 0;
    i++;
    index = 0;
    return true;
  }
  return false;
}

// Helper function for the process command
void Sequence(ourvector<char> singleSTR, ourvector<char> DNA,
              ourvector<int> &str_count) {
  int matchingNumber = 0;
  int dnaAtI;
  int bigSequence = 0;
  int index = 0;
  int isMatching = 0;
  int i = 0;

  while (i < DNA.size()) {
    index = 0;
    dnaAtI = i;
    for (int j = 0; j < singleSTR.size(); j++) {
      if (dnaAtI < DNA.size()) {
        isMatching = longSequence(singleSTR, DNA, dnaAtI, index, matchingNumber,
                                  bigSequence, i, j);
        if (isMatching == 1) {
          break;
        } 
        else {
          continue;
        }
        if (DNA.at(dnaAtI) == singleSTR[j]) {
          index++;
          if (index == singleSTR.size()) {
            i += index;
            matchingNumber++;
            break;
          } 
          else {
            dnaAtI++;
          }
        } 
        else {
          if (bigSequence < matchingNumber) {
            bigSequence = matchingNumber;
            matchingNumber = 0;
          }
          matchingNumber = 0;
          i++;
          index = 0;
          break;
        }
      } 
      else {
        if (bigSequence < matchingNumber) {
          bigSequence = matchingNumber;
        }
        matchingNumber = 0;
        i = dnaAtI;
        i++;
        break;
      }
    }
  }
  str_count.push_back(bigSequence);
}

// Function for user input "process"
void Process(ourvector<char> str, ourvector<char> DNA,
             ourvector<int> &strCounter) {
  cout << "Processing DNA..." << endl;

  // this vector stores each STR
  ourvector<char> singleSTR;
  // first get the str in the str vector
  for (int i = 0; i < str.size(); i++) {
    if (str[i] != ',') {
      singleSTR.push_back(str[i]);
    } 
    else if ((str[i] == ',') || (str[i] == '\0')) {
      Sequence(singleSTR, DNA, strCounter);
      singleSTR.clear();
    }
  }
}

// Helper function for Search, it will go through the data and check if its in
// the database
bool STRFound(ourvector<int> strCounter,
              ourvector<DNAProcessing> nameAndNumberDatabase, int &index) {
  cout << "Searching database..." << endl;
  int foundinDbCounter = 0;
  bool found = false;
  /* This will compare database with strcounters if it matches it will increment it and will return found in database to true */
  for (int i = 0; i < nameAndNumberDatabase.size(); i++) {
    for (int j = 0; j < strCounter.size(); j++) {
      if (nameAndNumberDatabase[i].STRnumbers[j] == strCounter[j]) {
        foundinDbCounter++;
        if (foundinDbCounter == strCounter.size()) {
          cout << "Found in database! DNA matches: ";
          index = i;
          found = true;
          break;
        }
      } 
      else {
        foundinDbCounter = 0;
        break;
      }
    }
  }
  return found;
}
// Function for user input Search
void Search(ourvector<int> strCounter,
            ourvector<DNAProcessing> nameAndNumberDatabase) {
  int personLocator;
  // This if statement will check if STR has been found between the database and
  // using DNA file if its found then it will output the person name depending
  if (STRFound(strCounter, nameAndNumberDatabase, personLocator) == true) {
    for (int i = 0; i < nameAndNumberDatabase[personLocator].personName.size();
         i++) {
      cout << nameAndNumberDatabase[personLocator].personName[i];
    }
  } 
  else {
    cout << "Not found in database.";
  }
  cout << endl;
}

// Function for user input Display
void display(ourvector<DNAProcessing> &database, ourvector<char> &str,
             ourvector<char> DNA, int &processed, ourvector<int> &str_count) {
  // checks if database is loaded by using size() function
  if (database.size() == 0) {
    cout << "No database loaded.";
  } 
  else {
    cout << "Database loaded: " << endl;
    for (int i = 0; i < database.size(); i++) {
      /* outputs the name from the database text file and outputs the name from
       * personName vector when database is loaded */
      for (int j = 0; j < database.at(i).personName.size(); j++) {
        cout << database.at(i).personName.at(j);
      }
      // outputs the numbers from the databse text file according to the name
      // when database is loaded
      for (int index = 0; index < database.at(i).STRnumbers.size(); index++) {
        cout << " ";
        cout << database.at(i).STRnumbers.at(index);
      }
      cout << endl;
    }
  }
  // checks if DNA is loaded or not with size() function
  if (DNA.size() == 0) {
    cout << endl << "No DNA loaded." << endl;
  } 
  else {
    cout << endl << "DNA loaded: " << endl;
    for (int i = 0; i < DNA.size(); i++) { /*This loop loads DNA and outputs it
                                            when its loaded and displayed*/
      cout << DNA.at(i);
    }
    cout << endl;
  }
  int j = 0;
  // checks if process has been done for DNA
  if (processed == false) {
    cout << endl << "No DNA has been processed." << endl;
  } 
  else if (processed == true) {
    cout << endl << "DNA processed, STR counts:" << endl;

    // This for loop will grab the DNA and then it will process it and output
    // it. It will also check if there is ',' and it will output the str count.
    for (int i = 0; i < str.size(); i++) {
      if (str[i] == ',') {
        cout << ":"
             << " " << str_count[j] << " ";
        j++;
        cout << endl;
        continue;
      }
      cout << str[i];
    }
    cout << endl;
  }
}
// this customcommand will get database input from user and create text file
// that you can use
void customAddCommand(ourvector<DNAProcessing> &database, ourvector<char> str,
                      string filename) {
  string customInput;
  getline(cin, customInput);
  ofstream customFile;
  stringstream temp(customInput);
  customFile.open("custom.txt");
  customFile << "name,AGATC,AATG,TATC" << endl;
  customInput[0] = toupper(customInput[0]);
  customFile << customInput;
}

int main() {

  ourvector<int> str_count; // stores the str count from file as integer
  ourvector<char> str;      // store the str from the database file as character
  ourvector<char> DNA;      // stores the dna from the dna file as character
  ourvector<DNAProcessing> nameAndNumberDatabase; // stores the name and number of sequence from the database file using the DNAProcessing struct
  ourvector<int> strCounter; // store the number of counts after entering the process commmand
  ourvector<DNAProcessing> database; // stores all the database 
  int dbCounter = 0;
  int dnaCounter = 0;
  int processed = 0;
  string filename;
  string input, file_name, command;
  cout << "Welcome to the DNA Profiling Application." << endl;

  while (command != "#") {
    cout << "Enter command or # to exit: ";
    getline(cin, input);
    stringstream fullInput(input);
    fullInput >> command >> file_name;

    // checks if first word of input is 'l' if it is that the condition goes forward
    if (input.at(0) == 'l') {
      // checks if user input is "load_db"
      if (command == "load_db") {
        /* This if condition will check if "load_dna" command has been used more
          than once if it is
          used more than once it will clear out the Database vector so you can
          load another Database. */
        if (dbCounter >= 1) {
          processed = 0;
          nameAndNumberDatabase.clear();
        }
        cout << "Loading database..." << endl;
        dbCounter++;
        loadDB(nameAndNumberDatabase, file_name, str);
      } 
        // checks if user input is "load_dna"
      else if (command == "load_dna") {
        /* This if condition will check if "load_dna" command has been used more
         than once if it is used more than once it will clear out the DNA vector so you can load
         another DNA. */
        if (dnaCounter >= 1) {
          processed = 0;
          DNA.clear();
        }
        cout << "Loading DNA..." << endl;
        dnaCounter++;
        loadDNA(file_name, DNA);
      }

    } 
    // checks if user input is "display"
    else if (command == "display") {
        display(nameAndNumberDatabase, str, DNA, processed, strCounter);
    } 
    // checks if user input is "process"
    else if (command == "process") {
      // This if condition will check the size of database and will determine if its loaded or not
        if (str.size() == 0 && nameAndNumberDatabase.size() == 0) {
          cout << "No database loaded." << endl;
        } 
        // This condition will check the size of DNA and will determine if its loaded or not
        else if (DNA.size() == 0) {
          cout << "No DNA loaded." << endl;
        } 
        // if both DNA and Database are loaded it will trigger the function to finish processing the DNA
        else {
          Process(str, DNA, strCounter);
          processed = true;
        }
    } 
    // checks if user input is "search"
    else if (command == "search") {
      // This if condition will check the size of database and will determine if its loaded or not
        if (str.size() == 0 && nameAndNumberDatabase.size() == 0) {
          cout << "No database loaded." << endl;
        } 
        // This condition will check the size of DNA and will determine if its loaded or not
        else if (DNA.size() == 0) {
          cout << "No DNA loaded." << endl;
        } 
        // This condition will check if DNA has been processed or not
        else if (processed == 0) {
          cout << "No DNA processed." << endl;
        } 
        // If DNA has been Processed it will trigger the search function to find the person
        else {
          Search(strCounter, nameAndNumberDatabase);
        }
    }
    // This is a custom command explained in the header all the way up top
    else if (command == "add_db") {
        cout << "Enter your new database: ";
        customAddCommand(database, str, filename);
    }
  }
}
