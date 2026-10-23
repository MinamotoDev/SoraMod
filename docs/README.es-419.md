# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Trainer para el modo Historia de GTA V Enhanced, por MinamotoDev. Compatible con teclado, mouse y control, con fuentes incluidas para el menú.

[Descargar la última versión](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Notas de la versión](../RELEASE_NOTES.md)

## Funciones

- Invencibilidad, restauración de salud y blindaje, eliminación del nivel de búsqueda, reposición de munición y reparación del vehículo.
- Genera vehículos por categoría, desde tarjetas a pantalla completa, al azar o por código de modelo, incluidos los modelos exclusivos de Online disponibles en tu juego.
- Fija vehículos y guarda su posición, placa, colores y modificaciones habituales; desfíjalos a distancia, teletranspórtate a su interior o márcalos en el mapa.
- Teletranspórtate al punto del mapa; cambia la hora, congela el reloj o modifica el tiempo.
- El menú sigue el idioma del juego.

## Instalación

1. Agrega `-nobattleye` al archivo `args.txt` de la carpeta del juego.
2. Instala [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) y un ASI Loader compatibles con tu versión del juego.
3. Descarga `SoraMod-0.4.1-win-x64.zip` y copia `SoraMod.asi` y la carpeta `SoraMod` en la carpeta del juego.
4. Entra al el modo Historia y presiona `F7` para abrir el menú.

Requiere Windows 10 o posterior de 64 bits y GTA V Enhanced. Solo modo Historia. Sal del juego antes de actualizar y sobrescribe los archivos del mod; `SoraMod/settings.ini` y `SoraMod/data` almacenan los ajustes y los datos guardados.

## Controles

| Acción | Teclado | Mando |
| --- | --- | --- |
| Abrir / ocultar | F7 | Mantén RB / R1 y presiona la cruceta derecha |
| Seleccionar en una lista | Arriba / Abajo | Cruceta arriba / abajo |
| Página siguiente de una lista | Derecha / Page Down | Cruceta derecha / RB |
| Página anterior; volver desde la primera página | Izquierda / Backspace | Cruceta izquierda |
| Moverse por las tarjetas a pantalla completa | Flechas: arriba/abajo por filas, izquierda/derecha a la tarjeta contigua | Cruceta arriba / abajo / izquierda / derecha |
| Confirmar | Enter | A / Cruz |
| Menú anterior; volver al juego desde el principal | Esc | B / Círculo |
| Pestañas de categorías a pantalla completa | Tab / Mayús+Tab | RB / LB (R1 / L1) |
| Página anterior directamente | Page Up | LB / L1 en listas normales |

## Datos de vehículos

«Generar por código de modelo» acepta códigos como `adder` y `oppressor2`. El campo empieza vacío. Los modelos disponibles dependen de la versión instalada del juego.

Abre «Vehículos fijados» dentro de un vehículo para fijarlo o desfijarlo. Los datos se guardan en `SoraMod/data/pinned-vehicles.dat`. La posición y las modificaciones se guardan cada 5 segundos; fijar y desfijar guarda de inmediato. Tras reiniciar el juego, los vehículos se restauran al acercarte a su posición guardada o al elegir teletransportarte a su interior. El archivo `.bak` situado junto a los datos permite recuperarlos si se dañan. Desfijar todos los vehículos los libera sin eliminarlos de la escena.

## Desinstalación y comentarios

Elimina `SoraMod.asi` y la carpeta `SoraMod` para desinstalar. Haz antes una copia de `SoraMod/data` si quieres volver a usar el mod. Al informar de errores, incluye la versión del juego, la de Script Hook V, el código del modelo y `SoraMod/SoraMod.log`.

## Compilación

Ejecuta `tools/build.ps1 -Cxx <ruta a g++.exe>` con MinGW-w64 GCC x64 (C++17). Para regenerar los glifos necesitas Python, Pillow, fontTools, las fuentes, `research/vehicles.json` y `research/natives.json`; ejecuta `tools/build-assets.py`. Las pruebas fuera del juego se inician con `tests/run.ps1`; configura las rutas de herramientas en los scripts. Fuentes de los recursos: [THIRD_PARTY.md](../THIRD_PARTY.md).

## Licencia

SoraMod se distribuye con [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. Las fuentes incluidas conservan sus licencias; consulta [THIRD_PARTY.md](../THIRD_PARTY.md).
