#include "modian/service/ui_protocol_service.h"

#include <nlohmann/json.hpp>

#include "modian/core/logger/logger_service.h"

namespace modian::inkstone::service {
	std::string ui_protocol_service::build_render_state_request(
		bool visible,
		double x, double y,
		const std::vector<std::string>& candidates,
		size_t highlight_index,
		int page_index, int total_pages
	) {
		nlohmann::json j;
		j["visible"] = visible;
		j["x"] = x;
		j["y"] = y;
		j["candidates"] = candidates;
		j["highlight_index"] = highlight_index;
		j["page_index"] = page_index;
		j["total_pages"] = total_pages;

		return j.dump(-1);
	}

    common::core::protocol::ui::v1::user_action ui_protocol_service::parse_user_action_response(const std::string& response) {
        common::core::protocol::ui::v1::user_action action{common::core::protocol::ui::v1::action_type::UNKNOWN, 0};

		try {
			const auto j = nlohmann::json::parse(response);

			if (const std::string type_str = j.value("type", "Unknown"); type_str == "SelectCandidate") {
				action.type = common::core::protocol::ui::v1::action_type::SELECT_CANDIDATE;
				action.payload = j.value("payload", 0);
			} else if (type_str == "PageNext") {
				action.type = common::core::protocol::ui::v1::action_type::PAGE_NEXT;
			} else if (type_str == "PagePrev") {
				action.type = common::core::protocol::ui::v1::action_type::PAGE_PREV;
			}
		} catch (const nlohmann::json::parse_error& e) {
			core::logger_service::logger()->error("JSON parse failed: {}", e.what());
		}

		return action;
	}
}
