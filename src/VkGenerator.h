#pragma once

#include "vulkan/vulkan.hpp"

enum class ELibrary
{
	GLFW,
	SDL2,
	NOT_SET,
};

#if defined( USE_SDL2 )
	constexpr ELibrary platform_lib = ELibrary::SDL2;
	using WindowHandle = SDL_Window;
#elseif defined( GLFWAPI )
	constexpr ELibrary platform_lib = ELibrary::GLFW;
	using WindowHandle = GLFWwindow;
#else
	constexpr ELibrary platform_lib = ELibrary::NOT_SET;
#endif

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
