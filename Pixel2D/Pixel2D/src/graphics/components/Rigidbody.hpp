#pragma once
#include "../../graphics/RigidbodyShape.hpp"
#include <memory>

namespace px
{
	struct Rigidbody
	{
		explicit Rigidbody(std::unique_ptr<RigidbodyShape> body) : body(std::move(body)) {}

		std::unique_ptr<RigidbodyShape> body;
	};
}