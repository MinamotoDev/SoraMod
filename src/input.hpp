#pragma once
#include "shv.hpp"
#include "natives.hpp"
#include "default_controls.hpp"
#include <Xinput.h>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

struct Button {
    bool before=false;
    DWORD next=0;
    bool update(bool down,bool repeat=false){
        DWORD now=GetTickCount();
        bool fire=down&&(!before||(repeat&&static_cast<LONG>(now-next)>=0));
        if(down&&!before)next=now+350;
        else if(fire)next=now+100;
        before=down;
        return fire;
    }
};

struct Input {
    Button toggle,up,down,left,right,accept,close,back,previousPage,nextPage,mouseAccept,mouseBack,previousCategory,nextCategory;
    bool open=false,u=false,d=false,l=false,r=false,a=false,hide=false,b=false,pu=false,pd=false;
    bool click=false,context=false,moved=false,keyboard=true,chord=false;
    int wheel=0;bool tabPrev=false,tabNext=false;WORD heldPad=0;unsigned heldKeys=0;
    float mouseX=0,mouseY=0,lastX=-1,lastY=-1;
    using GetState=DWORD(WINAPI*)(DWORD,XINPUT_STATE*);
    GetState getState=nullptr;
    void init(){
        wchar_t path[MAX_PATH];GetSystemDirectoryW(path,MAX_PATH);
        auto h=LoadLibraryW((std::wstring(path)+L"\\xinput1_4.dll").c_str());
        if(h)getState=SHV::address<GetState>(h,"XInputGetState");
    }
    static bool direction(const XINPUT_GAMEPAD& state,WORD mask,bool connected,bool fallback){return connected?(state.wButtons&mask)!=0:fallback;}
    WORD menuPad(bool paging,bool gallery)const{return heldPad&(0x300f|((paging||gallery||chord)?0x0300:0));}
    bool anyOwnedHeld(bool paging,bool gallery)const{unsigned mask=127|512|1024|2048|4096|(paging?384:0)|(gallery?8192|16384:0);return menuPad(paging,gallery)!=0||(heldKeys&mask)!=0||toggle.before;}
    void poll(bool foreground,int hotkey,bool menuVisible,bool gallery=false){
        WORD buttons=0;bool connected=false;XINPUT_STATE state{};
        if(getState)for(DWORD i=0;i<4;++i)if(getState(i,&state)==ERROR_SUCCESS){
            buttons=state.Gamepad.wButtons;connected=true;break;
        }
        auto key=[&](int vk){return foreground&&(GetAsyncKeyState(vk)&0x8000)!=0;};
        auto control=[&](int action){return foreground&&SHV::call<bool>(N::IS_DISABLED_CONTROL_PRESSED,2,action);};
        auto pad=[&](WORD mask){return foreground&&(buttons&mask)!=0;};
        keyboard=SHV::call<bool>(N::IS_USING_KEYBOARD_AND_MOUSE,0);
        auto frontend=[&](int action){return !keyboard&&control(action);};
        auto dpad=[&](WORD mask,int action){return foreground&&direction(state.Gamepad,mask,connected,frontend(action));};
        heldPad=foreground?buttons:0;
        if(!connected&&!keyboard){
            for(auto pair:std::vector<std::pair<WORD,int>>{{1,232},{2,233},{4,234},{8,235},{4096,223},{8192,225},{256,226},{512,227}})if(control(pair.second))heldPad|=pair.first;
        }
        heldKeys=0;
        for(auto pair:std::vector<std::pair<int,unsigned>>{{VK_UP,1},{VK_DOWN,2},{VK_LEFT,4},{VK_RIGHT,8},{VK_RETURN,16},{VK_ESCAPE,32},{VK_BACK,64},{VK_PRIOR,128},{VK_NEXT,256}})if(key(pair.first))heldKeys|=pair.second;
        if(control(237)||control(238))heldKeys|=512|1024;
        if(gallery&&key(VK_TAB)){heldKeys|=8192;if(key(VK_SHIFT))heldKeys|=16384;}
        chord=(pad(XINPUT_GAMEPAD_RIGHT_SHOULDER)||frontend(206))&&dpad(XINPUT_GAMEPAD_DPAD_RIGHT,235);
        open=toggle.update(key(hotkey)||chord);
        u=up.update(key(VK_UP)||dpad(XINPUT_GAMEPAD_DPAD_UP,232),true);
        d=down.update(key(VK_DOWN)||dpad(XINPUT_GAMEPAD_DPAD_DOWN,233),true);
        l=left.update(key(VK_LEFT)||dpad(XINPUT_GAMEPAD_DPAD_LEFT,234),gallery);
        r=right.update(key(VK_RIGHT)||dpad(XINPUT_GAMEPAD_DPAD_RIGHT,235),true);
        a=accept.update(key(VK_RETURN)||pad(XINPUT_GAMEPAD_A)||frontend(201));
        // Controller B and keyboard Esc return directly to the parent menu.
        hide=close.update(key(VK_ESCAPE)||pad(XINPUT_GAMEPAD_B)||frontend(202));
        b=back.update(key(VK_BACK));
        pu=previousPage.update(key(VK_PRIOR)||(!gallery&&(pad(XINPUT_GAMEPAD_LEFT_SHOULDER)||frontend(205))),true);
        pd=nextPage.update(key(VK_NEXT)||(!gallery&&(pad(XINPUT_GAMEPAD_RIGHT_SHOULDER)||frontend(206))),true);
        tabPrev=previousCategory.update(gallery&&((key(VK_TAB)&&key(VK_SHIFT))||pad(XINPUT_GAMEPAD_LEFT_SHOULDER)||frontend(205)),true);
        tabNext=nextCategory.update(gallery&&((key(VK_TAB)&&!key(VK_SHIFT))||pad(XINPUT_GAMEPAD_RIGHT_SHOULDER)||frontend(206)),true);
        // Cursor actions use the game's own bindings, including any primary/secondary swap.
        click=mouseAccept.update(control(237));
        context=mouseBack.update(control(238));
        wheel=(control(241)?-1:0)+(control(242)?1:0);
        if(control(241))heldKeys|=2048;if(control(242))heldKeys|=4096;
        if(menuVisible&&keyboard){
            mouseX=SHV::call<float>(N::GET_DISABLED_CONTROL_NORMAL,0,239);
            mouseY=SHV::call<float>(N::GET_DISABLED_CONTROL_NORMAL,0,240);
            moved=lastX>=0&&(std::abs(mouseX-lastX)>.0002f||std::abs(mouseY-lastY)>.0002f);
            lastX=mouseX;lastY=mouseY;
        }else{moved=false;lastX=lastY=-1;}
    }
};

