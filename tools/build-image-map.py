from pathlib import Path
import re,json
r=Path(__file__).resolve().parents[1]
s=(r/'research/VehicleSpawner.cpp').read_text(encoding='utf-8-sig')
rows={model.lower():(dictionary,texture) for model,dictionary,texture in re.findall(r'\{\s*VEHICLE_([A-Z0-9_]+)\s*,\s*"([^"]+)"\s*,\s*"([^"]+)"\s*\}',s)}
(r/'assets/vehicle-images.tsv').write_text(''.join(f'{m}|{d}|{t}\n' for m,(d,t) in sorted(rows.items())),encoding='utf-8')
