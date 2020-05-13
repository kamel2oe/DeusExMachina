#include <iostream>
#include <cstdint>
#include <xmmintrin.h>  
#include <emmintrin.h>
#include <windows.h>
#include <subauth.h>
#include <stdio.h>
#include "Memory.h"

Memory::Memory()
{
}

Memory::~Memory()
{
	CloseHandle(handle);
}

HANDLE Memory::Open()
{
	handle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, process_id);
	return handle;
}

uint32_t Memory::FindProcess(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (snap == INVALID_HANDLE_VALUE) {
		return 0;
	}

	PROCESSENTRY32 proc_entry{};
	proc_entry.dwSize = sizeof proc_entry;

	auto found_process = false;

	if (!!Process32First(snap, &proc_entry)) {
		do {
			if (name == proc_entry.szExeFile) {
				printf("Found Process name: %s pid: %i\n", proc_entry.szExeFile, proc_entry.th32ProcessID);
				found_process = true;
				process_id = proc_entry.th32ProcessID;
				break;
			}
		} while (!!Process32Next(snap, &proc_entry));
	}

	return found_process ? proc_entry.th32ProcessID : 0;
}

MODULEENTRY32 Memory::FindModule(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
	if (snap == INVALID_HANDLE_VALUE) {
		printf("FindModule INVALID_HANDLE_VALUE\n");
		return MODULEENTRY32{};
	}

	MODULEENTRY32 module_entry{};
	module_entry.dwSize = sizeof module_entry;

	auto found_module = false;
	if (!!Module32First(snap, &module_entry)) {
		do {
			if (name == module_entry.szModule) {
				printf("Found Module name: %s base: %X\n", module_entry.szModule, reinterpret_cast<uintptr_t>(module_entry.modBaseAddr));
				found_module = true;
				break;
			}
		} while (!!Module32Next(snap, &module_entry));
	}

	CloseHandle(snap);

	return found_module ? module_entry : MODULEENTRY32{};
}


uintptr_t Memory::FindPattern(MODULEENTRY32 module, const char* pattern, const char* mask) {
	SIZE_T bytesRead;
	DWORD oldprotect;
	MEMORY_BASIC_INFORMATION mbi = { 0 };

	auto scan = [](const char* pattern, const char* mask, char* begin, unsigned int size) -> char* {
		size_t patternLen = strlen(mask);
		for (unsigned int i = 0; i < size - patternLen; i++) {
			bool found = true;
			for (unsigned int j = 0; j < patternLen; j++) {
				if (mask[j] != '?' && pattern[j] != *(begin + i + j)) {
					found = false;
					break;
				}
			}
			if (found) { return (begin + i); }
		}
		return nullptr;
	};

	for (uintptr_t curr = (uintptr_t)module.modBaseAddr; curr < (uintptr_t)module.modBaseAddr + module.modBaseSize; curr += mbi.RegionSize) {
		if (!VirtualQueryEx(handle, (void*)curr, &mbi, sizeof(mbi))) continue;
		if (mbi.State != MEM_COMMIT || mbi.Protect == PAGE_NOACCESS) continue;

		char* buffer = new char[mbi.RegionSize];

		if (VirtualProtectEx(handle, mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &oldprotect)) {
			ReadProcessMemory(handle, mbi.BaseAddress, buffer, mbi.RegionSize, &bytesRead);
			VirtualProtectEx(handle, mbi.BaseAddress, mbi.RegionSize, oldprotect, &oldprotect);

			char* internalAddr = scan(pattern, mask, buffer, bytesRead);

			if (internalAddr != nullptr) { return curr + (uintptr_t)(internalAddr - buffer); }
		}
		delete[] buffer;
	}
	return 0x0;
}
