#define SORAMOD_OFFLINE
#include "../src/main.cpp"
std::ofstream output;std::unordered_map<int,std::string> paths;int nextTexture=0;U64 result[4]{};U64 native;std::vector<U64> parameters;
float asFloat(U64 n){float f;std::memcpy(&f,&n,4);return f;}
void start(U64 h){native=h;parameters.clear();}
void push(U64 a){parameters.push_back(a);}
U64* finish(){
 std::memset(result,0,sizeof(result));
 if(native==N::GET_ACTUAL_SCREEN_RESOLUTION){*reinterpret_cast<int*>(parameters[0])=1920;*reinterpret_cast<int*>(parameters[1])=1080;}
 if(native==N::GET_SAFE_ZONE_SIZE){float safe=1;std::memcpy(result,&safe,4);}
 if(native==N::DRAW_RECT&&output){output<<"[\"rect\","<<asFloat(parameters[0])<<','<<asFloat(parameters[1])<<','<<asFloat(parameters[2])<<','<<asFloat(parameters[3]);for(int i=4;i<8;++i)output<<','<<parameters[i];output<<"]\n";}
 return result;
}
int texture(const char* path){paths[++nextTexture]=path;return nextTexture;}
void draw(int id,int,int,int,float sx,float sy,float,float,float x,float y,float,float aspect,float red,float,float,float){if(output)output<<"[\"glyph\",\""<<paths[id]<<"\","<<sx<<','<<sy*aspect<<','<<x<<','<<y<<','<<red<<"]\n";}
int main(){SHV::init=start;SHV::push=push;SHV::invoke=finish;SHV::texture=texture;SHV::draw=draw;root="assets";readLanguage("en");readLanguage("zh-Hans");font.load("assets",2);
 std::ifstream data("assets/vehicles.tsv");std::string line;while(std::getline(data,line)){auto row=split(line,'|');if(row.size()<15)continue;all.push_back((int)vehicles.size());vehicles.push_back({row[0],row[12],hash(row[0]),row[1]=="1"});if(vehicles.size()==40)break;}
 for(int c=0;c<23;++c)availableClasses.push_back(c);for(size_t i=0;i<vehicles.size();++i)classified[i%23].push_back((int)i);
 page=3;selected=0;visible=true;for(int i=0;i<50;++i)render();output.open("build/gallery-preview.jsonl");render();output.close();
 page=0;selected=6;for(int i=0;i<50;++i)render();output.open("build/menu-preview.jsonl");render();output.close();
 page=4;selected=0;for(int i=0;i<50;++i)render();output.open("build/categories-preview.jsonl");render();output.close();
 page=6;selected=0;for(int i=0;i<50;++i)render();output.open("build/pinned-preview.jsonl");render();output.close();
 page=1;selected=3;for(int i=0;i<50;++i)render();output.open("build/spawn-preview.jsonl");render();output.close();
}
