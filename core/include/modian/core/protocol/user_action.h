#pragma once

namespace modian::inkstone::core::protocol::ui::v1 {
	enum class action_type : int {
		UNKNOWN = 0,
		SELECT_CANDIDATE,
		PAGE_NEXT,
		PAGE_PREV,
		QUIT
	};

	struct user_action {
		action_type type;
		size_t payload;
	};
}
