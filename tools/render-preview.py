from pathlib import Path
from PIL import Image,ImageDraw
import json
r=Path(__file__).resolve().parents[1]
for kind in ['gallery','menu','categories','pinned','spawn']:
 im=Image.new('RGBA',(1920,1080),(52,65,62,255))
 for line in (r/f'build/{kind}-preview.jsonl').read_text().splitlines():
  row=json.loads(line)
  if row[0]=='rect':
   _,x,y,w,h,red,green,blue,alpha=row
   overlay=Image.new('RGBA',im.size);draw=ImageDraw.Draw(overlay);draw.rectangle(((x-w/2)*1920,(y-h/2)*1080,(x+w/2)*1920,(y+h/2)*1080),fill=(red,green,blue,alpha));im=Image.alpha_composite(im,overlay)
  else:
   _,path,w,h,x,y,red=row;glyph=Image.open(r/path).convert('RGBA');glyph=glyph.resize((max(1,round(w*1920)),max(1,round(h*1080))))
   if red==0:
    alpha=glyph.getchannel('A');glyph=Image.new('RGBA',glyph.size,(0,0,0));glyph.putalpha(alpha)
   im.alpha_composite(glyph,(round(x*1920),round(y*1080)))
 im.convert('RGB').save(r/f'build/{kind}-preview.png')
 print('Rendered',kind)
