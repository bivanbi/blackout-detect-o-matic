#ifndef BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_STORAGE_H
#define BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_STORAGE_H

#include <unity.h>
#include <Arduino.h>

#include "PersistentStorage.h"
#include "RTCAdapter.h"

#define TEST_DIRECTORY "/test"

void initTestDirectory();

void listDir(String dirname, uint8_t levels);

#endif //BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_STORAGE_H
