# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Story-Modus-Trainer für GTA V Enhanced von MinamotoDev. Unterstützt Tastatur, Maus und Controller und verwendet mitgelieferte Schriften für das Menü.

[Neueste Version herunterladen](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Versionshinweise](../RELEASE_NOTES.md)

## Funktionen

- Unverwundbarkeit, Gesundheit und Panzerung auffüllen, Fahndungslevel entfernen, Munition auffüllen und das Fahrzeug reparieren.
- Fahrzeuge nach Kategorie, über Vollbildkarten, zufällig oder per Modellname spawnen, einschließlich der im Spiel verfügbaren Online-exklusiven Modelle.
- Fahrzeuge mit Position, Kennzeichen, Farben und gängigen Modifikationen sichern; aus der Ferne freigeben, hineinteleportieren oder auf der Karte markieren.
- Zum Kartenwegpunkt teleportieren; Uhrzeit ändern, Uhr anhalten oder Wetter wechseln.
- Das Menü folgt der Spielsprache.

## Installation

1. Füge `-nobattleye` zur Datei `args.txt` im Spielverzeichnis hinzu.
2. Installiere [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) und einen ASI Loader, die zu deiner Spielversion passen.
3. Lade `SoraMod-0.4.1-win-x64.zip` herunter und kopiere `SoraMod.asi` sowie den Ordner `SoraMod` in das Spielverzeichnis.
4. Starte den Story-Modus und öffne das Menü mit `F7`.

Benötigt Windows 10 oder neuer mit 64 Bit und GTA V Enhanced. Nur Story-Modus. Beende das Spiel vor einem Update und überschreibe die Mod-Dateien; `SoraMod/settings.ini` und `SoraMod/data` enthalten Einstellungen und gespeicherte Daten.

## Steuerung

| Aktion | Tastatur | Controller |
| --- | --- | --- |
| Öffnen / ausblenden | F7 | RB / R1 halten, dann Steuerkreuz rechts |
| Listeneintrag auswählen | Hoch / Runter | Steuerkreuz hoch / runter |
| Nächste Listenseite | Rechts / Bild ab | Steuerkreuz rechts / RB |
| Vorherige Listenseite; auf Seite eins zurück | Links / Rücktaste | Steuerkreuz links |
| Vollbildkarten auswählen | Pfeiltasten: hoch/runter zeilenweise, links/rechts zur Nachbarkarte | Steuerkreuz hoch / runter / links / rechts |
| Bestätigen | Eingabe | A / Kreuz |
| Übergeordnetes Menü; im Hauptmenü zurück zum Spiel | Esc | B / Kreis |
| Kategorietabs im Vollbild | Tab / Umschalt+Tab | RB / LB (R1 / L1) |
| Direkt zur vorherigen Seite | Bild auf | LB / L1 in Listen |

## Fahrzeugdaten

„Per Modellname spawnen“ akzeptiert Modellcodes wie `adder` und `oppressor2`. Das Eingabefeld ist anfangs leer. Verfügbare Modelle hängen von der installierten Spielversion ab.

Öffne „Gesicherte Fahrzeuge“, während du in einem Fahrzeug sitzt, um es zu sichern oder freizugeben. Die Daten liegen in `SoraMod/data/pinned-vehicles.dat`. Position und Modifikationen werden alle 5 Sekunden gespeichert; Sichern und Freigeben speichert sofort. Nach einem Spielneustart werden Fahrzeuge wiederhergestellt, wenn du dich ihrem gespeicherten Standort näherst oder hineinteleportierst. Die danebenliegende `.bak`-Datei dient zur Wiederherstellung beschädigter Daten. Das Freigeben aller Fahrzeuge entfernt sie nicht aus der Spielwelt.

## Deinstallation und Rückmeldungen

Lösche `SoraMod.asi` und den Ordner `SoraMod`. Sichere zuvor `SoraMod/data`, wenn du den Mod später weiterverwenden möchtest. Füge Fehlerberichten Spielversion, Script-Hook-V-Version, Modellcode und `SoraMod/SoraMod.log` bei.

## Kompilieren

Führe `tools/build.ps1 -Cxx <Pfad zu g++.exe>` mit MinGW-w64 GCC x64 (C++17) aus. Für die Neuerzeugung der Glyphen werden Python, Pillow, fontTools, die Schriftdateien, `research/vehicles.json` und `research/natives.json` benötigt; führe `tools/build-assets.py` aus. Offline-Tests starten über `tests/run.ps1`; passe die Werkzeugpfade in den Skripten an. Ressourcenquellen: [THIRD_PARTY.md](../THIRD_PARTY.md).

## Lizenz

SoraMod steht unter [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. Mitgelieferte Schriften behalten ihre eigenen Lizenzen; siehe [THIRD_PARTY.md](../THIRD_PARTY.md).
