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

	template<typename Factory>
	concept logger_factory = requires(Factory factory) {
		{ factory() } -> std::convertible_to<std::shared_ptr<base_logger>>;
	};

	class logger_service {
	public:
		static void print_logo();

		template<logger_factory Factory>
		static void update_logger(Factory&& factory) {
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