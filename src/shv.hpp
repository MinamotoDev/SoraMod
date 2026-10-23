#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <cstdint>
#include <cstring>
#include <string>
#include <stdexcept>
#include <type_traits>
using U64=std::uint64_t;
struct Vec3 { float x; std::uint32_t px; float y;std::uint32_t py;float z;std::uint32_t pz; };
#ifndef SORAMOD_OFFLINE
extern "C" void shvScriptWait(DWORD);
#endif
namespace SHV {
inline void (*init)(U64);inline void (*push)(U64);inline U64* (*invoke)();
inline void (*wait)(DWORD);inline void (*reg)(HMODULE,void(*)());inline void (*unreg)(HMODULE);
inline int (*texture)(const char*);inline void (*draw)(int,int,int,int,float,float,float,float,float,float,float,float,float,float,float,float);
inline U64* (*global)(int);inline int (*worldVehicles)(int*,int);
template<class T> T address(HMODULE h,const char* n){auto proc=GetProcAddress(h,n);T result;static_assert(sizeof(result)==sizeof(proc));std::memcpy(&result,&proc,sizeof(result));return result;}
inline bool bind(){auto h=GetModuleHandleW(L"ScriptHookV.dll");if(!h)return false;
#define API(v,n) v=address<decltype(v)>(h,n)
API(init,"?nativeInit@@YAX_K@Z");API(push,"?nativePush64@@YAX_K@Z");API(invoke,"?nativeCall@@YAPEA_KXZ");API(wait,"?scriptWait@@YAXK@Z");API(reg,"?scriptRegister@@YAXPEAUHINSTANCE__@@P6AXXZ@Z");API(unreg,"?scriptUnregister@@YAXPEAUHINSTANCE__@@@Z");API(texture,"?createTexture@@YAHPEBD@Z");API(draw,"?drawTexture@@YAXHHHHMMMMMMMMMMMM@Z");API(global,"?getGlobalPtr@@YAPEA_KH@Z");
API(worldVehicles,"?worldGetAllVehicles@@YAHPEAHH@Z");
#ifndef SORAMOD_OFFLINE
wait=shvScriptWait;
#endif
#undef API
return init&&push&&invoke&&wait&&reg&&unreg&&texture&&draw&&global;}
template<class T> U64 argument(T v){U64 r=0;if constexpr(std::is_pointer_v<T>)r=reinterpret_cast<U64>(v);else if constexpr(std::is_floating_point_v<T>){float f=static_cast<float>(v);std::memcpy(&r,&f,4);}else r=static_cast<U64>(v);return r;}
template<class R=void,class... A> R call(U64 hash,A...args){init(hash);(push(argument(args)),...);auto p=invoke();if constexpr(!std::is_void_v<R>){R r{};std::memcpy(&r,p,sizeof(R));return r;}}
}
