#include "Hooks.hpp"
#include "Source_Development_Kit.hpp"

FORCEINLINE void Hooks::Render_Graphical_Interface()
{
	static bool Checkbox = false;
	static float Slider = 0.f;
	static int Slider_Integer = 0;

	ImGui::Begin("Window");
	{
		ImGui::Checkbox("Checkbox", &Checkbox);
		ImGui::SliderInt("Slider Integer Type", &Slider_Integer, 0, 100);
		ImGui::SliderFloat("Slider", &Slider, 0.f, 420.f);
	}
	ImGui::End();
}

void STDMETHODCALLTYPE Initilization()
{
	std::uintptr_t Present_Address = Utilities::Find_Pattern("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 DB") + 2;
	std::uintptr_t Reset_Address = Utilities::Find_Pattern("gameoverlayrenderer.dll", "FF 15 ? ? ? ? 8B F8 85 FF 78 18") + 2;

	Hooks::Original_Present = **reinterpret_cast<decltype(&Hooks::Original_Present)*>(Present_Address);
	Hooks::Original_Reset = **reinterpret_cast<decltype(&Hooks::Original_Reset)*>(Reset_Address);

	**reinterpret_cast<void***>(Present_Address) = reinterpret_cast<void*>(&Hooks::User_Present);
	**reinterpret_cast<void***>(Reset_Address) = reinterpret_cast<void*>(&Hooks::User_Reset);
}

BOOL WINAPI DllMain(HINSTANCE Library_Instance, DWORD Call_Reason, LPVOID Reserved)
{
	DisableThreadLibraryCalls(Library_Instance);

	if (Call_Reason == DLL_PROCESS_ATTACH)
	{
		CreateThread(0, 0, LPTHREAD_START_ROUTINE(Initilization), 0, 0, 0);
	}
	return TRUE;
}