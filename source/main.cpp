#include <cstdio>
#include <imgui_impl_sdl3.h>
#include <imgui_impl_sdlrenderer3.h>
#include <SDL3/SDL.h>
#define SDL_MAIN_USE_CALLBACKS
#include <string>
#include <SDL3/SDL_main.h>
#include <dial/app.hpp>
#include <imgui_internal.h>

#ifndef MODULE_NAME
	#error "MODULE_NAME isn't defined. This is both used as a window title and a hard_assert module name."
#endif

static SDL_Window * window;
static SDL_Renderer * renderer;

SDL_AppResult SDL_AppInit(void**, int, char**)
{	ImGui::CreateContext();

	if
	(	!SDL_Init(SDL_INIT_VIDEO) ||
		!(window = SDL_CreateWindow(MODULE_NAME, 600, 400, SDL_WINDOW_RESIZABLE)) ||
		!(renderer = SDL_CreateRenderer(window, NULL)) ||
		!SDL_SetRenderVSync(renderer, 1) ||
		!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer) ||
		!ImGui_ImplSDLRenderer3_Init(renderer)
	)
		return SDL_APP_FAILURE;

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*)
{	ImGui_ImplSDLRenderer3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos({});
	ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
	if(ImGui::Begin("master", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus))
	{	dial::app::get().draw();
		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), renderer);

	SDL_RenderPresent(renderer);
	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void*, SDL_Event * event)
{	ImGui_ImplSDL3_ProcessEvent(event);
	if(event->type == SDL_EVENT_QUIT)
	{	return SDL_APP_SUCCESS;
	}
	else
		return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void*, SDL_AppResult app_result)
{	switch(app_result)
	{	case SDL_APP_SUCCESS:
			ImGui_ImplSDLRenderer3_Shutdown();
			ImGui_ImplSDL3_Shutdown();
			ImGui::DestroyContext();
			break;
		case SDL_APP_FAILURE:
			fprintf(stderr, "SDL application failure with error : '%s'\n", SDL_GetError());
			break;
	}
}
