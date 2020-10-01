#include "Modify/Modify.h"
#include <Utilities/Resource.h>
#include <Auravyx.h>
#include <thread>
#include <filesystem>

#ifdef __linux__ 
#elif _WIN32
#include <Windows.h>
#else
#endif
#include <Utilities\FileIO.h>
#include <Utilities\Log.h>

void sayHi()
{

}
typedef Mod*(__stdcall* DLLFunc)();

Modify* Modify::modify;

Modify::Modify()
{
}
Modify::~Modify()
{
}
Modify::Modify(Modify* m)
{
    modify = m;
}
void Modify::loadAllMods()
{
    std::vector<std::string> mods = FileIO::listDirectory(Resource::getResources()->DIR + "\\Mods\\enabled\\");
    for (auto m : mods)
    {

       
    }
}
void Modify::loadMod(std::string dir)
{
    HINSTANCE hGetProcIDDLL = LoadLibrary((LPCSTR)dir.c_str());
    if (!hGetProcIDDLL)
    {
        std::cout << "could not load the dynamic library" << std::endl;
    }
    Mod* (__stdcall * getMod)() = (Mod * (__stdcall *)())GetProcAddress(hGetProcIDDLL, "getMod");

    int* (__stdcall * setInstance)(Auravyx*) = (int* (__stdcall *)(Auravyx*))GetProcAddress(hGetProcIDDLL, "setInstance");

    void* (__stdcall * setContext)() = (void* (__stdcall*)())GetProcAddress(hGetProcIDDLL, "setContext");

    bool errorFlag = false;

    if (!getMod)
    {
        errorFlag = true;
        Log::out("Modify", "Could not find getMod!", LIGHT_GRAY);
    }
    if (!setInstance)
    {
        errorFlag = true;
        Log::out("Modify", "Could not find setInstance!", LIGHT_GRAY);
    }
    if (!setContext)
    {
        errorFlag = true;
        Log::out("Modify", "Could not find setContext!", LIGHT_GRAY);
    }
    if (errorFlag)
    {
        Log::out("Modify", "Loading mod aborted!", LIGHT_GRAY);
        return;
    }
    enabledModCount++;
    Mod* mod = getMod();
    setInstance(Auravyx::getAuravyx());
    mod->start();
}

void Modify::unloadMod(int id)
{
}

void Modify::render()
{
    //funci2(Auravyx::getAuravyx());
    //funci(Auravyx::getAuravyx());
}

int Modify::getEnabledModCount()
{
    return enabledModCount;
}

int Modify::getDisabledModCount()
{
    return disabledModCount;
}

Modify* Modify::getModify()
{
    return modify;
}

