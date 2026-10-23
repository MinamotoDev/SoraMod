#include "font.hpp"
#include "mp_vehicles.hpp"
#include "natives.hpp"
#include <Xinput.h>
#include <filesystem>
#include <set>
#include <deque>
#include <array>
#include <type_traits>
#include <random>
namespace fs=std::filesystem;
using SHV::call;
HMODULE module;
std::string root;
const std::array<std::string,13> languages={"en","fr","it","de","es","ja","ko","pl","pt-BR","ru","zh-Hans","es-419","zh-Hant"};
int language=0;
std::unordered_map<std::string,std::string> strings;
std::string tr(const std::string& s){auto i=strings.find(s);return i==strings.end()?s:i->second;}
void log(const std::string& s){std::ofstream(root+"/SoraMod.log",std::ios::app)<<s<<'\n';}
std::vector<std::string> split(const std::string& s,char sep){std::vector<std::string> out;std::stringstream in(s);std::string v;while(std::getline(in,v,sep))out.push_back(v);return out;}
std::uint32_t hash(const std::string& s){std::uint32_t h=0;for(unsigned char c:s){h+=static_cast<unsigned char>(std::tolower(c));h+=h<<10;h^=h>>6;}h+=h<<3;h^=h>>11;h+=h<<15;return h;}
struct Vehicle{std::string model,name;std::uint32_t id;bool online;int category=0;};
std::vector<Vehicle> vehicles;std::vector<int> all;
std::array<std::vector<int>,23> classified;
std::vector<int> availableClasses;
Font font;MpVehicles mp;
std::mt19937 randomEngine{std::random_device{}()};
int pickSurpriseIndex(std::mt19937& engine,const std::vector<int>& options){return options.empty()?-1:options[std::uniform_int_distribution<size_t>(0,options.size()-1)(engine)];}
bool visible=false,god=false,frozen=false,weatherOwned=false;int godPed=0;bool oldGod=false;
int page=0,selected=0,hour=12,weather=0;
int category=0,galleryCategory=-1;bool priority=true,galleryPaused=false,releaseShield=false;
struct Location{int page,selected,category;};
std::vector<Location> history;
bool isGrid(){return page==3;}
bool isVehiclePage(){return page==3||page==5;}
const std::vector<int>& vehicleList(){return page==5?classified[category]:page==3&&galleryCategory>=0?classified[galleryCategory]:all;}
void enterPage(int next,int nextCategory=0){history.push_back({page,selected,category});page=next;category=nextCategory;selected=0;}
void setMenuVisible(bool value);
void goBack(){if(history.empty()){setMenuVisible(false);return;}auto old=history.back();history.pop_back();page=old.page;selected=old.selected;category=old.category;}
void syncGalleryPause(bool wanted){if(wanted!=galleryPaused){call(N::SET_GAME_PAUSED,wanted);galleryPaused=wanted;}}
DWORD messageUntil=0,blockUntil=0;std::string message;
void setMenuVisible(bool value){visible=value;if(!value)releaseShield=true;if(!value)syncGalleryPause(false);blockUntil=GetTickCount()+200;}
void openMenu(){while(isGrid()&&!history.empty())goBack();if(isGrid()){page=0;selected=5;category=0;}setMenuVisible(true);}
void toggleMenu(){if(visible)setMenuVisible(false);else openMenu();}
void dismissMenu(){goBack();syncGalleryPause(false);blockUntil=GetTickCount()+200;}
int hotkey=VK_F7;
void note(const std::string& key){message=tr(key);messageUntil=GetTickCount()+3500;log(key);}
float canvasWidth=1920;
int pageCapacity(){return isGrid()?std::clamp((int)((canvasWidth-120)/420),2,6)*3:10;}
int pinnedCount();
int itemCount(){return isVehiclePage()?(int)vehicleList().size():page==4?(int)availableClasses.size():page==1?4:page==6?3:page==7?pinnedCount():page==8?3:page==9?2:12;}
bool hasPages(){return itemCount()>pageCapacity();}
void changePage(int delta){int total=itemCount(),capacity=pageCapacity();if(total==0)return;int current=selected/capacity,target=std::clamp(current+delta,0,(total-1)/capacity);if(target!=current)selected=std::min(total-1,target*capacity+selected%capacity);}
void moveGrid(int dx,int dy,int total){
 if(total<=0)return;int cols=pageCapacity()/3;
 if(dy){int row=selected/cols+dy;if(row>=0&&row<=(total-1)/cols)selected=std::min(total-1,row*cols+selected%cols);}
 if(dx)selected=std::clamp(selected+dx,0,total-1);
}
void previousLayer(){if(selected/pageCapacity()>0)changePage(-1);else goBack();blockUntil=GetTickCount()+200;}
void changeGalleryTab(int delta){std::vector<int> tabs={-1};tabs.insert(tabs.end(),availableClasses.begin(),availableClasses.end());auto found=std::find(tabs.begin(),tabs.end(),galleryCategory);int current=found==tabs.end()?0:(int)(found-tabs.begin());galleryCategory=tabs[(current+delta+(int)tabs.size())%tabs.size()];selected=0;}
void rect(float x,float y,float w,float h,int r,int g,int b,int a){call(N::DRAW_RECT,(x+w/2)/canvasWidth,(y+h/2)/1080.f,w/canvasWidth,h/1080.f,r,g,b,a,false);}
#include "input.hpp"
Input input;InputGate gate;
struct Pending{int kind=0;DWORD start=0;std::uint32_t model=0;float x=0,y=0;bool online=false;} pending;
std::deque<int> spawned;
bool isPinned(int entity);
struct FallingVehicle{int entity;DWORD protectUntil;};std::vector<FallingVehicle> falling;
float dropDistance(const Vec3& minimum,const Vec3& maximum){return std::max(7.f,std::max({std::abs(minimum.x),std::abs(maximum.x),std::abs(minimum.y),std::abs(maximum.y)})+4.f);}
void tickFalling(){for(auto i=falling.begin();i!=falling.end();){if(!call<bool>(N::DOES_ENTITY_EXIST,i->entity)){i=falling.erase(i);continue;}if(static_cast<LONG>(GetTickCount()-i->protectUntil)>=0&&(!call<bool>(N::IS_ENTITY_IN_AIR,i->entity)||GetTickCount()-i->protectUntil>25000)){call(N::SET_ENTITY_INVINCIBLE,i->entity,false,false);i=falling.erase(i);}else ++i;}}
void restoreGod(){if(godPed&&call<bool>(N::DOES_ENTITY_EXIST,godPed))call(N::SET_ENTITY_INVINCIBLE,godPed,oldGod,false);godPed=0;}
void cleanup(){for(auto& item:falling)if(call<bool>(N::DOES_ENTITY_EXIST,item.entity))call(N::SET_ENTITY_INVINCIBLE,item.entity,false,false);falling.clear();syncGalleryPause(false);restoreGod();god=false;if(frozen)call(N::PAUSE_CLOCK,false);frozen=false;if(weatherOwned)call(N::CLEAR_WEATHER_TYPE_PERSIST);weatherOwned=false;if(pending.kind==1)call(N::SET_MODEL_AS_NO_LONGER_NEEDED,pending.model);if(pending.kind==2)call(N::CLEAR_FOCUS);pending={};mp.restore();}
bool online(){return call<bool>(N::NETWORK_IS_SESSION_STARTED)||call<bool>(N::NETWORK_IS_SESSION_ACTIVE);}
void setGod(){restoreGod();if(god){godPed=call<int>(N::PLAYER_PED_ID);oldGod=call<bool>(N::GET_PLAYER_INVINCIBLE,call<int>(N::PLAYER_ID));call(N::SET_ENTITY_INVINCIBLE,godPed,true,false);}}
void spawn(const Vehicle& v){if(!call<bool>(N::IS_MODEL_IN_CDIMAGE,v.id)||!call<bool>(N::IS_MODEL_A_VEHICLE,v.id)){note("unavailable");return;}if(v.online&&!mp.enable()){note("compat");return;}pending={1,GetTickCount(),v.id,0,0,v.online};call(N::REQUEST_MODEL,v.id);note("loading");}
bool codeInput=false,codeDisplayed=false,codeEditing=false;DWORD codeReleasedAt=0,codeDisplayedAt=0;
bool codeModal(){return codeInput&&codeDisplayed;}
std::string normalizeCode(std::string text){
 auto first=text.find_first_not_of(" \t\r\n");if(first==std::string::npos)return {};
 text=text.substr(first,text.find_last_not_of(" \t\r\n")-first+1);
 if(text.size()>63)return {};
 for(char& c:text){if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')||(c>='0'&&c<='9')||c=='_'))return {};c=(char)std::tolower((unsigned char)c);}
 return text;
}
void spawnCode(const std::string& raw){auto code=normalizeCode(raw);if(code.empty()){note("invalidcode");return;}
 auto id=hash(code);auto found=std::find_if(vehicles.begin(),vehicles.end(),[&](const Vehicle& v){return v.id==id;});
 spawn(found!=vehicles.end()?*found:Vehicle{code,code,id,true});
}
bool codeOpeningHeld(){return input.accept.before||input.mouseAccept.before||input.close.before||input.mouseBack.before;}
void startCodeInput(){syncGalleryPause(false);codeInput=true;codeDisplayed=false;codeEditing=false;codeReleasedAt=GetTickCount();}
void tickCodeInput(DWORD now=GetTickCount()){
 if(!codeInput)return;
 if(!codeDisplayed){
  // The Enter/A/click that selected this item must not also submit the native dialog.
  if(codeOpeningHeld()){codeReleasedAt=now;return;}
  if(now-codeReleasedAt<150)return;
  codeDisplayed=true;codeDisplayedAt=now;
  call(N::DISPLAY_ONSCREEN_KEYBOARD,0,"FMMC_KEY_TIP8","","","","","",64);return;
 }
 int status=call<int>(N::UPDATE_ONSCREEN_KEYBOARD);
 if(status==0){codeEditing=true;return;}
 // Some builds report an inactive status while the native dialog is starting.
 if(status!=1&&status!=2&&!codeEditing&&now-codeDisplayedAt<1500)return;
 codeInput=false;codeDisplayed=false;releaseShield=true;blockUntil=now+250;
 if(status==1){const char* value=call<const char*>(N::GET_ONSCREEN_KEYBOARD_RESULT);if(value)spawnCode(value);}
 else if(status!=2){log("Onscreen keyboard became inactive: "+std::to_string(status));note("keyboardfailed");}
}
void teleport(){int blip=call<int>(N::GET_FIRST_BLIP_INFO_ID,8);if(!call<bool>(N::DOES_BLIP_EXIST,blip)){note("nomarker");return;}auto p=call<Vec3>(N::GET_BLIP_INFO_ID_COORD,blip);pending={2,GetTickCount(),0,p.x,p.y,false};call(N::SET_FOCUS_POS_AND_VEL,p.x,p.y,100.f,0.f,0.f,0.f);note("loading");}
void tickPending(){if(!pending.kind)return;int ped=call<int>(N::PLAYER_PED_ID);DWORD elapsed=GetTickCount()-pending.start;
 if(pending.kind==1){if(pending.online&&!mp.enable()){call(N::SET_MODEL_AS_NO_LONGER_NEEDED,pending.model);pending={};note("compat");return;}
 if(call<bool>(N::HAS_MODEL_LOADED,pending.model)){Vec3 minimum{},maximum{};call(N::GET_MODEL_DIMENSIONS,pending.model,&minimum,&maximum);auto pos=call<Vec3>(N::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS,ped,0.f,dropDistance(minimum,maximum),3.f-minimum.z);int veh=call<int>(N::CREATE_VEHICLE,pending.model,pos.x,pos.y,pos.z,call<float>(N::GET_ENTITY_HEADING,ped),false,true,false);call(N::SET_MODEL_AS_NO_LONGER_NEEDED,pending.model);pending={};if(!veh){note("unavailable");return;}call(N::SET_ENTITY_AS_MISSION_ENTITY,veh,true,true);call(N::SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER,veh,true);call(N::SET_ENTITY_INVINCIBLE,veh,true,false);call(N::SET_ENTITY_HAS_GRAVITY,veh,true);call(N::SET_ENTITY_VELOCITY,veh,0.f,0.f,-1.5f);falling.push_back({veh,GetTickCount()+5000});spawned.push_back(veh);while(spawned.size()>6){int old=spawned.front();spawned.pop_front();if(!isPinned(old)&&call<bool>(N::DOES_ENTITY_EXIST,old))call(N::SET_ENTITY_AS_NO_LONGER_NEEDED,&old);}setMenuVisible(false);note("done");}
 else if(elapsed>8000){call(N::SET_MODEL_AS_NO_LONGER_NEEDED,pending.model);pending={};note("timeout");}}
 else {float ground=0;bool found=false;for(float z:{1000.f,800.f,600.f,400.f,200.f,100.f,50.f,0.f}){call(N::REQUEST_COLLISION_AT_COORD,pending.x,pending.y,z);if(call<bool>(N::GET_GROUND_Z_FOR_3D_COORD,pending.x,pending.y,z,&ground,false,false)){found=true;break;}}
 if(found){int entity=call<bool>(N::IS_PED_IN_ANY_VEHICLE,ped,false)?call<int>(N::GET_VEHICLE_PED_IS_IN,ped,false):ped;call(N::SET_ENTITY_COORDS_NO_OFFSET,entity,pending.x,pending.y,ground+1.f,false,false,true);call(N::CLEAR_FOCUS);pending={};note("done");}else if(elapsed>8000){call(N::CLEAR_FOCUS);pending={};note("noground");}}
}
int repairTarget=0;DWORD repairDeadline=0,lastRepairTry=0;
void restoreVehicle(int vehicle){
 // Restore mechanical health before SET_VEHICLE_FIXED, which cannot revive a broken engine alone.
 call(N::SET_ENTITY_HEALTH,vehicle,std::max(1000,call<int>(N::GET_ENTITY_MAX_HEALTH,vehicle)),0,0);
 call(N::SET_VEHICLE_ENGINE_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_PETROL_TANK_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_BODY_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_UNDRIVEABLE,vehicle,false);
 call(N::SET_VEHICLE_FIXED,vehicle);
 call(N::SET_VEHICLE_DEFORMATION_FIXED,vehicle);
 call(N::SET_VEHICLE_ENGINE_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_PETROL_TANK_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_BODY_HEALTH,vehicle,1000.f);
 call(N::SET_VEHICLE_UNDRIVEABLE,vehicle,false);
 call(N::SET_VEHICLE_ENGINE_ON,vehicle,true,true,false);
}
void startRepair(int vehicle){repairTarget=vehicle;lastRepairTry=GetTickCount();repairDeadline=lastRepairTry+2000;restoreVehicle(vehicle);note("loading");}
void tickRepair(){
 if(!repairTarget)return;
 if(!call<bool>(N::DOES_ENTITY_EXIST,repairTarget)){repairTarget=0;note("repairfailed");return;}
 if(call<bool>(N::GET_IS_VEHICLE_ENGINE_RUNNING,repairTarget)&&call<bool>(N::IS_VEHICLE_DRIVEABLE,repairTarget,false)){repairTarget=0;note("done");return;}
 if(static_cast<LONG>(GetTickCount()-repairDeadline)>=0){repairTarget=0;note("repairfailed");return;}
 if(GetTickCount()-lastRepairTry>=100){restoreVehicle(repairTarget);lastRepairTry=GetTickCount();}
}
#include "pinned.hpp"
int pinnedCount(){return (int)pins.size();}
void activate(int delta=0){
 if(page==6){if(selected==0)toggleCurrentPin();else enterPage(selected==1?7:9);return;}
 if(page==7){if(selected>=0&&selected<(int)pins.size()){pinActionId=pins[selected].data.id;enterPage(8);}return;}
 if(page==8){auto p=findPin(pinActionId);if(!p){goBack();return;}if(selected==0){if(removePin(pinActionId))goBack();}else if(selected==1){pinTarget=pinActionId;p->retry=0;p->created=GetTickCount();note("loading");}else{snapshotPin(*p);call(N::SET_NEW_WAYPOINT,p->data.x,p->data.y);note("done");}return;}
 if(page==9){if(selected==1)clearPins();goBack();return;}
 if(page==1){if(selected==3){startCodeInput();return;}if(selected==0){int index=pickSurpriseIndex(randomEngine,all);if(index>=0)spawn(vehicles[index]);else note("unavailable");}else enterPage(selected==1?3:4);return;}
 if(page==4){if(selected>=0&&selected<(int)availableClasses.size())enterPage(5,availableClasses[selected]);return;}
 if(isVehiclePage()){const auto& list=vehicleList();if(selected>=0&&selected<(int)list.size())spawn(vehicles[list[selected]]);return;}
 if(selected==11){priority=!priority;WritePrivateProfileStringA("Menu","Priority",priority?"1":"0",(root+"/settings.ini").c_str());return;}
 int ped=call<int>(N::PLAYER_PED_ID),player=call<int>(N::PLAYER_ID);switch(selected){case 0:god=!god;setGod();break;case 1:call(N::SET_ENTITY_HEALTH,ped,call<int>(N::GET_ENTITY_MAX_HEALTH,ped),ped,0);call(N::SET_PED_ARMOUR,ped,100);note("done");break;case 2:call(N::SET_PLAYER_WANTED_LEVEL,player,0,false);call(N::SET_PLAYER_WANTED_LEVEL_NOW,player,false);note("done");break;case 3:call(N::REFILL_AMMO_INSTANTLY,ped);note("done");break;case 4:{int v=call<int>(N::GET_VEHICLE_PED_IS_IN,ped,false);if(v){startRepair(v);}else note("noveh");break;}case 5:enterPage(1);break;case 6:enterPage(6);break;case 7:teleport();break;case 8:hour=(hour+(delta<0?23:1))%24;call(N::SET_CLOCK_TIME,hour,0,0);break;case 9:frozen=!frozen;call(N::PAUSE_CLOCK,frozen);break;case 10:{weather=(weather+(delta<0?5:1))%6;const char* types[]={"EXTRASUNNY","CLOUDS","RAIN","THUNDER","FOGGY","XMAS"};call(N::SET_WEATHER_TYPE_NOW_PERSIST,types[weather]);weatherOwned=true;break;}}
}
std::vector<std::string> rows(){
 if(page==6){int e=call<int>(N::GET_VEHICLE_PED_IS_IN,call<int>(N::PLAYER_PED_ID),false);return {tr(e&&isPinned(e)?"unpinCurrent":"pinCurrent"),tr("viewPins")+"  ("+std::to_string(pins.size())+")",tr("clearPins")};}
 if(page==7){std::vector<std::string> out;for(auto& p:pins)out.push_back(pinName(p));return out;}
 if(page==8)return {tr("unpin"),tr("boardPin"),tr("markPin")};
 if(page==9)return {tr("cancel"),tr("clearPinsConfirm")};
 if(page==1)return{tr("surprise"),tr("all"),tr("categories"),tr("spawncode")};
 if(page==4){std::vector<std::string> out;for(int c:availableClasses)out.push_back(tr("class"+std::to_string(c))+"  ("+std::to_string(classified[c].size())+")");return out;}
 if(isVehiclePage()){std::vector<std::string> out;for(int i:vehicleList())out.push_back(vehicles[i].name);return out;}
 return {tr("god")+"  ["+tr(god?"on":"off")+"]",tr("heal"),tr("wanted"),tr("ammo"),tr("repair"),tr("spawn")+"  >",tr("pinned")+"  >",tr("waypoint"),tr("hour")+"  "+std::to_string(hour)+":00  >",tr("freeze")+"  ["+tr(frozen?"on":"off")+"]",tr("weather")+"  "+tr(std::array<std::string,6>{"clear","clouds","rain","thunder","fog","snow"}[weather])+"  >",tr("priority")+"  ["+tr(priority?"on":"off")+"]"};
}
struct Box{float x,y,w,h;bool contains(float px,float py)const{return px>=x&&py>=y&&px<x+w&&py<y+h;}};
inline constexpr float listTopOffset=135.f; // 8 px between the section title and menu items at 1080p.
struct Layout{
 float x=48,y=55;int columns=4,capacity=12,start=0,count=0;std::vector<Box> cards;std::vector<std::pair<int,Box>> tabs;
 Layout(int total){
  float safe=call<float>(N::GET_SAFE_ZONE_SIZE);x=48+(1-safe)*canvasWidth/2;y=55+(1-safe)*540;
  if(isGrid()){
   columns=std::clamp((int)((canvasWidth-120)/420),2,6);capacity=columns*3;start=selected/capacity*capacity;count=std::min(capacity,std::max(0,total-start));
   float width=(canvasWidth-120-(columns-1)*18)/columns;
   for(int i=0;i<count;++i)cards.push_back({60+(i%columns)*(width+18),200.f+(i/columns)*250,width,232});
   std::vector<int> ids={-1};ids.insert(ids.end(),availableClasses.begin(),availableClasses.end());
   auto current=std::find(ids.begin(),ids.end(),galleryCategory);int active=current==ids.end()?0:(int)(current-ids.begin());
   int slots=std::max(1,(int)((canvasWidth-120)/150)),first=std::clamp(active-slots/2,0,std::max(0,(int)ids.size()-slots));
   float tabWidth=(canvasWidth-120)/std::min(slots,(int)ids.size());
   for(int i=first;i<std::min(first+slots,(int)ids.size());++i)tabs.push_back({ids[i],{60+(i-first)*tabWidth,125,tabWidth-4,48}});
  }else{
   capacity=10;start=selected/capacity*capacity;count=std::min(capacity,std::max(0,total-start));
   for(int i=0;i<count;++i)cards.push_back({x,y+listTopOffset+i*39,510,39});
  }
 }
 float footerBaseline(int total)const{return isGrid()?994:y+listTopOffset+std::min(capacity,total)*39+28;}
 Box previousArrow(int total)const{float baseline=footerBaseline(total);return isGrid()?Box{canvasWidth-160,baseline-30,44,39}:Box{x+410,baseline-28,44,39};}
 Box nextArrow(int total)const{auto box=previousArrow(total);box.x+=54;return box;}
 int hit(float x,float y)const{for(size_t i=0;i<cards.size();++i)if(cards[i].contains(x,y))return start+(int)i;return -1;}
};
struct ImageRef{std::string dictionary,texture;};
std::unordered_map<std::string,ImageRef> images;
std::set<std::string> requestedImages;
void releaseImages(){for(const auto& d:requestedImages)call(N::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED,d.c_str());requestedImages.clear();}
void keepImages(const std::set<std::string>& wanted){
 for(auto it=requestedImages.begin();it!=requestedImages.end();)if(!wanted.count(*it)){call(N::SET_STREAMED_TEXTURE_DICT_AS_NO_LONGER_NEEDED,it->c_str());it=requestedImages.erase(it);}else ++it;
 for(const auto& d:wanted)if(!requestedImages.count(d)){call(N::REQUEST_STREAMED_TEXTURE_DICT,d.c_str(),false);requestedImages.insert(d);}
}
void vehicleImage(const Vehicle& v,Box box){
 auto it=images.find(v.model);bool drawn=false;
 if(it!=images.end()&&call<bool>(N::HAS_STREAMED_TEXTURE_DICT_LOADED,it->second.dictionary.c_str())){
  auto size=call<Vec3>(N::GET_TEXTURE_RESOLUTION,it->second.dictionary.c_str(),it->second.texture.c_str());
  if(size.x>4&&size.y>4){float width=box.w-24,height=std::min(163.f,width*size.y/size.x);width=height*size.x/size.y;
   call(N::DRAW_SPRITE,it->second.dictionary.c_str(),it->second.texture.c_str(),(box.x+box.w/2)/canvasWidth,(box.y+88)/1080.f,width/canvasWidth,height/1080.f,0.f,255,255,255,255,false,0);drawn=true;}
 }
 if(!drawn){ // Neutral vehicle outline; never substitute another model's photograph.
  float cx=box.x+box.w/2;rect(cx-55,box.y+70,110,35,100,108,114,255);rect(cx-34,box.y+48,68,25,100,108,114,255);rect(cx-43,box.y+102,19,15,42,46,50,255);rect(cx+24,box.y+102,19,15,42,46,50,255);
  font.text(tr("noimage"),box.x+12,box.y+147,box.w-24,false,.65f);
 }
}
void renderGallery(const Layout& layout,int total){
 rect(0,0,canvasWidth,1080,10,14,18,250);rect(60,54,canvasWidth-120,4,112,178,70,255);
 font.text("SoraMod",60,108,300,false,1.2f);
 font.text(tr("all"),380,108,canvasWidth-560,false,1.f);
 font.text("X",canvasWidth-90,108,40);
 for(auto& tab:layout.tabs){auto box=tab.second;bool active=tab.first==galleryCategory;rect(box.x,box.y,box.w,box.h,active?55:22,active?65:27,active?72:32,255);if(active)rect(box.x,box.y+box.h-3,box.w,3,112,178,70,255);font.text(tab.first<0?tr("all"):tr("class"+std::to_string(tab.first)),box.x+10,box.y+32,box.w-20,false,.8f);}
 std::set<std::string> wanted;const auto& list=vehicleList();
 for(int i=0;i<layout.count;++i){auto it=images.find(vehicles[list[layout.start+i]].model);if(it!=images.end())wanted.insert(it->second.dictionary);}keepImages(wanted);
 for(int i=0;i<layout.count;++i){int index=layout.start+i;auto box=layout.cards[i];bool active=index==selected;
  rect(box.x,box.y,box.w,box.h,active?67:27,active?79:33,active?88:39,255);if(active)rect(box.x,box.y,box.w,3,112,178,70,255);
  auto& v=vehicles[list[index]];vehicleImage(v,box);font.text(v.name,box.x+12,box.y+195,box.w-24,false,.85f);font.text(v.model,box.x+12,box.y+222,box.w-24,false,.65f);
 }
 if(total==0)font.text(tr("unavailable"),60,220,canvasWidth-120);
 float zoom=.8f;int pages=total?(total+layout.capacity-1)/layout.capacity:0;
 font.text(std::to_string(total?selected/layout.capacity+1:0)+" / "+std::to_string(pages),60,994,240,false,zoom);
 if(total>layout.capacity){auto left=layout.previousArrow(total),right=layout.nextArrow(total);font.text("<",left.x+10,994,30,false,zoom);font.text(">",right.x+10,994,30,false,zoom);}
 if(GetTickCount()<messageUntil)font.text(message,60,1040,canvasWidth-120,false,.8f);
}
void render(){int w=1920,h=1080;call(N::GET_ACTUAL_SCREEN_RESOLUTION,&w,&h);if(w<1||h<1)return;font.frame(w,h);canvasWidth=w*1080.f/h;
 auto list=rows();int n=(int)list.size();selected=std::clamp(selected,0,std::max(0,n-1));Layout layout(n);if(isGrid()){renderGallery(layout,n);return;}releaseImages();float x=layout.x,y=layout.y;int start=layout.start,count=layout.count;
 // Native rectangles use a 1920x1080 normalized canvas; text follows the same mapping.
 rect(x,y,510,88,10,14,18,235);rect(x,y+85,510,3,112,178,70,255);font.text("SoraMod",x+18,y+48,375,false,1.25f);font.text("X",x+470,y+48,30);
 rect(x,y+88,510,39,0,0,0,225);font.text(page>=6?(page==9?tr("clearPins"):page==8&&findPin(pinActionId)?pinName(*findPin(pinActionId)):tr("pinned")):page==5?tr("class"+std::to_string(category)):page==4?tr("categories"):page?tr("spawn"):tr("home"),x+14,y+115,475,false,.85f);
 for(int i=0;i<count;++i){bool sel=start+i==selected;float yy=y+listTopOffset+i*39;rect(x,yy,510,39,sel?245:0,sel?245:0,sel?245:0,sel?255:180);font.text(list[start+i],x+14,yy+28,480,sel,.85f);}
 int displayRows=std::min(layout.capacity,n);if(count<displayRows)rect(x,y+listTopOffset+count*39,510,(displayRows-count)*39,0,0,0,180);
 float bottom=layout.footerBaseline(n)-28;rect(x,bottom,510,39,8,8,8,225);float zoom=.75f;
 int pages=n?(n+layout.capacity-1)/layout.capacity:0;font.text(std::to_string(n?selected/layout.capacity+1:0)+" / "+std::to_string(pages),x+14,bottom+28,180,false,zoom);
 if(n>layout.capacity){auto left=layout.previousArrow(n),right=layout.nextArrow(n);font.text("<",left.x+10,bottom+28,30,false,zoom);font.text(">",right.x+10,bottom+28,30,false,zoom);}
 if(GetTickCount()<messageUntil){rect(x,bottom+44,510,45,0,0,0,220);font.text(message,x+14,bottom+75,480,false,.8f);}
}
void readLanguage(const std::string& locale){std::ifstream in(root+"/lang/"+locale+".ini");std::string line;while(std::getline(in,line)){auto p=line.find('=');if(p!=std::string::npos)strings[line.substr(0,p)]=line.substr(p+1);}}
void init(){wchar_t path[32768];GetModuleFileNameW(module,path,32768);root=(fs::path(path).parent_path()/"SoraMod").u8string();log("SoraMod 0.4.1 / MinamotoDev");int game=call<int>(N::GET_CURRENT_LANGUAGE);int map[]={0,1,3,2,4,8,7,9,6,12,5,11,10};language=game>=0&&game<13?map[game]:0;readLanguage("en");readLanguage(languages[language]);int face=language==6?1:language==10?2:language==12?3:0;if(!font.load(root,face))throw std::runtime_error("Bundled glyphs are missing");
 hotkey=GetPrivateProfileIntA("Controls","Keyboard",VK_F7,(root+"/settings.ini").c_str());if(hotkey<1||hotkey>254||(hotkey>=VK_NUMPAD0&&hotkey<=VK_DIVIDE))hotkey=VK_F7;
 priority=GetPrivateProfileIntA("Menu","Priority",1,(root+"/settings.ini").c_str())!=0;
 std::ifstream pictureFile(root+"/vehicle-images.tsv");std::string pictureLine;while(std::getline(pictureFile,pictureLine)){auto parts=split(pictureLine,'|');if(parts.size()==3)images[parts[0]]={parts[1],parts[2]};}
 input.init();std::ifstream in(root+"/vehicles.tsv");std::string line;std::set<std::uint32_t> known;
 while(std::getline(in,line)){auto c=split(line,'|');if(c.size()<15)continue;Vehicle v{c[0],c[language+2],hash(c[0]),c[1]=="1"};if(!call<bool>(N::IS_MODEL_IN_CDIMAGE,v.id)||!call<bool>(N::IS_MODEL_A_VEHICLE,v.id))continue;known.insert(v.id);int index=(int)vehicles.size();vehicles.push_back(v);all.push_back(index);}
 int total=call<int>(N::GET_NUM_DLC_VEHICLES);for(int i=0;i<std::min(total,4096);++i){U64 data[3]{};if(!call<bool>(N::GET_DLC_VEHICLE_DATA,i,data))continue;auto id=static_cast<std::uint32_t>(data[1]);if(known.count(id)||!call<bool>(N::IS_MODEL_A_VEHICLE,id))continue;known.insert(id);char label[32];std::snprintf(label,sizeof(label),"DLC %08X",id);all.push_back((int)vehicles.size());vehicles.push_back({label,label,id,true});}for(size_t i=0;i<vehicles.size();++i){vehicles[i].category=std::clamp(call<int>(N::GET_VEHICLE_CLASS_FROM_NAME,vehicles[i].id),0,22);classified[vehicles[i].category].push_back((int)i);}
 for(int c=0;c<23;++c)if(!classified[c].empty())availableClasses.push_back(c);
 log("Installed vehicle models: "+std::to_string(vehicles.size()));hour=call<int>(N::GET_CLOCK_HOURS);
}
void scriptMain(){try{init();bool wasOnline=false;DWORD mpTick=0;
 for(;;){
  if(online()){if(!wasOnline){visible=false;repairTarget=0;if(codeInput){if(codeDisplayed)call(N::CANCEL_ONSCREEN_KEYBOARD);codeInput=codeDisplayed=false;}if(pinTarget){call(N::CLEAR_FOCUS);pinTarget=0;}releaseImages();cleanup();log("Network session: trainer suspended");}wasOnline=true;SHV::wait(0);continue;}wasOnline=false;
  DWORD owner=0;GetWindowThreadProcessId(GetForegroundWindow(),&owner);bool foreground=owner==GetCurrentProcessId();
  if(!foreground&&visible)setMenuVisible(false);
  bool keyboard=call<bool>(N::IS_USING_KEYBOARD_AND_MOUSE,0);
  gate.apply(!codeModal()&&(visible||releaseShield||GetTickCount()<blockUntil),priority,hasPages()||isGrid(),keyboard,hotkey,visible&&isGrid(),input.menuPad(hasPages()||isGrid(),isGrid()),input.heldKeys,isGrid());
  if(visible&&!codeInput&&keyboard)call(N::SET_MOUSE_CURSOR_THIS_FRAME);
  input.poll(foreground,hotkey,visible,isGrid());
  if(!visible&&releaseShield&&!input.anyOwnedHeld(hasPages()||isGrid(),isGrid()))releaseShield=false;
  if(!codeInput&&input.open&&!call<bool>(N::IS_PAUSE_MENU_ACTIVE))toggleMenu();
  if(visible&&!codeInput&&input.hide)dismissMenu();
  if(god&&call<int>(N::PLAYER_PED_ID)!=godPed)setGod();
  if((!spawned.empty()||!pins.empty())&&GetTickCount()-mpTick>1000){mpTick=GetTickCount();mp.enable();}
  tickPending();tickFalling();tickRepair();tickCodeInput();if(!pending.kind&&!codeInput)tickPins();
  if(visible){
   int w=1920,h=1080;call(N::GET_ACTUAL_SCREEN_RESOLUTION,&w,&h);canvasWidth=h>0?w*1080.f/h:1920.f;
   int count=(int)rows().size();selected=std::clamp(selected,0,std::max(0,count-1));Layout layout(count);
   if(!pending.kind&&!pinTarget&&!codeInput&&GetTickCount()>=blockUntil&&!input.hide){
    if(isGrid()&&!input.open&&(input.tabPrev||input.tabNext)){changeGalleryTab(input.tabPrev?-1:1);count=itemCount();layout=Layout(count);}
    if((input.l&&!isGrid())||input.b||input.context)previousLayer();
    else{
     if(count){
      int first=selected/layout.capacity*layout.capacity,last=std::min(count-1,first+layout.capacity-1);
      if(isGrid()){moveGrid((input.r&&!input.open?1:0)-(input.l?1:0),(input.d?1:0)-(input.u?1:0),count);}
      else {if(input.u)selected=std::max(first,selected-1);if(input.d)selected=std::min(last,selected+1);}
      if(input.wheel)selected=std::clamp(selected+input.wheel,first,last);
      if(input.pu)changePage(-1);
      if(input.pd&&!input.open)changePage(1);
      if(input.r&&!input.open&&!isGrid()){if(count>layout.capacity)changePage(1);else if(page==0&&(selected==8||selected==10))activate(1);}
     }
     bool clicked=false;
     if(input.keyboard){
      float mx=input.mouseX*canvasWidth,my=input.mouseY*1080;int hit=layout.hit(mx,my);
      bool navigating=(input.heldKeys&(1|2|4|8|16|128|256|8192))||(input.heldPad&0x330f);
      if(hit>=0&&(input.click||(input.moved&&!navigating)))selected=hit;
      Box closeBox=isGrid()?Box{canvasWidth-98,68,42,48}:Box{layout.x+462,layout.y+12,44,50};
      if(input.click){
       bool tabClicked=false;for(auto& tab:layout.tabs)if(tab.second.contains(mx,my)){galleryCategory=tab.first;selected=0;tabClicked=true;break;}
       if(closeBox.contains(mx,my))setMenuVisible(false);
       else if(tabClicked){}
       else if(count>layout.capacity&&layout.previousArrow(count).contains(mx,my))previousLayer();
       else if(count>layout.capacity&&layout.nextArrow(count).contains(mx,my))changePage(1);
       else clicked=hit>=0;
      }
     }
     if(visible&&(input.a||clicked))activate();
    }
   }
  }
  syncGalleryPause(visible&&isGrid()&&!pending.kind);
  gate.apply(!codeModal()&&(visible||releaseShield||GetTickCount()<blockUntil),priority,hasPages()||isGrid(),input.keyboard,hotkey,visible&&isGrid(),input.menuPad(hasPages()||isGrid(),isGrid()),input.heldKeys,isGrid());

  if(visible&&!codeInput){if(input.keyboard)call(N::SET_MOUSE_CURSOR_THIS_FRAME);render();}else releaseImages();
  SHV::wait(0);
 }
 }catch(const std::exception& e){log(e.what());releaseImages();cleanup();}}
BOOL APIENTRY DllMain(HMODULE h,DWORD reason,LPVOID){if(reason==DLL_PROCESS_ATTACH){module=h;DisableThreadLibraryCalls(h);if(SHV::bind())SHV::reg(h,scriptMain);}else if(reason==DLL_PROCESS_DETACH&&SHV::unreg)SHV::unreg(h);return TRUE;}
