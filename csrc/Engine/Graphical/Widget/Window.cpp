﻿import xe.Graphical.Widget.Window;

#include "SDL3/SDL.h"
#include "SDL3/SDL_vulkan.h"
#include "vulkan/vulkan.h"

import xe.Core.xeOrdinals;
import xe.Core.xeString;
import xe.Core.CoreClrOutput;
import xe.Core.xeBaseDataType;

namespace xe
{

	vec2_i32 xeWindow::GetWindowSize()
	{
		vec2_i32 window_size;
		SDL_GetWindowSize(window, &window_size.x, &window_size.y);
		return window_size;
	}

	bool xeWindow::CreatWindow(xeInt32 w, xeInt32 h, xeString name)
	{
		Uint32 count_instance_extensions;
		if (!SDL_Init(SDL_INIT_VIDEO) != 0)
		{
			XE_ERROR_OUTPUT(std::format("LIB <SDL> :Failed to initialize SDL: {0}", SDL_GetError()).c_str());
			return false;
		}

		const char* const* sdl_instance_extensions = SDL_Vulkan_GetInstanceExtensions(&count_instance_extensions);
		
		if (sdl_instance_extensions == nullptr)
		{
			XE_ERROR_OUTPUT(std::format("<LIB : SDL3> Failed to get the number of required instance extensions: {0}\n", SDL_GetError()).c_str());
			return false;
		}

		if(!window_vulkan_instance.SetVulkanInstanceContext(sdl_instance_extensions, count_instance_extensions, "XE_SDL3_VK"))
		{
			XE_ERROR_OUTPUT("<LIB : VULKAN> Failed to setup vulkan instance context");
			return false;
		}

		window = SDL_CreateWindow(name.c_str(), w, h, SDL_WINDOW_VULKAN);
		if (!window)
		{
			XE_ERROR_OUTPUT(std::format("Failed to create window: {0}", SDL_GetError()).c_str());
			return false;
		}
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);

		if (!SDL_Vulkan_CreateSurface(window, window_vulkan_instance.GetInstance(), nullptr, window_vulkan_instance.GetSurface()))
		{
			XE_ERROR_OUTPUT(std::format("<LIB : SDL3> Failed to create vulkan surface: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		if(!window_vulkan_instance.SetVulkanLogicalDevice())
		{
			XE_ERROR_OUTPUT(std::format("<LIB : VULKAN> Failed to create vulkan logical device: {0}\n", SDL_GetError()).c_str());
			return false;
		}
		return false;
	}

	void xeWindow::StartWindowEnvet()
	{
		window_envet_loop = std::thread(&xeWindow::WindowEnvetThtead, this);
		XE_INFO_OUTPUT("Start window envet list");

		window_envet_loop.join();
		XE_INFO_OUTPUT("End window envet list");
	}

	void xeWindow::WindowRendering()
	{
		XE_INFO_OUTPUT("Start window render");
		SDL_Event event{};
		while (true)
		{
			if(SDL_WaitEvent(&event))
			{
				switch (event.type)
				{
				case SDL_EVENT_QUIT:
					goto ABORT_WINDOW_RENDER;

				case SDL_EVENT_KEY_DOWN:
					break;

				case SDL_EVENT_WINDOW_EXPOSED:
					break;
				}
			}
		}
	ABORT_WINDOW_RENDER:
		XE_INFO_OUTPUT("End window render");
	}

	xeWindow::~xeWindow()
	{
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	void xeWindow::WindowEnvetThtead()
	{
		std::this_thread::sleep_for(std::chrono::seconds(10));

	}
}