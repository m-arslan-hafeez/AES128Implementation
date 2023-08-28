// #include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
// #include<time.h>
// #include <ctype.h>
// #include <fcntl.h>
// #include <errno.h>
// #ifdef __linux__
// #define _GNU_SOURCE
// #include <unistd.h>
// #include <sys/syscall.h>
// #endif


// void randombytes(uint8_t *out, size_t outlen) {
//   ssize_t ret;
//   while(outlen > 0) {
//     ret = syscall(SYS_getrandom, out, outlen, 0);
//     if(ret == -1 && errno == EINTR)
//       continue;
//     else if(ret == -1)
//       abort();

//     out += ret;
//     outlen -= ret;
//   }
// }

 static uint64_t bytesToUInt64(const uint8_t x[8])
 {
  uint64_t r;
  r  = (uint64_t)x[0];
  r |= (uint64_t)x[1] << 8;
  r |= (uint64_t)x[2] << 16;
  r |= (uint64_t)x[3] << 24;
  r |= (uint64_t)x[4] << 32;
  r |= (uint64_t)x[5] << 40;
  r |= (uint64_t)x[6] << 48;
  r |= (uint64_t)x[7] << 56;
  return r;
}

// uint64_t bytesToUInt64(const uint8_t bytes[8]) {
//     uint64_t result = 0;
//     for (int i = 0; i < 8; i++) {
//         result = (result << 8) | bytes[i];
//     }
//     return result;
// }

static void uint64ToBytes(uint8_t x[8], uint64_t u) {
  unsigned int i;

  for(i=0;i<8;i++)
  {
    x[i] = u >> 8*i;
    printf("%d ", x[i]);    
}
 printf("\n\n");
  }

int main(){
    uint8_t in[32];
    uint8_t in1[16];
    uint8_t out[32];
    uint64_t res;
    for (int i=0; i<32; i++){
      in[i]=i+1;
     // printf("%d", in[i] );
    }
    printf("\n\n");
    //in[32] = [78,34,,4,3,,54,5,4,6,46,54,65,6,5,7,67,,6,6,,6,75,5,34,34,54,65,45,34,53,56,24,22,13,53];
    // randombytes(in, sizeof(in));
    // clock_t start_time, end;
    // double cputime;
    // start_time = clock();
    for (int j=0; j<sizeof(in); j++){
      printf("%d ", in[j]);

    }
     printf("\n\n");

    for (int i = 0; i < sizeof(in); i += 8) {
        

        // Call the function with the current chunk
        res=bytesToUInt64(&in[i]);
        printf("%ld ", res);

        printf("\n");

        uint64ToBytes(out, res);
    }
    
//end = clock();
       // cputime=(end - start_time)/1000;
       // printf("%f Total TIme: ", cputime);

    // for(int i=0; i<sizeof(in); i++){
    //     printf("%d ", in[i]);
    // }




return 0;

}
