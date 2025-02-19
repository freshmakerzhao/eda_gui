//
// Created by DELL on 2025/2/6.
//

#ifndef HYBRDLINK_MEMORYUTILITIES_H
#define HYBRDLINK_MEMORYUTILITIES_H

#ifdef WIN32
#include <windows.h>
#include <psapi.h>
#include <tlhelp32.h>
#else

#endif

#include <QTimer>
#include <QDebug>

class MemoryUtilities : public QObject {
    Q_OBJECT

public:
    static MemoryUtilities *instance();

    void setWatchMemory(DWORD pid, int cycle);

    void stopWatch();

    float getPeakMemory();

    float getGainMemory();

private:
    MemoryUtilities();

    ~MemoryUtilities();

    Q_DISABLE_COPY(MemoryUtilities);

    void ListProcessChildren();

    void GetProcessMemoryUsage(DWORD processID);

    QTimer *timer;

    float _lastMemory;
    float _curMemory;
    float _peakMemory;
    DWORD _pid;

public slots:
    void checkMemoryUsage();
};


#endif //HYBRDLINK_MEMORYUTILITIES_H
