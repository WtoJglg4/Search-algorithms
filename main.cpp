#include <iostream>
#include <cstring>
#include <windows.h>
#include <chrono>
#include <limits.h>
using namespace std;
using namespace chrono;

//CONSTANTS
const int   MIN_INT = -1000,
            MAX_INT = 1000,
            LENGTH = 200000, 
            FILES_QUANTITY = 4;

//SEARCHING
int BetterLinearSearch(int[], int, int, int&);
int SentinelLinearSearch(int[], int, int, int&);
int OrderedArraySearch(int[], int, int, int&);
int BinarySearch(int[], int, int, int&);
int(*Funcs[])(int[], int, int, int&){BetterLinearSearch, SentinelLinearSearch, OrderedArraySearch, BinarySearch};

//VARIABLES
int Array[LENGTH];

//GENERATING INTEGER SEQUENCES
void AscendingSequenceInt(int[], int);
void RandomSequenceInt(int[], int);

//PRINTING RESULTS
void ClearAllFiles(int);
void PrintResFile(int, int, int64_t, int, FILE*);

//ERROR HANDLERS
int ErrorHandler(int, int, double, double, int, int);
void PrintError(int);

//FILES
const char* Files[] = {"output/BLS.txt", "output/SLS.txt", "output/OAS.txt", "output/BS.txt"};

int main(){
    ClearAllFiles(FILES_QUANTITY);

    int target = MAX_INT + 1;
    for(int len = 10000; len <= 200000; len = len + 19000){
        RandomSequenceInt(Array, len);
        for(int j = 0; j < 4; j++){
            if (j == 2){
                AscendingSequenceInt(Array, len);
            }
            FILE* file = fopen(Files[j], "a+");
            int temp = target;
            
            int comparations = -1;
            swap(Array[0], temp);
            auto begin = steady_clock::now();
            int res = Funcs[j](Array, len, target, comparations);
            auto end = steady_clock::now();
            swap(Array[0], temp);
            auto duration = duration_cast<microseconds>(end - begin);
            PrintResFile(res, len, duration.count(), 0, file);
            
            comparations = -1;
            swap(Array[len/2], temp);
            begin = steady_clock::now();
            res = Funcs[j](Array, len, target, comparations);
            end = steady_clock::now();
            swap(Array[len/2], temp);
            duration = duration_cast<microseconds>(end - begin);
            PrintResFile(res, len, duration.count(), 0, file);

            comparations = -1;
            swap(Array[len-1], temp);
            begin = steady_clock::now();
            res = Funcs[j](Array, len, target, comparations);
            end = steady_clock::now();
            swap(Array[len-1], temp);
            duration = duration_cast<microseconds>(end - begin);
            PrintResFile(res, len, duration.count(), 0, file);
            fprintf(file, "\n");


            fclose(file);
        }
        
    }
}

//searching
int BetterLinearSearch(int Array[], int len, int target, int &comparations){
    for (int i = 0; i < len; i++){
        comparations += 2;
        if (Array[i] == target){
            return i;
        }
    }
    return -1;
}

int SentinelLinearSearch(int Array[], int len, int target, int &comparations){
    int lastInd = len - 1, last = Array[lastInd], i = 0;
    Array[lastInd] = target;
    while (Array[i] != target){
        comparations++;
        i++;
    }
    comparations += 2; //последнее сравнение, когда в цикл не заходим + сравнение в if далее

    Array[lastInd] = last;
    if (i < lastInd || Array[lastInd] == target){
        return i;
    }
    return -1;
}

int OrderedArraySearch(int Array[], int len, int target, int &comparations){
    int lastInd = len - 1, last = Array[lastInd], i = 0;
    Array[lastInd] = INT32_MAX;
    while (target > Array[i]){
        i++;
    }
    Array[lastInd] = last;
    if(Array[i] == target){
        return i;
    }
    return -1;
}

int BinarySearch(int Array[], int target, int len, int &comparations){
    int answer = -1, left = 0, right = len - 1;
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

//print result to file
void PrintResFile(int res, int len, int64_t time, int comparations, FILE *file){
    fprintf(file, "%d %d %d %d\n", res, len, time, comparations);
}

//clear all files
void ClearAllFiles(int len){
    for (int i = 0; i < len; i++){
        FILE *file = fopen(Files[i], "w");
        fclose(file);
    }
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