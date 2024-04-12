#include <stdio.h>
#include <stdlib.h>
#include <ctype.h> 

// RecordType
struct RecordType
{
    int id;
    char name;
    int order;
    struct RecordType *next;
};

// HashType
#define HASH_SIZE 23

struct HashType
{
    struct RecordType *head;
};

// Function prototypes
int parseData(char* inputFileName, struct RecordType** ppData);
void printRecords(struct RecordType pData[], int dataSz);
int hash(int x, int hashSize);
void insertRecord(struct HashType hashTable[], struct RecordType *record);
void displayRecordsInHash(struct HashType hashTable[], int hashSize);

// Compute the hash function
int hash(int x, int hashSize)
{
    // Modified hash function to distribute records evenly across the available indices
    return x % hashSize;
}

// Function to insert a record into the hash table
void insertRecord(struct HashType hashTable[], struct RecordType *record)
{
    int index = hash(record->id, HASH_SIZE); // Calculate the index using the hash function
    struct RecordType *current = hashTable[index].head;
    struct RecordType *prev = NULL;

    // Traverse the list to find the correct position to insert the record
    while (current != NULL && current->id < record->id) {
        prev = current;
        current = current->next;
    }

    // Insert the record into the list
    if (prev == NULL) {
        // If the record should be the new head
        record->next = hashTable[index].head;
        hashTable[index].head = record;
    } else {
        // Insert between prev and current
        prev->next = record;
        record->next = current;
    }
}

// Function to display records in the hash structure
void displayRecordsInHash(struct HashType hashTable[], int hashSize)
{
    printf("Records in Hash Table:\n");
    for (int i = 0; i < hashSize; ++i)
    {
        struct RecordType *current = hashTable[i].head;
        // Bubble sort to sort records within each index
        while (current != NULL)
        {
            struct RecordType *next = current->next;
            while (next != NULL)
            {
                // Compare IDs to determine ordering
                if ((islower(current->name) && isupper(next->name)) || // Capital letters before lowercase
                    (current->name == next->name && current->id < next->id)) // Larger IDs first
                {
                    // Swap records if the conditions are met
                    int tempId = current->id;
                    char tempName = current->name;
                    int tempOrder = current->order;
                    current->id = next->id;
                    current->name = next->name;
                    current->order = next->order;
                    next->id = tempId;
                    next->name = tempName;
                    next->order = tempOrder;
                }
                next = next->next;
            }
            current = current->next;
        }
    }

    // Print sorted records
    for (int i = 0; i < hashSize; ++i)
    {
        struct RecordType *current = hashTable[i].head;
        if (current != NULL)
        {
            printf("Index %d -> ", i);
            while (current != NULL)
            {
                printf("%d, %c, %d -> ", current->id, current->name, current->order);
                current = current->next;
            }
            printf("\n");
        }
    }
}

// Function to parse input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
            pRecord->next = NULL; // Initialize next pointer to NULL
        }

        fclose(inFile);
    }

    return dataSz;
}

// Function to print the records
void printRecords(struct RecordType pData[], int dataSz)
{
    printf("\nRecords:\n");
    for (int i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

int main(void)
{
    struct RecordType *pRecords;
    int recordSz = 0;

    // Parse data from file
    recordSz = parseData("input_lab_9.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Create a hash table
    struct HashType hashTable[HASH_SIZE] = {0}; // Initialize all pointers to NULL

    // Insert records into the hash table
    for (int i = 0; i < recordSz; ++i)
    {
        insertRecord(hashTable, &pRecords[i]);
    }

    // Display records in the hash table
    displayRecordsInHash(hashTable, HASH_SIZE);

    // Free dynamically allocated memory
    free(pRecords);

    return 0;
}
