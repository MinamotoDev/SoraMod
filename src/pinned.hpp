#pragma once
#include <cmath>
#include <limits>
#pragma pack(push,1)
struct SavedVehicle {
 std::uint64_t id=0;std::uint32_t model=0;float x=0,y=0,z=0,heading=0;
 char plate[16]{};int colours[4]{},custom[6]{},mods[50]{},variations[50]{},toggles[6]{},extras[15]{};
 int customPrimary=0,customSecondary=0,wheels=0,livery=-1,livery2=-1,tint=0,plateType=0,neon[7]{},smoke[3]{},xenon=-1;
};
struct PinHeader{std::uint32_t magic=0x504d4f53,version=1,count=0,checksum=0;};
#pragma pack(pop)
struct Pin {SavedVehicle data;int entity=0;bool release=false,waitingCollision=false;DWORD requested=0,retry=0,created=0;};
std::vector<Pin> pins;
bool pinsWritable=true,pinsLoaded=false;DWORD lastPinSave=0;std::uint64_t pinTarget=0,pinActionId=0;
std::uint32_t pinChecksum(const void* data,size_t bytes){auto p=(const unsigned char*)data;std::uint32_t hash=2166136261U;while(bytes--)hash=(hash^*p++)*16777619U;return hash;}
bool validPin(const SavedVehicle& d){return d.id&&d.model&&std::isfinite(d.x)&&std::isfinite(d.y)&&std::isfinite(d.z)&&std::isfinite(d.heading)&&std::abs(d.x)<100000&&std::abs(d.y)<100000&&std::abs(d.z)<100000&&d.plate[15]==0;}
bool readPinsFile(const fs::path& file,std::vector<Pin>& out){
 std::ifstream in(file,std::ios::binary|std::ios::ate);if(!in)return false;auto bytes=in.tellg();if(bytes<(std::streamoff)sizeof(PinHeader)||bytes>16000000)return false;
 in.seekg(0);PinHeader h{};in.read((char*)&h,sizeof(h));if(h.magic!=0x504d4f53||h.version!=1||h.count>10000||bytes!=(std::streamoff)(sizeof(h)+h.count*sizeof(SavedVehicle)))return false;
 std::vector<SavedVehicle> data(h.count);in.read((char*)data.data(),data.size()*sizeof(SavedVehicle));if(!in||pinChecksum(data.data(),data.size()*sizeof(SavedVehicle))!=h.checksum)return false;
 std::set<std::uint64_t> ids;out.clear();for(auto& d:data){if(!validPin(d)||!ids.insert(d.id).second){out.clear();return false;}out.push_back({d});}return true;
}
fs::path pinPath(){return fs::u8path(root)/"data"/"pinned-vehicles.dat";}
void loadPins(){auto path=pinPath();std::error_code ec;if(!fs::exists(path,ec)&&!fs::exists(path.wstring()+L".bak",ec)){pinsLoaded=true;return;}
 if(!readPinsFile(path,pins)&&!readPinsFile(path.wstring()+L".bak",pins)){pinsWritable=false;note("pindataerror");}pinsLoaded=true;
}
bool savePins(){if(!pinsWritable)return false;try{
 auto path=pinPath();fs::create_directories(path.parent_path());auto temp=path.wstring()+L".tmp";std::vector<SavedVehicle> data;for(auto& p:pins)data.push_back(p.data);
 PinHeader h;h.count=(std::uint32_t)data.size();h.checksum=pinChecksum(data.data(),data.size()*sizeof(SavedVehicle));
 HANDLE file=CreateFileW(temp.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr);if(file==INVALID_HANDLE_VALUE)return false;
 DWORD wrote=0;bool ok=WriteFile(file,&h,sizeof(h),&wrote,nullptr)&&wrote==sizeof(h);
 DWORD size=(DWORD)(data.size()*sizeof(SavedVehicle));if(ok&&size)ok=WriteFile(file,data.data(),size,&wrote,nullptr)&&wrote==size;
 if(ok)ok=FlushFileBuffers(file)!=0;CloseHandle(file);if(!ok)return false;
 std::vector<Pin> checked;if(!readPinsFile(temp,checked))return false;
 // Only rotate a valid primary file into the backup; a recovered backup stays intact.
 if(readPinsFile(path,checked)&&!CopyFileW(path.c_str(),(path.wstring()+L".bak").c_str(),FALSE))return false;
 return MoveFileExW(temp.c_str(),path.c_str(),MOVEFILE_REPLACE_EXISTING|MOVEFILE_WRITE_THROUGH)!=0;
 }catch(const fs::filesystem_error&){return false;}}
