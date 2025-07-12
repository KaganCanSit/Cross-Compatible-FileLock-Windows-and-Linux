/*
* @file UnixFileLock.hpp
* @brief Unix/Linux-specific file locking implementation
* @author Kagan Can Sit
*
* In the Linux operating system, more than one header must be defined for file lock operations.
* Flock may be preferred as the old one, and fcntl may be preferred as the more recent one.
* Be sure to review the documentation below to get information about generally used methods and this library.
* @see https://www.gnu.org/software/libc/manual/html_node/File-Locks.html
* @see https://man7.org/linux/man-pages/man2/fcntl.2.html
*/

#pragma once

#if defined(__linux) || defined(__linux__) || defined(__unix__) || defined(__APPLE__)

#include <algorithm>
#include <filesystem>
#include <functional>
#include <chrono>
#include <mutex>
#include <unordered_set>
#include <utility>
#include <thread>

#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "FileLockStrategy.hpp"

namespace file_lock {
	namespace detail {
		/**
		 * @brief Unix/Linux file locking implementation using POSIX fcntl()
		 *
		 * This implementation uses fcntl() for the actual file locking and maintains
		 * an in-process lock table to ensure that only one lock per file per process
		 * can exist at any time, providing consistent behavior across platforms.
		 */
		class UnixFileLock final : public IFileLockStrategy {
		public:
			explicit UnixFileLock(const std::filesystem::path& file_path) noexcept :
				m_filePath(file_path),
				m_fileDescriptor(-1),
				m_isLocked(false) {
			}

			~UnixFileLock() noexcept override {
				CleanupResources();
			}

			// Move constructor
			UnixFileLock(UnixFileLock&& other) noexcept :
				m_filePath(std::move(other.m_filePath)),
				m_fileDescriptor(std::exchange(other.m_fileDescriptor, -1)),
				m_isLocked(std::exchange(other.m_isLocked, false)) {
			}

			// Move assignment operator
			UnixFileLock& operator=(UnixFileLock&& other) noexcept {
				if (this != &other) {
					CleanupResources();
					m_filePath = std::move(other.m_filePath);
					m_fileDescriptor = std::exchange(other.m_fileDescriptor, -1);
					m_isLocked = std::exchange(other.m_isLocked, false);
				}
				return *this;
			}

			[[nodiscard]] bool lock() noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileDescriptor = open(m_filePath.c_str(), O_RDWR | O_CREAT, 0600);
				if (m_fileDescriptor == -1) {
					return false;
				}

				struct flock lockInfo = {
					.l_type = F_WRLCK,      // Exclusive write lock
					.l_whence = SEEK_SET,   // From beginning of file
					.l_start = 0,           // Start at byte 0
					.l_len = 0              // Lock entire file (0 = until EOF)
				};

				// Lock entire file (blocking - waits until lock is available)
				if (fcntl(m_fileDescriptor, F_SETLKW, &lockInfo) == 0) {
					m_isLocked = true;
					return true;
				}

				close(m_fileDescriptor);
				m_fileDescriptor = -1;
				return false;
			}


			[[nodiscard]] bool try_lock() noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileDescriptor = open(m_filePath.c_str(), O_RDWR | O_CREAT, 0600);
				if (m_fileDescriptor == -1) {
					return false;
				}

				struct flock lockInfo = {
					.l_type = F_WRLCK,      // Exclusive write lock
					.l_whence = SEEK_SET,   // From beginning of file
					.l_start = 0,           // Start at byte 0
					.l_len = 0              // Lock entire file (0 = until EOF)
				};

				// Lock entire file (non-blocking)
				if (fcntl(m_fileDescriptor, F_SETLK, &lockInfo) == 0) {
					m_isLocked = true;
					return true;
				}

				close(m_fileDescriptor);
				m_fileDescriptor = -1;
				return false;
			}

			[[nodiscard]] bool try_lock_for(std::chrono::milliseconds timeout) noexcept override {
				if (m_isLocked) {
					return true;
				}

				m_fileDescriptor = open(m_filePath.c_str(), O_RDWR | O_CREAT, 0600);
				if (m_fileDescriptor == -1) {
					return false;
				}

				auto start = std::chrono::steady_clock::now();
				auto deadline = start + timeout;

				struct flock lockInfo = {
					.l_type = F_WRLCK,      // Exclusive write lock
					.l_whence = SEEK_SET,   // From beginning of file
					.l_start = 0,           // Start at byte 0
					.l_len = 0              // Lock entire file (0 = until EOF)
				};

				// Try acquire lock with polling
				while (std::chrono::steady_clock::now() < deadline) {
					if (fcntl(m_fileDescriptor, F_SETLK, &lockInfo) == 0) {
						m_isLocked = true;
						return true;
					}

					// If error is not EAGAIN/EWOULDBLOCK, fail immediately
					if (errno != EAGAIN && errno != EWOULDBLOCK) {
						close(m_fileDescriptor);
						m_fileDescriptor = -1;
						return false;
					}

					// Sleep for a short time before retry
					auto remaining = deadline - std::chrono::steady_clock::now();
					auto sleep_time = std::min<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(remaining), std::chrono::milliseconds(10));
					if (sleep_time.count() > 0) {
						std::this_thread::sleep_for(sleep_time);
					}
				}

				close(m_fileDescriptor);
				m_fileDescriptor = -1;
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
				if (m_fileDescriptor != -1) {
					if (m_isLocked) {
						struct flock unlockInfo = {
							.l_type = F_UNLCK,      // Unlock
							.l_whence = SEEK_SET,
							.l_start = 0,
							.l_len = 0
						};

						fcntl(m_fileDescriptor, F_SETLK, &unlockInfo);
						m_isLocked = false;
					}
					close(m_fileDescriptor);
					m_fileDescriptor = -1;
				}
			}

			std::filesystem::path m_filePath{ "" };
			int m_fileDescriptor{ -1 };
			bool m_isLocked{ false };
		};
	} // namespce detail
}  // namespace file_lock

#endif  // __linux || __unix__ || __APPLE__
