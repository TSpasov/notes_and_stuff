// logger.h

#ifndef LOGGER_H
#define LOGGER_H

#include <string>

#ifdef auto_test  // Check if auto_test is defined
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) {
        // Code to log messages
        std::cout << message << std::endl;
    }

private:
    Logger() {}  // Private constructor to enforce singleton pattern
};
#else
// If auto_test is NOT defined, provide an empty logger class that does nothing
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string&) {
        // Do nothing if logging is disabled
    }

private:
    Logger() {}
};
#endif  // auto_test

#endif  // LOGGER_H


// second version

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>
#include <type_traits>

// Logger class with conditional log method
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // Enable logging if auto_test is true
    template <bool EnableLogging = true, typename std::enable_if<EnableLogging, int>::type = 0>
    void log(const std::string& message) {
        std::cout << message << std::endl;
    }

    // Disable logging if auto_test is false (SFINAE)
    template <bool EnableLogging = true, typename std::enable_if<!EnableLogging, int>::type = 0>
    void log(const std::string&) {
        // Do nothing (logging disabled)
    }

private:
    Logger() {}  // Private constructor
};

#endif  // LOGGER_H

// usage

// main.cpp
#include "logger.h"

// Define auto_test as true or false at compile time
constexpr bool auto_test = false;  // Set to true or false to enable/disable logging

int main() {
    // Use the Logger instance
    Logger::getInstance().log<auto_test>("This message will not be logged if auto_test is false");
    return 0;
}


: //third version

#ifndef LOGGER_H
#define LOGGER_H

#include <iostream>
#include <string>

// Define a logging policy
template <bool EnableLogging>
struct LoggingPolicy;

// Specialization for logging enabled
template <>
struct LoggingPolicy<true> {
    static void log(const std::string& message) {
        std::cout << message << std::endl;
    }
};

// Specialization for logging disabled (no-op)
template <>
struct LoggingPolicy<false> {
    static void log(const std::string&) {
        // Do nothing
    }
};

// Logger class using the policy
template <bool EnableLogging>
class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    void log(const std::string& message) {
        LoggingPolicy<EnableLogging>::log(message);  // Use the policy to decide behavior
    }

private:
    Logger() {}
};

#endif  // LOGGER_H


//usage

#include "logger.h"

// Control logging at compile time using a policy
constexpr bool auto_test = true;  // Set to false to disable logging

int main() {
    // Use the logger with a logging policy
    Logger<auto_test>::getInstance().log("This is a test message");
    return 0;
}


