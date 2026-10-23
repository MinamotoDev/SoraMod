# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

GTA V 增強版故事模式修改器，作者 MinamotoDev。支援鍵盤、滑鼠與控制器，使用內建字型顯示選單。

[下載最新版本](https://github.com/MinamotoDev/SoraMod/releases/latest) · [更新說明](../RELEASE_NOTES.md)

## 功能

- 無敵、恢復生命與護甲、消除通緝、補充彈藥；修復目前載具。
- 依類別、全螢幕卡片、隨機或輸入代號產生載具，支援目前遊戲可用的線上模式專屬車款。
- 固定車輛，儲存位置、車牌、顏色與常用改裝；支援遠端解除固定、傳送上車與地圖標點。
- 傳送到地圖標點；調整時間、凍結時鐘、切換天氣。
- 選單會隨遊戲語言切換。

## 安裝

1. 在遊戲目錄的 `args.txt` 中加入 `-nobattleye`。
2. 安裝與遊戲版本相容的 [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) 和 ASI Loader。
3. 下載 `SoraMod-0.4.1-win-x64.zip`，將 `SoraMod.asi` 與 `SoraMod` 資料夾複製到遊戲根目錄。
4. 進入故事模式，按 `F7` 開啟選單。

需要 64 位元 Windows 10 或更新版本及 GTA V 增強版。僅支援故事模式。更新時退出遊戲並覆寫模組檔案；`SoraMod/settings.ini` 與 `SoraMod/data` 用來存放設定資料。

## 操作

| 操作 | 鍵盤 | 控制器 |
| --- | --- | --- |
| 開啟 / 收起 | F7 | 按住 RB / R1，再按方向鍵右 |
| 一般清單選擇項目 | 上下方向鍵 | 控制器方向鍵上下 |
| 一般清單下一頁 | 右方向鍵 / Page Down | 控制器方向鍵右 / RB |
| 一般清單上一頁；第一頁返回上層 | 左方向鍵 / Backspace | 控制器方向鍵左 |
| 全螢幕卡片移動 | 上下切換列、左右移至相鄰卡片 | 控制器方向鍵上下左右 |
| 確認 | Enter | A / Cross |
| 返回上層；主選單返回遊戲 | Esc | B / Circle |
| 全螢幕類別分頁 | Tab / Shift+Tab | RB / LB（R1 / L1） |
| 直接上一頁 | Page Up | 一般清單中 LB / L1 |

## 載具資料

「輸入代號產生載具」接受 `adder`、`oppressor2` 等模型代號，輸入框初始為空白。實際可產生的車款取決於安裝的遊戲版本。

在車內進入「已固定的載具」即可固定或解除固定目前車輛。紀錄位於 `SoraMod/data/pinned-vehicles.dat`，每 5 秒儲存位置與改裝，固定與解除時立即儲存。重新啟動遊戲後，接近儲存位置或選擇傳送上車時恢復車輛；旁邊的 `.bak` 檔案用於資料損壞時還原。清除固定只解除保留，不會刪除場景中的車輛。

## 解除安裝與問題回報

刪除 `SoraMod.asi` 與 `SoraMod` 資料夾即可解除安裝；如需繼續使用 Mod，請先備份 `SoraMod/data`。回報問題時請附上遊戲版本、Script Hook V 版本、車款代號與 `SoraMod/SoraMod.log`。

## 建置

使用 x64 MinGW-w64 GCC（C++17）執行 `tools/build.ps1 -Cxx <g++.exe 路徑>`。重新產生字形需要 Python、Pillow、fontTools、字型檔及 `research/vehicles.json`、`research/natives.json`，執行 `tools/build-assets.py`。離線測試入口為 `tests/run.ps1`，需設定指令碼中的工具路徑。資源來源見 [THIRD_PARTY.md](../THIRD_PARTY.md)。

## 開源授權

SoraMod 採用 [GNU GPL v3（GPL-3.0-only）](../LICENSE)，著作權 © 2026 MinamotoDev。內建字型採用各自的授權條款，見 [THIRD_PARTY.md](../THIRD_PARTY.md)。
