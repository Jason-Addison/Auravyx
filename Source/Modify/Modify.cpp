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
int Modify::loadMod(const std::string& dir)
{
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

