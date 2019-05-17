#pragma once

#include "VkGenerator.hpp"
#include <set>
#include <iostream>

namespace VkGen
{
	inline void VkGenerator::SelfTest( )
	{
		m_isDestroyed = false;

		CreateWindow();
		CreateInstance( );
		CreateSurface( );
		PickPhysicalDevice( );
		CreateLogicalDevice( );

		Destroy();
	}

	inline void VkGenerator::CreateWindow()
	{
		if( platform_lib == ELibrary::SDL2 )
		{
		}
		else if( platform_lib == ELibrary::GLFW )
		{
			glfwInit( );

			glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );
			glfwWindowHint( GLFW_RESIZABLE, GLFW_TRUE );

			m_window_handle = glfwCreateWindow( static_cast<int>( m_buffer_resolution[0] ),
												static_cast<int>( m_buffer_resolution[1] ),
												"Vulkan Generator Window", nullptr, nullptr );

			if( m_window_handle == nullptr )
			{
				std::cerr << "Failed to create GLFW window" << std::endl;
				glfwTerminate( );
				return;
			}

			glfwSetWindowPos( m_window_handle, 0, 30 );
			glfwHideWindow( m_window_handle );
		}
	}

	inline QueueFamilyIndices VkGenerator::FindQueueFamilies( const vk::PhysicalDevice _physical_device )
	{
		QueueFamilyIndices indices;
		auto queueProperties = _physical_device.getQueueFamilyProperties();

		int i = 0;
		for ( const auto& family : queueProperties )
		{
			if( family.queueCount > 0 && family.queueFlags & vk::QueueFlagBits::eGraphics )
			{
				indices.graphics_family = i;
			}

			auto presentSupport = _physical_device.getSurfaceSupportKHR(i, m_surface);

			if( family.queueCount > 0 && presentSupport )
			{
				indices.present_family = i;
			}

			if( indices.IsComplete( ) )
			{
				break;
			}

			i++;
		}

		return indices;
	}

	inline VkBool32 VkGenerator::CheckDeviceExtensionSupport( const vk::PhysicalDevice _physical_device )
	{
		auto extensions = _physical_device.enumerateDeviceExtensionProperties();

		std::set<std::string> requiredExtensions( m_device_extensions.begin( ), m_device_extensions.end( ) );

		for( const auto& extension : extensions )
		{
			requiredExtensions.erase( extension.extensionName );
		}

		return requiredExtensions.empty( );
	}

	inline SwapChainSupportDetails VkGenerator::QuerySwapChainSupport( const vk::PhysicalDevice _physical_device )
	{
		SwapChainSupportDetails details;

		details.capabilities = _physical_device.getSurfaceCapabilitiesKHR( m_surface );
		details.formats = _physical_device.getSurfaceFormatsKHR( m_surface );
		details.presentModes = _physical_device.getSurfacePresentModesKHR( m_surface );

		return details;
	}

	inline VkBool32 VkGenerator::IsDeviceSuitable( const vk::PhysicalDevice _physical_device )
	{
		QueueFamilyIndices indices = FindQueueFamilies( _physical_device );
		const VkBool32 extensionSupported = CheckDeviceExtensionSupport( _physical_device );

		bool swapChainAdequate = false;

		if( extensionSupported )
		{
			SwapChainSupportDetails swapChainSupport = QuerySwapChainSupport( _physical_device );
			swapChainAdequate = !swapChainSupport.formats.empty( ) && !swapChainSupport.presentModes.empty( );
		}

		vk::PhysicalDeviceFeatures supportedFeatures = _physical_device.getFeatures();

		return indices.IsComplete( ) && extensionSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy;
	}

	inline VkBool32 VkGenerator::ValidationLayerSupport( ) const
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
			uint32_t glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );

			required_extensions = std::vector<const char*>( glfwExtensions, glfwExtensions + glfwExtensionCount );

			if( m_validation )
			{
				required_extensions.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
			}
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
			auto surface = VkSurfaceKHR(m_surface);
			if( glfwCreateWindowSurface( VkInstance(m_instance), m_window_handle, nullptr, &surface ) != VK_SUCCESS )
			{
				throw std::runtime_error( "failed to create window surface!" );
			}

			m_surface = vk::SurfaceKHR(surface);
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
			if( IsDeviceSuitable( device ) )
			{
				m_physical_device = device;
				break;
			}
		}

		assert( ( "failed to find suitable physical device", m_physical_device != nullptr ) );
	}

	inline void VkGenerator::CreateLogicalDevice( )
	{
		QueueFamilyIndices indices = FindQueueFamilies( m_physical_device );

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

	inline bool VkGenerator::IsDestroyed() const
	{
		return m_isDestroyed;
	}

	inline void VkGenerator::Destroy()
	{
		if (IsDestroyed())
		{
			return;
		}

		m_device.waitIdle();

		DestroyDevice();
		DestroySurface();
		DestroyInstance();

		m_isDestroyed = true;
	}

	inline void VkGenerator::DestroyDevice()
	{
		if (m_device == nullptr)
		{
			return;
		}

		m_device.destroy();
	}

	inline void VkGenerator::DestroyInstance( )
	{
		if(m_instance == nullptr)
		{
			return;
		}

		m_instance.destroy();
	}

	inline void VkGenerator::DestroySurface( )
	{
		if (m_instance == nullptr || m_surface == nullptr)
		{
			return;
		}

		m_instance.destroySurfaceKHR(m_surface);
	}
}
