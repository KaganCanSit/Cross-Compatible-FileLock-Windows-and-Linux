/*
* @file FileLockStrategy.hpp
* @brief Simple cross-platform file locking strategy interface
* @author Kagan Can Sit
*
* This file provides a simple cross-platform file locking mechanism for Linux and Windows using C++20. It abstracts
* platform-specific details using the Strategy pattern.
*/

#pragma once

#include <chrono>
#include <filesystem>
#include <memory>
#include <utility>

namespace file_lock {

	class FileLockContext; // Forward declaration

	namespace detail {
		class IFileLockStrategy {
		public:
			/**
			* @brief Virtual destructor for proper polymorphic cleanup
			*/
			virtual ~IFileLockStrategy() = default;

			/**
			* @brief Attempts to acquire an exclusive lock on the file
			* @return true if lock was successfully acquired, false otherwise
			* @note This is a blocking call - waits until lock becomes available
			*/
			[[nodiscard]] virtual bool lock() noexcept = 0;

			/**
			 * @brief Attempts to acquire an exclusive lock on the file without blocking
			 * @return true if lock was successfully acquired immediately, false otherwise
			 * @note This is a non-blocking call - returns immediately
			 */
			[[nodiscard]] virtual bool try_lock() noexcept = 0;


			/**
			* @brief Attempts to acquire an exclusive lock on the file with timeout
			* @param timeout Maximum time to wait for lock acquisition
			* @return true if lock was successfully acquired within timeout, false otherwise
			* @note This is a blocking call - the lock will attempt to be acquired during the timeout.
			*/
			[[nodiscard]] virtual bool try_lock_for(std::chrono::milliseconds timeout) noexcept = 0;

			/**
			 * @brief Releases the file lock
			 */
			virtual void unlock() noexcept = 0;

		protected:
			IFileLockStrategy() = default;

			// Disable copy and move operations
			IFileLockStrategy(const IFileLockStrategy&) = delete;
			IFileLockStrategy& operator=(const IFileLockStrategy&) = delete;
			IFileLockStrategy(IFileLockStrategy&&) noexcept = default;
			IFileLockStrategy& operator=(IFileLockStrategy&&) noexcept = default;
		};
	}

	/**
	 * @brief Pure RAII wrapper for automatic file lock management
	 *
	 * This class provides complete automatic file lock management:
	 * - Locks are acquired during construction or via factory methods
	 * - Locks are automatically released during destruction
	 * - Move semantics for efficient ownership transfer
	 */
	class FileLockContext {
	public:
		/**
		* @brief Constructor for immediate lock acquisition
		* @param strategy Platform-specific lock strategy
		*/
		explicit FileLockContext(std::unique_ptr<detail::IFileLockStrategy> strategy) noexcept : m_strategy(std::move(strategy)), m_isLocked(false) {
			if (m_strategy) {
				m_isLocked = m_strategy->lock();
			}
		}

		/**
		 * @brief Constructor for pre-acquired locks
		 * @param strategy Platform-specific lock strategy
		 * @param alreadyLocked Whether the strategy already holds a lock
		 */
		FileLockContext(std::unique_ptr<detail::IFileLockStrategy> strategy, bool alreadyLocked) noexcept
			: m_strategy(std::move(strategy)), m_isLocked(alreadyLocked) {
		}

		/**
		 * @brief The destructive function provides automatic release of the lock.
		 */
		~FileLockContext() noexcept {
			if (m_isLocked && m_strategy) {
				m_strategy->unlock();
			}
		}

		/**
		* @brief Returns whether lock acquisition was successful
		* @return true if lock was acquired successfully during construction
		* @note This only indicates initial lock success, not current state
		*/
		[[nodiscard]] bool IsLockAcquired() const noexcept {
			return m_isLocked;
		}

		/**
		 * @brief Check if context is valid (has a strategy)
		 * @return true if context has a valid strategy, false otherwise
		 */
		[[nodiscard]] operator bool() const noexcept {
			return m_strategy != nullptr;
		}

		// Disable copy operations
		FileLockContext(const FileLockContext&) = delete;
		FileLockContext& operator=(const FileLockContext&) = delete;

		// Allow move operations
		FileLockContext(FileLockContext&& other) noexcept : m_strategy(std::move(other.m_strategy)), m_isLocked(std::exchange(other.m_isLocked, false)) {};
		FileLockContext& operator=(FileLockContext&& other) noexcept {
			if (this != &other) {
				// If you have one, leave the current lock because the other lock will be taken over.
				if (m_isLocked && m_strategy) {
					m_strategy->unlock();
				}

				m_strategy = std::move(other.m_strategy);
				m_isLocked = std::exchange(other.m_isLocked, false);
			}
			return *this;
		}

	private:
		std::unique_ptr<detail::IFileLockStrategy> m_strategy{ nullptr };
		bool m_isLocked{ false };
	};
} // namespace file_lock
