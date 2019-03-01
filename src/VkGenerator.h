#pragma once

#include "vulkan/vulkan.hpp"

enum class ELibrary
{
	GLFW,
	SDL2,
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
