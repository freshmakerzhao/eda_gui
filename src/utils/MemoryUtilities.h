//
// Created by DELL on 2025/2/6.
//

#ifndef HYBRDLINK_MEMORYUTILITIES_H
#define HYBRDLINK_MEMORYUTILITIES_H

#ifdef WIN32
#include <windows.h>
#include <psapi.h>
#else

#endif

#include <QTimer>
#include <QDebug>

class MemoryUtilities : public QObject {
    Q_OBJECT

public:
    MemoryUtilities(DWORD pid, int cycle);
    float getCurrentMemory();
    float getMaxMemory();

private:
    QTimer *timer;
    float curMemory;
    float maxMemory;
    DWORD pid;

public slots:
    void checkMemoryUsage();
};


#endif //HYBRDLINK_MEMORYUTILITIES_H
