# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Trainer pour le mode Histoire de GTA V Enhanced, par MinamotoDev. Compatible clavier, souris et manette, avec polices intégrées pour le menu.

[Télécharger la dernière version](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Notes de version](../RELEASE_NOTES.md)

## Fonctionnalités

- Invincibilité, restauration de la santé et de l’armure, suppression de l’indice de recherche, réapprovisionnement en munitions et réparation du véhicule.
- Faites apparaître des véhicules par catégorie, galerie plein écran, sélection aléatoire ou nom de modèle, y compris les modèles exclusifs à Online disponibles dans votre jeu.
- Épinglez des véhicules et sauvegardez leur position, plaque, couleurs et modifications courantes ; détachez-les à distance, téléportez-vous à bord ou placez un repère sur la carte.
- Téléportez-vous au repère de la carte ; réglez l’heure, figez l’horloge ou changez la météo.
- Le menu suit la langue du jeu.

## Installation

1. Ajoutez `-nobattleye` dans `args.txt`, situé dans le dossier du jeu.
2. Installez [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) et un ASI Loader compatibles avec votre version du jeu.
3. Téléchargez `SoraMod-0.4.1-win-x64.zip` et copiez `SoraMod.asi` ainsi que le dossier `SoraMod` dans le dossier du jeu.
4. Lancez le mode Histoire et appuyez sur `F7` pour ouvrir le menu.

Nécessite Windows 10 ou version ultérieure en 64 bits et GTA V Enhanced. Mode Histoire uniquement. Quittez le jeu avant toute mise à jour et remplacez les fichiers du mod ; `SoraMod/settings.ini` et `SoraMod/data` contiennent les réglages et les données sauvegardées.

## Commandes

| Action | Clavier | Manette |
| --- | --- | --- |
| Ouvrir / masquer | F7 | Maintenir RB / R1, puis croix directionnelle droite |
| Sélectionner dans une liste | Haut / Bas | Croix directionnelle haut / bas |
| Page suivante d’une liste | Droite / Page Down | Croix directionnelle droite / RB |
| Page précédente ; retour depuis la première page | Gauche / Retour arrière | Croix directionnelle gauche |
| Déplacement dans la galerie plein écran | Flèches : haut/bas par ligne, gauche/droite vers la carte voisine | Croix directionnelle haut / bas / gauche / droite |
| Confirmer | Entrée | A / Croix |
| Menu parent ; retour au jeu depuis le menu principal | Échap | B / Rond |
| Onglets de catégories en plein écran | Tab / Maj+Tab | RB / LB (R1 / L1) |
| Page précédente directement | Page Up | LB / L1 dans les listes |

## Données des véhicules

« Créer par nom de modèle » accepte des codes tels que `adder` et `oppressor2`. La saisie est vide à l’ouverture. Les modèles disponibles dépendent de votre version du jeu.

Ouvrez « Véhicules épinglés » à bord d’un véhicule pour l’épingler ou le détacher. Les données sont stockées dans `SoraMod/data/pinned-vehicles.dat`. La position et les modifications sont sauvegardées toutes les 5 secondes, et immédiatement lors d’un épinglage ou détachement. Après un redémarrage du jeu, les véhicules sont restaurés lorsque vous approchez de leur emplacement sauvegardé ou choisissez de vous téléporter à bord. Le fichier `.bak` voisin permet la récupération en cas de corruption. Tout détacher libère les véhicules sans les supprimer de la scène.

## Désinstallation et retours

Supprimez `SoraMod.asi` et le dossier `SoraMod`. Sauvegardez d’abord `SoraMod/data` si vous comptez réutiliser le mod. Pour signaler un problème, joignez la version du jeu, celle de Script Hook V, le code du modèle et `SoraMod/SoraMod.log`.

## Compilation

Exécutez `tools/build.ps1 -Cxx <chemin vers g++.exe>` avec MinGW-w64 GCC x64 (C++17). La génération des glyphes nécessite Python, Pillow, fontTools, les polices, `research/vehicles.json` et `research/natives.json` ; lancez `tools/build-assets.py`. Les tests hors jeu se lancent via `tests/run.ps1` ; configurez les chemins des outils dans les scripts. Sources des ressources : [THIRD_PARTY.md](../THIRD_PARTY.md).

## Licence

SoraMod est distribué sous [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. Les polices intégrées conservent leurs licences ; voir [THIRD_PARTY.md](../THIRD_PARTY.md).
