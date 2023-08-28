/*
The code is an implementation of AES128 encryption and decryption. 
It provides a command-line interface for the user to choose between encoding and decoding operations.

The encoding operation reads plain text from an "input.txt" file, 
reads a key from a "key.txt" file, encrypts the plain text using AES128 encryption, 
and stores the encrypted data in an "encryption.aes" file.

The decoding operation reads encrypted data from the "encryption.aes" file, 
reads the key from the "key.txt" file, decrypts the encrypted data using AES128 decryption, 
and stores the decrypted data in an "outputtext.txt" file.
*/
#include<iostream>

#include<fstream>

#include<cstring>

#include<sstream>

#include "key_expand.h"

#include "encoding.h"

#include "decoding.h"

#include <typeinfo>
 // #include<windows.h>
using namespace std;
int main() {
  //we will read from file input.txt
  int extendedlength = 0;
  int choice;
  string myText;
  label:
    cout << "Welcome to 128 bits AES encryption" << endl;
  cout << endl;
  cout << "Enter you choice " << endl;
  cout << "1- Encoding" << endl;
  cout << "2- Decoding" << endl;
  cin >> choice;
  //Key expanded out side the oprions 
  string k;

  ifstream infile;
  infile.open("key.txt");
  if (infile.is_open()) {
    getline(infile, k); // The first line of file should be the key
    infile.close();
  } else cout << "Unable to open file";

  istringstream tempkey(k);
  unsigned char key[32];
  unsigned char firstHalf[16];
  unsigned char secondHalf[16];

  unsigned int x;
  for (int i = 0; i < 32; i++) {
    tempkey >> hex >> x;
    key[i] = x;
    if (i < 16) {
      firstHalf[i] = key[i];
    }
    if (i > 16) {
      secondHalf[i - 16] = key[i];
    }
    cout << hex << static_cast < int > (key[i]);
  }

  cout << endl;
  //extending key
  unsigned char extendedkeys1[176];
  unsigned char extendedkeys2[176];
  Key_extenxion(firstHalf, extendedkeys1);
  Key_extenxion(secondHalf, extendedkeys2);

  switch (choice) {
  case 1: {
    //encryption of text data
    ifstream File;
    string filepath = "encryption.aes";
    //clearing encryption.aes before editing
    File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
    if (!File.is_open() || File.fail()) {
      File.close();
      printf("\nError : failed to erase file content !");
    }
    File.close();


    //reading plain text from input.txt 
    fstream newfile;
    
    newfile.open("input.txt", ios::in | ios::binary); //open a file to perform read operation using file object


    if (newfile.is_open()) { //checking whether the file is open
      cout << "Reading plain text from input.txt .........\n";
      // Sleep(1000);
      string tp;
      cout << "Reading KEY from key.txt ......\n";
      // Sleep(1000);
      cout << "Now encrypting ....\n";
      // Sleep(1000);
      cout << "writing encrypted data in encryption.aes ..\n";
      // Sleep(1000);
      cout << endl;
      while (getline(newfile, tp)) {
        //read data from file object and put it into string.
        int messlength = tp.length();
        int extendedlength;
        if ((messlength % 32) != 0) // changed from 16 to 32 
        {
          extendedlength = messlength + (32 - (messlength % 32));
        } else {
          extendedlength = messlength;
        }
        unsigned char * encryptedtext = new unsigned char[extendedlength];
        for (int i = 0; i < extendedlength; i++) {
          if (i < messlength)
            encryptedtext[i] = tp[i];
          else
            encryptedtext[i] = 0;
        }
        //getting key from key.txt
        // string k;

        // ifstream infile;
        // infile.open("key.txt");
        // if (infile.is_open()) {
        //     getline(infile, k); // The first line of file should be the key
        //     infile.close();
        // } else cout << "Unable to open file";

        // istringstream tempkey(k);
        // unsigned char key[32];
        // unsigned char firstHalf[16];
        // unsigned char secondHalf[16];

        // unsigned int x;
        // for (int i = 0; i < 32; i++) {
        //     tempkey >> hex >> x;
        // 	key[i]=x;
        // 	if (i<16){firstHalf[i] = key[i];}
        // 	if (i>16){secondHalf[i-16] = key[i];}
        // cout << hex << static_cast<int>(key[i]);
        //     }

        // cout<<endl;
        // //extending key
        // unsigned char extendedkeys1[32];
        // unsigned char extendedkeys2[32];

        // Key_extenxion(firstHalf, extendedkeys1);
        // Key_extenxion(secondHalf, extendedkeys2);

        //encrypting our plain text
        for (int i = 0; i < extendedlength; i += 32) {
          unsigned char * temp = new unsigned char[32];
          for (int j = 0; j < 32; j++) {
            temp[j] = encryptedtext[i + j];
          }
          encryption(temp, extendedkeys1, extendedkeys2);
          for (int j = 0; j < 32; j++) {
            encryptedtext[i + j] = temp[j];
          }
        }
        //storing our encrypted data in encryption.aes	 		 
        ofstream fout; // Create Object of Ofstream
        ifstream fin;
        fin.open("encryption.aes");
        fout.open("encryption.aes", ios::app); // Append mode
        if (fin.is_open())
          fout << encryptedtext << "\n"; // Writing data to file
        fin.close();
        fout.close();
      }
      cout << "128-bit AES encryption is done sucessfully\n";
      cout << "Data has been appended to file encryption.aes\n";
      newfile.close(); //close the file object.
    }
    break;
  }

  case 2: {
    cout << "Reading encrypted data from encryption.txt .........\n";
    // Sleep(1000);
    string tp;
    cout << "Reading KEY from key.txt ......\n";
    // Sleep(1000);
    cout << "Now Decrypting ....\n";
    // Sleep(1000);
    cout << "writing decrypted data in outputtext.txt ..\n";
    // Sleep(1000);
    cout << endl;
    cout << "Following is our decrypted text:- \n";
    //clearing outputtext file
    ifstream File;
    string filepath = "outputtext.txt";
    File.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
    if (!File.is_open() || File.fail()) {
      File.close();
      printf("\nError : failed to erase file content !");
    }
    File.close();

    ifstream MyReadFile;
    MyReadFile.open("encryption.aes", ios::in | ios::binary);
    if (MyReadFile.is_open()) {
      while (getline(MyReadFile, myText)) {
        cout.flush();
        char * x;
        x = & myText[0];
        int messlength = strlen(x);
        char * msg = new char[myText.size() + 1];

        strcpy(msg, myText.c_str());

        int n = strlen((const char * ) msg);
        unsigned char * decryptedtext = new unsigned char[n];
        //decrypting our encrypted data
        for (int i = 0; i < n; i++) {
          decryptedtext[i] = (unsigned char) msg[i];
        }
        //reading key from key.txt file
        // string k;
        // ifstream infile;
        // infile.open("key.txt");
        // if (infile.is_open()) {
        //     getline(infile, k); // The first line of file should be the key
        //     infile.close();
        // } else cout << "Unable to open file";
        // istringstream tempkey(k);
        // unsigned char key[32];
        // unsigned char firstHalf[16];
        // unsigned char secondHalf[16];
        // unsigned int x1;
        // for (int i = 0; i < 32; i++) {
        //     tempkey >> hex >> x1;
        //     key[i] = x1;
        // 	if (i<16){firstHalf[i] = key[i];}
        // 	if (i>16){secondHalf[i-16] = key[i];}
        // }

        // //extending key
        // unsigned char extendedkeys1[32];
        // unsigned char extendedkeys2[32];
        // Key_extenxion(firstHalf, extendedkeys1);
        // Key_extenxion(secondHalf, extendedkeys2);

        //decrypting our data  
        for (int i = 0; i < messlength; i += 32) {
          unsigned char * temp = new unsigned char[32];
          for (int j = 0; j < 32; j++)
            temp[j] = decryptedtext[i + j];
          decryption(temp, extendedkeys1, extendedkeys2);
          for (int j = 0; j < 32; j++)
            decryptedtext[i + j] = temp[j];
        }
        //printing our plain text
        for (int i = 0; i < messlength; i++) {
          cout << decryptedtext[i];
          if (decryptedtext[i] == 0 && decryptedtext[i - 1] == 0)
            break;
        }
        //storing plain text in outputtext.txt file
        cout << endl;
        ofstream fout; // Create Object of Ofstream
        ifstream fin;
        fin.open("outputtext.txt");
        fout.open("outputtext.txt", ios::app); // Append mode
        if (fin.is_open())
          fout << decryptedtext << "\n"; // Writing data to file

        fin.close();
        fout.close(); // Closing the file
        // Sleep(500);
      }
    } else {
      cout << "Can not open input file\n ";
    }
    cout << "\n Data has been appended to file outputtext.txt\n";
    MyReadFile.close();
    break;
  }
  }
}