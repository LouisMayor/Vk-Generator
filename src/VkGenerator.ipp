#pragma once

#include "VkGenerator.hpp"
#include <set>

namespace VkGen
{
	inline void VkGenerator::SelfTest( )
	{
		CreateInstance( );
		PickPhysicalDevice( );
		CreateLogicalDevice( );
	}

	inline bool VkGenerator::isDeviceSuitable( const vk::PhysicalDevice _physical_device ) const
	{
		return false;
	}

	inline QueueFamilyIndices VkGenerator::FindQueueFamily( const vk::PhysicalDevice _physical_device ) const
	{
		QueueFamilyIndices indices;
		// todo: find queue family
		return indices;
	}

	inline bool VkGenerator::ValidationLayerSupport( ) const
	{
		// todo: check layer support
		return true;
	}

	inline std::vector<const char*> VkGenerator::GetRequiredExtensions( ) const
	{
		std::vector<const char*> required_extensions;

		if( platform_lib == ELibrary::SDL2 )
		{
			// grab SDL extensions
		}
		else if( platform_lib == ELibrary::GLFW )
		{
			// grab GLFW extensions
		}

		assert( ( "failed to get required extensions", required_extensions.size( ) > 0 ) );
		return required_extensions;
	}

	inline void VkGenerator::CreateSurface( )
	{
		if( platform_lib == ELibrary::SDL2 )
		{
			// create SDL surface
		}
		else if( platform_lib == ELibrary::GLFW )
		{
			// create GLFW surface
		}
	}

	inline void VkGenerator::CreateInstance( )
	{
		if( m_validation )
		{
			assert( ( "validation layers not available, despite being requested", ValidationLayerSupport( ) ) );
		}

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
			m_validation ? m_validation_layers.size( ) : 0,
			m_validation ? m_validation_layers.data( ) : nullptr,
			extensions.size( ),
			extensions.data( )
		};

		const vk::Result res = vk::createInstance( &create_info, nullptr, &m_instance );
		assert( ( "failed to create an instance", res == vk::Result::eSuccess ) );
	}

	inline void VkGenerator::PickPhysicalDevice( )
	{
		std::vector<vk::PhysicalDevice> devices = m_instance.enumeratePhysicalDevices( );
		assert( ( "failed to enumerate physical devices", devices.size( ) > 0 ) );

		for( const auto& device : devices )
		{
			if( isDeviceSuitable( device ) )
			{
				m_physical_device = device;
				break;
			}
		}

		assert( ( "failed to find suitable physical device", m_physical_device != nullptr ) );
	}

	inline void VkGenerator::CreateLogicalDevice( )
	{
		QueueFamilyIndices indices = FindQueueFamily( m_physical_device );

		std::vector<vk::DeviceQueueCreateInfo> queue_create_info = {};
		std::set<int> unique_queue_families = { indices.graphics_family, indices.present_family };

		float queue_priority = 1.0f;
		for( int queue_family : unique_queue_families )
		{
			queue_create_info.push_back
			(
				{
						{},
						static_cast<uint32_t>( queue_family ),
						1,
						&queue_priority
				}
			);
		}

		vk::PhysicalDeviceFeatures device_features = { };
		device_features.samplerAnisotropy = VK_TRUE;
		device_features.fillModeNonSolid = VK_TRUE;
		device_features.fragmentStoresAndAtomics = VK_TRUE;

		vk::DeviceCreateInfo device_create_info =
		{
			{},
			queue_create_info.size( ),
			queue_create_info.data( ),
			m_validation ? m_validation_layers.size( ) : 0,
			m_validation ? m_validation_layers.data( ) : nullptr,
			m_device_extensions.size( ),
			m_device_extensions.data( ),
			&device_features
		};

		const vk::Result res = m_physical_device.createDevice( &device_create_info, nullptr, &m_device );
		assert( ( "failed to create device", res == vk::Result::eSuccess ) );

		m_graphics_queue = m_device.getQueue( indices.graphics_family, 0 );
		m_present_queue = m_device.getQueue( indices.present_family, 0 );
	}
}