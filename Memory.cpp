#include <iostream>
#include <cstdint>
#include <xmmintrin.h>  
#include <emmintrin.h>
#include <windows.h>
#include <subauth.h>
#include <stdio.h>
#include "Memory.h"

Memory* mem = new Memory();

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

uint64_t Memory::FindModule(const std::string& name)
{
	const auto snap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);
	if (snap == INVALID_HANDLE_VALUE) {
		printf("FindModule INVALID_HANDLE_VALUE\n");
		return 0;
	}

	MODULEENTRY32 module_entry{};
	module_entry.dwSize = sizeof module_entry;

	auto found_module = false;
	if (!!Module32First(snap, &module_entry)) {
		do {
			if (name == module_entry.szModule) {
				printf("Found Module name: %s base: %016llX\n", module_entry.szModule, reinterpret_cast<uint64_t>(module_entry.modBaseAddr));
				found_module = true;
				break;
			}
		} while (!!Module32Next(snap, &module_entry));
	}

	CloseHandle(snap);

	return found_module ? reinterpret_cast<uint64_t>(module_entry.modBaseAddr) : 0;
}