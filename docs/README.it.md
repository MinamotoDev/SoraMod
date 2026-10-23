# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Trainer per la modalità Storia di GTA V Enhanced, di MinamotoDev. Supporta tastiera, mouse e controller, con caratteri integrati per il menu.

[Scarica l’ultima versione](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Note di rilascio](../RELEASE_NOTES.md)

## Funzionalità

- Invincibilità, ripristino di salute e armatura, azzeramento del livello di sospetto, rifornimento di munizioni e riparazione del veicolo.
- Genera veicoli per categoria, dalla galleria a schermo intero, casualmente o tramite codice modello, inclusi i modelli esclusivi di Online disponibili nel gioco.
- Salva i veicoli con posizione, targa, colori e modifiche comuni; rimuovili dai salvati a distanza, teletrasportati al loro interno o segnali sulla mappa.
- Teletrasportati al punto sulla mappa; cambia l’ora, ferma l’orologio o modifica il meteo.
- Il menu segue la lingua del gioco.

## Installazione

1. Aggiungi `-nobattleye` al file `args.txt` nella cartella del gioco.
2. Installa [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) e un ASI Loader compatibili con la tua versione del gioco.
3. Scarica `SoraMod-0.4.1-win-x64.zip` e copia `SoraMod.asi` e la cartella `SoraMod` nella cartella del gioco.
4. Avvia la modalità Storia e premi `F7` per aprire il menu.

Richiede Windows 10 o successivo a 64 bit e GTA V Enhanced. Solo modalità Storia. Esci dal gioco prima di aggiornare e sovrascrivi i file del mod; `SoraMod/settings.ini` e `SoraMod/data` contengono impostazioni e dati salvati.

## Comandi

| Azione | Tastiera | Controller |
| --- | --- | --- |
| Apri / nascondi | F7 | Tieni premuto RB / R1, poi croce direzionale destra |
| Seleziona nella lista | Su / Giù | Croce direzionale su / giù |
| Pagina successiva della lista | Destra / Page Down | Croce direzionale destra / RB |
| Pagina precedente; indietro dalla prima pagina | Sinistra / Backspace | Croce direzionale sinistra |
| Spostati nella galleria a schermo intero | Frecce: su/giù per riga, sinistra/destra tra schede adiacenti | Croce direzionale su / giù / sinistra / destra |
| Conferma | Invio | A / Croce |
| Menu precedente; torna al gioco dal menu principale | Esc | B / Cerchio |
| Schede delle categorie a schermo intero | Tab / Maiusc+Tab | RB / LB (R1 / L1) |
| Pagina precedente direttamente | Page Up | LB / L1 nelle liste |

## Dati dei veicoli

“Genera tramite codice modello” accetta codici come `adder` e `oppressor2`. Il campo è inizialmente vuoto. I modelli disponibili dipendono dalla versione del gioco installata.

Apri “Veicoli salvati” mentre sei a bordo per salvare il veicolo o rimuoverlo dai salvati. I dati si trovano in `SoraMod/data/pinned-vehicles.dat`. Posizione e modifiche vengono salvate ogni 5 secondi, e subito quando aggiungi o rimuovi un veicolo. Dopo il riavvio del gioco, i veicoli vengono ripristinati quando ti avvicini alla posizione salvata o scegli di teletrasportarti al loro interno. Il file `.bak` accanto ai dati serve per il recupero in caso di corruzione. Rimuovere tutti i salvati libera i veicoli senza eliminarli dalla scena.

## Disinstallazione e segnalazioni

Elimina `SoraMod.asi` e la cartella `SoraMod`. Prima salva una copia di `SoraMod/data` se intendi riutilizzare il mod. Per segnalare problemi, indica versione del gioco, versione di Script Hook V, codice modello e `SoraMod/SoraMod.log`.

## Compilazione

Esegui `tools/build.ps1 -Cxx <percorso di g++.exe>` con MinGW-w64 GCC x64 (C++17). Per rigenerare i glifi servono Python, Pillow, fontTools, i caratteri, `research/vehicles.json` e `research/natives.json`; esegui `tools/build-assets.py`. I test fuori dal gioco si avviano con `tests/run.ps1`; configura i percorsi degli strumenti negli script. Fonti delle risorse: [THIRD_PARTY.md](../THIRD_PARTY.md).

## Licenza

SoraMod è distribuito con [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. I caratteri integrati mantengono le proprie licenze; vedi [THIRD_PARTY.md](../THIRD_PARTY.md).
