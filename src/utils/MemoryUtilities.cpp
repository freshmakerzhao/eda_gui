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
    SIZE_T curMemory = 0;
    // 得到进程占用
    GetProcessMemoryUsage(_pid, curMemory);
    // 得到其子进程占用
    ListProcessChildren(_pid, curMemory);
    _curMemory = curMemory / 1024.0 / 1024.0;
    _peakMemory = _curMemory > _peakMemory ? _curMemory : _peakMemory;
}

void MemoryUtilities::GetProcessMemoryUsage(DWORD processID, SIZE_T &totalMemoryUsage) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    if (hProcess == NULL) {
        qDebug() << "Unable to open process with ID " << processID;
        return;
    }

    PROCESS_MEMORY_COUNTERS pmc;
    if (GetProcessMemoryInfo(hProcess, &pmc, sizeof(pmc))) {
        totalMemoryUsage += pmc.WorkingSetSize; // 累加内存使用量
    } else {
        qDebug() << "Failed to get memory information for process " << processID;
    }

    CloseHandle(hProcess);
}

void MemoryUtilities::ListProcessChildren(DWORD parentPID, SIZE_T &totalMemoryUsage) {
    PROCESSENTRY32 pe32;
    HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hSnapshot == INVALID_HANDLE_VALUE) {
        qDebug() << "Failed to create snapshot";
        return;
    }

    pe32.dwSize = sizeof(PROCESSENTRY32);
    if (Process32First(hSnapshot, &pe32)) {
        do {
            if (pe32.th32ParentProcessID == parentPID) {
                // 发现子进程，监控它的内存使用
                GetProcessMemoryUsage(pe32.th32ProcessID, totalMemoryUsage);
            }
        } while (Process32Next(hSnapshot, &pe32));
    }

    CloseHandle(hSnapshot);
}