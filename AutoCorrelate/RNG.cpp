// AutoCorrelate.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <signal.h>

/* windowed autocorrelation */
#define MAXLAG 16
int window[MAXLAG] = { 1, 2, 3, 4, 5, 6, 7, 8 };
int correlation[MAXLAG];

void printAuto(int* bits, int size, int maxj)
{
    int x, j, sum;
    for (j = 1; j <= maxj; j++) {
        for (x = j, sum = 0; x < size; x++) {
            sum += (bits[x] ^ bits[x - j]);
        }
        printf("Lag[%4d] = %5d (expected %5d)\n", j, sum, (size - j) / 2);
    }
}

void simpleBiasedPRNG(int* bits, int size)
{
    int l = 0;
    int x;
    for (x = 0; x < size; x++) {
        if (rand() & 1) {
            bits[x] = l;
        }
        else {
            l = bits[x] = rand() & 1;
        }
        printf("Lag[%4d] = %5d (expected %5d)\n", x + 1, bits[x], (size - x) / 2);
    }
}

void wincor_add_bit(int bit)
{
    int x;
    /* compute lags */
    for (x = 0; x < MAXLAG; x++) {
        correlation[x] += window[x] ^ bit;
    }
        /* shift */
    for (x = 0; x < MAXLAG - 1; x++) {
        window[x] = window[x + 1];
    }
    window[MAXLAG - 1] = bit;

    for (x = 0; x < MAXLAG; x++) {
        printf("Lag[%4d] = %5d (expected %5d)\n", x + 1, window[x], (MAXLAG - x) / 2);
    }
}

volatile int x, quit, capture;
void sighandle(int signo) { 
    capture = x;
    quit = 1;
}

int main()
{
    std::cout << "Hello World!\n";
    int* bits = new int[8]{ 1, 2, 1, 3, 1, 4, 1, 0 };

    printAuto(bits, 8, 8);

    printf("\n");

    simpleBiasedPRNG(bits, 8);

    printf("\n");

    wincor_add_bit(8);

    int y;
    //signal(SIGALRM, sighandle);
    for (y = 0; y < 16; y++) {
        quit = 0;
        //alarm(1);
        while (!quit){
            x ^= 1;
        }
        printf("%d", capture);
        fflush(stdout);
    }
    printf("\n");
    return 0;

    system("pause");
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
