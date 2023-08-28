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
    ifstream read_key_file;
    read_key_file.open("key.txt");
    if (read_key_file.is_open()) {
        getline(read_key_file, k); // The first line of file should be the key
        read_key_file.close();
    } else {
        cout << "Error: Unable to open key file" << endl;
        return 1; // Return an error code
    }  

    istringstream temp_key(k);
    unsigned char key[32];
    unsigned char first_half[16];
    unsigned char second_half[16];
    unsigned int x;

    for (int i = 0; i < 32; i++) {
        tempkey >> hex >> x;
        key[i] = x;
        if (i < 16) {
            first_half[i] = key[i];
        } else {
            second_half[i - 16] = key[i];
        }
    }

    //extending key
    unsigned char extended_key1[176];
    unsigned char extended_key2[176];
    keyExtension(first_half, extended_key1);
    keyExtension(second_half, extended_key2); //end of extended key

    //begin accessing the encryptio.aes 
    //encryption.aes being cleared for new data to be wriiten
    //make a wish
    int user_choice;
    cout << "Welcome to 128 bits AES encryption.";
do {
    cout <<endl<< "Enter you choice " << endl;
    cout << "1- Ecryption" << endl;
    cout << "2- Decryption" << endl;
    cout << "3- Exit" << endl;
    cin >> user_choice;

    switch (user_choice) {
    case 1: {
        ifstream read_encrypted_file;
        string filepath = "encryption.aes";
        read_encrypted_file.open(filepath.c_str(), ifstream::out | ifstream::trunc);  // Clearing encryption.aes before editing
        if (!read_encrypted_file.is_open() || read_encrypted_file.fail()) {
            read_encrypted_file.close();
            cout << "Error: Unable to create/open/erase encryption.aes for writing" << endl;
            return 1; // Return an error code
        }
        read_encrypted_file.close();

        // begin reading input text.
        fstream read_input_file;
        read_input_file.open("input.txt", ios::in | ios::binary); // Open the newfile in binary mode

        //checking whether the file is open
        if (read_input_file.is_open()) { 
            cout << "Reading plain text from input.txt .........\n";
            cout << "Reading KEY from key.txt ......\n";
            cout << "Now encrypting ....\n";
            cout << "writing encrypted data in encryption.aes ..\n";
            cout << endl;

            // Get the newfile size
            read_input_file.seekg(0, ios::end);
            streampos input_file_size = read_input_file.tellg();
            read_input_file.seekg(0, ios::beg);

            // Read the newfile in chunks of 32 bytes
            const int chunk_size = 32;
            char buffer[chunk_size];
            int remaining_bytes = input_file_size; // Number of bytes left to read

            // cout << "the number of bytes that will be read in order to "<< remainingBytes << endl;
            while (remaining_bytes > 0) {
            int bytes_read = min(chunk_size, remaining_bytes); // Number of bytes to read in this iteration

            // cout << "\nthe number of bytes that remainign\n "<< remainingBytes << "\nnow processed\n" << bytesRead<< endl;
                // Read a chunk of data
                read_input_file.read(buffer, bytes_read);                       
                if (bytes_read < chunk_size) {// Pad the remaining space with null characters
                    for (int i = bytes_read; i < chunk_size; ++i) {
                        buffer[i] = '\0';
                    }
                }


                unsigned char * temp = new unsigned char[chunk_size];
                for (int j = 0; j < chunk_size; j++) {
                    temp[j] = buffer[j];
                }

                unsigned char temp_left[16];
                unsigned char temp_right[16];

                for (int i = 0; i < 16; i++)
                {
                    temp_left[i] = temp[i];
                    temp_right[i] = temp[i + 16];
                }

                //Encrypting the file
                encryptionLeft(temp_left, extended_key1);
	            encryptionRight(temp_right, extended_key2); 

                    uint8_t in[32]; // fixed at the moment
                    uint64_t res; 

                    for (int i = 0; i < 32; i++) { //Populate the in array
                        in[i] = static_cast<uint8_t>(i + 1);
                    }
                    
                    cout << endl << "this is encted\n ";
                    for (int j = 0; j < sizeof(temp_left); j++) { //print in the array
                        cout << temp_left[j] << " ";
                    }
                    cout << endl;

                    uint64_t state[25];  // Initialize your state array here

                    for (int i = 0 ; i < 25 ; i++){
                        state[i]=0;
                    }

                    for (int i = 0; i < sizeof(temp_right); i += 8) {
                        res = bytesToUInt64(&temp_right[i]); 
                        cout << res << " " << endl;
                        state[i/8]= res;
                        }
                    for (int i = 0; i < sizeof(in); i += 8) {
                        res = bytesToUInt64(&in[i]); 
                        cout << res << " " << endl;
                        state[i/8 + 2]= res;
                        }

                    for (int j = 0; j < 25; ++j) {
                        cout << static_cast<uint64_t>(state[j]) << " ";
                    }
                    cout <<  "\n New values after the keecak permutation\n ";


                    // // Call the permutation function
                    KeccakF1600_StatePermute(state);

                    // // Print the updated state array
                    for (int i = 0; i < 25; ++i) {
                        cout << state[i] << " ";
                    }
                    cout << endl;

                for (int i = 0 ; i < 19 ; i++){
                        state[i+6]=0;
                    }

                    cout <<  "\n New values after the states tailed with zeros\n ";
                        // Print the updated state array
                    for (int i = 0; i < 25; ++i) {
                        cout << state[i] << " ";
                    }

                    // bits to byte for two arrays from state
                    uint8_t out[8]; //output testing
                    uint8_t out1[8]; //output testing
                    
                    unsigned char new_left_temp[16];

                    uint64ToBytes(out, state[0]);
                    uint64ToBytes(out1, state[1]);

                    for (int k =0; k < 8; k++){
                        new_left_temp[k]=out[k];
                        new_left_temp[k+8]=out1[k];
                    }

                    cout <<  "\n New values after the state in newlefttemp\n ";
                        // Print the updated state array
                    for (int i = 0; i < 25; ++i) {
                        cout << new_left_temp[i] << " ";
                    }
                    
                    for (int i = 0; i<16 ; i++){
                        new_left_temp[i]^=left_temp[i];  
                    }

                    cout <<  "\n New values after the state in templeft XORing\n ";
                    // Print the updated state array
                    for (int i = 0; i < 25; ++i) {
                        cout << temp_left[i] << " ";
                    }
                //Encrypting the file***********************************

                // Update the remaining bytes
                remaining_bytes -= bytes_read;

                // storing our encrypted data in encryption.aes	 		 
                ofstream file_write; // Create Object of Ofstream
                fout.open("encryption.aes", ios::app); // Append mode
                if (file_write.is_open())
                    file_write << temp; // Writing data to file
                file_write.close();
                // Check if we have reached the end of the newfile
                if (read_input_file.eof()) {
                    read_input_file.close();
                    break;
                }

            }
            cout << "\n128-bit AES encryption is done sucessfully!\n";
            cout << "Data has been appended to file encryption.aes"<<endl;

        } else {
            cout << "Failed to open the input file." << endl;
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