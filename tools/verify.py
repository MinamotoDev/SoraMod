from pathlib import Path
import json,re,ctypes,zipfile,hashlib,shutil,struct,subprocess
from PIL import Image,ImageDraw,ImageFont
import sys
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/"research/python-deps"))
from fontTools.ttLib import TTFont
r=Path(__file__).resolve().parents[1]
langs=['en','fr','it','de','es','ja','ko','pl','pt-BR','ru','zh-Hans','es-419','zh-Hant']
keys=None
vehicle=[x.split('|') for x in (r/'assets/vehicles.tsv').read_text(encoding='utf-8').splitlines()]
assert len(vehicle)==921 and len({x[0] for x in vehicle})==921
assert all(len(x)==15 and x[0]==x[0].lower() for x in vehicle)
images=[line.split('|') for line in (r/'assets/vehicle-images.tsv').read_text().splitlines()]
assert all(len(x)==3 and x[1] and x[2] for x in images)
mapped={x[0] for x in images}&{x[0] for x in vehicle}
assert len(mapped)>600
print('Preview mappings matching catalog:',len(mapped))
for model in ['oppressor2','deluxo','toreador','scramjet','vigilante','raiju']:
 assert any(x[0]==model and x[1]=='1' for x in vehicle),model
for i,lang in enumerate(langs):
 content=dict(x.split('=',1) for x in (r/'assets/lang'/f'{lang}.ini').read_text(encoding='utf-8').splitlines() if '=' in x)
 if keys is None:keys=set(content)
 assert set(content)==keys,lang
 assert 'surprise' in content and 'favorites' not in content,lang
 if lang=='zh-Hans':assert content['categories']=='分类' and content['surprise']=='给我个惊喜！'
 if lang=='zh-Hant':assert content['categories']=='類別' and content['surprise']=='給我個驚喜！'
 face={'ko':1,'zh-Hans':2,'zh-Hant':3}.get(lang,0)
 cmap=TTFont(r/'assets/fonts/NotoSansCJK-Regular.ttc',fontNumber=face).getBestCmap()
 used=''.join(content.values())+''.join(x[i+2] for x in vehicle)+'SoraMod 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>[]()/:-_.?'
 for c in set(used):
  if ord(c)<32:continue
  assert ord(c) in cmap or ord(c) in TTFont(r/"assets/fonts/NotoSans-Regular.ttf").getBestCmap(),(lang,c,'font cmap')
  assert (r/'assets/glyphs'/str(face)/f'{ord(c):X}.png').exists(),(lang,c,'glyph')
  assert (r/'assets/glyphs'/str(face)/f'{ord(c):X}-1.png').exists(),(lang,c,'glyph overflow')
# Check every referenced native's argument count against the primary NativeDB.
natives={v['name']:v for ns in json.loads((r/'research/natives.json').read_text()).values() for v in ns.values()}
s='\n'.join(p.read_text(encoding='utf-8-sig') for p in (r/'src').iterdir() if p.suffix in ('.cpp','.hpp') and p.name!='natives.hpp')
for match in re.finditer(r'call(?:<[^>]*>)?\(N::([A-Z_0-9]+)',s):
 p=match.end();level=1;commas=0;quote=None
 while level:
  c=s[p];p+=1
  if quote:
   if c==quote and s[p-2]!='\\':quote=None
   continue
  if c in '\"\'':quote=c
  elif c in '([{':level+=1
  elif c in ')]}':level-=1
  elif c==',' and level==1:commas+=1
 expected=len(natives[match[1]]['params'])
 assert commas==expected,(match[1],commas,expected)
# Load/unload the compiled PE without injecting it into a game process.
binary=r/'build/SoraMod.asi';data=binary.read_bytes();pe=struct.unpack_from('<I',data,0x3c)[0]
assert data[pe:pe+4]==b'PE\0\0' and struct.unpack_from('<H',data,pe+4)[0]==0x8664
imports=subprocess.check_output([str(r/'research/toolchain/w64devkit/bin/objdump.exe'),'-p',str(binary)],text=True)
assert 'DLL Name: ScriptHookV.dll' in imports and '?scriptWait@@YAXK@Z' in imports
subprocess.run([sys.executable,str(r/'tests/loader.py')],check=True)
# Contact sheet uses exactly the bundled glyph sprites, not a substitute font.
canvas=Image.new('RGB',(1920,2000),'#303b40');draw=ImageDraw.Draw(canvas)
for i,lang in enumerate(langs):
 x=20+(i%4)*475;y=15+(i//4)*495
 strings=dict(x.split('=',1) for x in (r/'assets/lang'/f'{lang}.ini').read_text(encoding='utf-8').splitlines() if '=' in x)
 face={'ko':1,'zh-Hans':2,'zh-Hant':3}.get(lang,0)
 metrics={int(c,16):float(a) for c,a in (x.split() for x in (r/'assets/glyphs'/str(face)/'metrics.txt').read_text().splitlines())}
 def text(value,xx,baseline,limit,black=False,zoom=.85):
  length=sum(metrics.get(ord(c),0) for c in value);z=min(zoom,limit/max(1,length))
  for c in value:
   glyph=Image.open(r/'assets/glyphs'/str(face)/f'{ord(c):X}.png').convert('RGBA');size=max(1,round(48*z));glyph=glyph.resize((size,size))
   if black:
    alpha=glyph.getchannel('A');glyph=Image.new('RGBA',glyph.size,(0,0,0));glyph.putalpha(alpha)
   canvas.paste(glyph,(round(xx),round(baseline-35*z)),glyph);xx+=metrics.get(ord(c),0)*z
 draw.rectangle((x,y,x+450,y+80),fill='#0a0e12');draw.rectangle((x,y+78,x+450,y+80),fill='#70b246');text('SoraMod',x+14,y+50,420,zoom=1.2)
 text(strings['home'],x+14,y+107,420)
 for j,key in enumerate(['god','heal','wanted','ammo','repair','spawn','waypoint','hour']):
  yy=y+120+j*37;draw.rectangle((x,yy,x+450,yy+37),fill='#f5f5f5' if j==0 else '#121619');text(strings[key],x+12,yy+28,425,j==0)
 text(lang,x+12,y+450,425,zoom=.8)
canvas.save(r/'build/menu-languages.png')
print('PASS: 13 language keys, all bundled glyphs and font cmap, 921 unique models, Online-only model availability, native arity, x64 PE imports and loader registration, multilingual preview')
