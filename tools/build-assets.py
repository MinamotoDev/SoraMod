from pathlib import Path
import json,re,sys
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/"research/python-deps"))
from fontTools.ttLib import TTFont
from PIL import Image,ImageDraw,ImageFont
root=Path(__file__).resolve().parents[1]
langs=['en','fr','it','de','es','ja','ko','pl','pt-BR','ru','zh-Hans','es-419','zh-Hant']
source=json.loads((root/'research/vehicles.json').read_text(encoding='utf-8-sig'))
fields=['English','French','Italian','German','Spanish','Japanese','Korean','Polish','Portuguese','Russian','SimplifiedChinese','Mexican','TraditionalChinese']
rows=[]
for v in sorted(source,key=lambda v:v['Name'].lower()):
 model=v['Name'].lower(); names=[v['DisplayName'].get(f) or v['DisplayName'].get('English') or model for f in fields]
 names=[n.replace('|','/').replace('\n',' ').replace('\r',' ') for n in names]
 rows.append('|'.join([model,'1' if v['DlcName'].lower().startswith('mp') else '0',*names]))
(root/'assets/vehicles.tsv').write_text('\n'.join(rows)+'\n',encoding='utf-8')
compat=['Online vehicle support is unavailable for this game build','Véhicules Online non pris en charge sur cette version','Veicoli Online non supportati in questa versione','Online-Fahrzeuge für diese Spielversion nicht unterstützt','Vehículos Online no compatibles con esta versión','このゲームバージョンのオンライン車両には未対応です','이 게임 버전의 온라인 차량은 지원되지 않습니다','Pojazdy Online nieobsługiwane w tej wersji','Veículos Online incompatíveis com esta versão','Транспорт Online не поддерживается в этой версии','当前版本的线上车型防消失适配未就绪','Vehículos Online no compatibles con esta versión','目前版本的線上車款防消失適配尚未就緒']
chars={i:set('SoraMod 0.1.0 0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz<>[]()/:-_.?') for i in range(4)}
for k,lang in enumerate(langs):
 p=root/'assets/lang'/f'{lang}.ini';text=p.read_text(encoding='utf-8')
 if 'compat=' not in text:text+='compat='+compat[k]+'\n';p.write_text(text,encoding='utf-8')
 face={'ja':0,'ko':1,'zh-Hans':2,'zh-Hant':3}.get(lang,0)
 for line in text.splitlines():chars[face].update(line.partition('=')[2])
 for v in source:chars[face].update(v['DisplayName'].get(fields[k]) or v['Name'])
for face,codes in chars.items():
 out=root/'assets/glyphs'/str(face);out.mkdir(parents=True,exist_ok=True)
 font=ImageFont.truetype(str(root/'assets/fonts/NotoSansCJK-Regular.ttc'),56,index=face)
 cmap=TTFont(root/"assets/fonts/NotoSansCJK-Regular.ttc",fontNumber=face).getBestCmap()
 fallback=ImageFont.truetype(str(root/"assets/fonts/NotoSans-Regular.ttf"),56)
 fallback_cmap=TTFont(root/"assets/fonts/NotoSans-Regular.ttf").getBestCmap()
 metrics=[]
 for c in sorted(codes):
  if ord(c)<32:continue
  chosen=font if ord(c) in cmap else fallback
  assert ord(c) in cmap or ord(c) in fallback_cmap,(face,c)
  advance=chosen.getlength(c)/2
  im=Image.new('RGBA',(96,96));d=ImageDraw.Draw(im);d.text((2,70),c,font=chosen,fill=(255,255,255,255),anchor='ls')
  im.save(out/(f'{ord(c):X}.png'),optimize=True);im.save(out/(f'{ord(c):X}-1.png'),optimize=True);metrics.append(f'{ord(c):X} {advance:.3f}')
 (out/'metrics.txt').write_text('\n'.join(metrics),encoding='utf-8')
 print('font face',face,'glyphs',len(metrics))
# Generate only the native constants referenced by project source from the primary NativeDB.
natives={v['name']:h for g in json.loads((root/'research/natives.json').read_text()).values() for h,v in g.items()}
used=set()
for p in [p for p in (root/'src').iterdir() if p.suffix in ('.cpp','.hpp') and p.name!='natives.hpp']:used.update(re.findall(r'N::([A-Z0-9_]+)',p.read_text(encoding='utf-8-sig')))
missing=used-natives.keys();assert not missing,missing
(root/'src/natives.hpp').write_text('#pragma once\n#include <cstdint>\nnamespace N {\n'+''.join(f'inline constexpr std::uint64_t {n}={natives[n]}ULL;\n' for n in sorted(used))+'}\n',encoding='utf-8')
print('vehicles',len(rows),'native constants',len(used))
