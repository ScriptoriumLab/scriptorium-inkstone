#pragma once

#include <string>

namespace modian::inkstone::core::protocol {
	class composition_protocol {
	public:
		enum class message_type {
			NONE = 'N',
			UPDATE = 'U',
			COMMIT = 'C'
		};

		composition_protocol();
		composition_protocol(const message_type& type, std::string payload);

		std::string encode() const;

		static composition_protocol create_update(std::string payload);
		static composition_protocol create_commit(std::string payload);

	private:
		message_type type;
		std::string payload;
	};
}