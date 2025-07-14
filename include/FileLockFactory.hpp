/**
* @file FileLockFactory.hpp
* @brief Factory class for creating file lock strategies based on the operating system
* @author Kagan Can Sit
*
* Factory pattern implementation for creating appropriate file lock strategies based on the target platform.
*/

#pragma once

#include <chrono>
#include <filesystem>
#include <memory>

#include "FileLockStrategy.hpp"
#include "UnixFileLock.hpp"
#include "WindowsFileLock.hpp"

namespace file_lock {
	class FileLockFactory {
	public:
		/**
		 * * @brief Creates a file lock context with BLOCKING acquisition
		 * Uses strategy->lock() internally - will wait until lock is available
		 *
		 * This method creates a lock context that will attempt to acquire
		 * a file lock immediately during construction. If the file is already
		 * locked, this call will block until the lock becomes available.
		 *
		 * @param file_path Path to the file to be locked
		 * @return Unique pointer to file lock context, or nullptr if unsupported platform
		 */
		[[nodiscard]] static std::unique_ptr<FileLockContext> CreateLockContext(const std::filesystem::path& file_path) noexcept {
			auto strategy = CreateStrategyInternal(file_path);
			if (!strategy || !strategy->lock()) {
				return nullptr;
			}
			return std::make_unique<FileLockContext>(std::move(strategy), true); // already lock
		}

		/**
		 * @brief Creates a file lock context with NON-BLOCKING acquisition
		 * Uses strategy->try_lock() internally - fails immediately if locked
		 *
		 * This method creates a lock context that will attempt to acquire
		 * a file lock immediately without blocking. If the file is already
		 * locked, this call will fail immediately and return nullptr.
		 *
		 * @param file_path Path to the file to be locked
		 * @return Unique pointer to file lock context, or nullptr if unsupported platform
		 */
		[[nodiscard]] static std::unique_ptr<FileLockContext> CreateTryLockContext(const std::filesystem::path& file_path) noexcept {
			auto strategy = CreateStrategyInternal(file_path);
			if (!strategy || !strategy->try_lock()) {
				return nullptr;
			}
			return std::make_unique<FileLockContext>(std::move(strategy), true); // already locked
		}

		/**
		 * @brief Creates a file lock context with TIMEOUT-BASED acquisition
		 * Uses strategy->try_lock_for() internally - waits up to timeout
		 *
		 * This method creates a lock context that will attempt to acquire
		 * a file lock with a specified timeout. If the lock cannot be acquired
		 * within the timeout period, this call will fail and return nullptr.
		 *
		 * @param file_path Path to the file to be locked
		 * @return Unique pointer to file lock context, or nullptr if unsupported platform or lock failed
		 */
		[[nodiscard]] static std::unique_ptr<FileLockContext> CreateTimedLockContext(const std::filesystem::path& file_path, std::chrono::milliseconds timeout) noexcept {
			auto strategy = CreateStrategyInternal(file_path);
			if (!strategy || !strategy->try_lock_for(timeout)) {
				return nullptr;
			}
			return std::make_unique<FileLockContext>(std::move(strategy), true); // already locked
		}

	private:
		/**
		 * @brief Internal method to create platform-specific strategy
		 *
		 * This method is not exposed to users and creates the appropriate platform-specific
		 * file locking implementation based on compile-time platform detection.
		 *
		 * @param file_path Path to the file to be locked
		 * @return Unique pointer to platform-specific strategy, or nullptr if unsupported
		 */
		[[nodiscard]] static std::unique_ptr<detail::IFileLockStrategy> CreateStrategyInternal(const std::filesystem::path& file_path) noexcept {
#if defined(_WIN32) || defined(_WIN64)
			return std::make_unique<detail::WindowsFileLock>(file_path);
#elif defined(__linux) || defined(__linux__) || defined(__unix__) || defined(__APPLE__)
			return std::make_unique<detail::UnixFileLock>(file_path);
#else
			return nullptr;  // Unsupported platform
#endif
		}

		// Static-only class - delete other calls
		FileLockFactory() = delete;
		~FileLockFactory() = delete;
		FileLockFactory(const FileLockFactory&) = delete;
		FileLockFactory& operator=(const FileLockFactory&) = delete;
		FileLockFactory(FileLockFactory&&) = delete;
		FileLockFactory& operator=(FileLockFactory&&) = delete;
	};
} // namespace file_lock
