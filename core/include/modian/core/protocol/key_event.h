#pragma once

#include <string>

namespace modian::inkstone::core::protocol::input::v1 {
	class key_event {
	public:
		explicit key_event(std::string content);

		std::string content;
	};
}
