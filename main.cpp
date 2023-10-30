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
            LENGTH = 200000, // не изменяется
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
int ErrorHandler(int, int, int);
void PrintError(int);

//FILES
const char* Files[] = {"output/BLS.txt", "output/SLS.txt", "output/OAS.txt", "output/BS.txt"};

int main(){
    int err = ErrorHandler(MIN_INT, MAX_INT, LENGTH);
    if (err != 0){
        PrintError(err); //обработка ошибок
    } 
    ClearAllFiles(FILES_QUANTITY); //открыли и очистили все файлы

    for(int len = 10000; len <= 200000; len = len + 19000){ //цикл по размерностям массива [10^4, ..., 2*10^5]
        RandomSequenceInt(Array, len); //генерируем рандом - массив для BLS, SLS

        for(int j = 0; j < FILES_QUANTITY; j++){ //цикл по всем функция сортировки
            int target = MAX_INT + 1; //искомое значение для BLS, SLS
            if (j == 2){
                AscendingSequenceInt(Array, len); //генерируем неубывающий массив для OAS, BS
            }
            FILE* file = fopen(Files[j], "a+"); //открываем нужный файл в режиме записи, сохраняя предыдущие записи
            int temp = target;
            if (j > 1){ 
                temp = Array[0]; //для SLS, BS целевое значение MAX_INT + 1 не подходит (из-за условия упорядоченности массива)
                target = Array[0]; //поэтому в разных случаях искомый элемент будет из [Arr[0], Arr[len/2], Arr[len-1]](первый, средний и последний)
            }

            int comparations = 0; //счетчик сравнений
            swap(Array[0], temp); //для каждой функции устанавливает выбранное заранее искомое значение из target в нужное(!) место в массиве
            auto begin = steady_clock::now(); //начало отсчета времени
            int res = Funcs[j](Array, len, target, comparations); //поиск элемента в массиве(вызов функции из массива)
            auto end = steady_clock::now(); //конец отсчета времени
            swap(Array[0], temp); //возвращаем массив в исходное состояние (искомый элемент удален)
            auto duration = duration_cast<microseconds>(end - begin); //вычисление длительности
            PrintResFile(res, len, duration.count(), comparations, file); //печать результата, длины массива, времени, числа сравнений в файл
            // //для OAS, BS в файлах значения comparation по умолчанию 0

            //устанавливаем искомый элемент для OAS, BS
            if (j > 1){
                temp = Array[len/2];
                target = Array[len/2];
            }

            //далее все по аналогии с первым вызовом
            //всего вызова 3, по одному для каждого положения искомого элемента в массиве 
            comparations = 0; 
            swap(Array[len/2], temp);
            begin = steady_clock::now();
            res = Funcs[j](Array, len, target, comparations);
            end = steady_clock::now();
            swap(Array[len/2], temp);
            duration = duration_cast<microseconds>(end - begin);
            PrintResFile(res, len, duration.count(), comparations, file);

            if (j > 1){
                temp = Array[len-1];
                target = Array[len-1];
            }
            comparations = 0;
            swap(Array[len-1], temp);
            begin = steady_clock::now();
            res = Funcs[j](Array, len, target, comparations);
            end = steady_clock::now();
            swap(Array[len-1], temp);
            duration = duration_cast<microseconds>(end - begin);
            PrintResFile(res, len, duration.count(), comparations, file);
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
    comparations++; //последнее сравнение в цикле
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

    Array[lastInd] = last; //восстановили исходный массив
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
    Array[lastInd] = last; //восстановили исходный массив
    if(Array[i] == target){
        return i;
    }
    return -1;
}

int BinarySearch(int Array[], int len, int target, int &comparations){
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

//print result to file
void PrintResFile(int res, int len, int64_t time, int comparations, FILE *file){
    fprintf(file, "ResIndex: %d\tLen: %d\tTime: %d\tComps: %d\n", res, len, time, comparations);
}

//error handler
int ErrorHandler(int minInt, int maxInt, int length){
    if (maxInt <= minInt) return 1;
    if (length < 1) return 2;
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
            printf("LENGTH must be greater then 0. error code: %d\n", error);
            break;
    }
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