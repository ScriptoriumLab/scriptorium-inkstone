#pragma once

#include <string>
#include <vector>

#include "modian/core/protocol/user_action.h"

namespace modian::inkstone::service {
	class ui_protocol_service {
	public:
		static std::string build_render_state_request(
			bool visible,
			double x, double y,
			const std::vector<std::string>& candidates,
			size_t highlight_index,
			int page_index, int total_pages);

		static core::protocol::ui::v1::user_action parse_user_action_response(const std::string& response);
	};
}
