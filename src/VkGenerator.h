#pragma once

#include "vulkan/vulkan.hpp"

enum class ELibrary : unsigned char
{
	GLFW = 1 << 1,
	SDL2 = 1 << 2,
};

class VkGenerator
{
public:
	/* constructor/destructor */
	VkGenerator( ) = default;
	~VkGenerator( ) = default;

	/* copy */
	VkGenerator( const VkGenerator& _other ) = delete;
	void operator=( const VkGenerator& _other ) = delete;

	/* move */
	VkGenerator( const VkGenerator&& _other ) = delete;
	void operator=( const VkGenerator&& _other ) = delete;
private:
};
