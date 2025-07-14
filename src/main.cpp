/**
* @file main.cpp
* @brief Comprehensive examples of pure RAII cross-platform file locking
* @author Kagan Can Sit
*
* This file demonstrates the pure RAII approach to cross-platform file locking.
* All examples show automatic resource management without any manual lock/unlock operations.
* The library ensures identical behavior across Windows, Linux, macOS, and other Unix systems.
*
* @warning .txt files is not automatically deleted. However, such lock files are usually located in /tmp etc.
*/

#include <chrono>
#include <iostream>
#include <thread>

#include "../include/FileLockFactory.hpp"

void TestBlockingLock() {
	std::cout << "\nTest - Blocking Lock Start\n";

	auto lock = file_lock::FileLockFactory::CreateLockContext("TestBlockingLock.txt");
	if (lock != nullptr) {
		std::cout << "Blocking lock is acquire, sleep 5 seconds\n";
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	else {
		std::cerr << "[FAIL] - Blocking lock is not acquire!\n"; // Sooner or later the lock had to be acquired.
	}

	std::cout << "Test - Blocking Lock End\n";
}

void TestNonBlockingLock() {
	std::cout << "\nTest - Non Blocking Lock Start\n";

	auto lock = file_lock::FileLockFactory::CreateTryLockContext("TestNonBlocking.txt");
	if (lock != nullptr) {
		std::cout << "Non-Blocking lock is acquire, sleep 5 seconds\n";
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	else {
		std::cerr << "Non-Blocking lock is not acquire!\n"; // A direct error may be returned.
	}

	std::cout << "Test - Non Blocking Lock End\n";
}

void TestTimedLock() {
	std::cout << "\nTest - Timed Lock Start\n";

	auto start = std::chrono::steady_clock::now();

	std::cout << "Time lock timeout parameter set 10 second!\n";
	auto lock = file_lock::FileLockFactory::CreateTimedLockContext("TestTimedLock.txt", std::chrono::seconds(10));
	if (lock != nullptr) {
		std::cout << "Time lock is acquire, sleep 5 seconds\n";
		std::this_thread::sleep_for(std::chrono::seconds(5));
	}
	else {
		std::cerr << "[FAIL] - Time lock (Blocking lock) is not acquire!\n"; // Sooner or later the lock had to be acquired.
	}

	auto duration = std::chrono::steady_clock::now() - start;
	auto sec = std::chrono::duration_cast<std::chrono::seconds>(duration).count();

	std::cout << "Function start and end time range: " << sec << " second\n";
	std::cout << "Test - Timed Lock End\n";
}

int main() {
	std::cout << "===============================================================================================\n";
	std::cout << "======================= Cross-Platform File Lock Library - Simple Tests =======================\n";
	std::cout << "===============================================================================================\n";
	std::cout << "There will be no locking in the same process or thread for testing the application anyway.\n"
		"So compile the application and call it two or more times from the console etc.\n";
	std::cout << "===============================================================================================\n";

	//TestBlockingLock();
	//TestNonBlockingLock();
	//TestTimedLock();

	std::cout << std::endl;
}
