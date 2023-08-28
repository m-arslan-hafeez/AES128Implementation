#include <iostream>
#include <cstdint>
#include <cstring>

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
    uint8_t in[32];
    uint8_t out[8];
    uint64_t res;

    for (int i = 0; i < 32; i++) {
        in[i] = static_cast<uint8_t>(i + 1);
    }

    for (int j = 0; j < sizeof(in); j++) {
        std::cout << static_cast<int>(in[j]) << " ";
    }

    std::cout << std::endl;

    uint64_t state[25];  // Initialize your state array here

    for (int i = 0 ; i < 25 ; i++){
        state[i]=0;
    }

    for (int i = 0; i < sizeof(in); i += 8) {
        res = bytesToUInt64(&in[i]); 
        std::cout << res << " " << std::endl;
        state[i/8 + 2]= res;
        }
    for (int j = 0; j < 25; ++j) {
            std::cout << static_cast<uint64_t>(state[j]) << " ";}

    return 0;
}
