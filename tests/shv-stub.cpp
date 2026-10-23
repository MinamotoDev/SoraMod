#include <windows.h>
#include <cstdint>
static int registrations=0;
extern "C" void fakeInit(std::uint64_t){}
extern "C" void fakePush(std::uint64_t){}
extern "C" std::uint64_t* fakeCall(){static std::uint64_t r[4]{};return r;}
extern "C" void fakeWait(DWORD){}
extern "C" void fakeRegister(HMODULE,void(*)()){++registrations;}
extern "C" void fakeUnregister(HMODULE){--registrations;}
extern "C" int fakeTexture(const char*){return 1;}
extern "C" void fakeDraw(int,int,int,int,float,float,float,float,float,float,float,float,float,float,float,float){}
extern "C" std::uint64_t* fakeGlobal(int){static std::uint64_t v=0;return &v;}
extern "C" __declspec(dllexport) int RegistrationCount(){return registrations;}
