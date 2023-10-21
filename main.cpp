#include <iostream>
#include <cstring>
#include <windows.h>
#include <chrono>
using namespace std;

//CONSTANTS
const int   MIN_INT = -100,
            MAX_INT = 100,
            LENGTH = 200;

//SEARCHING
int BetterLinearSearch(int[], int, int, int);
int SentinelLinearSearch(int[], int, int, int);
int OrderedArraySearch(int[], int, int, int);
int BinarySearch(int[], int, int, int);

//VARIABLES
int Array[LENGTH];

//GENERATING INTEGER SEQUENCES
void AscendingSequenceInt(int[], int);
void RandomSequenceInt(int[], int);

//PRINTING SEQUENCES
void PrintArrConsoleInt(int[], int); 
void PrintArrFileInt(int[], int, FILE*, int64_t);

//POINTERS INTEGER FUNCTIONS
void (*funcInt1)(int[], int) = AscendingSequenceInt;
void (*funcInt3)(int[], int) = RandomSequenceInt;

//ERROR HANDLERS
int ErrorHandler(int, int, double, double, int, int);
void PrintError(int);

//FILES

int main(){
    int Array[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    cout << BinarySearch(Array, 2, 0, 9);
}

//searching
int BinarySearch(int Array[], int target, int left, int right){
    int answer = -1;
    while(left < right){
        int m = (left + right) / 2;
        if (Array[m] < target){
            left = m + 1;
        } else{
            right = m;
        }
    }
    if (Array[left] == target){
        answer = left;
    } 
    return answer;
}

//error handler
int ErrorHandler(int minInt, int maxInt, double minDouble, double maxDouble, int length, int intervalLength){
    if (maxInt <= minInt) return 1;
    if (maxDouble <= minDouble) return 2;
    if (length < 1) return 3;
    if (length < intervalLength) return 4;
    if (intervalLength < 1) return 5;
    return 0;
}

//print an error
void PrintError(int error){
    printf("ERROR: ");
    switch (error){
        case 1:
            printf("MIN_INT must be less then MAX_INT. error code: %d\n", error);
            break;
        case 2:
            printf("MIN_DOUBLE must be less then MAX_DOUBLE. error code: %d\n", error);
            break;
        case 3:
            printf("LENGTH must be greater then 0. error code: %d\n", error);
            break;
        case 4:
            printf("INTERVAL_LENGTH must be less then LENGTH. error code: %d\n", error);
            break;
        case 5:
            printf("INTERVAL_LENGTH must be greater then 0. error code: %d\n", error);
            break;
    }
}


//print int array to console
void PrintArrConsoleInt(int Array[], int len){
    if (len >= 100000){
        return;
    }
    for (int i = 0; i < len; i++){
        char end[3];
        if (i != len - 1){
            strcpy(end, ", ");
        } else{
            strcpy(end, "]\n");
        }
        printf("%d%s", Array[i], end);
    }
}

//print int array to file
void PrintArrFileInt(int Array[], int len, FILE *file, int64_t time){
    fprintf(file, "Время работы алгоритма по формированию последовательности: %d(мкС)\n", time);
    if (len >= 100000){
        return;
    }
    for (int i = 0; i < len; i++){
        fprintf(file, "%d\n", Array[i]);
    }
    fprintf(file, "\n");
}

//generates an ascending integer sequence
void AscendingSequenceInt(int Array[], int len){
    double k = ((MAX_INT - MIN_INT) * 1.0) / (len * 1.0), b = MIN_INT;
    for (int i = 0; i < len; i++){
        Array[i] = k * i + b;
    }
}

//generates a random integer sequence
void RandomSequenceInt(int Array[], int len){
    srand(time(NULL));
    for (int i = 0; i < len; i++){
        Array[i] = MIN_INT + rand() % (MAX_INT - MIN_INT + 1);
    }
}