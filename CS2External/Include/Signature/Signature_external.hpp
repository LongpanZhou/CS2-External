#include <Windows.h>
#include <vector>
#include <Psapi.h>
#include <iostream>

class Signature
{
private:
    std::vector<unsigned char> patternToBytes(const char* pattern)
    {
        std::vector<unsigned char> bytes;
        char* start = (char*)pattern;
        char* end = (char*)pattern + strlen(pattern);

        for (char* current = start; current < end; current++)
        {
            if (*current == '?')
            {
                ++current;
                if (*current == '?')
                    current++;
                bytes.push_back('\?');
            }
            else
            {
                bytes.push_back(strtoul(current, &current, 16));
            }
        }
        return bytes;
    }

public:
    std::vector<unsigned char> readBytes(HANDLE hProcess, uintptr_t pAddress, size_t length)
    {
        std::vector<unsigned char> cBytes(length);
        SIZE_T bytesRead;
        if (!ReadProcessMemory(hProcess, (LPCVOID)pAddress, cBytes.data(), length, &bytesRead) || bytesRead != length)
        {
            std::cerr << "Failed to read memory at address: " << std::hex << pAddress
                << ". Error Code: " << GetLastError() << std::endl;
            cBytes.clear();
        }
        return cBytes;
    }

    bool patchBytes(HANDLE hProcess, uintptr_t pAddress, const char* patternBytes)
    {
        DWORD pOld;
        std::vector<unsigned char> cBytes = patternToBytes(patternBytes);

        if (!VirtualProtectEx(hProcess, (LPVOID)pAddress, cBytes.size(), PAGE_EXECUTE_READWRITE, &pOld))
        {
            std::cerr << "Failed to change memory protection at address: " << std::hex << pAddress
                << ". Error Code: " << GetLastError() << std::endl;
            return false;
        }

        SIZE_T bytesWritten;
        if (!WriteProcessMemory(hProcess, (LPVOID)pAddress, cBytes.data(), cBytes.size(), &bytesWritten) || bytesWritten != cBytes.size())
        {
            std::cerr << "Failed to write memory at address: " << std::hex << pAddress
                << ". Error Code: " << GetLastError() << std::endl;
            return false;
        }

        VirtualProtectEx(hProcess, (LPVOID)pAddress, cBytes.size(), pOld, NULL);
        return true;
    }

    uintptr_t signatureScanner(HANDLE hProcess, HMODULE hModule, const char* pattern)
    {
        std::vector<unsigned char> patternBytes = patternToBytes(pattern);

        MODULEINFO moduleInfo;
        if (!GetModuleInformation(hProcess, hModule, &moduleInfo, sizeof(MODULEINFO)))
        {
            std::cerr << "Failed to retrieve module information. Error Code: " << GetLastError() << std::endl;
            return 0;
        }

        IMAGE_DOS_HEADER dosHeader;
        if (!ReadProcessMemory(hProcess, moduleInfo.lpBaseOfDll, &dosHeader, sizeof(IMAGE_DOS_HEADER), nullptr) ||
            dosHeader.e_magic != IMAGE_DOS_SIGNATURE)
        {
            std::cerr << "Invalid DOS header at base address: " << std::hex << moduleInfo.lpBaseOfDll
                << ". Error Code: " << GetLastError() << std::endl;
            return 0;
        }

        IMAGE_NT_HEADERS ntHeader;
        BYTE* ntHeaderAddress = (BYTE*)moduleInfo.lpBaseOfDll + dosHeader.e_lfanew;
        if (!ReadProcessMemory(hProcess, ntHeaderAddress, &ntHeader, sizeof(IMAGE_NT_HEADERS), nullptr) ||
            ntHeader.Signature != IMAGE_NT_SIGNATURE)
        {
            std::cerr << "Invalid NT header at address: " << std::hex << (uintptr_t)ntHeaderAddress
                << ". Error Code: " << GetLastError() << std::endl;
            return 0;
        }

        uintptr_t PeStart = (uintptr_t)moduleInfo.lpBaseOfDll + ntHeader.OptionalHeader.BaseOfCode;
        uintptr_t PeEnd = PeStart + ntHeader.OptionalHeader.SizeOfCode;

        for (uintptr_t i = PeStart; i < PeEnd - patternBytes.size(); i++)
        {
            bool pFound = true;

            for (size_t j = 0; j < patternBytes.size(); ++j)
            {
                unsigned char byte;
                if (!ReadProcessMemory(hProcess, (LPCVOID)(i + j), &byte, sizeof(byte), nullptr) ||
                    (patternBytes[j] != '\?' && patternBytes[j] != byte))
                {
                    pFound = false;
                    break;
                }
            }

            if (pFound)
                return i;
        }

        return 0;
    }
};

inline Signature signature;
