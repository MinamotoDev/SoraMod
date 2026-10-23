# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

MinamotoDev가 제작한 GTA V 인핸스드 스토리 모드 트레이너입니다. 키보드, 마우스, 컨트롤러를 지원하며 메뉴에 내장 글꼴을 사용합니다.

[최신 버전 다운로드](https://github.com/MinamotoDev/SoraMod/releases/latest) · [릴리스 노트](../RELEASE_NOTES.md)

## 기능

- 무적, 체력과 방탄복 회복, 지명 수배 해제, 탄약 보충, 차량 수리.
- 분류, 전체 화면 카드, 무작위 선택 또는 모델명 입력으로 차량을 생성합니다. 설치된 게임에서 사용할 수 있는 온라인 전용 차량도 지원합니다.
- 차량을 고정하고 위치, 번호판, 색상 및 일반 개조를 저장합니다. 원격 고정 해제, 차량 안으로 순간 이동, 지도 목적지 설정을 지원합니다.
- 지도 목적지로 순간 이동하고 시간 변경, 시계 정지, 날씨 전환을 할 수 있습니다.
- 메뉴는 게임 언어에 맞춰 전환됩니다.

## 설치

1. 게임 폴더의 `args.txt`에 `-nobattleye`를 추가합니다.
2. 게임 버전에 맞는 [Script Hook V](https://www.dev-c.com/gtav/scripthookv/)와 ASI Loader를 설치합니다.
3. `SoraMod-0.4.1-win-x64.zip`을 다운로드하고 `SoraMod.asi`와 `SoraMod` 폴더를 게임 폴더에 복사합니다.
4. 스토리 모드에 들어가 `F7`을 눌러 메뉴를 엽니다.

64비트 Windows 10 이상과 GTA V 인핸스드가 필요합니다. 스토리 모드 전용입니다. 업데이트 전 게임을 종료한 뒤 모드 파일을 덮어쓰세요. `SoraMod/settings.ini`와 `SoraMod/data`에 설정과 저장 데이터가 보관됩니다.

## 조작

| 동작 | 키보드 | 컨트롤러 |
| --- | --- | --- |
| 열기 / 닫기 | F7 | RB / R1을 누른 상태에서 방향 패드 오른쪽 |
| 일반 목록 항목 선택 | 위 / 아래 | 방향 패드 위 / 아래 |
| 일반 목록 다음 페이지 | 오른쪽 / Page Down | 방향 패드 오른쪽 / RB |
| 이전 페이지; 첫 페이지에서는 상위 메뉴 | 왼쪽 / Backspace | 방향 패드 왼쪽 |
| 전체 화면 카드 이동 | 위/아래는 행 이동, 왼쪽/오른쪽은 인접 카드 | 방향 패드 상하좌우 |
| 확인 | Enter | A / × |
| 상위 메뉴; 주 메뉴에서는 게임으로 복귀 | Esc | B / ○ |
| 전체 화면 분류 탭 | Tab / Shift+Tab | RB / LB(R1 / L1) |
| 바로 이전 페이지 | Page Up | 일반 목록에서 LB / L1 |

## 차량 데이터

“모델명으로 차량 생성”에 `adder`, `oppressor2` 같은 모델명을 입력할 수 있습니다. 입력란은 처음에 비어 있습니다. 생성 가능한 차종은 설치된 게임 버전에 따라 달라집니다.

차량 안에서 “고정된 차량”을 열면 현재 차량을 고정하거나 해제할 수 있습니다. 기록은 `SoraMod/data/pinned-vehicles.dat`에 저장됩니다. 위치와 개조는 5초마다, 고정 및 해제 시에는 즉시 저장됩니다. 게임 재시작 후 저장 위치에 접근하거나 차량 안으로 순간 이동을 선택하면 차량을 복원합니다. 옆의 `.bak` 파일은 데이터 손상 복구용입니다. 전체 고정 해제는 차량을 장면에서 삭제하지 않고 고정만 해제합니다.

## 삭제 및 오류 제보

`SoraMod.asi`와 `SoraMod` 폴더를 삭제하면 제거됩니다. 다시 사용할 계획이라면 먼저 `SoraMod/data`를 백업하세요. 오류 제보에는 게임 버전, Script Hook V 버전, 모델명과 `SoraMod/SoraMod.log`를 첨부하세요.

## 빌드

x64 MinGW-w64 GCC(C++17)로 `tools/build.ps1 -Cxx <g++.exe 경로>`를 실행합니다. 글리프를 다시 생성하려면 Python, Pillow, fontTools, 글꼴 파일, `research/vehicles.json`, `research/natives.json`이 필요하며 `tools/build-assets.py`를 실행하세요. 게임 외부 테스트는 `tests/run.ps1`로 시작하며 스크립트의 도구 경로를 설정해야 합니다. 리소스 출처는 [THIRD_PARTY.md](../THIRD_PARTY.md)를 참고하세요.

## 라이선스

SoraMod는 [GNU GPL v3(GPL-3.0-only)](../LICENSE)로 배포됩니다. Copyright © 2026 MinamotoDev. 내장 글꼴에는 별도 라이선스가 적용됩니다. [THIRD_PARTY.md](../THIRD_PARTY.md)를 참고하세요.
