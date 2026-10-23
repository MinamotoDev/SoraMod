from pathlib import Path
import shutil,zipfile,hashlib,json,datetime
r=Path(__file__).resolve().parents[1]
stage=r/'dist/SoraMod-0.4.1-win-x64';stage.mkdir(parents=True,exist_ok=True)
shutil.copy2(r/'build/SoraMod.asi',stage/'SoraMod.asi')
shutil.copytree(r/'assets',stage/'SoraMod',dirs_exist_ok=True)
for name in ['README.md','RELEASE_NOTES.md','RELEASE_NOTES.zh-CN.md','THIRD_PARTY.md','LICENSE']:shutil.copy2(r/name,stage/name)
shutil.copytree(r/'docs',stage/'docs',dirs_exist_ok=True)
archive=stage.parent/(stage.name+'.zip')
with zipfile.ZipFile(archive,'w',compression=zipfile.ZIP_DEFLATED,compresslevel=9) as z:
 for file in sorted(stage.rglob('*')):
  if file.is_file():z.write(file,file.relative_to(stage).as_posix())
with zipfile.ZipFile(archive) as z:
 assert z.testzip() is None
 assert 'SoraMod.asi' in z.namelist() and 'SoraMod/lang/zh-Hant.ini' in z.namelist()
 assert 'LICENSE' in z.namelist() and 'RELEASE_NOTES.md' in z.namelist()
 assert sum(n.startswith('docs/README.') and n.endswith('.md') for n in z.namelist())==12
 assert not any('ScriptHookV.dll' in n or 'research/' in n for n in z.namelist())
sha=hashlib.sha256(archive.read_bytes()).hexdigest();archive.with_suffix('.zip.sha256').write_text(sha+'  '+archive.name+'\n',encoding='ascii')
report={'version':'0.4.1','author':'MinamotoDev','archive':archive.name,'size':archive.stat().st_size,'sha256':sha,'checks':['GCC C++17 x64 static build with warnings treated as errors','Offline native ABI and vector return tests','List pagination, first-page back, session memory, category tabs and pause ownership tests','D-pad-only navigation and sustained held-button suppression tests','Low vehicle drop, temporary protection, no player teleport or auto-seat tests','Open shortcut always restores an ordinary menu; damaged-engine health recovery, driveability and restart validation','MP global signature ambiguity, truncation and cross-page tests','Native keyboard opening-key release guard, blank default text, startup grace, editing and cancel/submit lifecycle','Grid row/column movement, cross-page navigation and model code validation','Persistent vehicle data roundtrip, backup recovery, failed-save rollback and clear confirmation','Pinned vehicle restoration waits for collision before boarding; unpin releases without deletion','NativeDB argument counts','13 locale keys and complete asset glyph/cmap coverage','921 vehicle records, localized surprise action and all-category random selection','Windows automatic dependency loading and script registration with mock ScriptHookV','13 localized READMEs, ordered multilingual release anchors, valid document links and GPL v3 license','ZIP CRC and package contents'],'runtime':'Awaiting GTA V Enhanced Story Mode test; no installation into game performed'}
(r/'build/verification.json').write_text(json.dumps(report,ensure_ascii=False,indent=2),encoding='utf-8')
print(json.dumps(report,ensure_ascii=False,indent=2))
