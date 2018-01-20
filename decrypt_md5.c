// **********************************************
//
//              Desenvolvido por 
//        Paulo Caetano Virote de Souza
//
// **********************************************
// **********************************************
// 
// Programa utilizado para quebrar uma hash md5
// 
// Para compilar basta rodar o comando:
// $ gcc -fopenmp decrypt_md5.c -o nomeDaSaida
// $ gcc -fopenmp decrypt_md5.c -o teste  -I /usr/local/opt/openssl/include -L /usr/local/opt/openssl/lib
// $ gcc main.c -o teste  -I /usr/local/opt/openssl/
// $ gcc -fopenmp main.c -o teste  -I /usr/local/opt/openssl/ -L /usr/local/opt/openssl/lib
// 
// **********************************************
// **********************************************

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "md5.h"
 
// *******************************
// Funções de tempo
// *******************************

struct timeval start, stop;
unsigned diffsec, diffusec;
int a[2];

void compute_time_and_flush();

void time_log_start() {
	gettimeofday(&start, NULL);
}

void time_log_stop() {
	gettimeofday(&stop, NULL);
	compute_time_and_flush();
}

void compute_time_and_flush() {
		diffsec = stop.tv_sec - start.tv_sec;
		diffusec = (stop.tv_usec - start.tv_usec) >= 0 ? (stop.tv_usec - start.tv_usec) : 1000000 - stop.tv_usec;

		printf("%d.%d seconds\n", diffsec, diffusec);
}

// *******************************
// MD5
// *******************************

// MD5 functions
#define F(x,y,z) ((x & y) | (~x & z))
#define G(x,y,z) ((x & z) | (y & ~z))
#define H(x,y,z) (x ^ y ^ z)
#define I(x,y,z) (y ^ (x | ~z))

// Rotate a 32 bit number left by n bits
#define ROTATE(x,n) ((x << n) | (x >> (32-n)))

// Swap endianess of 32 bit number
#define SWAP(x) ( ((x >> 24) & 0x000000ff) | ((x <<  8) & 0x00ff0000) \
                | ((x >>  8) & 0x0000ff00) | ((x << 24) & 0xff000000) ); 


// CONTANTS -------------------------------------------------------------------

// Magic numbers
static const uint32_t A = 0x67452301, 
                      B = 0xEFCDAB89, 
                      C = 0x98BADCFE, 
                      D = 0x10325476;

