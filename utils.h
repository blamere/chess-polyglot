#include <fstream>
#include <algorithm>
#include <cassert>
#include <string.h>
#include "constants.h"

#ifndef POLYGLOT_UTILS_H_INCLUDED
#define POLYGLOT_UTILS_H_INCLUDED
/** Reads length bytes into an integer
*   param file File pointer to the open book file descriptor
*   param length
*   param result Pointer to the integer into which to put the result
*/
int ReadIntFromFile(FILE *file, int length, uint64_t *r){
    int c;
    for (int i = 0;i < length;i++) {
        c = fgetc(file);
        if (c == EOF){
            return 1;
        }
        (*r) = ((*r) << 8) + c;
    }
    return 0;
}

/** Reads one entry from the book file
*   param File pointer to the open book file descriptor
*   param entry Pointer to the entry object into which to read the entry
*/
int ReadEntryFromFile(FILE *file, Entry *entry){
    uint64_t r;
    int ret = ReadIntFromFile(file, 8, &r);
    if(ret) return 1;

    entry->key = r;
    ret = ReadIntFromFile(file, 2, &r);
    if(ret) return 1;

    entry->move = r;
    ret = ReadIntFromFile(file, 2, &r);
    if(ret) return 1;

    entry->weight = r;
    ret = ReadIntFromFile(file, 4, &r);
    if(ret) return 1;

    entry->learn = r;
    return 0;
}

/** Finds the first move in a book file
*   param bookFile The book file path
*   param key A polyglot hash of the position
*/
int FindKey(FILE *file, uint64_t key) {
    fseek(file, 0, SEEK_END);
    int start = 0;
    int end = (int)ftell(file) / sizeof(Entry) - 1;
    int middle;
    Entry entry;

    assert(start <= end);

    fseek(file, 0, SEEK_SET);

    while (start < end) {
        middle = (start + end) / 2;
        assert(middle >= start && middle < end);
        fseek(file, middle * sizeof(Entry), SEEK_SET);
        ReadEntryFromFile(file, &entry);

        if (key <= entry.key) {
            end = middle;
        } else {
            start = middle + 1;
        }
    }

    assert(start == end);
    return start;
}

/** Converts the polyglot format move to a SAN move
*   param moveStr The char array into which the SAN move is copied
*   param move The polyglot format move
*/
void MoveToString(char moveStr[6], uint16_t move) {
    int from = (move >> 6) & 077;
    int fromRow = (from >> 3) & 0x7;
    int fromFile = from & 0x7;
    int to = move & 077;
    int toRow = (to >> 3) & 0x7;
    int toFile = to & 0x7;
    int promotion = (move >> 12) & 0x7;

    moveStr[0] = fromFile + 'a';
    moveStr[1] = fromRow + '1';
    moveStr[2] = toFile + 'a';
    moveStr[3] = toRow + '1';

    if (promotion) {
        moveStr[4] = promotionPieces[promotion];
        moveStr[5] = '\0';
    } else {
        moveStr[4] = '\0';
    }

    //Convert the castling moves to standard notation
    if (!strcmp(moveStr, "e1h1")) {
        strcpy(moveStr, "e1g1");
    } else if (!strcmp(moveStr, "e1a1")) {
        strcpy(moveStr, "e1c1");
    } else if (!strcmp(moveStr, "e8h8")) {
        strcpy(moveStr, "e8g8");
    } else if (!strcmp(moveStr, "e8a8")) {
        strcpy(moveStr, "e8c8");
    }

    if (!strcmp(moveStr, "a1a1")) {
        strcpy(moveStr, "");
    }
}

uint16_t FindInternal(uint64_t key, const char* bookFile, bool searchBest) {
    Entry entry;
    uint16_t bestWeight = 0;
    uint64_t totalWeight = 0;
    uint16_t move = 0;
    FILE *file = fopen(bookFile, "rb");
    if (!file) {
        return 0;
    }

    int offset = FindKey(file, key);
    fseek(file, offset * sizeof(Entry), SEEK_SET);

    while (true) {
        int ret = ReadEntryFromFile(file, &entry);
        if (ret) {
            break;
        }
        if (entry.key != key) {
            break;
        }

        bestWeight = std::max(bestWeight, entry.weight);
        totalWeight += entry.weight;

        if (searchBest) {
            if (entry.weight == bestWeight) {
                move = entry.move;
            }
        } else if (totalWeight && rkiss.rand<unsigned>() % totalWeight < entry.weight) {
            move = entry.move;
        }
    }

    return move;
}

#endif // #ifndef POLYGLOT_UTILS_H_INCLUDED
