#include<iostream>

#include<fstream>

#include<cstring>

#include<sstream>

#include <algorithm> // For min

#include <iomanip>    // For hex

#include "key_expand.h"

#include "encoding.h"

#include "decoding.h"

#include <typeinfo>

#include<string.h>

using namespace std;

int main() {

    //Key expanded out side the oprions 
    string k;

    ifstream infile;
    infile.open("key.txt");
    if (infile.is_open()) {
        getline(infile, k); // The first line of file should be the key
        infile.close();
    } else {
        cout << "Error: Unable to open key file" << endl;
        return 1; // Return an error code
    }  

    istringstream tempkey(k);
    unsigned char key[32];
    unsigned char firstHalf[16];
    unsigned char secondHalf[16];

    unsigned int x;
    for (int i = 0; i < 32; i++) {
        tempkey >> hex >> x;
        key[i] = x;
        cout << hex << static_cast < int > (key[i]);
        if (i < 16) {
            firstHalf[i] = key[i];
        } else {
            secondHalf[i - 16] = key[i];
        }
    }
    cout << endl;
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << hex << static_cast < int > (firstHalf[i]);
    }
    cout << endl;
    cout << endl;
    for (int i = 0; i < 16; i++) {
        cout << hex << static_cast < int > (secondHalf[i]);
    }
    //extending key
    unsigned char extendedkeys1[176];
    unsigned char extendedkeys2[176];
    Key_extenxion(firstHalf, extendedkeys1);
    Key_extenxion(secondHalf, extendedkeys2); //end of extended key

    // printitng different attributes like size of characters and size in bytes etcc
    cout << endl;
    cout << endl;
    for (int i = 0; i < 176; i++) {
        cout << hex << static_cast < int > (extendedkeys1[i]);
    }
    cout << endl;
    cout << endl;
    for (int i = 0; i < 176; i++) {
        cout << hex << static_cast < int > (extendedkeys2[i]);
    }
    int sizeInBytes = sizeof(extendedkeys1);
    int numberOfCharacters = sizeof(extendedkeys1) / sizeof(extendedkeys1[0]);

    std::cout << "\nNumber of characters in the array: " << numberOfCharacters << std::endl;

    std::cout << "\nSize of in bytes: " << sizeInBytes << std::endl;

    //Key expamsion complete 

    //begin accessing the encryptio.aes 

    //encryption.aes being cleared for new data to be wriiten

    //make a wish
    int choice;
    cout << "Welcome to 128 bits AES encryption" << endl;
