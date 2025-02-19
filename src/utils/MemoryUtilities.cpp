//
// Created by DELL on 2025/2/6.
//

#include "MemoryUtilities.h"
#include <QDebug>

MemoryUtilities *MemoryUtilities::instance()
{
    static MemoryUtilities instance;
    return &instance;
}

void MemoryUtilities::setWatchMemory(DWORD pid, int cycle)
{
    _pid = pid;
    _peakMemory = 0.0f;
    _lastMemory = 0.0f;
    timer->start(cycle);
}

void MemoryUtilities::stopWatch()
{
    timer->stop();
}

float MemoryUtilities::getPeakMemory()
{
    return _peakMemory;
}

float MemoryUtilities::getGainMemory()
{
    float curMemory = _curMemory;
    float gainMemory = curMemory - _lastMemory;
    _lastMemory = curMemory;
    return gainMemory;
}

MemoryUtilities::MemoryUtilities() : _peakMemory(0.0f), _lastMemory(0.0f)
{
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MemoryUtilities::checkMemoryUsage);
}

MemoryUtilities::~MemoryUtilities() {}

void MemoryUtilities::checkMemoryUsage() {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, _pid);
    if (hProcess == NULL) {
        // Unable to open process with ID
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        _curMemory = pmc.WorkingSetSize / 1024.0 / 1024.0;
        _peakMemory = std::max(_curMemory, _curMemory);
    }
    CloseHandle(hProcess);
}
