#include "modian/core/logger/console_logger.h"
#include <iostream>

namespace modian::inkstone::core {
	void console_logger::sink_it(const log_level level, const std::string_view msg) {
		switch (level) {
		case log_level::debug: std::cout << "[DEBUG] "; break;
		case log_level::info:  std::cout << "[INFO]  "; break;
		case log_level::error: std::cout << "[ERROR] "; break;
		}
		std::cout << msg << "\n";
	}
}