bool pinLive(const Pin& p){return p.entity&&call<bool>(N::DOES_ENTITY_EXIST,p.entity)&&call<std::uint32_t>(N::GET_ENTITY_MODEL,p.entity)==p.data.model;}
Pin* findPin(std::uint64_t id){for(auto& p:pins)if(p.data.id==id)return &p;return nullptr;}
bool isPinned(int entity){for(auto& p:pins)if(p.entity==entity&&pinLive(p))return true;return false;}
void snapshotPin(Pin& p){if(!pinLive(p)||p.waitingCollision)return;auto& d=p.data;int e=p.entity;auto pos=call<Vec3>(N::GET_ENTITY_COORDS,e,true);d.x=pos.x;d.y=pos.y;d.z=pos.z;d.heading=call<float>(N::GET_ENTITY_HEADING,e);
 const char* plate=call<const char*>(N::GET_VEHICLE_NUMBER_PLATE_TEXT,e);if(plate){std::memset(d.plate,0,sizeof(d.plate));std::strncpy(d.plate,plate,15);}
 call(N::GET_VEHICLE_COLOURS,e,&d.colours[0],&d.colours[1]);call(N::GET_VEHICLE_EXTRA_COLOURS,e,&d.colours[2],&d.colours[3]);
 d.customPrimary=call<bool>(N::GET_IS_VEHICLE_PRIMARY_COLOUR_CUSTOM,e);d.customSecondary=call<bool>(N::GET_IS_VEHICLE_SECONDARY_COLOUR_CUSTOM,e);
 call(N::GET_VEHICLE_CUSTOM_PRIMARY_COLOUR,e,&d.custom[0],&d.custom[1],&d.custom[2]);call(N::GET_VEHICLE_CUSTOM_SECONDARY_COLOUR,e,&d.custom[3],&d.custom[4],&d.custom[5]);
 for(int i=0;i<50;++i){d.mods[i]=call<int>(N::GET_VEHICLE_MOD,e,i);d.variations[i]=call<bool>(N::GET_VEHICLE_MOD_VARIATION,e,i);}
 for(int i=0;i<6;++i)d.toggles[i]=call<bool>(N::IS_TOGGLE_MOD_ON,e,17+i);
 for(int i=0;i<15;++i)d.extras[i]=call<bool>(N::IS_VEHICLE_EXTRA_TURNED_ON,e,i);
 d.wheels=call<int>(N::GET_VEHICLE_WHEEL_TYPE,e);d.livery=call<int>(N::GET_VEHICLE_LIVERY,e);d.livery2=call<int>(N::GET_VEHICLE_LIVERY2,e);d.tint=call<int>(N::GET_VEHICLE_WINDOW_TINT,e);d.plateType=call<int>(N::GET_VEHICLE_NUMBER_PLATE_TEXT_INDEX,e);
 call(N::GET_VEHICLE_NEON_COLOUR,e,&d.neon[0],&d.neon[1],&d.neon[2]);for(int i=0;i<4;++i)d.neon[3+i]=call<bool>(N::GET_VEHICLE_NEON_ENABLED,e,i);
 call(N::GET_VEHICLE_TYRE_SMOKE_COLOR,e,&d.smoke[0],&d.smoke[1],&d.smoke[2]);d.xenon=call<int>(N::GET_VEHICLE_XENON_LIGHT_COLOR_INDEX,e);
}
void applyPin(Pin& p){auto& d=p.data;int e=p.entity;
 call(N::SET_VEHICLE_MOD_KIT,e,0);call(N::SET_VEHICLE_WHEEL_TYPE,e,d.wheels);
 for(int i=0;i<50;++i)if(i<17||i>22)call(N::SET_VEHICLE_MOD,e,i,d.mods[i],d.variations[i]!=0);
 for(int i=0;i<6;++i)call(N::TOGGLE_VEHICLE_MOD,e,17+i,d.toggles[i]!=0);
 call(N::SET_VEHICLE_COLOURS,e,d.colours[0],d.colours[1]);call(N::SET_VEHICLE_EXTRA_COLOURS,e,d.colours[2],d.colours[3]);
 if(d.customPrimary)call(N::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR,e,d.custom[0],d.custom[1],d.custom[2]);
 if(d.customSecondary)call(N::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR,e,d.custom[3],d.custom[4],d.custom[5]);
 for(int i=0;i<15;++i)if(call<bool>(N::DOES_EXTRA_EXIST,e,i))call(N::SET_VEHICLE_EXTRA,e,i,!d.extras[i]);
 if(d.livery>=0)call(N::SET_VEHICLE_LIVERY,e,d.livery);if(d.livery2>=0)call(N::SET_VEHICLE_LIVERY2,e,d.livery2);
 call(N::SET_VEHICLE_WINDOW_TINT,e,d.tint);call(N::SET_VEHICLE_NUMBER_PLATE_TEXT,e,d.plate);call(N::SET_VEHICLE_NUMBER_PLATE_TEXT_INDEX,e,d.plateType);
 call(N::SET_VEHICLE_NEON_COLOUR,e,d.neon[0],d.neon[1],d.neon[2]);for(int i=0;i<4;++i)call(N::SET_VEHICLE_NEON_ENABLED,e,i,d.neon[3+i]!=0);
 call(N::SET_VEHICLE_TYRE_SMOKE_COLOR,e,d.smoke[0],d.smoke[1],d.smoke[2]);call(N::SET_VEHICLE_XENON_LIGHT_COLOR_INDEX,e,d.xenon);
}
void releasePin(Pin& p){if(p.requested)call(N::SET_MODEL_AS_NO_LONGER_NEEDED,p.data.model);if(pinLive(p)){if(p.waitingCollision)call(N::FREEZE_ENTITY_POSITION,p.entity,false);if(p.release){int e=p.entity;call(N::SET_ENTITY_AS_NO_LONGER_NEEDED,&e);}}}
bool removePin(std::uint64_t id){auto old=pins;auto it=std::find_if(pins.begin(),pins.end(),[&](const Pin& p){return p.data.id==id;});if(it==pins.end())return false;Pin removed=*it;pins.erase(it);if(!savePins()){pins=old;note("pinsaveerror");return false;}if(pinTarget==id){pinTarget=0;call(N::CLEAR_FOCUS);}releasePin(removed);note("done");return true;}
void toggleCurrentPin(){int ped=call<int>(N::PLAYER_PED_ID);int e=call<int>(N::GET_VEHICLE_PED_IS_IN,ped,false);if(!e){note("noveh");return;}
 for(auto& p:pins)if(p.entity==e&&pinLive(p)){removePin(p.data.id);return;}
 Pin p;p.entity=e;p.data.model=call<std::uint32_t>(N::GET_ENTITY_MODEL,e);FILETIME time{};GetSystemTimeAsFileTime(&time);p.data.id=((std::uint64_t)time.dwHighDateTime<<32)|time.dwLowDateTime;while(findPin(p.data.id))++p.data.id;
 p.release=!call<bool>(N::IS_ENTITY_A_MISSION_ENTITY,e)||std::find(spawned.begin(),spawned.end(),e)!=spawned.end();snapshotPin(p);if(!validPin(p.data)){note("unavailable");return;}
 pins.push_back(p);if(!savePins()){pins.pop_back();note("pinsaveerror");return;}
 call(N::SET_ENTITY_AS_MISSION_ENTITY,e,true,true);call(N::SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER,e,true);spawned.erase(std::remove(spawned.begin(),spawned.end(),e),spawned.end());note("done");
}
void clearPins(){auto old=pins;pins.clear();if(!savePins()){pins=old;note("pinsaveerror");return;}if(pinTarget)call(N::CLEAR_FOCUS);pinTarget=0;for(auto& p:old)releasePin(p);note("done");}
std::string pinName(const Pin& p){auto it=std::find_if(vehicles.begin(),vehicles.end(),[&](const Vehicle& v){return v.id==p.data.model;});char id[16];std::snprintf(id,sizeof(id),"%08X",p.data.model);return (it!=vehicles.end()?it->name:std::string(id))+"  ["+p.data.plate+"]";}
void boardPin(Pin& p){int ped=call<int>(N::PLAYER_PED_ID);if(call<int>(N::GET_VEHICLE_PED_IS_IN,ped,false)==p.entity){pinTarget=0;call(N::CLEAR_FOCUS);setMenuVisible(false);return;}
 int seat=-2,seats=call<int>(N::GET_VEHICLE_MODEL_NUMBER_OF_SEATS,p.data.model);for(int s=-1;s<seats-1;++s)if(call<bool>(N::IS_VEHICLE_SEAT_FREE,p.entity,s,false)){seat=s;break;}
 if(seat!=-2){call(N::SET_PED_INTO_VEHICLE,ped,p.entity,seat);setMenuVisible(false);note("done");}else note("noseat");pinTarget=0;call(N::CLEAR_FOCUS);
}
void tickPins(){int ped=call<int>(N::PLAYER_PED_ID);if(!call<bool>(N::DOES_ENTITY_EXIST,ped)||call<bool>(N::IS_PLAYER_SWITCH_IN_PROGRESS)||call<bool>(N::IS_SCREEN_FADED_OUT))return;
 if(!pinsLoaded)loadPins();auto pos=call<Vec3>(N::GET_ENTITY_COORDS,ped,true);DWORD now=GetTickCount();
 for(auto& p:pins){auto& d=p.data;bool target=pinTarget==d.id;
 if(pinLive(p)){
  if(p.waitingCollision){call(N::REQUEST_COLLISION_AT_COORD,d.x,d.y,d.z);if(target)call(N::SET_FOCUS_POS_AND_VEL,d.x,d.y,d.z,0.f,0.f,0.f);
   if(call<bool>(N::HAS_COLLISION_LOADED_AROUND_ENTITY,p.entity)){call(N::FREEZE_ENTITY_POSITION,p.entity,false);p.waitingCollision=false;}
  }
  if(target&&!p.waitingCollision)boardPin(p);
  else if(target&&now-p.created>10000){pinTarget=0;call(N::CLEAR_FOCUS);note("timeout");}continue;
 }
 p.entity=0;float distance=(pos.x-d.x)*(pos.x-d.x)+(pos.y-d.y)*(pos.y-d.y);
 if(!target&&distance>500.f*500.f)continue;if(p.retry&&(LONG)(now-p.retry)<0)continue;
 auto catalog=std::find_if(vehicles.begin(),vehicles.end(),[&](const Vehicle& v){return v.id==d.model;});bool needsMp=catalog==vehicles.end()||catalog->online;
 if(!call<bool>(N::IS_MODEL_IN_CDIMAGE,d.model)||!call<bool>(N::IS_MODEL_A_VEHICLE,d.model)||(needsMp&&!mp.enable())){if(target){pinTarget=0;call(N::CLEAR_FOCUS);note("unavailable");}p.retry=now+30000;continue;}
 if(!p.requested){p.requested=now;call(N::REQUEST_MODEL,d.model);}
 call(N::REQUEST_COLLISION_AT_COORD,d.x,d.y,d.z);if(target)call(N::SET_FOCUS_POS_AND_VEL,d.x,d.y,d.z,0.f,0.f,0.f);
 if(call<bool>(N::HAS_MODEL_LOADED,d.model)){
  // Reuse a matching saved vehicle loaded by the base game's save instead of duplicating it.
  if(SHV::worldVehicles){int entities[1024];int count=std::clamp(SHV::worldVehicles(entities,1024),0,1024);for(int i=0;i<count;++i){int e=entities[i];if(isPinned(e)||call<std::uint32_t>(N::GET_ENTITY_MODEL,e)!=d.model)continue;auto q=call<Vec3>(N::GET_ENTITY_COORDS,e,true);const char* plate=call<const char*>(N::GET_VEHICLE_NUMBER_PLATE_TEXT,e);if(plate&&std::string(plate)==d.plate&&std::abs(q.x-d.x)<15&&std::abs(q.y-d.y)<15&&std::abs(q.z-d.z)<10){p.entity=e;p.release=!call<bool>(N::IS_ENTITY_A_MISSION_ENTITY,e);break;}}}
  if(!p.entity){p.entity=call<int>(N::CREATE_VEHICLE,d.model,d.x,d.y,d.z,d.heading,false,true,false);p.release=true;if(p.entity){call(N::FREEZE_ENTITY_POSITION,p.entity,true);p.waitingCollision=true;p.created=now;applyPin(p);}}
  if(p.entity){call(N::SET_ENTITY_AS_MISSION_ENTITY,p.entity,true,true);call(N::SET_VEHICLE_HAS_BEEN_OWNED_BY_PLAYER,p.entity,true);}
  call(N::SET_MODEL_AS_NO_LONGER_NEEDED,d.model);p.requested=0;p.retry=now+10000;
 }else if(now-p.requested>8000){call(N::SET_MODEL_AS_NO_LONGER_NEEDED,d.model);p.requested=0;p.retry=now+30000;if(target){pinTarget=0;call(N::CLEAR_FOCUS);note("timeout");}}
 }
 if(now-lastPinSave>5000){lastPinSave=now;for(auto& p:pins)snapshotPin(p);if(!pins.empty()&&pinsWritable&&!savePins())note("pinsaveerror");}
}
