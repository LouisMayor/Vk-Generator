#pragma once

#include "vulkan/vulkan.hpp"

struct QueueFamilyIndices
{
	int graphics_family = -1;
	int present_family = -1;

	bool IsComplete( ) const
	{
		return graphics_family >= 0 && present_family >= 0;
	}
};

enum class ELibrary
{
	GLFW,
	SDL2,
	NOT_SET,
};

#if defined( USE_SDL2 )
	constexpr ELibrary platform_lib = ELibrary::SDL2;
	using WindowHandle = SDL_Window;
#elif defined( GLFWAPI )
	constexpr ELibrary platform_lib = ELibrary::GLFW;
	using WindowHandle = GLFWwindow;
#else
	constexpr ELibrary platform_lib = ELibrary::NOT_SET;
#endif

class VkGenerator
{
	/* public functions */
public:
	/* constructor/destructor */
	VkGenerator( const bool _enable_validation ) : m_validation(_enable_validation)
	{ /* hello there! */ }
	~VkGenerator( ) = default;

	/* copy */
	VkGenerator( const VkGenerator& _other ) = delete;
	void operator=( const VkGenerator& _other ) = delete;

	/* move */
	VkGenerator( const VkGenerator&& _other ) = delete;
	void operator=( const VkGenerator&& _other ) = delete;

	/* Test */
	void SelfTest( );

	/* private functions */
private:
	VkGenerator( ) = default;

	std::vector<const char*> GetRequiredExtensions( ) const;
	bool ValidationLayerSupport( ) const;
	QueueFamilyIndices FindQueueFamily( ) const;

	void CreateInstance( );
	void PickPhysicalDevice( );
	void CreateLogicalDevice( );
	void CreateSurface( );

	/* public members */
public:

	/* private members */
private:
	vk::Instance m_instance;

	bool m_validation = false;

	const std::vector<const char*> m_validation_layers =
	{
		"VK_LAYER_LUNARG_standard_validation"
	};

	const std::vector<const char*> m_device_extensions =
	{
		VK_KHR_SWAPCHAIN_EXTENSION_NAME
	};
};
