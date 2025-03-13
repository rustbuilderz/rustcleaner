#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <thread>
#include <chrono>
#include <windows.h>
#include <shlobj.h>
#include <cstdlib>

void slowtype(const std::string& text, int delay = 50) {
    for (char c : text) {
        std::cout << c << std::flush;
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
    std::cout << std::endl;
}

void loading_dots(const std::string& text = "Loading", int delay = 500, int cycles = 3) {
    for (int i = 0; i < cycles; ++i) {
        std::cout << "\r" << text << "...";
        std::cout.flush();
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
        std::cout << "\r" << std::string(text.length(), ' ') << "\r";
    }
}

bool isAdmin() {
    BOOL isAdmin = FALSE;
    PSID administratorsGroup;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
        DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &administratorsGroup)) {
        CheckTokenMembership(NULL, administratorsGroup, &isAdmin);
        FreeSid(administratorsGroup);
    }

    return isAdmin;
}

bool registryKeyExists(const std::string& key) {
    std::string command = "reg query \"" + key + "\"";
    return system(command.c_str()) == 0;
}

void cleaner() {
    const std::vector<std::string> directories = {
        "C:\\Program Files (x86)\\Steam\\userdata",
        "C:\\Program Files (x86)\\Steam\\appcache",
        "C:\\Program Files (x86)\\Steam\\config",
        "C:\\Program Files (x86)\\Steam\\logs",
        "C:\\Program Files (x86)\\Steam\\dumps",
        "C:\\Program Files (x86)\\Steam\\temp"
    };

    loading_dots("[+] Loading..");
    system("cls");

    for (const auto& dir_path : directories) {
        try {
            if (std::filesystem::exists(dir_path)) {
                for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
                    try {
                        if (std::filesystem::is_directory(entry)) {
                            std::filesystem::remove_all(entry);
                            std::cout << "[+] Deleted directory " << entry.path() << std::endl;
                        }
                        else {
                            std::filesystem::remove(entry);
                            std::cout << "[+] Deleted file " << entry.path() << std::endl;
                        }
                    }
                    catch (const std::filesystem::filesystem_error& e) {
                        std::cout << "[!] Permission denied for " << entry.path() << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                    }
                }
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << dir_path << " not found" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    std::string key1 = "HKEY_CURRENT_USER\\SOFTWARE\\Facepunch Studios LTD\\Rust";
    if (registryKeyExists(key1)) {
        if (system(("reg delete \"" + key1 + "\" /f").c_str()) == 0) {
            std::cout << "[+] Deleted registry key " << key1 << std::endl;
        }
        else {
            std::cout << "[-] Failed to delete registry key " << key1 << std::endl;
        }
    }
    else {
        std::cout << "[-] Registry key not found: " << key1 << std::endl;
    }

    std::string key2 = "HKEY_CURRENT_USER\\SOFTWARE\\Valve\\Steam\\Users";
    if (registryKeyExists(key2)) {
        if (system(("reg delete \"" + key2 + "\" /f").c_str()) == 0) {
            std::cout << "[+] Deleted registry key " << key2 << std::endl;
        }
        else {
            std::cout << "[-] Failed to delete registry key " << key2 << std::endl;
        }
    }
    else {
        std::cout << "[-] Registry key not found: " << key2 << std::endl;
    }

    std::string file_path = "C:\\Eos_seed.bin";
    if (std::filesystem::exists(file_path)) {
        try {
            if (std::filesystem::remove(file_path)) {
                std::cout << "[+] Deleted " << file_path << std::endl;
            }
            else {
                std::cout << "[-] Failed to delete " << file_path << std::endl;
            }
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cout << "[-] Error while deleting " << file_path << ": " << e.what() << std::endl;
        }
    }
    else {
        std::cout << "[-] File not found: " << file_path << std::endl;
    }
}

void exit_program() {
    system("cls");
    slowtype("[+] Everything should be cleaned.");
    slowtype("[!] Closing :D");
    std::this_thread::sleep_for(std::chrono::seconds(3));
}

int main() {
    if (!isAdmin()) {
        std::cout << "This script requires administrator privileges. Please run it as an administrator." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        return 0;
    }


    slowtype("[!] Steam cleaner by rustbuilderz.\n");

    slowtype("[!] Press Enter to continue..");
    std::cin.get();
    system("cls");

    slowtype("[!] WARNING: please log out of Steam and press Enter again.");
    std::cin.get();

    slowtype("[+] Please wait");
    std::this_thread::sleep_for(std::chrono::seconds(2));

    system("taskkill /im steam.exe /f");
    system("cls");

    cleaner();
    exit_program();

    return 0;
}
