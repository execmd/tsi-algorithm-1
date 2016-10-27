#include <iostream>
#include <iomanip>

using namespace std;

const bool IN_TEST = false; // If true, you need to provide interaction with program
const bool SEARCHABLE_EXISTS = 0; // 0 - random, 1 - searchable exists
const unsigned int TEST_COUNT = 10; // Count of tests
const unsigned int TEST_STEP = 100; // Increment array size each test by this amount

const unsigned int RANDOM_MIN = 100;
const unsigned int RANDOM_MAX = 1000;

const unsigned int ARRAY_SIZE = 100;

const unsigned int HASH_SORT_EMPTY = -1;


bool askGenerateSearchKey();

/*
 * Prints array content
 *
 * @param arr[] Array pointer to modify
 * @param cols Array length
 * @return void
 */
void printArray(unsigned int arr[], unsigned int size);

/*
 * Hash function which takes element and array size to calculate index
 *   and summ with attempt
 */
int hashKey(unsigned int key, unsigned int size, unsigned int attempt);

int getKey();

unsigned int searchByIteration(unsigned int *oldArr, unsigned int size, unsigned int key);

unsigned int searchByHash(unsigned int *oldArr, unsigned int size, unsigned int key);

/*
 * Returns a random Real number between range
 *
 * @param min A left-side range border
 * @param max A right-side range border
 * @return Return random float number
 */
int getRandom(unsigned int min, unsigned int max);

int main() {
    srand((unsigned) time(NULL));

    unsigned int key;

    if (IN_TEST) {
        unsigned int arrSize = ARRAY_SIZE;

        unsigned int comparisonBarrier = 0;
        unsigned int comparisonHash = 0;

        for (unsigned int i = 0; i < TEST_COUNT; i++) {
            unsigned int *arr = new unsigned int [arrSize];

            for (unsigned int y = 0; y < arrSize; y++) {
                arr[y] = (unsigned int) getRandom(RANDOM_MIN, RANDOM_MAX);
            }

            if (SEARCHABLE_EXISTS) {
                key = arr[arrSize / 4];
            } else {
                key = getRandom(RANDOM_MIN, RANDOM_MAX);
            }

            printArray(arr, arrSize);
            cout << "Key: " << key << endl;

            comparisonBarrier += searchByIteration(arr, arrSize, key);
            comparisonHash += searchByHash(arr, arrSize, key);

            arrSize += TEST_STEP;
        }

        cout << "Average comparison count for iteration: " << (comparisonBarrier / TEST_COUNT) << endl;
        cout << "Average comparison count for hashfunct: " << (comparisonHash    / TEST_COUNT) << endl;

    } else {
        // Create array with provided sizes
        unsigned int *arr = new unsigned int [ARRAY_SIZE];

        for (unsigned int y = 0; y < ARRAY_SIZE; y++) {
            arr[y] = getRandom(RANDOM_MIN, RANDOM_MAX);
        }

        // Display our array
        cout << "Array: " << endl;
        printArray(arr, ARRAY_SIZE);

        if(IN_TEST || askGenerateSearchKey()) {
            key = getRandom(RANDOM_MIN, RANDOM_MAX);
        } else {
            key = getKey();
        }

        cout << "Search key is: " << key << endl;

        unsigned int comparisonCount;

        comparisonCount = searchByIteration(arr, ARRAY_SIZE, key);
        cout << "Comparison count by iteration with barrier: " << comparisonCount << endl;

        comparisonCount = searchByHash(arr, ARRAY_SIZE, key);
        cout << "Comparison count by hash function: " << comparisonCount << endl;
    }

    return 0;
}

bool askGenerateSearchKey() {
    bool receivedAnswer, generate;
    char ch;
    do {
        cout << "Generate search key? (y/n) [y]: \n";

        if (cin.peek() == '\n') {
            ch = 'y';
        } else {
            cin.get(ch);
        }

        cin.clear();
        cin.ignore();

        switch (ch) {
            case 'Y':
            case 'y':
                receivedAnswer = true;
                generate = true;
                break;
            case 'N':
            case 'n':
                receivedAnswer = true;
                generate = false;
                break;
            default:
                receivedAnswer = false;
        }
    } while (!receivedAnswer);

    return generate;
}

void printArray(unsigned int arr[], unsigned int size) {
    for (int y = 0; y < size; y++) {
        if (y % 10 == 0) {
            cout << endl;
        }
        cout << setw(5) << arr[y];
    }
    cout << endl;
}

int hashKey(unsigned int key, unsigned int size, unsigned int attempt) {
    int hk = key % size + attempt;

    if (hk > size) {
        hk = hk - size;
    }

    return hk;
}

int getKey() {
    int key;
    cout << "Please enter key:\n";
    cin >> key;

    return key;
}

int getRandom(unsigned int min, unsigned int max) {
    return min + (rand() % (max - min + 1));
}

unsigned int searchByIteration(unsigned int *oldArr, unsigned int size, unsigned int key) {
    // Create array with one more element for barrier
    unsigned int *arr = new unsigned  int [size + 1], comparisonCount = 0;

    // Prepare array. Append with searchable
    for (unsigned int y = 0; y < size; y++) {
        arr[y] = oldArr[y];
    }

    arr[size] = key;

    // Search
    unsigned short i = 0;
    while (arr[i] != key) {
        comparisonCount++;
        i++;
    }

    comparisonCount++; // Also count the comparison below
    if (i != size) {
        cout << "Key found! Position: " << i << endl;
    } else {
        cout << "Key not found" << endl;
    }

    return comparisonCount;
    cout << "Comparison count " << comparisonCount << endl;
}

unsigned int searchByHash(unsigned int *oldArr, unsigned int oldSize, unsigned int key) {
    unsigned int size = oldSize + 1,
        *arr = new unsigned  int [size],// We need extra element
        comparisonCount = 0;

    // Set all elements value to HASH_SORT_EMPTY as empty value
    for (unsigned int i = 0; i < size; i++) {
        arr[i] = HASH_SORT_EMPTY;
    }

    // Sort array by hash fn
    for (unsigned int i = 0; i < oldSize; i++) {
        unsigned int attempt = 0, hashK, el;

        do {
            hashK = hashKey(oldArr[i], oldSize, attempt++);
            el = arr[hashK];
        } while (!(el == HASH_SORT_EMPTY || el == oldArr[i]));

        if (el == HASH_SORT_EMPTY) {
            arr[hashK] = oldArr[i];
        }
    }

    if (!IN_TEST) {
        cout << "Sorted array: " << endl;
        printArray(arr, size);
    }

    // Perform search
    int attempt = 0, hashK, el;
    do {
        hashK = hashKey(key, oldSize, attempt++);
        el = arr[hashK];
        comparisonCount += 2;
    } while (!(el == HASH_SORT_EMPTY || el == key));

    if (el != HASH_SORT_EMPTY) {
        cout << "Key found! Position: " << hashK << endl;
    } else {
        cout << "Key not found" << endl;
    }

    return comparisonCount;
}