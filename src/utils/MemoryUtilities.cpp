//
// Created by DELL on 2025/2/6.
//

#include "MemoryUtilities.h"
#include <QDebug>

MemoryUtilities::MemoryUtilities(DWORD pid, int cycle): pid(pid), maxMemory(0) {
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MemoryUtilities::checkMemoryUsage);
    timer->start(cycle);
}

float MemoryUtilities::getCurrentMemory() {return curMemory;}
float MemoryUtilities::getMaxMemory() {return maxMemory;}

void MemoryUtilities::checkMemoryUsage() {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (hProcess != NULL) {
        PROCESS_MEMORY_COUNTERS pmc;
        if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
            curMemory = pmc.WorkingSetSize / 1024.0 / 1024.0;
            if(curMemory > maxMemory)
                maxMemory = curMemory;
        }
        CloseHandle(hProcess);
    }
}