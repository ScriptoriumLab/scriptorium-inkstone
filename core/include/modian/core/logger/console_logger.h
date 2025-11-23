#pragma once
#include "modian/core/logger/base_logger.h"

namespace modian::inkstone::core {
	class console_logger final : public base_logger {
	public:
		~console_logger() override = default;

		void sink_it(log_level level, std::string_view msg) override;

		[[nodiscard]] std::string_view type() const override { return "console_logger"; }
	};
}