from html.parser import HTMLParser
from pathlib import Path
class Parser(HTMLParser):
 def __init__(self):super().__init__();self.rows=[];self.row=None;self.cell=None
 def handle_starttag(self,tag,attrs):
  if tag=='tr':self.row=[]
  if tag in ('td','th') and self.row is not None:self.cell=[]
 def handle_data(self,text):
  if self.cell is not None:self.cell.append(text)
 def handle_endtag(self,tag):
  if tag in ('td','th') and self.cell is not None:self.row.append(''.join(self.cell).strip());self.cell=None
  if tag=='tr' and self.row is not None:self.rows.append(self.row);self.row=None
r=Path(__file__).resolve().parents[1];parser=Parser();parser.feed((r/'research/controls.html').read_text(encoding='utf-8'))
keys={'TAB':8192,'LEFT SHIFT':16384,'ARROW UP':1,'ARROW DOWN':2,'ARROW LEFT':4,'ARROW RIGHT':8,'ENTER':16,'ESC':32,'BACKSPACE':64,'PAGEUP':128,'PAGEDOWN':256,'LEFT MOUSE BUTTON':512,'RIGHT MOUSE BUTTON':1024,'SCROLLWHEEL UP':2048,'SCROLLWHEEL DOWN':4096}
pads={'DPAD UP':1,'DPAD DOWN':2,'DPAD LEFT':4,'DPAD RIGHT':8,'A':4096,'B':8192,'LB':256,'RB':512}
rows=[]
for row in parser.rows:
 if len(row)!=4 or not row[0].isdigit():continue
 mask=0
 for token in row[2].split(' / '):mask|=keys.get(token.strip(),0)
 pad=pads.get(row[3],0)
 if mask or pad:rows.append((int(row[0]),pad,mask))
assert len(rows)>70,len(rows)
header='''#pragma once
// Factual default control associations from the Cfx.re GTA V control reference.
// https://docs.fivem.net/docs/game-references/controls/
// Used while a reserved physical key/button is held, alongside runtime binding checks.
struct DefaultMenuBinding { int action; unsigned pad,keys; };
inline constexpr DefaultMenuBinding defaultMenuBindings[]={
'''+''.join(f'    {{{action},{pad},{keys}}},\n' for action,pad,keys in rows)+'};\n'
(r/'src/default_controls.hpp').write_text(header,encoding='utf-8')
print('Default menu-key associations:',len(rows))
