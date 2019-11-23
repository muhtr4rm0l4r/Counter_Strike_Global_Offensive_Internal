#include "Hooks.hpp"

extern IMGUI_API LRESULT ImGui_ImplDX9_WndProcHandler(HWND, UINT, WPARAM, LPARAM);

decltype(Hooks::Original_Present) Hooks::Original_Present = nullptr;
decltype(Hooks::Original_Reset) Hooks::Original_Reset = nullptr;

HWND Main_Window_Handle = NULL;
WNDPROC Original_Used_Window_Proc = NULL;

BOOL CALLBACK Find_Main_Window_Handle(HWND Window_Handle, LPARAM Process_Identifier)
{
	DWORD Handle_Window_Process_Identifier = NULL;

	GetWindowThreadProcessId(Window_Handle, &Handle_Window_Process_Identifier);

	if (Handle_Window_Process_Identifier != Process_Identifier)
	{
		return TRUE;
	}

	Main_Window_Handle = Window_Handle;

	return FALSE;
}

LRESULT STDMETHODCALLTYPE Hooks::User_Window_Proc(HWND Window_Handle, UINT Message_Type, WPARAM Word_Parameter, LPARAM Long_Parameter)
{
	ImGui_ImplDX9_WndProcHandler(Window_Handle, Message_Type, Word_Parameter, Long_Parameter);

	return CallWindowProc(Original_Used_Window_Proc, Window_Handle, Message_Type, Word_Parameter, Long_Parameter);
}

HRESULT STDMETHODCALLTYPE Hooks::User_Present(IDirect3DDevice9* This_Pointer, const RECT* Source, const RECT* Dest, HWND Window_Override, const RGNDATA* Dirty_Region)
{
	static bool Is_Initialized = false;

	if (!Is_Initialized)
	{
		EnumWindows(Find_Main_Window_Handle, GetCurrentProcessId());

		if (Main_Window_Handle != NULL)
		{
			Original_Used_Window_Proc = reinterpret_cast<WNDPROC>(
				SetWindowLongPtr(Main_Window_Handle, GWLP_WNDPROC, LONG_PTR(Hooks::User_Window_Proc))
				);

			ImGui_ImplDX9_Init(Main_Window_Handle, This_Pointer);
			Is_Initialized = true;
		}
	}
	else
	{
		ImGui_ImplDX9_NewFrame();
		Hooks::Render_Graphical_Interface();
		ImGui::Render();
	}
	return Hooks::Original_Present(This_Pointer, Source, Dest, Window_Override, Dirty_Region);
}

HRESULT STDMETHODCALLTYPE Hooks::User_Reset(IDirect3DDevice9* This_Pointer, D3DPRESENT_PARAMETERS* Parameters)
{
	ImGui_ImplDX9_InvalidateDeviceObjects();
	ImGui_ImplDX9_CreateDeviceObjects();

	return Hooks::Original_Reset(This_Pointer, Parameters);
}