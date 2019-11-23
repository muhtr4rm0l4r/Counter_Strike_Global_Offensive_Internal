#include "Includes.hpp"

#define IN_RANGE(X, A,B) (X >= A && X <= B)
#define Get_Bits(X) (IN_RANGE((X & (~0x20)), 'A', 'F') ? ((X & (~0x20)) - 'A' + 0xA): (IN_RANGE(X, '0', '9') ? X - '0': 0))
#define Get_Byte(X) (Get_Bits(X[0]) << 4 | Get_Bits(X[1]))

namespace Utilities
{
	static uintptr_t Find_Pattern(const uintptr_t& Start_Address, const uintptr_t& End_Address, const char* Pattern_Target)
	{
		const char* Pattern = Pattern_Target;

		uintptr_t First_Match = 0;

		for (uintptr_t Position = Start_Address; Position < End_Address; Position++)
		{
			if (!*Pattern)
			{
				return First_Match;
			}

			const uint8_t Current_Pattern = *reinterpret_cast<const uint8_t*>(Pattern);
			const uint8_t Current_Memory = *reinterpret_cast<const uint8_t*>(Position);

			if (Current_Pattern == '\?' || Current_Memory == Get_Byte(Pattern))
			{
				if (!First_Match)
				{
					First_Match = Position;
				}

				if (!Pattern[2])
				{
					return First_Match;
				}

				Pattern += Current_Pattern != '\?' ? 3 : 2;
			}
			else
			{
				Pattern = Pattern_Target;
				First_Match = 0;
			}
		}
		return NULL;
	}

	static uintptr_t Find_Pattern(const char* Module, const char* Pattern_Target)
	{
		MODULEINFO Module_Information = { 0 };

		if (!GetModuleInformation(GetCurrentProcess(), GetModuleHandleA(Module), &Module_Information, sizeof(MODULEINFO)))
		{
			return NULL;
		}

		const uintptr_t Start_Address = uintptr_t(Module_Information.lpBaseOfDll);
		const uintptr_t End_Address = Start_Address + Module_Information.SizeOfImage;

		return Find_Pattern(Start_Address, End_Address, Pattern_Target);
	}
};