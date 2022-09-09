/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smun <smun@student.42seoul.kr>             +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/08 16:23:53 by smun              #+#    #+#             */
/*   Updated: 2022/09/09 16:31:00 by smun             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "std.hpp"

constexpr static Uint32 GetWindowFlags()
{
	return SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_VULKAN;
	// Uint32 flags = 0;
	// flags |= SDL_WINDOW_RESIZABLE;
	// flags |= SDL_WINDOW_ALLOW_HIGHDPI;
	// #ifdef __APPLE__
	// 	flags |= SDL_WINDOW_METAL;
	// #else
	// 	flags |= SDL_WINDOW_VULKAN;
	// #endif
	// return (flags);
}

static void ValidateLayers(std::vector<std::string_view>& layers)
{
	VkResult res;
	uint32_t instanceLayerCount = 0;
	res = vkEnumerateInstanceLayerProperties(&instanceLayerCount, NULL);
	if (!res)
		throw std::runtime_error("vkEnumerateInstanceLayerProperties failed (1)");
	std::vector<VkLayerProperties> instanceLayers(instanceLayerCount);
	res = vkEnumerateInstanceLayerProperties(&instanceLayerCount, instanceLayers.data());
	if (!res)
		throw std::runtime_error("vkEnumerateInstanceLayerProperties failed (2)");
	for (const auto& layer : layers)
	{
		const auto check = [&](const VkLayerProperties& p) { return !strcmp(p.layerName, layer); };
		const auto found = std::find_if(instanceLayers.begin(), instanceLayers.end(), check);
		if (found == instanceLayers.end())
		{
			std::cerr << "Layer not found: " << layer << std::endl;
			std::remove(layers.begin(), layers.end(), layer);
		}
	}
}

static void ValidateExtensions(std::vector<std::string_view>& extensions)
{
	VkResult res;
	uint32_t instanceExtensionCount = 0;
	res = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, NULL);
	if (!res)
		throw std::runtime_error("vkEnumerateInstanceExtensionProperties failed (1)");
	std::vector<VkExtensionProperties> instanceExtensions(instanceExtensionCount);
	res = vkEnumerateInstanceExtensionProperties(NULL, &instanceExtensionCount, instanceExtensions.data());
	if (!res)
		throw std::runtime_error("vkEnumerateInstanceExtensionProperties failed (2)");
	for (const auto& extension : extensions)
	{
		const auto check = [&](const VkExtensionProperties& p) { return !strcmp(p.extensionName, extension); };
		const auto found = std::find_if(instanceExtensions.begin(), instanceExtensions.end(), check);
		if (found == instanceExtensions.end())
		{
			std::cout << "Extension not found: " << extension << std::endl;
			std::remove(extensions.begin(), extensions.end(), extension);
		}
	}
}

int main(int argc, char* argv[])
{
	(void)argc;
	(void)argv;
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		throw std::runtime_error("SDL_Init failed (" + std::string(SDL_GetError()) + ")");
	SDL_Window* win = SDL_CreateWindow(
		"scop",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		1024, 768,
		GetWindowFlags()
	);
	if (win == NULL)
		throw std::runtime_error("SDL_CreateWindow failed (" + std::string(SDL_GetError()) + ")");

	const VkApplicationInfo app = {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "scop",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "scop",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_0,
	};
	std::vector<std::string_view> layers = {
		"VK_LAYER_LUNARG_api_dump",
		"VK_LAYER_KHRONOS_validation",
	};
	std::vector<std::string_view> extensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
#if defined(VK_USE_PLATFORM_WIN32_KHR)
		VK_KHR_WIN32_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_DISPLAY_KHR)
		VK_KHR_DISPLAY_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_METAL_EXT)
		VK_EXT_METAL_SURFACE_EXTENSION_NAME,
#elif defined(VK_USE_PLATFORM_XLIB_KHR)
		VK_KHR_XLIB_SURFACE_EXTENSION_NAME,
#endif
		VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		VK_EXT_DEBUG_UTILS_EXTENSION_NAME,
		VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME
	};

	ValidateLayers(layers);
	ValidateExtensions(extensions);

	const foundPortabilitySubset = std::find(
		extensions.begin(),
		extensions.end(),
		VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);
	const VkInstanceCreateInfo inst = {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pNext = NULL,
		.flags = foundPortabilitySubset != extensions.end() ? VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR : 0,
		.pApplicationInfo = &app,
		.enabledLayerCount = static_cast<uint32_t>(layers.size()),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = static_cast<uint32_t>(extensions.size()),
		.ppEnabledExtensionNames = extensions.data(),
	};


	bool running = true;
	for(;running;)
	{
		SDL_Event e;
		if (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
				case SDL_QUIT:
					running = false;
					break;
			}
		}
		// display
	}

	SDL_DestroyWindow(win);
	SDL_Quit();

	return EXIT_SUCCESS;
}
