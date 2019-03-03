#pragma once

#include "VkGenerator.hpp"

inline void VkGenerator::SelfTest()
{
	CreateInstance( );
}

inline QueueFamilyIndices VkGenerator::FindQueueFamily() const
{
	QueueFamilyIndices indices;

	// todo: find queue family

	return indices;
}

inline bool VkGenerator::ValidationLayerSupport() const
{
	// todo: check layer support
	return true;
}

inline std::vector<const char*> VkGenerator::GetRequiredExtensions( ) const
{
	std::vector<const char*> required_extensions;
#if defined( USE_SDL2 )
	// grab SDL extensions
#elif defined( GLFWAPI )
	// grab GLFW extensions
#endif
	assert( ( "failed to get required extensions", required_extensions.size( ) > 0 ) );
	return required_extensions;
}

inline void VkGenerator::CreateSurface( )
{
#if defined( USE_SDL2 )
	// create sdl surface
#elif defined( GLFWAPI )
	// create GLFW surface
#endif
}

inline void VkGenerator::CreateInstance()
{
	assert( ( "validation layers not available, despite being requested", m_validation && ValidationLayerSupport( ) ) );

	const auto extensions = GetRequiredExtensions( );

	vk::ApplicationInfo app_info =
	{
		"Insert App Name",
		1,
		"Insert Engine Name",
		1,
		VK_API_VERSION_1_0
	};

	vk::InstanceCreateInfo create_info =
	{
		{},
		&app_info,
		m_validation ? m_validation_layers.size() : 0,
		m_validation ? m_validation_layers.data() : nullptr,
		extensions.size(),
		extensions.data()
	};

	const vk::Result res = vk::createInstance( &create_info, nullptr, &m_instance );
	assert( ( "createInstance failed", res == vk::Result::eSuccess ) );
}

inline void VkGenerator::PickPhysicalDevice()
{

}

inline void VkGenerator::CreateLogicalDevice()
{

}
