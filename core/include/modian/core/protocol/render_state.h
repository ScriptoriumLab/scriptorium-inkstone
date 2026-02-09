#pragma once

#include <vector>
#include <string>

namespace modian::inkstone::core::protocol::ui::v1 {
	struct render_state {
		bool visible{false};
		double x{0.0};
		double y{0.0};
		std::vector<std::string> candidates;
		int highlight_index{0};
		int page_index{0};
		int total_pages{0};
	};
}