do {
    cout << "Enter you choice " << endl;
    cout << "1- Encoding" << endl;
    cout << "2- Decoding" << endl;
    cout << "3- Exit" << endl;
    cin >> choice;

    switch (choice) {
    case 1: {
        cout << "1- choosed" << endl;

        ifstream encryptedFile;
        string filepath = "encryption.aes";
        //clearing encryption.aes before editing
        encryptedFile.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
        if (!encryptedFile.is_open() || encryptedFile.fail()) {
            encryptedFile.close();
            cout << "Error: Unable to create/open/erase encryption.aes for writing" << endl;
            return 1; // Return an error code
        }
        encryptedFile.close();

        // begin reading input text.

        std::fstream newfile;
        newfile.open("input.txt", std::ios::in | std::ios::binary); // Open the newfile in binary mode

        if (newfile.is_open()) { //checking whether the file is open
            cout << "Reading plain text from input.txt .........\n";
            cout << "Reading KEY from key.txt ......\n";
            cout << "Now encrypting ....\n";
            cout << "writing encrypted data in encryption.aes ..\n";

            cout << endl;

            // Get the newfile size
            newfile.seekg(0, std::ios::end);
            std::streampos newfileSize = newfile.tellg();
            newfile.seekg(0, std::ios::beg);
            // Read the newfile in chunks of 32 bytes
            const int chunkSize = 32;
            char buffer[chunkSize];

            int remainingBytes = newfileSize; // Number of bytes left to read
            cout<< "length of input : "<< newfileSize << endl;
            while (remainingBytes > 0) {
                
                int bytesRead = std::min(chunkSize, remainingBytes); // Number of bytes to read in this iteration
                // Read a chunk of data
                newfile.read(buffer, bytesRead);

                // Process the chunk of data (buffer) here

                // for (int i = 0; i < bytesRead; ++i) { // Replace newline characters with spaces
                //     if (buffer[i] == '\n') {
                //         buffer[i] = ' ';
                //     }
                // }                              
                if (bytesRead < chunkSize) {// Pad the remaining space with null characters
                    for (int i = bytesRead; i < chunkSize; ++i) {
                        buffer[i] = '\0';
                    }
                }

                unsigned char * temp = new unsigned char[chunkSize];
                for (int j = 0; j < chunkSize; j++) {
                    temp[j] = buffer[j];
                }
                //print temp
                // for (int j = 0; j < chunkSize; j++) {
                //     cout<< temp[j];
                // }

                encryption(temp, extendedkeys1, extendedkeys2);
                // decryption(temp, extendedkeys1, extendedkeys2); 
                // cout<<endl<<" : ";
                // for (int j = 0; j < chunkSize; j++) {
                //     cout<< temp[j];
                // }
                // cout<<endl<<" : ";
                for (int j = 0; j < chunkSize; j++) {
                    buffer[j] = temp[j];
                }
                // Update the remaining bytes
                remainingBytes -= bytesRead;

                // delete[] temp;

                //storing our encrypted data in encryption.aes	 		 
                ofstream fout; // Create Object of Ofstream
                ifstream fin;
                // fin.open("encryption.aes");
                fout.open("encryption.aes", ios::app); // Append mode
                if (fout.is_open())
                    fout << temp; // Writing data to file
                // fin.close();
                fout.close();
                // Check if we have reached the end of the newfile
                if (newfile.eof()) {
                    newfile.close();
                    break;
                }

            }
            cout << "\n128-bit AES encryption is done :0\n";
            cout << "Data has been appended to file encryption.aes\n";

        } else {
            std::cout << "Failed to open the input file." << std::endl;
            return 1;
        }

        break;
    }
    case 2: {
        cout << "2- choosed" << endl;
        //clearing outputtext file
        ifstream decrptFile;
        string filepath = "outputtext.txt";
        decrptFile.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);
        if (!decrptFile.is_open() || decrptFile.fail()) {
            decrptFile.close();
             cout << "Error: Unable to create/open/erase outputtext.txt for writing" << endl;
             return 1; // Return an error code
        }
        decrptFile.close();
        //reading encrypted file
        string myText;
        ifstream encryptedFile;
        encryptedFile.open("encryption.aes", ios::in | ios::binary);
        if (encryptedFile.is_open()) {
            cout << "Reading encrypted data from encryption.txt .........\n";

            cout << "Reading KEY from key.txt ......\n";

            cout << "Now Decrypting ....\n";

            cout << "Writing decrypted data in outputtext.txt ..\n";

            cout << "Following is our decrypted text:- \n";

            // Get the newfile size
            encryptedFile.seekg(0, std::ios::end);
            // std::streampos newfileSize = encryptedFile.tellg();
            int newfileSize = encryptedFile.tellg();
            encryptedFile.seekg(0, std::ios::beg);
            // Read the newfile in chunks of 32 bytes
            const int chunkSize = 32;
            char buffer[chunkSize];


            int remainingBytes = newfileSize; // Number of bytes left to read
            while (remainingBytes > 0) {
                int bytesRead = std::min(chunkSize, remainingBytes); // Number of bytes to read in this iteration
               
                // Read a chunk of data
                // encryptedFile.read(buffer, bytesRead);
                encryptedFile.read(buffer, chunkSize);

                unsigned char * temp = new unsigned char[chunkSize];
                for (int j = 0; j < chunkSize; j++) {
                    temp[j] = buffer[j];
                }
                decryption(temp, extendedkeys1, extendedkeys2); 

                // Determine the actual length of the decrypted data
                // int actualDecryptedLength = bytesRead;
                // for (; actualDecryptedLength > 0; --actualDecryptedLength) {
                //     if (temp[actualDecryptedLength - 1] != '\0') {
                //         break;
                //     }
                // }
                for (int i = 0; i < chunkSize; i++) {//printing the decrypted text
                    cout << temp[i];
                }

                for (int j = 0; j < chunkSize; j++) {
                    buffer[j] = temp[j];
                }
                // Update the remaining bytes
                remainingBytes -= chunkSize;
                // delete[] temp;
                //storing plain text in outputtext.txt file
                cout << endl;

                ofstream fout; // Create Object of Ofstream
                // ifstream fin;
                // fin.open("outputtext.txt");
                fout.open("outputtext.txt", ios::app); // Append mode
                if (fout.is_open())
                    fout << temp ; // Writing data to file

                // fin.close();
                fout.close(); // Closing the file
            }
        } else {
            std::cout << "Failed to open the encrypted file." << std::endl;
            return 1;
        }
        break;
    }
    case 3:{
            cout << "Exiting the program." << endl;
            break;}
    default: {
        std::cout << "Invalid choice. Please select again." << std::endl;
    }
    }
 } while (choice != 3); //here do ends

  return 0;

}