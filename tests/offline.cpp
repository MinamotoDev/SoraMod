#define SORAMOD_OFFLINE
#include "../src/main.cpp"
#include <cassert>
#include <iostream>
struct Invocation{U64 hash;std::vector<U64> arguments;};
U64 testResult[4]{};U64 currentHash;std::vector<U64> args;
std::vector<Invocation> calls;
bool keyboardMode=false;int keyboardStatus=0;
bool bindingMode=false,spawnMode=false,pinMode=false;int pinExists=77;bool pinCollision=true;
float fakeEngineHealth=-4000.f,fakeTankHealth=-1000.f;bool fakeUndriveable=true,fakeEngineRunning=false,restartBlocked=false,badRepairOrder=false;
void fakeInit(U64 h){currentHash=h;args.clear();}
void fakePush(U64 x){args.push_back(x);}
U64* fakeInvoke(){
 calls.push_back({currentHash,args});
 if(keyboardMode){static U64 value[1];value[0]=currentHash==N::UPDATE_ONSCREEN_KEYBOARD?(U64)keyboardStatus:currentHash==N::GET_ONSCREEN_KEYBOARD_RESULT?reinterpret_cast<U64>("adder"):0;return value;}
 if(pinMode){
  static U64 value[4];std::memset(value,0,sizeof(value));
  if(currentHash==N::PLAYER_PED_ID)value[0]=10;
  if(currentHash==N::DOES_ENTITY_EXIST)value[0]=(args[0]==10||args[0]==(U64)pinExists);
  if(currentHash==N::GET_ENTITY_MODEL)value[0]=123;
  if(currentHash==N::IS_MODEL_IN_CDIMAGE||currentHash==N::IS_MODEL_A_VEHICLE||currentHash==N::HAS_MODEL_LOADED||currentHash==N::IS_VEHICLE_SEAT_FREE)value[0]=1;
  if(currentHash==N::HAS_COLLISION_LOADED_AROUND_ENTITY)value[0]=pinCollision;
  if(currentHash==N::GET_VEHICLE_MODEL_NUMBER_OF_SEATS)value[0]=4;
  if(currentHash==N::GET_VEHICLE_PED_IS_IN)value[0]=0;
  if(currentHash==N::GET_ENTITY_COORDS){Vec3 p{10,0,20,0,30,0};std::memcpy(value,&p,sizeof(p));}
  if(currentHash==N::GET_VEHICLE_NUMBER_PLATE_TEXT)value[0]=reinterpret_cast<U64>("SORA");
  if(currentHash==N::CREATE_VEHICLE){pinExists=88;value[0]=88;}
  return value;
 }
 if(spawnMode){
  static U64 value[4];std::memset(value,0,sizeof(value));
  if(currentHash==N::HAS_MODEL_LOADED||currentHash==N::DOES_ENTITY_EXIST)value[0]=1;
  if(currentHash==N::PLAYER_PED_ID)value[0]=10;
  if(currentHash==N::GET_VEHICLE_PED_IS_IN)value[0]=77;
  if(currentHash==N::SET_VEHICLE_ENGINE_HEALTH)std::memcpy(&fakeEngineHealth,&args[1],4);
  if(currentHash==N::SET_VEHICLE_PETROL_TANK_HEALTH)std::memcpy(&fakeTankHealth,&args[1],4);
  if(currentHash==N::SET_VEHICLE_UNDRIVEABLE)fakeUndriveable=args[1]!=0;
  if(currentHash==N::SET_VEHICLE_FIXED&&fakeEngineHealth<=0)badRepairOrder=true;
  if(currentHash==N::SET_VEHICLE_ENGINE_ON)fakeEngineRunning=!restartBlocked&&args[1]&&fakeEngineHealth>0&&fakeTankHealth>0&&!fakeUndriveable;
  if(currentHash==N::GET_IS_VEHICLE_ENGINE_RUNNING)value[0]=fakeEngineRunning;
  if(currentHash==N::IS_VEHICLE_DRIVEABLE)value[0]=!fakeUndriveable&&fakeEngineHealth>0;
  if(currentHash==N::CREATE_VEHICLE)value[0]=77;
  if(currentHash==N::GET_MODEL_DIMENSIONS){*reinterpret_cast<Vec3*>(args[1])={-2,0,-5,0,-1,0};*reinterpret_cast<Vec3*>(args[2])={2,0,5,0,2,0};}
  if(currentHash==N::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS){float distance,z;std::memcpy(&distance,&args[2],4);std::memcpy(&z,&args[3],4);Vec3 position{100,0,200+distance,0,30+z,0};std::memcpy(value,&position,sizeof(position));}
  return value;
 }
 if(bindingMode&&currentHash==N::GET_CONTROL_INSTRUCTIONAL_BUTTONS_STRING){
  static std::string value;static U64 result[1];
  std::unordered_map<int,std::string> mapped={{187,"t_DOWN"},{188,"t_UP"},{189,"t_LEFT"},{190,"t_RIGHT"},{201,"t_ENTER"},{202,"t_ESC"},{177,"t_BACKSPACE"},{237,"mouse_primary"},{238,"mouse_secondary"},{241,"wheel_up"},{242,"wheel_down"},{24,"mouse_primary"},{25,"mouse_secondary"},{32,"t_W"},{33,"t_S"},{34,"t_A"},{35,"t_D"},{21,"t_SHIFT"},{75,"t_ENTER"},{205,"t_PGUP"},{206,"t_PGDN"}};
  int action=(int)args[1];value=mapped.count(action)?mapped[action]:"unused_"+std::to_string(action);result[0]=reinterpret_cast<U64>(value.c_str());return result;
 }
 return testResult;
}
int textureCount=0;float drawnWidth=0,drawnHeight=0,drawnAspect=0;
int fakeTexture(const char*){return ++textureCount;}
void fakeDraw(int,int index,int,int duration,float w,float h,float,float,float,float,float,float aspect,float,float,float,float){assert(duration>0&&index<64);drawnWidth=w;drawnHeight=h;drawnAspect=aspect;}
int countCalls(U64 hash){return (int)std::count_if(calls.begin(),calls.end(),[&](const Invocation& c){return c.hash==hash;});}
bool disabled(int action){return std::any_of(calls.begin(),calls.end(),[&](const Invocation& c){return c.hash==N::DISABLE_CONTROL_ACTION&&c.arguments[1]==(U64)action;});}
int main(){
 static_assert(sizeof(Vec3)==24);static_assert(offsetof(Vec3,y)==8);static_assert(offsetof(Vec3,z)==16);
 assert(SHV::argument(1.0f)==0x3f800000);assert(SHV::argument(-1)==~U64(0));
 SHV::init=fakeInit;SHV::push=fakePush;SHV::invoke=fakeInvoke;SHV::texture=fakeTexture;SHV::draw=fakeDraw;
 call(N::CREATE_VEHICLE,0x12345678U,1.f,2.f,3.f,90.f,false,true,false);
 assert(currentHash==N::CREATE_VEHICLE&&args.size()==8&&args[6]==1&&args[7]==0);
 Vec3 sample{1,0,2,0,3,0};std::memcpy(testResult,&sample,sizeof(sample));auto pos=call<Vec3>(N::GET_BLIP_INFO_ID_COORD,5);assert(pos.x==1&&pos.y==2&&pos.z==3);std::memset(testResult,0,sizeof(testResult));
 Button button;assert(button.update(true));assert(!button.update(true));assert(!button.update(false));assert(button.update(true));
 std::vector<unsigned char> pattern={0x2d,0,0,0,0,0x2c,0,0,0,0x56,0,0,0x71,0x2e,0,0,0x62,0x56,0x34,0x12};
 assert(MpVehicles::decodeGlobal(pattern)==0x123456);auto crossing=std::vector<unsigned char>(0x3ffb,0);crossing.insert(crossing.end(),pattern.begin(),pattern.end());assert(MpVehicles::decodeGlobal(crossing)==0x123456);
 auto duplicate=pattern;duplicate.insert(duplicate.end(),pattern.begin(),pattern.end());assert(MpVehicles::decodeGlobal(duplicate)==-1);pattern.pop_back();assert(MpVehicles::decodeGlobal(pattern)==-1);
 root="assets";readLanguage("en");assert(tr("god")=="Invincibility");readLanguage("zh-Hans");assert(tr("god")!="Invincibility");assert(tr("categories")!="categories");
 Font f;assert(f.load("assets",2));f.frame(2560,1440);f.text("SoraMod",50,90,470);assert(textureCount>0);assert(std::abs(drawnWidth*2560-drawnHeight*drawnAspect*1440)<.01f);assert(std::abs(drawnAspect-2560.f/1440)<.01f);
 f.frame(1920,1080);int before=textureCount;f.text(std::string(100,'Q'),0,100,5000);assert(textureCount-before==2); // Each texture has a separate 64-instance budget.
 vehicles={{"test","Test vehicle",1,false,7},{"test2","Other vehicle",2,true,7}};all={0,1};classified[7]={0,1};availableClasses={7};
 page=0;selected=5;visible=true;history.clear();activate();assert(page==1&&selected==0&&history.size()==1);
 selected=2;activate();assert(page==4&&rows().size()==1);activate();assert(page==5&&category==7&&rows().size()==2&&!isGrid());
 selected=1;setMenuVisible(false);auto remembered=history.size();setMenuVisible(true);assert(page==5&&selected==1&&category==7&&history.size()==remembered);
 previousLayer();assert(page==4&&selected==0);previousLayer();assert(page==1&&selected==2);selected=1;activate();assert(page==3&&isGrid());
 syncGalleryPause(true);calls.clear();dismissMenu();assert(visible&&page==1&&!galleryPaused);assert(countCalls(N::SET_GAME_PAUSED)==1);
 previousLayer();assert(page==0&&selected==5);selected=8;hour=23;activate();assert(hour==0&&currentHash==N::SET_CLOCK_TIME);
 page=1;selected=1;history.clear();enterPage(3);selected=13;setMenuVisible(false);calls.clear();openMenu();assert(visible&&page==1&&selected==1&&!isGrid()&&!galleryPaused);assert(countCalls(N::SET_GAME_PAUSED)==0);
 page=3;history.clear();setMenuVisible(false);openMenu();assert(page==0&&!isGrid());
 // A 23-entry category list is exactly 3 pages, including a short final page.
 availableClasses.clear();for(int i=0;i<23;++i)availableClasses.push_back(i);
 page=1;selected=2;history.clear();enterPage(4);selected=0;changePage(1);assert(selected==10);changePage(1);assert(selected==20);changePage(1);assert(selected==20);previousLayer();assert(page==4&&selected==10);previousLayer();assert(page==4&&selected==0);previousLayer();assert(page==1&&selected==2);
 page=1;selected=2;history.clear();enterPage(4);selected=20;visible=true;dismissMenu();assert(visible&&page==1&&selected==2); // Esc/controller B skip all list pages and return to the parent.
 page=0;selected=5;history.clear();visible=true;dismissMenu();assert(!visible&&page==0); // Root closes into gameplay.
 page=4;selected=20;canvasWidth=1920;Layout listLayout(23);assert(listLayout.start==20&&listLayout.count==3&&listLayout.capacity==10);assert(listLayout.hit(listLayout.x+1,listLayout.y+136)==20);assert(listLayout.hit(listLayout.x+1,listLayout.y+130)==-1);float footer=listLayout.footerBaseline(23);selected=0;assert(Layout(23).footerBaseline(23)==footer);
 page=3;selected=13;Layout layout(100);assert(layout.columns==4&&layout.capacity==12&&layout.start==12);assert(layout.hit(61,201)==12);assert(layout.hit(59,201)==-1);assert(!layout.cards[0].contains(layout.cards[0].x+layout.cards[0].w,201));
 galleryCategory=-1;selected=3;changeGalleryTab(1);assert(galleryCategory==0&&selected==0);changeGalleryTab(-1);assert(galleryCategory==-1);galleryCategory=7;assert(vehicleList().size()==2);page=5;category=7;assert(!isGrid()&&pageCapacity()==10&&vehicleList().size()==2);
 XINPUT_GAMEPAD padState{};padState.sThumbLX=32767;padState.sThumbLY=-32768;assert(!Input::direction(padState,XINPUT_GAMEPAD_DPAD_UP,true,true));assert(!Input::direction(padState,XINPUT_GAMEPAD_DPAD_DOWN,true,true));padState.wButtons=XINPUT_GAMEPAD_DPAD_DOWN;assert(Input::direction(padState,XINPUT_GAMEPAD_DPAD_DOWN,true,false));
 Input held;held.heldPad=XINPUT_GAMEPAD_DPAD_DOWN;assert(held.anyOwnedHeld(false,false));held.heldPad=XINPUT_GAMEPAD_B;assert(held.anyOwnedHeld(false,false));held.heldPad=0;held.heldKeys=32;assert(held.anyOwnedHeld(false,false));held.heldKeys=0;assert(!held.anyOwnedHeld(false,false));
 std::mt19937 deterministic(7);std::vector<int> everyType={0,1,2,3,4};std::set<int> seen;for(int i=0;i<2000;++i)seen.insert(pickSurpriseIndex(deterministic,everyType));assert(seen.size()==everyType.size());assert(pickSurpriseIndex(deterministic,{})==-1);
 bindingMode=true;InputGate testGate;calls.clear();testGate.apply(true,true,false,true,VK_F7);assert(disabled(24)&&disabled(25)&&disabled(75));assert(!disabled(32)&&!disabled(33)&&!disabled(21));assert(disabled(1)&&disabled(2));assert(disabled(14)&&disabled(16)&&disabled(99)&&disabled(115)&&disabled(261)&&disabled(334));assert(countCalls(N::DISABLE_ALL_CONTROL_ACTIONS)==0);
 calls.clear();testGate.apply(true,true,false,false,VK_F7,false,XINPUT_GAMEPAD_DPAD_DOWN);assert(disabled(19)&&disabled(20)&&!disabled(32));
 calls.clear();testGate.apply(true,true,false,false,VK_F7,false,XINPUT_GAMEPAD_DPAD_DOWN);assert(disabled(19)&&disabled(20)); // Repeat the whole held state, not a just-pressed event.
 calls.clear();testGate.apply(true,true,false,false,VK_F7,false,XINPUT_GAMEPAD_B);assert(disabled(45)&&disabled(80)&&disabled(140));
 calls.clear();testGate.apply(true,true,false,true,VK_F7,false,0,16);assert(disabled(18));
 calls.clear();testGate.apply(true,true,false,false,VK_F7,false,0,2048);assert(disabled(99)&&disabled(261)&&!disabled(32));
 calls.clear();testGate.apply(true,false,false,true,VK_F7);assert(countCalls(N::DISABLE_ALL_CONTROL_ACTIONS)==3);
 calls.clear();testGate.apply(true,true,true,true,VK_F7,true);assert(countCalls(N::DISABLE_ALL_CONTROL_ACTIONS)==3);
 calls.clear();testGate.apply(false,true,false,true,VK_F7);assert(calls.empty());bindingMode=false;
 galleryPaused=false;calls.clear();syncGalleryPause(true);syncGalleryPause(true);setMenuVisible(false);assert(countCalls(N::SET_GAME_PAUSED)==2&&!galleryPaused); // Acquire/release once, including hide/focus-loss path.
 spawnMode=true;calls.clear();page=0;selected=4;activate();assert(countCalls(N::SET_VEHICLE_FIXED)==1&&countCalls(N::SET_VEHICLE_UNDRIVEABLE)==2&&countCalls(N::SET_VEHICLE_ENGINE_ON)==1);auto engine=std::find_if(calls.begin(),calls.end(),[](const Invocation& c){return c.hash==N::SET_VEHICLE_ENGINE_ON;});assert(engine->arguments[1]==1&&engine->arguments[2]==1&&engine->arguments[3]==0);
 assert(fakeEngineHealth==1000.f&&fakeTankHealth==1000.f&&!fakeUndriveable&&fakeEngineRunning&&!badRepairOrder);tickRepair();assert(repairTarget==0&&message==tr("done"));
 restartBlocked=true;startRepair(77);assert(!fakeEngineRunning);repairDeadline=GetTickCount()-1;tickRepair();assert(repairTarget==0&&message==tr("repairfailed"));restartBlocked=false;
 calls.clear();pending={1,GetTickCount(),1,0,0,false};visible=true;tickPending();assert(pending.kind==0&&!visible&&spawned.back()==77&&!falling.empty());
 assert(countCalls(N::SET_ENTITY_COORDS_NO_OFFSET)==0);assert(countCalls(0xF75B0D629E1C063DULL)==0);assert(countCalls(N::SET_ENTITY_HAS_GRAVITY)==1&&countCalls(N::SET_ENTITY_VELOCITY)==1);assert(countCalls(N::SET_ENTITY_INVINCIBLE)==1);
 auto offset=std::find_if(calls.begin(),calls.end(),[](const Invocation& c){return c.hash==N::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS;});assert(offset!=calls.end());float distance,height;std::memcpy(&distance,&offset->arguments[2],4);std::memcpy(&height,&offset->arguments[3],4);assert(distance==9.f&&height==4.f);
 falling.back().protectUntil=GetTickCount()-1;calls.clear();tickFalling();assert(falling.empty()&&countCalls(N::SET_ENTITY_INVINCIBLE)==1);spawnMode=false;

 // Grid arrows follow visual rows/columns, including page boundaries and short last rows.
 page=3;canvasWidth=1920;selected=1;moveGrid(0,-1,26);assert(selected==1);
 moveGrid(0,1,26);assert(selected==5);moveGrid(1,0,26);assert(selected==6);moveGrid(-1,0,26);assert(selected==5);
 selected=10;moveGrid(0,1,26);assert(selected==14);moveGrid(0,-1,26);assert(selected==10);
 selected=11;moveGrid(1,0,26);assert(selected==12);moveGrid(-1,0,26);assert(selected==11);
 selected=22;moveGrid(0,1,26);assert(selected==25);moveGrid(0,1,26);assert(selected==25);
 page=3;canvasWidth=1280;selected=2;moveGrid(0,1,100);assert(selected==4);canvasWidth=1920;
 assert(normalizeCode("  OpPrEsSoR2 \n")=="oppressor2");assert(normalizeCode("adder;delete").empty());assert(normalizeCode(" ").empty());assert(normalizeCode(std::string(64,'x')).empty());assert(hash(normalizeCode("ADDER"))==hash("adder"));
 // The menu confirmation must be fully released before the native dialog opens.
 page=1;selected=3;calls.clear();input.accept.before=true;keyboardMode=true;activate();DWORD began=codeReleasedAt;
 assert(codeInput&&!codeDisplayed&&countCalls(N::DISPLAY_ONSCREEN_KEYBOARD)==0);
 tickCodeInput(began+500);assert(!codeDisplayed&&countCalls(N::UPDATE_ONSCREEN_KEYBOARD)==0);
 input.accept.before=false;input.mouseAccept.before=true;tickCodeInput(began+600);assert(!codeDisplayed);input.mouseAccept.before=false;
 tickCodeInput(began+749);assert(!codeDisplayed);tickCodeInput(began+750);assert(codeDisplayed&&countCalls(N::DISPLAY_ONSCREEN_KEYBOARD)==1);
 auto dialog=std::find_if(calls.begin(),calls.end(),[](const Invocation& c){return c.hash==N::DISPLAY_ONSCREEN_KEYBOARD;});assert(std::string(reinterpret_cast<const char*>(dialog->arguments[3])).empty());
 keyboardStatus=-1;tickCodeInput(began+751);assert(codeInput);keyboardStatus=3;tickCodeInput(began+800);assert(codeInput);
 keyboardStatus=0;tickCodeInput(began+900);tickCodeInput(began+60000);assert(codeInput&&codeEditing); // No timeout while typing.
 keyboardStatus=2;tickCodeInput(began+60001);assert(!codeInput&&!codeDisplayed&&countCalls(N::GET_ONSCREEN_KEYBOARD_RESULT)==0);
 // A genuinely failed native launch exits after the startup grace period.
 startCodeInput();began=codeReleasedAt;tickCodeInput(began+150);keyboardStatus=-1;tickCodeInput(began+1650);assert(!codeInput&&message==tr("keyboardfailed"));// A completed dialog reads its result once in the completion tick.
 startCodeInput();began=codeReleasedAt;tickCodeInput(began+150);keyboardStatus=0;tickCodeInput(began+160);keyboardStatus=1;int reads=countCalls(N::GET_ONSCREEN_KEYBOARD_RESULT);tickCodeInput(began+170);assert(!codeInput&&countCalls(N::GET_ONSCREEN_KEYBOARD_RESULT)==reads+1);tickCodeInput(began+180);assert(countCalls(N::GET_ONSCREEN_KEYBOARD_RESULT)==reads+1);keyboardMode=false;
 // Persistence roundtrip, backup recovery, corruption rejection and failed-save rollback.
 auto resourceRoot=root;root="build/pin-tests-"+std::to_string(GetTickCount64());pins.clear();pinsWritable=true;pinsLoaded=false;
 SavedVehicle saved;saved.id=17;saved.model=123;saved.x=10;saved.y=20;saved.z=30;saved.heading=180;std::strcpy(saved.plate,"SORA");saved.mods[11]=3;saved.customPrimary=1;saved.custom[0]=220;
 pins.push_back({saved});assert(savePins());pins[0].data.x=40;assert(savePins());pins.clear();loadPins();assert(pins.size()==1&&pins[0].data.x==40&&pins[0].data.mods[11]==3&&pins[0].data.custom[0]==220);
 {std::ofstream broken(pinPath(),std::ios::binary|std::ios::trunc);broken<<"broken";}pins.clear();loadPins();assert(pins.size()==1&&pins[0].data.x==10);assert(savePins());
 {std::ofstream broken(pinPath(),std::ios::binary|std::ios::trunc);broken<<"broken";std::ofstream brokenBackup(fs::path(pinPath().wstring()+L".bak"),std::ios::binary|std::ios::trunc);brokenBackup<<"broken";}
 pins.clear();pinsWritable=true;loadPins();assert(!pinsWritable&&!savePins());
 pins.push_back({saved});assert(!removePin(17)&&pins.size()==1);clearPins();assert(pins.size()==1);
 pinsWritable=true;assert(savePins());page=6;selected=2;history.clear();activate();assert(page==9&&selected==0&&pins.size()==1);activate();assert(page==6&&pins.size()==1);
 selected=2;activate();selected=1;calls.clear();activate();assert(page==6&&pins.empty()&&countCalls(N::SET_ENTITY_AS_NO_LONGER_NEEDED)==0);pins.clear();loadPins();assert(pins.empty());

 // Missing vehicles restore from stored coordinates; boarding waits for collision.
 pins.push_back({saved});vehicles.push_back({"pin-test","Pin test",123,false});pinMode=true;pinExists=0;pinCollision=false;pinsLoaded=true;lastPinSave=GetTickCount();calls.clear();pinTarget=17;
 tickPins();assert(pins[0].entity==88&&pins[0].waitingCollision&&countCalls(N::CREATE_VEHICLE)==1&&countCalls(N::SET_PED_INTO_VEHICLE)==0);
 auto create=std::find_if(calls.begin(),calls.end(),[](const Invocation& c){return c.hash==N::CREATE_VEHICLE;});float sx;std::memcpy(&sx,&create->arguments[1],4);assert(sx==10);
 tickPins();assert(countCalls(N::SET_PED_INTO_VEHICLE)==0&&pinTarget==17);pinCollision=true;tickPins();assert(!pins[0].waitingCollision&&pinTarget==0&&countCalls(N::SET_PED_INTO_VEHICLE)==1);
 assert(isPinned(88));calls.clear();assert(removePin(17)&&pins.empty());assert(countCalls(N::SET_ENTITY_AS_NO_LONGER_NEEDED)==1&&countCalls(0xEA386986E786A54FULL)==0); // Unpin releases ownership without deleting the vehicle.
 pins.clear();pinMode=false;
 // Test directory is created only under build and uniquely named for this process.
 fs::remove_all(fs::u8path(root));root=resourceRoot;pinsWritable=true;pinsLoaded=false;

 std::cout<<"PASS: native input release guard, blank default, startup grace, editing and cancel/submit lifecycle, grid row/column navigation and cross-page movement, model code validation, persistent pin roundtrip/corruption recovery/two-step clear,  paginated category lists and first-page back, gallery tabs, session memory, D-pad-only input, held-key gameplay suppression, wheel block, all-category random, low vehicle drop and no player movement/auto-seat, temporary protection cleanup, ABI and asset rendering\n";
}