// Compare the game's actual binding identifiers, so remapped game actions sharing a
// menu key are consumed without disabling unrelated movement, shooting or driving.
class InputGate {
    std::vector<int> blocked;
    DWORD updated=0;
    bool oldKeyboard=false,oldPaging=false,oldTabbed=false;
    static std::string binding(int action){
        const char* p=SHV::call<const char*>(N::GET_CONTROL_INSTRUCTIONAL_BUTTONS_STRING,0,action,true);
        return p?std::string(p):std::string();
    }
public:
    static bool overlaps(const std::string& value,const std::set<std::string>& reserved){
        return !value.empty()&&reserved.count(value)!=0;
    }
    void apply(bool active,bool priority,bool paging,bool keyboard,int hotkey,bool force=false,WORD heldPad=0,unsigned heldKeys=0,bool tabbed=false){
        if(!active)return;
        if(force||!priority){for(int group=0;group<3;++group)SHV::call(N::DISABLE_ALL_CONTROL_ACTIONS,group);return;}
        if(updated==0||GetTickCount()-updated>1000||keyboard!=oldKeyboard||paging!=oldPaging||tabbed!=oldTabbed){
            std::set<std::string> reserved;
            std::vector<int> menu={187,188,189,190,201,202};
            if(paging){menu.push_back(205);menu.push_back(206);}
            if(tabbed)menu.push_back(204);
            menu.insert(menu.end(),{237,238,241,242});
            if(keyboard)menu.insert(menu.end(),{177,200});
            for(int action:menu){auto value=binding(action);if(!value.empty())reserved.insert(value);}
            if(hotkey>=VK_F1&&hotkey<=VK_F24)reserved.insert("t_F"+std::to_string(hotkey-VK_F1+1));
            blocked.clear();
            for(int action=0;action<=360;++action)if(overlaps(binding(action),reserved))blocked.push_back(action);
            blocked.insert(blocked.end(),menu.begin(),menu.end());
            oldKeyboard=keyboard;oldPaging=paging;oldTabbed=tabbed;updated=GetTickCount();
        }
        for(int group=0;group<3;++group){
            for(int action:blocked)SHV::call(N::DISABLE_CONTROL_ACTION,group,action,true);
            unsigned ownedKeys=heldKeys&~(paging?0u:384u);
            if(keyboard)ownedKeys|=2048|4096; // Suppress every default wheel action, including secondary bindings.
            for(const auto& binding:defaultMenuBindings)if((binding.pad&heldPad)||(binding.keys&ownedKeys))SHV::call(N::DISABLE_CONTROL_ACTION,group,binding.action,true);
            // Mouse motion belongs to the menu cursor; an unused controller right stick remains free.
            if(keyboard)for(int action:{1,2,3,4,5,6,270,271,272,273})SHV::call(N::DISABLE_CONTROL_ACTION,group,action,true);
        }
    }
};
