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
    std::vector<std::string> mods = FileIO::listDirectory(Resource::DIR + "\\Mods\\enabled\\");
    for (auto m : mods)
    {

       
    }
}
Mod* e;
void Modify::loadMod(std::string dir)
{
    std::cout << dir;
    HINSTANCE hGetProcIDDLL = LoadLibrary((LPCSTR)dir.c_str());

    if (!hGetProcIDDLL)
    {
        std::cout << "could not load the dynamic library" << std::endl;
    }

    Mod* (__stdcall * getMod)() = (Mod * (__stdcall *)())GetProcAddress(hGetProcIDDLL, "getMod");

    int* (__stdcall * setInstance)(Auravyx*) = (int* (__stdcall *)(Auravyx*))GetProcAddress(hGetProcIDDLL, "setInstance");

    void* (__stdcall * setContext)() = (void* (__stdcall*)())GetProcAddress(hGetProcIDDLL, "setContext");

    if (!getMod)
    {
        std::cout << "could not locate the function" << dir << " " << std::endl;
    }
    if (!setInstance)
    {
        std::cout << "could not locate the function 2" << dir << " " << std::endl;
    }
    if (!setContext)
    {
        std::cout << "could not locate the function 3" << dir << " " << std::endl;
    }
    Mod* mod = getMod();
    setInstance(Auravyx::getAuravyx());
    //funci2(Auravyx::getAuravyx());
    mod->start();
    //Modify::getModify()->enabledModCount++;
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

