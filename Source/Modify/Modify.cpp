#include "Modify/Modify.h"
#include <Resource.h>
#include <Auravyx.h>
#include <thread>
#include <filesystem>

void sayHi()
{

}
typedef int(__stdcall* DLLFunc)(Auravyx*);
typedef int(__stdcall* DLLFunc2)();
DLLFunc funci;
DLLFunc2 funci2;
void Modify::loadAllMods()
{
    std::vector<std::string> mods = FileIO::listDirectory(Resource::DIR + "\\Mods\\enabled\\");
    for (auto m : mods)
    {
        if (std::filesystem::is_directory(m))
        {
            std::vector<std::string> confFiles = FileIO::listDirectory(m, "conf");
            std::vector<std::string> dllFiles = FileIO::listDirectory(m, "dlls");
            std::vector<std::string> soFiles = FileIO::listDirectory(m, "so");
            std::string modName = "? Mod Name ?";
            std::string modVersion = "? Mod Version ?";
            std::string gameVersion = "? Game Version ?";

            if (confFiles.size() >= 1)
            {
                std::map<std::string, std::string> configEntries = FileIO::readConfig(confFiles.at(0));
                std::map<std::string, std::string>::const_iterator modNameIter = configEntries.find("mod-name");
                std::map<std::string, std::string>::const_iterator modVersionIter = configEntries.find("mod-version");
                std::map<std::string, std::string>::const_iterator modGameVersionIter = configEntries.find("game-version");
                if (modNameIter != configEntries.end())
                {
                    modName = modNameIter->second;
                }
                if (modVersionIter != configEntries.end())
                {
                    modVersion = modVersionIter->second;
                }
                if (modGameVersionIter != configEntries.end())
                {
                    gameVersion = modGameVersionIter->second;
                }
            }
            else
            {
                if (dllFiles.size() >= 1)
                {
                    modName = FileIO::getFileNameNoEXT(dllFiles.at(0));
                }
            }
            /*HINSTANCE hGetProcIDDLL = LoadLibrary((LPCSTR)m.c_str());

            if (!hGetProcIDDLL)
            {
                std::cout << "could not load the dynamic library" << std::endl;
            }

            funci = (DLLFunc)GetProcAddress(hGetProcIDDLL, "begin");// ? start@TestMod@@AEAAXXZ");
            //funci2 = (DLLFunc2)GetProcAddress(hGetProcIDDLL, "epic");// ? start@TestMod@@AEAAXXZ");
            //funci2 = (DLLFunc)GetProcAddress(hGetProcIDDLL, "poop");
            if (!funci)
            {
                std::cout << "could not locate the function" << m << " " << std::endl;
            }
            if (!funci2)
            {
                std::cout << "could not locate the function" << m << " " << std::endl;
            }*/

            //Auravyx::getAuravyx()->getModify()->enabledModCount++;
        //funci(Auravyx::getAuravyx());
        }
    }
}

void Modify::render()
{
    //funci(Auravyx::getAuravyx());
    if (Auravyx::getAuravyx()->getWindow()->getController()->isKeyDown(GLFW_KEY_1))
    {
        //printf("down");
    }
    else
    {

    }
}

int Modify::getEnabledModCount()
{
    return enabledModCount;
}

int Modify::getDisabledModCount()
{
    return disabledModCount;
}

