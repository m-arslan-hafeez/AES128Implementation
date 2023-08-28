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

#include <cstdint>  

#include <cstdlib>

#include "keccak.h" //KEECAK



using namespace std; 

//BYTES to bits
uint64_t bytesToUInt64(const uint8_t x[8]) {
    uint64_t r = 0;
    for (int i = 0; i < 8; ++i) {
        r |= static_cast<uint64_t>(x[i]) << (i * 8);
    }
    return r;
}

//bits to BYTES
void uint64ToBytes(uint8_t x[8], uint64_t u) {
    for (unsigned int i = 0; i < 8; ++i) {
        x[i] = static_cast<uint8_t>(u >> (8 * i));
    }
}

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
        if (i < 16) {
            firstHalf[i] = key[i];
        } else {
            secondHalf[i - 16] = key[i];
        }
    }
    //extending key
    unsigned char extendedkeys1[176];
    unsigned char extendedkeys2[176];
    Key_extenxion(firstHalf, extendedkeys1);
    Key_extenxion(secondHalf, extendedkeys2); //end of extended key

    //begin accessing the encryptio.aes 

    //encryption.aes being cleared for new data to be wriiten

    //make a wish
    int choice;
    cout << "Welcome to 128 bits AES encryption.";
do {
    cout <<endl<< "Enter you choice " << endl;
    cout << "1- Encoding" << endl;
    cout << "2- Decoding" << endl;
    cout << "3- Exit" << endl;
    cin >> choice;

    switch (choice) {
    case 1: {
        ifstream encryptedFile;
        string filepath = "encryption.aes";
       
        encryptedFile.open(filepath.c_str(), std::ifstream::out | std::ifstream::trunc);  // Clearing encryption.aes before editing
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

            cout << "the number of bytes that will be read in order to "<< remainingBytes << endl;
            while (remainingBytes > 0) {
                
                int bytesRead = std::min(chunkSize, remainingBytes); // Number of bytes to read in this iteration

            cout << "\nthe number of bytes that remainign\n "<< remainingBytes << "\nnow processed\n" << bytesRead<< endl;


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


                //Encrypting the file***********************************

                encryption(temp, extendedkeys1, extendedkeys2);
                
                //Encrypting the file***********************************

                // Update the remaining bytes
                remainingBytes -= bytesRead;

                //                 cout << "\n Currently these word encrypted:\n";

                // for (int i = 0; i < 32; i++){

                //     cout << temp[i];
                // }
                // cout<<endl;

                //storing our encrypted data in encryption.aes	 		 
                ofstream fout; // Create Object of Ofstream
                fout.open("encryption.aes", ios::app); // Append mode
                if (fout.is_open())
                    fout << temp; // Writing data to file
                fout.close();
                // delete[] temp;
                // Check if we have reached the end of the newfile
                if (newfile.eof()) {
                    newfile.close();
                    break;
                }

            }
            cout << "\n128-bit AES encryption is done sucessfully!\n";
            cout << "Data has been appended to file encryption.aes"<<endl;

        } else {
            std::cout << "Failed to open the input file." << std::endl;
            return 1;
        }

        break;
    }
    case 2: {
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
            cout << "Reading encrypted data from encryption.aes...\n";

            cout << "Reading KEY from key.txt...\n";

            cout << "Now Decrypting...\n"<<endl;

            cout << "Writing decrypted data in outputtext.txt...\n";

            cout << "Following is our decrypted text: \n"<<endl;

            // Get the newfile size
            encryptedFile.seekg(0, std::ios::end);
            std::streampos newfileSize = encryptedFile.tellg();
            encryptedFile.seekg(0, std::ios::beg);
            // Read the newfile in chunks of 32 bytes
            const int chunkSize = 32;
            char buffer[chunkSize];

            int remainingBytes = newfileSize; // Number of bytes left to read
            while (remainingBytes > 0) {
                int bytesRead = std::min(chunkSize, remainingBytes); // Number of bytes to read in this iteration
               
                // Read a chunk of data
                encryptedFile.read(buffer, bytesRead);

                unsigned char * temp = new unsigned char[chunkSize];
                for (int j = 0; j < chunkSize; j++) {
                    temp[j] = buffer[j];
                }

                //Decrypting the file***********************************

                decryption(temp, extendedkeys1, extendedkeys2);

                //Decrypting the file***********************************

                // Determine the actual length of the decrypted data
                int actualDecryptedLength = bytesRead;

                for (; actualDecryptedLength > 0; --actualDecryptedLength) {
                    if (temp[actualDecryptedLength - 1] != '\0') {
                        break;
                    }
                }

                for (int i = 0; i < chunkSize; i++) {//printing the decrypted text
                    cout << temp[i];
                }

                // Update the remaining bytes
                remainingBytes -= chunkSize;
                //storing plain text in outputtext.txt file

                ofstream fout; // Create Object of Ofstream
                fout.open("outputtext.txt", ios::app); // Append mode
                if (fout.is_open())
                    fout << temp ; // Writing data to file
                // delete[] temp;
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