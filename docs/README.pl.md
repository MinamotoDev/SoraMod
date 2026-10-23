# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Trainer trybu fabularnego GTA V Enhanced autorstwa MinamotoDev. Obsługuje klawiaturę, mysz i kontroler oraz korzysta z dołączonych czcionek menu.

[Pobierz najnowszą wersję](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Informacje o wydaniu](../RELEASE_NOTES.md)

## Funkcje

- Nieśmiertelność, przywracanie zdrowia i pancerza, usuwanie poziomu poszukiwania, uzupełnianie amunicji i naprawa pojazdu.
- Tworzenie pojazdów według kategorii, z pełnoekranowych kart, losowo lub według kodu modelu, w tym modeli dostępnych wyłącznie w Online, obecnych w zainstalowanej grze.
- Przypinanie pojazdów z zapisem pozycji, tablicy, kolorów i typowych modyfikacji; zdalne odpinanie, teleportacja do środka i oznaczanie na mapie.
- Teleportacja do punktu na mapie; zmiana czasu, zatrzymanie zegara i zmiana pogody.
- Menu korzysta z języka gry.

## Instalacja

1. Dodaj `-nobattleye` do pliku `args.txt` w folderze gry.
2. Zainstaluj [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) i ASI Loader zgodne z wersją gry.
3. Pobierz `SoraMod-0.4.1-win-x64.zip` i skopiuj `SoraMod.asi` oraz folder `SoraMod` do folderu gry.
4. Uruchom tryb fabularny i naciśnij `F7`, aby otworzyć menu.

Wymagane są 64-bitowy Windows 10 lub nowszy oraz GTA V Enhanced. Tylko tryb fabularny. Przed aktualizacją zamknij grę i nadpisz pliki moda; `SoraMod/settings.ini` i `SoraMod/data` zawierają ustawienia i zapisane dane.

## Sterowanie

| Działanie | Klawiatura | Kontroler |
| --- | --- | --- |
| Otwórz / ukryj | F7 | Przytrzymaj RB / R1 i naciśnij krzyżak w prawo |
| Wybór pozycji na liście | Góra / Dół | Krzyżak w górę / dół |
| Następna strona listy | Prawo / Page Down | Krzyżak w prawo / RB |
| Poprzednia strona; powrót z pierwszej strony | Lewo / Backspace | Krzyżak w lewo |
| Karty pełnoekranowe | Góra/dół zmienia wiersz, lewo/prawo wybiera sąsiednią kartę | Krzyżak w górę / dół / lewo / prawo |
| Potwierdź | Enter | A / Krzyżyk |
| Menu nadrzędne; powrót do gry z menu głównego | Esc | B / Kółko |
| Zakładki kategorii na pełnym ekranie | Tab / Shift+Tab | RB / LB (R1 / L1) |
| Bezpośrednio poprzednia strona | Page Up | LB / L1 na zwykłych listach |

## Dane pojazdów

„Stwórz według kodu modelu” przyjmuje kody takie jak `adder` i `oppressor2`. Pole początkowo jest puste. Dostępne modele zależą od zainstalowanej wersji gry.

Otwórz „Przypięte pojazdy”, siedząc w pojeździe, aby go przypiąć lub odpiąć. Dane są zapisywane w `SoraMod/data/pinned-vehicles.dat`. Pozycja i modyfikacje są zapisywane co 5 sekund, a przypinanie i odpinanie zapisuje je natychmiast. Po ponownym uruchomieniu gry pojazdy są przywracane po zbliżeniu się do zapisanej pozycji lub wybraniu teleportacji do środka. Plik `.bak` obok danych służy do odzyskiwania po uszkodzeniu. Odpięcie wszystkich pojazdów zwalnia je bez usuwania ze sceny.

## Usuwanie i zgłaszanie błędów

Usuń `SoraMod.asi` i folder `SoraMod`. Najpierw wykonaj kopię `SoraMod/data`, jeśli zamierzasz ponownie używać moda. Zgłaszając błąd, podaj wersję gry, wersję Script Hook V, kod modelu i `SoraMod/SoraMod.log`.

## Kompilacja

Uruchom `tools/build.ps1 -Cxx <ścieżka do g++.exe>` z MinGW-w64 GCC x64 (C++17). Generowanie glifów wymaga Python, Pillow, fontTools, czcionek, `research/vehicles.json` i `research/natives.json`; uruchom `tools/build-assets.py`. Testy poza grą uruchamia `tests/run.ps1`; ustaw ścieżki narzędzi w skryptach. Źródła zasobów: [THIRD_PARTY.md](../THIRD_PARTY.md).

## Licencja

SoraMod jest udostępniany na [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. Dołączone czcionki zachowują własne licencje; zobacz [THIRD_PARTY.md](../THIRD_PARTY.md).
