#pragma once
#include "Includes.hpp"

namespace Hooks 
{
	extern HRESULT (STDMETHODCALLTYPE* Original_Present) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	extern HRESULT (STDMETHODCALLTYPE* Original_Reset) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
	
	extern HRESULT (STDMETHODCALLTYPE User_Present) (IDirect3DDevice9*, const RECT*, const RECT*, HWND, const RGNDATA*);
	extern HRESULT (STDMETHODCALLTYPE User_Reset) (IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);

	extern LRESULT (STDMETHODCALLTYPE User_Window_Proc) (HWND, UINT, WPARAM, LPARAM);

	extern FORCEINLINE void (Render_Graphical_Interface) ();
};