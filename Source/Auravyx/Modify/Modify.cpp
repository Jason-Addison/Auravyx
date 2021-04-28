#include "Auravyx/Modify/Modify.h"
#include <Auravyx/Utility/IO/Resource.h>
#include <thread>
#include <filesystem>
#include "Auravyx/Modify/Mod.h"

#ifdef __linux__ 
#elif _WIN32
#include <Windows.h>
#else
#endif
#include <Auravyx/Utility/IO/FileIO.h>
#include <Logger/Log.h>

void sayHi()
{

}
#ifdef _WIN32
#else
#define __stdcall
#endif
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
    std::vector<std::string> mods = FileIO::listDirectory(Resource::getInstance().DIR + "/Mods/Enabled/");
    for (auto m : mods)
    {

       
    }
}
int Modify::loadMod(const std::string& dir)
{
#ifdef _WIN32
    HINSTANCE hGetProcIDDLL = LoadLibrary((LPCSTR)dir.c_str());
    if (!hGetProcIDDLL)
    {
        Log::out("         - [!] Could not load mod library.", RED);
        return 1;
    }
    Mod* (__stdcall * getMod)() = (Mod * (__stdcall *)())GetProcAddress(hGetProcIDDLL, "getMod");


    //@TODO Change to singleton
    //int* (__stdcall * setInstance)(Auravyx*) = (int* (__stdcall *)(Auravyx*))GetProcAddress(hGetProcIDDLL, "setInstance");

    void* (__stdcall * setContext)() = (void* (__stdcall*)())GetProcAddress(hGetProcIDDLL, "setContext");

    bool errorFlag = false;

    if (!getMod)
    {
        errorFlag = true;
        Log::out("         - [!] Could not find getMod.", RED);
    }
    /*if (!setInstance)
    {
        errorFlag = true;
        Log::out("         - [!] Could not find setInstance.", RED);
    }*/
    if (!setContext)
    {
        errorFlag = true;
        Log::out("         - [!] Could not find setContext.", RED);
    }
    if (errorFlag)
    {
        return 1;
    }
    enabledModCount++;
    Mod* mod = getMod();
    //setInstance(Auravyx::getInstance());
    mod->start();
#endif
    return 0;
}

void Modify::unloadMod(const int id)
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

