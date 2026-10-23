# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

MinamotoDev による GTA V エンハンストのストーリーモード用トレーナー。キーボード、マウス、コントローラーに対応し、メニューには同梱フォントを使用します。

[最新版をダウンロード](https://github.com/MinamotoDev/SoraMod/releases/latest) · [リリースノート](../RELEASE_NOTES.md)

## 機能

- 無敵、体力とアーマーの回復、手配度の解除、弾薬の補充、車両の修理。
- カテゴリー、全画面カード、ランダム選択、モデル名入力で車両を出現。インストール済みのゲームで利用できるオンライン専用車両にも対応。
- 車両の位置、ナンバープレート、色、一般的な改造を保存。離れた場所からの保存解除、車内へのワープ、マップの目的地設定に対応。
- マップの目的地へワープ。時刻の変更、時計の停止、天候の切り替え。
- メニューはゲームの言語に合わせて切り替わります。

## インストール

1. ゲームフォルダー内の `args.txt` に `-nobattleye` を追加します。
2. ゲームのバージョンに対応した [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) と ASI Loader をインストールします。
3. `SoraMod-0.4.1-win-x64.zip` をダウンロードし、`SoraMod.asi` と `SoraMod` フォルダーをゲームフォルダーにコピーします。
4. ストーリーモードに入り、`F7` でメニューを開きます。

64 ビット版 Windows 10 以降と GTA V エンハンストが必要です。ストーリーモード専用です。更新時はゲームを終了して MOD ファイルを上書きします。`SoraMod/settings.ini` と `SoraMod/data` に設定と保存データが格納されます。

## 操作

| 操作 | キーボード | コントローラー |
| --- | --- | --- |
| 開く / 閉じる | F7 | RB / R1 を押しながら方向パッド右 |
| 通常リストの項目選択 | ↑ / ↓ | 方向パッド上 / 下 |
| 通常リストの次ページ | → / Page Down | 方向パッド右 / RB |
| 前ページ・最初のページでは上の階層へ | ← / Backspace | 方向パッド左 |
| 全画面カードの移動 | 上下で行を移動、左右で隣のカードへ | 方向パッド上下左右 |
| 決定 | Enter | A / × |
| 上の階層へ・メインメニューではゲームに戻る | Esc | B / ○ |
| 全画面のカテゴリータブ | Tab / Shift+Tab | RB / LB（R1 / L1） |
| 直接前ページへ | Page Up | 通常リストで LB / L1 |

## 車両データ

「モデル名で車両を出現」には `adder`、`oppressor2` などのモデル名を入力できます。入力欄は最初は空欄です。利用できる車種はインストール済みのゲームバージョンによって異なります。

車両に乗った状態で「保存済み車両」を開くと、現在の車両を保存・解除できます。記録は `SoraMod/data/pinned-vehicles.dat` に保存されます。位置と改造は 5 秒ごとに、保存・解除の操作時は即座に書き込まれます。ゲーム再起動後は保存位置に近づくか、車内へのワープを選ぶと車両を復元します。隣の `.bak` ファイルは破損時の復旧に使用します。すべての保存を解除しても、場面内の車両は削除されません。

## アンインストールと不具合報告

`SoraMod.asi` と `SoraMod` フォルダーを削除するとアンインストールできます。再利用する場合は先に `SoraMod/data` をバックアップしてください。不具合報告にはゲームのバージョン、Script Hook V のバージョン、モデル名、`SoraMod/SoraMod.log` を添付してください。

## ビルド

x64 MinGW-w64 GCC（C++17）で `tools/build.ps1 -Cxx <g++.exe のパス>` を実行します。グリフの再生成には Python、Pillow、fontTools、フォントファイル、`research/vehicles.json`、`research/natives.json` が必要です。`tools/build-assets.py` を実行してください。ゲーム外テストは `tests/run.ps1` から実行し、スクリプト内のツールパスを設定します。素材の出典は [THIRD_PARTY.md](../THIRD_PARTY.md) を参照してください。

## ライセンス

SoraMod は [GNU GPL v3（GPL-3.0-only）](../LICENSE) で公開しています。Copyright © 2026 MinamotoDev。同梱フォントにはそれぞれのライセンスが適用されます。[THIRD_PARTY.md](../THIRD_PARTY.md) を参照してください。
