/*
* @file WindowsFileLock.hpp
* @brief This file contains the implementation of a file lock strategy for the Windows operating system.
* @author Kagan Can Sit
*
* Windows file locking implementation using Win32 API LockFileEx.
* Be sure to review the documentation below to get information about generally used methods and this library.
*
* @see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfile
* @see https://learn.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-lockfileex
*
* @warning This implementation requires Windows Vista or later
*/

#pragma once

#if defined(_WIN32) || defined(_WIN64)

#include <algorithm>
#include <filesystem>
#include <chrono>
#include <utility>
#include <thread>

#include <windows.h>

#include "FileLockStrategy.hpp"

namespace file_lock {
	namespace detail {
		/**
		 * @brief Windows file locking implementation using Win32 API
		 *
		 * This implementation uses the Win32 LockFileEx API which provides
		 * kernel-level mutual exclusion. Windows automatically ensures that
		 * only one lock per file can exist system-wide.
		 */
		class WindowsFileLock final : public IFileLockStrategy {
		public:
			explicit WindowsFileLock(const std::filesystem::path& file_path) noexcept :
				m_filePath(file_path),
				m_fileHandle(INVALID_HANDLE_VALUE),
				m_isLocked(false) {
			}

			~WindowsFileLock() noexcept override {
				CleanupResources();
			}

			// Move constructor
			WindowsFileLock(WindowsFileLock&& other) noexcept :
				m_filePath(std::move(other.m_filePath)),
				m_fileHandle(std::exchange(other.m_fileHandle, INVALID_HANDLE_VALUE)),
				m_isLocked(std::exchange(other.m_isLocked, false)) {
			}

			// Move assignment operator
			WindowsFileLock& operator=(WindowsFileLock&& other) noexcept {
				if (this != &other) {
					CleanupResources();
					m_filePath = std::move(other.m_filePath);
					m_fileHandle = std::exchange(other.m_fileHandle, INVALID_HANDLE_VALUE);
					m_isLocked = std::exchange(other.m_isLocked, false);
				}
				return *this;
			}

			[[nodiscard]] bool lock() noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileHandle = CreateFileW(m_filePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (m_fileHandle == INVALID_HANDLE_VALUE) {
					return false;
				}

				// Lock entire file (blocking - waits until lock is available)
				OVERLAPPED overlapped{}; // Note: NOT using LOCKFILE_FAIL_IMMEDIATELY flag for blocking behavior
				if (LockFileEx(m_fileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped)) {
					m_isLocked = true;
					return true;
				}

				CloseHandle(m_fileHandle);
				m_fileHandle = INVALID_HANDLE_VALUE;
				return false;
			}

			[[nodiscard]] bool try_lock() noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileHandle = CreateFileW(m_filePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (m_fileHandle == INVALID_HANDLE_VALUE) {
					return false;
				}

				// Lock entire file (non-blocking)
				OVERLAPPED overlapped{};
				if (LockFileEx(m_fileHandle, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY, 0, MAXDWORD, MAXDWORD, &overlapped)) {
					m_isLocked = true;
					return true;
				}

				CloseHandle(m_fileHandle);
				m_fileHandle = INVALID_HANDLE_VALUE;
				return false;
			}


			[[nodiscard]] bool try_lock_for(std::chrono::milliseconds timeout) noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileHandle = CreateFileW(m_filePath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
				if (m_fileHandle == INVALID_HANDLE_VALUE) {
					return false;
				}

				auto start = std::chrono::steady_clock::now();
				auto deadline = start + timeout;

				// Try acquire lock with polling
				while (std::chrono::steady_clock::now() < deadline) {
					OVERLAPPED overlapped{};
					if (LockFileEx(m_fileHandle, LOCKFILE_EXCLUSIVE_LOCK, 0, MAXDWORD, MAXDWORD, &overlapped)) {
						m_isLocked = true;
						return true;
					}

					// If error is not ERROR_LOCK_VIOLATION, fail immediately
					if (GetLastError() != ERROR_LOCK_VIOLATION) {
						CloseHandle(m_fileHandle);
						m_fileHandle = INVALID_HANDLE_VALUE;
						return false;
					}

					// Sleep for a short time before retry
					auto remaining = deadline - std::chrono::steady_clock::now();
					auto sleep_time = std::min<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(remaining), std::chrono::milliseconds(10));
					if (sleep_time.count() > 0) {
						std::this_thread::sleep_for(sleep_time);
					}
				}

				CloseHandle(m_fileHandle);
				m_fileHandle = INVALID_HANDLE_VALUE;
				return false;
			}

			void unlock() noexcept override {
				CleanupResources();
			}
		private:
			/**
			* @brief Internal cleanup method - not virtual / CppCheck warning PVS-Studio/PC-Lint
			*/
			void CleanupResources() noexcept {
				if (m_fileHandle != INVALID_HANDLE_VALUE) {
					if (m_isLocked) {
						OVERLAPPED overlapped = {};
						UnlockFileEx(m_fileHandle, 0, MAXDWORD, MAXDWORD, &overlapped);
						m_isLocked = false;
					}
					CloseHandle(m_fileHandle);
					m_fileHandle = INVALID_HANDLE_VALUE;
				}
			}

			std::filesystem::path m_filePath{ "" };
			HANDLE m_fileHandle{ INVALID_HANDLE_VALUE };
			bool m_isLocked{ false };
		};
	} // namespace detail
} // namespace file_lock

#endif // _WIN32 || _WIN64
