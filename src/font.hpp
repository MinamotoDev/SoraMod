#pragma once
#include "shv.hpp"
#include <unordered_map>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cmath>
inline std::vector<std::uint32_t> utf8(const std::string& s){std::vector<std::uint32_t> out;for(size_t i=0;i<s.size();){auto c=static_cast<unsigned char>(s[i++]);std::uint32_t n=c;int extra=0;if(c>=0xf0){n=c&7;extra=3;}else if(c>=0xe0){n=c&15;extra=2;}else if(c>=0xc0){n=c&31;extra=1;}while(extra--&&i<s.size())n=(n<<6)|(static_cast<unsigned char>(s[i++])&63);out.push_back(n);}return out;}
class Font {
 struct Glyph {float advance;int texture[2]={-1,-1};int instance=0;};std::unordered_map<std::uint32_t,Glyph> glyphs;std::string base;int face=0;float width=1920,height=1080,scale=1;int loaded=0;
public:
 bool load(const std::string& root,int f){glyphs.clear();base=root;face=f;std::ifstream file(base+"/glyphs/"+std::to_string(face)+"/metrics.txt");std::string code;float advance;while(file>>code>>advance)glyphs.emplace(std::stoul(code,nullptr,16),Glyph{advance});return !glyphs.empty();}
 void frame(int w,int h){width=static_cast<float>(w);height=static_cast<float>(h);scale=height/1080;loaded=0;for(auto& p:glyphs)p.second.instance=0;}
 float measure(const std::string& s,float zoom=1)const{float n=0;for(auto c:utf8(s)){auto g=glyphs.find(c);if(g!=glyphs.end())n+=g->second.advance*zoom;}return n;}
 void text(const std::string& s,float x,float y,float limit,bool black=false,float zoom=1){float edge=x+limit;float z=zoom;if(measure(s,z)>limit)z=std::max(0.55f,limit/std::max(1.0f,measure(s)));for(auto c:utf8(s)){auto it=glyphs.find(c);if(it==glyphs.end())it=glyphs.find('?');if(it==glyphs.end())continue;auto& g=it->second;if(x+g.advance*z>edge+1)break;
  int slot=g.instance/64;
  if(c!=32&&slot<2&&g.texture[slot]<0&&loaded<10){std::ostringstream hex;hex<<std::hex<<std::uppercase<<c;g.texture[slot]=SHV::texture((base+"/glyphs/"+std::to_string(face)+"/"+hex.str()+(slot?"-1":"")+".png").c_str());++loaded;}
  if(c!=32&&slot<2&&g.texture[slot]>=0){float color=black?0.f:1.f;SHV::draw(g.texture[slot],g.instance%64,20,100,48*z*scale/width,48*z*scale/width,0,0,x*scale/width,(y-35*z)*scale/height,0,width/height,color,color,color,1);}
  if(c!=32)++g.instance;
  x+=g.advance*z;}}
};
