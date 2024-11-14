#pragma once

namespace ez_grpc {

	using ez_event_handler = void(*)(void*, int, void**, int*);

	class connect
	{
	public:
		connect() = default;
		virtual ~connect() = default;

	public:
		enum class connect_status {
			CREATE = 0,
			PROCESS,
			DISCONNECT,
			FINISH,
			MAX
		};

	protected:
		bool connected_;
		int client_port_;
		int server_port_;
		connect_status status_;
	};

}