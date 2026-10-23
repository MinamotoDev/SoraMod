# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

GTA V 增强版故事模式修改器，作者 MinamotoDev。支持键盘、鼠标与手柄，使用内置字库显示菜单。

[下载最新版本](https://github.com/MinamotoDev/SoraMod/releases/latest) · [更新说明](../RELEASE_NOTES.md)

## 功能

- 无敌、恢复生命与护甲、消除通缉、补充弹药；修复当前载具。
- 按分类、全屏卡片、随机或输入代号生成载具，支持当前游戏可用的线上专属车型。
- 固定车辆，保存位置、车牌、颜色和常用改装；支持远程解除固定、传送上车与地图标点。
- 传送到地图标点；调整时间、冻结时钟、切换天气。
- 菜单随游戏语言切换。

## 安装

1. 在游戏目录的 `args.txt` 中添加 `-nobattleye`。
2. 安装与游戏版本兼容的 [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) 和 ASI Loader。
3. 下载 `SoraMod-0.4.1-win-x64.zip`，将 `SoraMod.asi` 与 `SoraMod` 文件夹复制到游戏根目录。
4. 进入故事模式，按 `F7` 打开菜单。

需要 64 位 Windows 10 或更高版本及 GTA V 增强版。仅支持故事模式。更新时退出游戏并覆盖模组文件；`SoraMod/settings.ini` 和 `SoraMod/data` 用于存放设置数据。

## 操作

| 操作 | 键盘 | 手柄 |
| --- | --- | --- |
| 打开 / 收起 | F7 | 按住 RB / R1，再按十字键右 |
| 普通列表选择项目 | 上下方向键 | 十字键上下 |
| 普通列表下一页 | 右方向键 / Page Down | 十字键右 / RB |
| 普通列表上一页；第一页返回上级 | 左方向键 / Backspace | 十字键左 |
| 全屏卡片移动 | 上下跨行、左右相邻 | 十字键上下左右 |
| 确认 | Enter | A / Cross |
| 返回上级；主菜单返回游戏 | Esc | B / Circle |
| 全屏类别标签 | Tab / Shift+Tab | RB / LB（R1 / L1） |
| 直接上一页 | Page Up | 普通列表中 LB / L1 |

## 载具数据

“输入代号生成载具”接受 `adder`、`oppressor2` 等模型代号，输入框初始为空白。实际可生成车型取决于安装的游戏版本。

在车内进入“已固定的载具”即可固定或解除固定当前车辆。记录位于 `SoraMod/data/pinned-vehicles.dat`，每 5 秒保存位置和改装，固定与解除时立即保存。重启游戏后，接近保存位置或选择传送上车时恢复车辆；旁边的 `.bak` 文件用于损坏恢复。清除固定只解除保留，不删除场景中的车辆。

## 卸载与反馈

删除 `SoraMod.asi` 和 `SoraMod` 文件夹即可卸载；如需要继续使用 Mod，先备份 `SoraMod/data`。反馈请附游戏版本、Script Hook V 版本、车型代号和 `SoraMod/SoraMod.log`。

## 构建

使用 x64 MinGW-w64 GCC（C++17）运行 `tools/build.ps1 -Cxx <g++.exe 路径>`。重新生成字形需要 Python、Pillow、fontTools、字体文件及 `research/vehicles.json`、`research/natives.json`，运行 `tools/build-assets.py`。离线回归入口为 `tests/run.ps1`，需配置脚本中的工具路径。资源来源见 [THIRD_PARTY.md](../THIRD_PARTY.md)。

## 开源协议

SoraMod 使用 [GNU GPL v3（GPL-3.0-only）](../LICENSE)，版权所有 © 2026 MinamotoDev。内置字体采用各自的授权协议，见 [THIRD_PARTY.md](../THIRD_PARTY.md)。
