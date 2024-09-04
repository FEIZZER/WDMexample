#pragma once
namespace ez_grpc {
	class Handler {
	public:
		virtual void Handle(const void* const buffer, const int length) = 0;
	};
}
