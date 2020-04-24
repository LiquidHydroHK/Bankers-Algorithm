#pragma once
#ifndef BANKERSALGORITHM_H
#define BANKERSALGOROTHM_H
#endif

#include <string>

class Process {
private:
    int pID;
    unsigned int resourceNum;
    int* allocation;
    int* max;
    int* need;
    bool executed;

public:
    Process(int pID, unsigned int rNum, int alloc[], int max[]);

    void computeNeed();
    void changeExecuted();

    int getProcessID() const;
    unsigned int getrNum() const;
    int* getNeed() const;
    void getAlloc(int*& emptyAlloc) const;
    void getMax(int*& emptyMax) const;
    bool getExecuted() const;
};