static const uint32_t K[64] = 
  { 0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

// Byte selectors
static const uint32_t M[64] = 
  { 0, 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,
    1, 6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,
    5, 8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,
    0, 7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9 };

// Shift values
static const uint32_t S[64] = 
  { 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21 };


// FUNCTIONS ------------------------------------------------------------------

void md5_init (md5_t* self) {
    self->bufsize = 0;
    self->bitsize = 0;
    self->rawhash[0] = A;
    self->rawhash[1] = B;
    self->rawhash[2] = C;
    self->rawhash[3] = D;
}

void md5_digest (md5_t* self) {
    
    uint32_t data[16], a, b, c, d;
    
    a = self->rawhash[0];
    b = self->rawhash[1];
    c = self->rawhash[2];
    d = self->rawhash[3];

    for (int i = 0, j = 0; i < 16; ++i, j+=4)
        data[i] = (self->buffer[j]          ) + (self->buffer[j + 1] <<  8) +
                  (self->buffer[j + 2] << 16) + (self->buffer[j + 3] << 24);

    for (int i = 0; i < 64; ++i) {

        uint32_t func, temp;

        if      (i < 16) func = F(b,c,d);
        else if (i < 32) func = G(b,c,d);
        else if (i < 48) func = H(b,c,d);
        else if (i < 64) func = I(b,c,d);

        temp = d;
        d = c;
        c = b;
        b = b + ROTATE(a + func + data[M[i]] + K[i], S[i]);
        a = temp;
    }

    self->rawhash[0] += a;
    self->rawhash[1] += b;
    self->rawhash[2] += c;
    self->rawhash[3] += d;
}

void md5_update (md5_t* self, byte_t* data, size_t length) {

    for (size_t i = 0; i < length; ++i) {
        self->buffer[self->bufsize] = data[i];
        self->bufsize++;
        if (self->bufsize == 64) {
            md5_digest(self);
            self->bitsize += 512;
            self->bufsize = 0;
        }
    }
}

void md5_hash (md5_t* self, uint32_t hash[4]) {

    size_t i = self->bufsize;

    self->buffer[i++] = 0x80; // append single bit to message
    while (i < 64) self->buffer[i++] = 0x00; // pad with zeros

    if (self->bufsize >= 55) { 
        md5_digest(self);
        for (i = 0; i < 64; ++i) self->buffer[i] = 0x00;
    }

    self->bitsize += self->bufsize * 8;
    self->buffer[56] = self->bitsize;
    self->buffer[57] = self->bitsize >> 8;
    self->buffer[58] = self->bitsize >> 16;
    self->buffer[59] = self->bitsize >> 24;
    self->buffer[60] = self->bitsize >> 32;
    self->buffer[61] = self->bitsize >> 40;
    self->buffer[62] = self->bitsize >> 48;
    self->buffer[63] = self->bitsize >> 56; 

    md5_digest(self);

    hash[0] = SWAP(self->rawhash[0]);
    hash[1] = SWAP(self->rawhash[1]);
    hash[2] = SWAP(self->rawhash[2]);
    hash[3] = SWAP(self->rawhash[3]);
}
// ===================


// *******************************
// Métodos Comparativos
// *******************************

#define START   0x21
#define END     0x7a

int compare (const uint32_t a[], const uint32_t b[]) {
    return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

int brute_force_fixed_length (const uint32_t hash[4], byte_t* result, byte_t* test, int p, int len) {

    static md5_t MD5;
    static uint32_t check[4];
    
    byte_t c;

    if (p < len-1) {
        for (c = START; c <= END; ++c) {
            test[p] = c;
            if (brute_force_fixed_length(hash, result, test, p + 1, len)) 
                return 1;
        }
    } else {
        for (c = START; c <= END; ++c) {
            test[p] = c;

            md5_init(&MD5);
            md5_update(&MD5, test, len);
            md5_hash(&MD5, check);

            if (compare(hash, check)) {
                strcpy(result, test);
                return 1;
            }
        }
    }

    return 0;
}

int brute_force (const uint32_t hash[4], byte_t* result, int maxlen) {
    
    byte_t str[maxlen+1];

    for (int i = 0; i < maxlen; i++)
        if (brute_force_fixed_length (hash, result, str, 0, i)) return 1;

    return 0;
}

// *******************************
// Main
// *******************************

int main (){
    // Iniciar contagem de tempo
	time_log_start();

    uint32_t hash[4];
    char hexstring[33] = {0};
    char result[11];
	int nThreads, tId;

    printf("Informe a hash a ser quebrada:  ");
    fgets(hexstring, 33, stdin);
    for (int i = 0; i < 4; i++) sscanf(&hexstring[i * 8], "%8x", &hash[i]);


    // // omp_set_num_threads();
    #pragma omp parallel private(tId)
    {
        tId = omp_get_thread_num();
        printf("Thread número %d\n", tId);

        if(tId == 0){
            nThreads = omp_get_num_threads();
            printf("Total de threads: %d\n", nThreads);
        }
    }

    // printf("Quebrando hash: %08x%08x%08x%08x...\n", hash[0], hash[1], hash[2], hash[3]);
    printf("Quebrando hash...\n");
    
    brute_force(hash, result, 10);

    printf("\n===================================\n");
    printf("Texto original:   \n%s\n", result);
    printf("===================================\n");

    // Mostrar tempo de execução
    printf("\n===================================\n");
    printf("Tempo de Execução:\n");
	time_log_stop();
    printf("===================================\n");

	// system("pause");
	return 0;
}

// 24b91372fa71abbdf7f69b88834cfaa7
// @T$1