#pragma once

#include <memory>
#include <mutex>
#include <atomic>
#include <string_view>

#include "modian/core/logger/base_logger.h"

#ifdef MODIAN_LOGGER_EXPORTS
#define MODIAN_LOGGER_API __declspec(dllexport)
#else
#define MODIAN_LOGGER_API __declspec(dllimport)
#endif

namespace modian::inkstone::core {
	extern MODIAN_LOGGER_API std::atomic<int> update_logger_times;

	class logger_service {
	public:
		static void print_logo();

		template<typename LoggerFactory>
		static void update_logger(LoggerFactory&& factory) {
			if (update_logger_times.load(std::memory_order_acquire) > 0) {
				return;
			}

			std::lock_guard lock(mutex_);

			if (update_logger_times.load(std::memory_order_relaxed) > 0) {
				return;
			}

			instance = std::move(factory());

			update_logger_times.fetch_add(1, std::memory_order_release);
		}

		static std::shared_ptr<base_logger> logger();

		static void shutdown();
	private:
		logger_service() = default;

		static std::shared_ptr<base_logger> instance;
		static std::mutex mutex_;
	};
}