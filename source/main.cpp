#ifdef __EMSCRIPTEN__
	#include <emscripten.h>
#endif
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

	extern bool(*dial_internal_entry_point)();

	if
	(	!SDL_Init(SDL_INIT_VIDEO) ||
		!(window = SDL_CreateWindow(MODULE_NAME, 600, 400, SDL_WINDOW_RESIZABLE)) ||
		!(renderer = SDL_CreateRenderer(window, NULL)) ||
		!SDL_SetRenderVSync(renderer, 1) ||
		!ImGui_ImplSDL3_InitForSDLRenderer(window, renderer) ||
		!ImGui_ImplSDLRenderer3_Init(renderer) ||
		!(!dial_internal_entry_point || dial_internal_entry_point())
	)
		return SDL_APP_FAILURE;

		// Moonlight theme by Madam-Herta https://github.com/Patitotective/ImThemes
		ImGuiStyle & style = ImGui::GetStyle();
		style.Alpha = 1.0f;
		style.DisabledAlpha = 1.0f;
		style.WindowPadding = ImVec2(12.0f, 12.0f);
		style.WindowRounding = 11.5f;
		style.WindowBorderSize = 0.0f;
		style.WindowMinSize = ImVec2(20.0f, 20.0f);
		style.WindowTitleAlign = ImVec2(0.5f, 0.5f);
		style.WindowMenuButtonPosition = ImGuiDir_Right;
		style.ChildRounding = 0.0f;
		style.ChildBorderSize = 1.0f;
		style.PopupRounding = 0.0f;
		style.PopupBorderSize = 1.0f;
		style.FramePadding = ImVec2(20.0f, 3.4f);
		style.FrameRounding = 11.9f;
		style.FrameBorderSize = 0.0f;
		style.ItemSpacing = ImVec2(4.3f, 5.5f);
		style.ItemInnerSpacing = ImVec2(7.1f, 1.8f);
		style.CellPadding = ImVec2(12.1f, 9.2f);
		style.ColumnsMinSpacing = 4.9f;
		style.ScrollbarSize = 11.6f;
		style.ScrollbarRounding = 15.9f;
		style.GrabMinSize = 3.7f;
		style.GrabRounding = 20.0f;
		style.TabRounding = 0.0f;
		style.TabBorderSize = 0.0f;
		style.TabCloseButtonMinWidthUnselected = 0.0f;
		style.ColorButtonPosition = ImGuiDir_Right;
		style.ButtonTextAlign = ImVec2(0.5f, 0.5f);
		style.SelectableTextAlign = ImVec2(0.0f, 0.0f);
		style.Colors[ImGuiCol_Text] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_TextDisabled] = ImVec4(0.27450982f, 0.31764707f, 0.4509804f, 1.0f);
		style.Colors[ImGuiCol_WindowBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_ChildBg] = ImVec4(0.09411765f, 0.101960786f, 0.11764706f, 1.0f);
		style.Colors[ImGuiCol_PopupBg] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_Border] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
		style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_FrameBg] = ImVec4(0.11372549f, 0.1254902f, 0.15294118f, 1.0f);
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
		style.Colors[ImGuiCol_TitleBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.15686275f, 0.16862746f, 0.19215687f, 1.0f);
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_CheckMark] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_SliderGrab] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(1.0f, 0.79607844f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_Button] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.18039216f, 0.1882353f, 0.19607843f, 1.0f);
		style.Colors[ImGuiCol_ButtonActive] = ImVec4(0.15294118f, 0.15294118f, 0.15294118f, 1.0f);
		style.Colors[ImGuiCol_Header] = ImVec4(0.14117648f, 0.16470589f, 0.20784314f, 1.0f);
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4(0.105882354f, 0.105882354f, 0.105882354f, 1.0f);
		style.Colors[ImGuiCol_HeaderActive] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_Separator] = ImVec4(0.12941177f, 0.14901961f, 0.19215687f, 1.0f);
		style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
		style.Colors[ImGuiCol_SeparatorActive] = ImVec4(0.15686275f, 0.18431373f, 0.2509804f, 1.0f);
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4(0.14509805f, 0.14509805f, 0.14509805f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.972549f, 1.0f, 0.49803922f, 1.0f);
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_Tab] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_TabHovered] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_TabActive] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocused] = ImVec4(0.078431375f, 0.08627451f, 0.101960786f, 1.0f);
		style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.1254902f, 0.27450982f, 0.57254905f, 1.0f);
		style.Colors[ImGuiCol_PlotLines] = ImVec4(0.52156866f, 0.6f, 0.7019608f, 1.0f);
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.039215688f, 0.98039216f, 0.98039216f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4(0.88235295f, 0.79607844f, 0.56078434f, 1.0f);
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.95686275f, 0.95686275f, 0.95686275f, 1.0f);
		style.Colors[ImGuiCol_TableHeaderBg] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
		style.Colors[ImGuiCol_TableBorderStrong] = ImVec4(0.047058824f, 0.05490196f, 0.07058824f, 1.0f);
		style.Colors[ImGuiCol_TableBorderLight] = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
		style.Colors[ImGuiCol_TableRowBg] = ImVec4(0.11764706f, 0.13333334f, 0.14901961f, 1.0f);
		style.Colors[ImGuiCol_TableRowBgAlt] = ImVec4(0.09803922f, 0.105882354f, 0.12156863f, 1.0f);
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.9372549f, 0.9372549f, 0.9372549f, 1.0f);
		style.Colors[ImGuiCol_DragDropTarget] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavHighlight] = ImVec4(0.26666668f, 0.2901961f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.49803922f, 0.5137255f, 1.0f, 1.0f);
		style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);
		style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.19607843f, 0.1764706f, 0.54509807f, 0.5019608f);

	return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void*)
{
	#ifdef __EMSCRIPTEN__
		SDL_SetWindowSize
		(	window,
			emscripten_run_script_int("document.documentElement.clientWidth"),
			emscripten_run_script_int("document.documentElement.clientHeight")
		);
	#endif

	ImGui_ImplSDLRenderer3_NewFrame();
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
{	extern bool(*dial_internal_exit_point)();

	if(dial_internal_exit_point && !dial_internal_exit_point())
	{	fputs("DIAL user exit point failed.", stderr);
		return;
	}

	switch(app_result)
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
