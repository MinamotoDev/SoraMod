#pragma once
#include "shv.hpp"
#include <vector>
#include <array>
#include <fstream>
#include <algorithm>
// Script table and bytecode layouts are resolved from the running Enhanced build.
// No version-specific global index is hardcoded, and every read/write is bounded.
class MpVehicles {
    std::uintptr_t table=0;int index=-1,previous=0;bool changed=false;
    template<class T> static bool read(std::uintptr_t at,T& out){SIZE_T n=0;return at&&ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(at),&out,sizeof(out),&n)&&n==sizeof(out);}
    static bool bytes(std::uintptr_t at,void* out,size_t count){SIZE_T n=0;return at&&ReadProcessMemory(GetCurrentProcess(),reinterpret_cast<void*>(at),out,count,&n)&&n==count;}
    static std::vector<size_t> find(const std::vector<unsigned char>& b,const std::vector<int>& p){std::vector<size_t> out;for(size_t i=0;i+p.size()<=b.size();++i){bool ok=true;for(size_t j=0;j<p.size();++j)if(p[j]>=0&&b[i+j]!=p[j]){ok=false;break;}if(ok)out.push_back(i);}return out;}
    bool locateTable(){auto base=reinterpret_cast<std::uintptr_t>(GetModuleHandleW(nullptr));IMAGE_DOS_HEADER dos{};IMAGE_NT_HEADERS64 nt{};if(!read(base,dos)||!read(base+dos.e_lfanew,nt)||nt.Signature!=IMAGE_NT_SIGNATURE)return false;
        size_t hits=0;std::uintptr_t result=0;for(int i=0;i<nt.FileHeader.NumberOfSections;++i){IMAGE_SECTION_HEADER s{};auto at=base+dos.e_lfanew+offsetof(IMAGE_NT_HEADERS64,OptionalHeader)+nt.FileHeader.SizeOfOptionalHeader+i*sizeof(s);if(!read(at,s))return false;if(!(s.Characteristics&IMAGE_SCN_MEM_EXECUTE)||s.Misc.VirtualSize>128*1024*1024)continue;
            std::vector<unsigned char> code(s.Misc.VirtualSize);if(!bytes(base+s.VirtualAddress,code.data(),code.size()))continue;
            for(auto pos:find(code,{0x48,0x03,0x05,-1,-1,-1,-1,0x4c,0x85,0xc0,0x0f,0x84,-1,-1,-1,-1,0xe9})){std::int32_t relative;std::memcpy(&relative,code.data()+pos+3,4);result=base+s.VirtualAddress+pos+7+relative;++hits;}}
        if(hits!=1)return false;table=result;return true;}
public:
    static int decodeGlobal(const std::vector<unsigned char>& code){
        int index=-1,matches=0;
        for(auto p:find(code,{0x2d,-1,-1,-1,-1,0x2c,-1,-1,-1,0x56,-1,-1,0x71,0x2e,-1,-1,0x62})){
            if(p+20>code.size())continue;
            index=code[p+17]|code[p+18]<<8|code[p+19]<<16;++matches;
        }
        return matches==1&&index>0?index:-1;
    }
    bool enable(){if(index>=0){auto p=SHV::global(index);int v=0;if(!read(reinterpret_cast<std::uintptr_t>(p),v)||v<0||v>1)return false;int one=1;SIZE_T n=0;return WriteProcessMemory(GetCurrentProcess(),p,&one,4,&n)&&n==4;}
        if(!table&&!locateTable())return false;std::uintptr_t items=0;int count=0;if(!read(table,items)||!read(table+24,count)||count<=0||count>10000)return false;
        for(int i=0;i<count;++i){std::uintptr_t header=0;std::uint32_t hash=0;auto item=items+i*16;if(!read(item,header)||!read(item+12,hash)||hash!=0x39da738b)continue;
            std::uintptr_t pages=0;std::uint32_t length=0,ownHash=0;if(!read(header+16,pages)||!read(header+28,length)||!read(header+88,ownHash)||ownHash!=hash||length==0||length>16*1024*1024)return false;
            std::vector<unsigned char> code(length);
            for(std::uint32_t page=0;page<(length+0x3fff)/0x4000;++page){
                std::uintptr_t address=0;if(!read(pages+page*8,address))return false;
                size_t size=std::min<std::uint32_t>(0x4000,length-page*0x4000);
                if(!bytes(address,code.data()+page*0x4000,size))return false;
            }
            int found=decodeGlobal(code);if(found<0)return false;auto ptr=SHV::global(found);if(!read(reinterpret_cast<std::uintptr_t>(ptr),previous)||previous<0||previous>1)return false;index=found;changed=previous==0;return enable();}
        return false;}
    void restore(){if(changed&&index>=0){auto p=SHV::global(index);int value=0;SIZE_T n=0;if(read(reinterpret_cast<std::uintptr_t>(p),value)&&value==1)WriteProcessMemory(GetCurrentProcess(),p,&previous,4,&n);}changed=false;index=-1;}
};
