#ifndef BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_LOGGER_H
#define BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_LOGGER_H

#include <Arduino.h>
#include <unity.h>

#include "Configuration.h"
#include "PersistentStorage.h"
#include "Logger.h"
#include "PersistentLogger.h"

#define TEST_DIRECTORY "/test"
#define TEST_LOG_DIRECTORY TEST_DIRECTORY "/test-log"
#define TEST_LOG_FILE_NAME "test.log"
#define TEST_LOG_FILE_PATH TEST_LOG_DIRECTORY "/" TEST_LOG_FILE_NAME

#endif //BLACKOUT_DETECT_O_MATIC_TEST_PERSISTENT_LOGGER_H
