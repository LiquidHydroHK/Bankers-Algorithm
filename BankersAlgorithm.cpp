// BankersAlgorithm.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include "BankersAlgorithm.h"


Process::Process(int pID, unsigned int rNum, int alloc[], int max[]) :
	pID(pID), resourceNum(rNum) {
	this->allocation = new int[resourceNum];
	this->max = new int[resourceNum];
	this->need = new int[resourceNum];
	this->executed = false;
	for (unsigned int i = 0; i < resourceNum; i++) {
		this->allocation[i] = alloc[i];
		this->max[i] = max[i];
	}
}

int Process::getProcessID() const { return this->pID; }

unsigned int Process::getrNum() const { return this->resourceNum; }

void Process::getAlloc(int*& emptyAlloc) const {
	emptyAlloc = this->allocation;
}

void Process::getMax(int*& emptyMax) const {
	emptyMax = this->max;
}

void Process::computeNeed() {
	for (unsigned int i = 0; i < this->resourceNum; i++) {
		need[i] = max[i] - allocation[i];
	}
}

void Process::changeExecuted() {
	executed = true;
}

bool Process::getExecuted() const {
	return this->executed;
}

int* Process::getNeed() const {
	return this->need;
}

typedef std::vector<Process> process_list;
typedef std::vector<Process> safeSequence;

void insertProcess(process_list &pList, Process &p) {
	pList.push_back(p);
}

bool checkNeedSmallerThanWork(Process &p, int work[]) {
	int* need = p.getNeed();
	for (unsigned int i = 0; i < p.getrNum(); i++) {
		if (need[i] > work[i]) {
			return false;
		}
	}
	return true;
}

void printExecSequence(safeSequence& seq) {
	std::cout << "< ";
	for (unsigned int i = 0; i < seq.size(); i++) {
		if (i == seq.size() - 1) {
			std::cout << "P" << seq[i].getProcessID();
		}
		else {
			std::cout << "P" << seq[i].getProcessID() << ", ";
		}
	}
	std::cout << " >" << std::endl;
}

void computeSafeSequence(safeSequence& seq, process_list& pList, int available[]) {
	int* work = new int[pList[0].getrNum()];
	for (unsigned int i = 0; i < pList[0].getrNum(); i++) {
		work[i] = available[i];
	}
	// All work should have the same dimension
	for (unsigned int i = 0; i < pList.size(); i++) {
		pList[i].computeNeed();
	}

	for (unsigned int i = 0; i < pList.size(); i++) {
		for (unsigned int j = 0; j < pList.size(); j++) {
			if (!(pList[j].getExecuted()) && checkNeedSmallerThanWork(pList[j], work)) {
				pList[j].changeExecuted(); // Finished working
				int* emptyAlloc;
				pList[j].getAlloc(emptyAlloc);
				for (unsigned int k = 0; k < pList[j].getrNum(); k++) {
					work[k] += emptyAlloc[k];
				}
				seq.push_back(pList[j]);
				break;
			}
		}
	}
	if (seq.size() != pList.size()) {
		std::cout << std::endl;
		std::cout << "The system is not in a safe state." << std::endl;
		std::cout << "Expected " << pList.size() << ", " << seq.size() << " achieved." << std::endl;
		printExecSequence(seq);
		std::cout << std::endl;
		std::cout << "Disclaimer: This algorithm can only calculate POTENTIAL execution sequence that is safe." << std::endl;
		std::cout << "            The execution may not be UNIQUE. There might exist another possible execution sequence that is safe." << std::endl;
		std::cout << "            Lower PID in this program has higher execution priority." << std::endl;
	}
	else {
		std::cout << std::endl;
		std::cout << "The system is in a safe state." << std::endl;
		std::cout << "Expected " << pList.size() << ", " << seq.size() << " achieved." << std::endl;
		printExecSequence(seq);
		std::cout << std::endl;
		std::cout << "Disclaimer: This algorithm can only calculate POTENTIAL execution sequence that is safe." << std::endl;
		std::cout << "            The execution may not be UNIQUE. There might exist another possible execution sequence that is safe." << std::endl;
		std::cout << "            Lower PID in this program has higher execution priority." << std::endl;
	}
}

int main()
{
	// Declaration of local variable
	int rNum, pNum;
	process_list pList;
	safeSequence seq;
	std::cout << "===================================================" << std::endl;
	std::cout << "Banker algorithm: Safe Execution Sequence Finder" << std::endl;
	std::cout << "===================================================" << std::endl;
	std::cout << "Please enter the number of resource(s) available: ";
	std::cin >> rNum;
	std::cout << "How many processes are there? ";
	std::cin >> pNum;
	std::cout << "===================================================" << std::endl;
	int* available = new int[rNum];
	int** processesAlloc = new int* [pNum];
	int** processesMax = new int* [pNum];
	for (int i = 0; i < pNum; i++) {
		processesAlloc[i] = new int[rNum];
		processesMax[i] = new int[rNum];
	}
	for (int i = 0; i < pNum; i++) {
		std::cout << "Process " << i << ":" << std::endl;
		for (int j = 0; j < rNum; j++) {
			int alloc;
			std::cout << "Allocation " << j << ": ";
			std::cin >> alloc;
			processesAlloc[i][j] = alloc;
		}
		std::cout << "===================================================" << std::endl;
		for (int j = 0; j < rNum; j++) {
			int alloc;
			std::cout << "Max " << j << ": ";
			std::cin >> alloc;
			processesMax[i][j] = alloc;
		}
		std::cout << "===================================================" << std::endl;
	}
	std::cout << "Enter available at Snapshot S_0: " << std::endl;
	for (int i = 0; i < rNum; i++) {
		int avail;
		std::cout << "Available " << i << ": ";
		std::cin >> avail;
		available[i] = avail;
	}
	std::cout << "===================================================" << std::endl;
	std::cout << "Creating processes..." << std::endl;
	std::cout << "===================================================" << std::endl;

	for (int i = 0; i < pNum; i++) {
		Process p(i, rNum, processesAlloc[i], processesMax[i]);
		insertProcess(pList, p);
	}

	computeSafeSequence(seq, pList, available);

	return 0;
}
