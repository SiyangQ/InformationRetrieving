/* main.c
*
* Created by Siyang Qiu (siyangq@student.unimelb.edu.au) 17/Aug./2020
*
* A dictionary to store business records from csv files using linked list. 
* Output the records which matched the key specified in the command line 
  arguments.
* Lastly, the ability to type in keys to search for in stdin
*
* Optional functionality to print search results to stdout with a slight 
  modification of codes. 
*
* To run the program, type:
./dict inputFileName.csv outputFileName.txt lookupKey1 lookupKey2 ...
*
*/

#include "list.h"

#define TRUE 1
#define FALSE 0
#define NUM_ARGC 3
#define MAX_STRING_LENGTH 128
#define MAX_LINE_LENGTH 512
#define DATA_FILE_NAME 1
#define OUTPUT_FILE_NAME 2
#define ONE_CHAR 1
#define FIRST_ELEMENT 0
#define FIRST_CHAR 0
#define ONE 1

/* function declarations */
void *readCSVLine(char *inputLine);
void *readCSVField(char *input, int *index);
void endStringAtNewLine(char *str);


int main(int argc, char *argv[]) {

    /* check CLA count is 3 as expected*/
    assert(argc == NUM_ARGC);

    /* create new linked list */
    list_t *list = newList();
    
    /* get file name from CLA and read file */
    FILE *filePtr;
    filePtr = fopen(argv[DATA_FILE_NAME], "r");
    assert(filePtr);

    char lineBuffer[MAX_LINE_LENGTH];

    /* skip first line */
    fgets(lineBuffer, MAX_LINE_LENGTH, filePtr);

    /* read data into dictionary */
    while (fgets(lineBuffer, MAX_LINE_LENGTH, filePtr)) {
        listAppend(list, readCSVLine(lineBuffer));
    }

    /* close data file */
    fclose(filePtr);
    
    /* open output file for writing */
    filePtr = fopen(argv[OUTPUT_FILE_NAME], "w");
    assert(filePtr);

    char key[MAX_STRING_LENGTH];

    /* read key from stdin and search in dictionary for each key */
    fgets(key, MAX_STRING_LENGTH, stdin);
    endStringAtNewLine(key);
    
    while (strlen(key) > ONE_CHAR) {
        searchKey(list, key, filePtr);
        key[FIRST_ELEMENT] = '\0';

        fgets(key, MAX_STRING_LENGTH, stdin);
        endStringAtNewLine(key);
    }

    /* close file */
    fclose(filePtr);

    /* free dictionary */
    freeList(list);
}

// take out \n character for fgets
void endStringAtNewLine(char *str) {
    int i = FIRST_CHAR;
    while (str[i] != '\0') {
        if (str[i] == '\n' || str[i] == '\r') {
            str[i] = '\0';
            return;
        }
        i++;
    }
}

// read a line of csv file and store in a record type struct
void *readCSVLine(char *inputLine) {
    // create space for a new record
    record_t *newRecord = (record_t *)malloc(sizeof(record_t));
    assert(newRecord);
    int lineIndex = FIRST_CHAR;

    // read each field of record and store in struct
    newRecord->censusYear = readCSVField(inputLine, &lineIndex);
    newRecord->blockID = readCSVField(inputLine, &lineIndex);
    newRecord->propertyID = readCSVField(inputLine, &lineIndex);
    newRecord->basePropertyID = readCSVField(inputLine, &lineIndex);
    newRecord->CLUESmallArea = readCSVField(inputLine, &lineIndex);
    newRecord->tradingName = readCSVField(inputLine, &lineIndex);
    newRecord->industryCode = readCSVField(inputLine, &lineIndex);
    newRecord->industryDescription = readCSVField(inputLine, &lineIndex);
    newRecord->xCoordinate = readCSVField(inputLine, &lineIndex);
    newRecord->yCoordinate = readCSVField(inputLine, &lineIndex);
    newRecord->location = readCSVField(inputLine, &lineIndex);

    return newRecord;
}

// read a , separated field of csv field
void *readCSVField(char *input, int *index) {

    int i = FIRST_CHAR, previousCharIsQuote = FALSE, isQuoted = FALSE;
    char outputBuffer[MAX_STRING_LENGTH];

    // check if quoted string
    if (input[*index] == '"') {
        (*index)++;
        isQuoted = TRUE;
    }

    // copy character to buffer until end of field
    while (input[*index] != '\r' && input[*index] != '\0' && 
        input[*index] != '\n') {
            
        if (input[*index] == ',' && !isQuoted) {
            (*index)++;
            break;
        } else if (input[*index] == '"') {
            if (!previousCharIsQuote) {
                (*index)++;
                previousCharIsQuote = TRUE;
                isQuoted = FALSE;
                continue;
            } else {
                previousCharIsQuote = FALSE;
                isQuoted = TRUE;
            }
        }
        outputBuffer[i++] = input[*index];
        (*index)++;
    }
    outputBuffer[i] = '\0';
    
    // copy string from buffer to a new suitable sized location
    char *newStr = (char *)malloc(sizeof(char)*strlen(outputBuffer)+ONE);
    assert(newStr);
    strcpy(newStr, outputBuffer);

    return newStr;
}

// print a record to stdout
void printOutputToStdout(record_t *record) {

    printf("%s --> Census year: %s || Block ID: %s || Property ID: %s || "
        "Base property ID: %s || CLUE small area: %s || "
        "Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || "
        "x coordinate: %s || y coordinate: %s || Location: %s || \n", 
        record->tradingName, record->censusYear, record->blockID,
        record->propertyID, record->basePropertyID, record->CLUESmallArea,
        record->industryCode, record->industryDescription, record->xCoordinate,
        record->yCoordinate, record->location);
}

// print a record to file
void printOutputToFile(record_t *record, FILE *filePtr) {

    fprintf(filePtr, "%s --> Census year: %s || Block ID: %s || "
        "Property ID: %s || Base property ID: %s || CLUE small area: %s || "
        "Industry (ANZSIC4) code: %s || Industry (ANZSIC4) description: %s || "
        "x coordinate: %s || y coordinate: %s || Location: %s || \n", 
        record->tradingName, record->censusYear, record->blockID,
        record->propertyID, record->basePropertyID, record->CLUESmallArea,
        record->industryCode, record->industryDescription, record->xCoordinate,
        record->yCoordinate, record->location);
}

// print a not found to file
void printNotFoundToFile(char *key, FILE *filePtr) {
    fprintf(filePtr, "%s --> NOTFOUND \n", key);
}

// print a not found to stdout
void printNotFoundToStdOut(char *key) {
    printf("%s --> NOTFOUND \n", key);
}

// free all data in a record type struct
void deleteRecord(record_t *rec) {
    free(rec->censusYear);
    free(rec->blockID);
    free(rec->propertyID);
    free(rec->basePropertyID);
    free(rec->CLUESmallArea);
    free(rec->tradingName);
    free(rec->industryCode);
    free(rec->industryDescription);
    free(rec->xCoordinate);
    free(rec->yCoordinate);
    free(rec->location);
}

// check if record match the key, if match, print to file or stdout
int checkMatchKey(record_t *rec, char* key, FILE *filePtr) {
    if (!strcmp(rec->tradingName, key)) {
        if (filePtr) printOutputToFile(rec, filePtr);
        else printOutputToStdout(rec);
        return 1;
    }
    return 0;
}