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
