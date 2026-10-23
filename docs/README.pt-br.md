# SoraMod

[English](../README.md) · [Français](README.fr.md) · [Italiano](README.it.md) · [Deutsch](README.de.md) · [Español (España)](README.es-es.md) · [日本語](README.ja.md) · [한국어](README.ko.md) · [Polski](README.pl.md) · [Português (Brasil)](README.pt-br.md) · [Русский](README.ru.md) · [简体中文](README.zh-cn.md) · [Español (Latinoamérica)](README.es-419.md) · [正體中文](README.zh-tw.md)

Trainer para o Modo História de GTA V Enhanced, por MinamotoDev. Compatível com teclado, mouse e controle, com fontes incluídas para o menu.

[Baixar a versão mais recente](https://github.com/MinamotoDev/SoraMod/releases/latest) · [Notas da versão](../RELEASE_NOTES.md)

## Recursos

- Invencibilidade, restauração de vida e colete, remoção do nível de procurado, reposição de munição e reparo do veículo.
- Gere veículos por categoria, cartões em tela cheia, seleção aleatória ou código de modelo, incluindo modelos exclusivos do Online disponíveis no seu jogo.
- Fixe veículos e salve posição, placa, cores e modificações comuns; desafixe-os remotamente, teleporte-se para dentro deles ou marque-os no mapa.
- Teleporte-se para o ponto no mapa; ajuste a hora, congele o relógio ou altere o clima.
- O menu acompanha o idioma do jogo.

## Instalação

1. Adicione `-nobattleye` ao arquivo `args.txt` na pasta do jogo.
2. Instale [Script Hook V](https://www.dev-c.com/gtav/scripthookv/) e um ASI Loader compatíveis com a sua versão do jogo.
3. Baixe `SoraMod-0.4.1-win-x64.zip` e copie `SoraMod.asi` e a pasta `SoraMod` para a pasta do jogo.
4. Entre no Modo História e pressione `F7` para abrir o menu.

Requer Windows 10 ou posterior de 64 bits e GTA V Enhanced. Apenas Modo História. Saia do jogo antes de atualizar e sobrescreva os arquivos do mod; `SoraMod/settings.ini` e `SoraMod/data` armazenam configurações e dados salvos.

## Controles

| Ação | Teclado | Controle |
| --- | --- | --- |
| Abrir / ocultar | F7 | Segure RB / R1 e pressione o direcional para a direita |
| Selecionar item em lista | Cima / Baixo | Direcional para cima / baixo |
| Próxima página da lista | Direita / Page Down | Direcional para a direita / RB |
| Página anterior; voltar na primeira página | Esquerda / Backspace | Direcional para a esquerda |
| Mover entre cartões em tela cheia | Setas: cima/baixo por linha, esquerda/direita para o cartão vizinho | Direcional para cima / baixo / esquerda / direita |
| Confirmar | Enter | A / X |
| Menu anterior; voltar ao jogo no menu principal | Esc | B / Círculo |
| Abas de categorias em tela cheia | Tab / Shift+Tab | RB / LB (R1 / L1) |
| Página anterior diretamente | Page Up | LB / L1 em listas normais |

## Dados dos veículos

“Gerar pelo código do modelo” aceita códigos como `adder` e `oppressor2`. O campo começa vazio. Os modelos disponíveis dependem da versão instalada do jogo.

Abra “Veículos fixados” dentro de um veículo para fixá-lo ou desafixá-lo. Os registros ficam em `SoraMod/data/pinned-vehicles.dat`. A posição e as modificações são salvas a cada 5 segundos; fixar e desafixar salva imediatamente. Após reiniciar o jogo, os veículos são restaurados ao se aproximar da posição salva ou ao escolher se teleportar para dentro deles. O arquivo `.bak` ao lado dos dados permite recuperá-los em caso de corrupção. Desafixar todos libera os veículos sem removê-los da cena.

## Desinstalação e relatos de erros

Exclua `SoraMod.asi` e a pasta `SoraMod` para desinstalar. Faça um backup de `SoraMod/data` antes se pretende usar o mod novamente. Ao relatar erros, inclua a versão do jogo, a versão do Script Hook V, o código do modelo e `SoraMod/SoraMod.log`.

## Compilação

Execute `tools/build.ps1 -Cxx <caminho para g++.exe>` com MinGW-w64 GCC x64 (C++17). Para gerar os glifos novamente, são necessários Python, Pillow, fontTools, as fontes, `research/vehicles.json` e `research/natives.json`; execute `tools/build-assets.py`. Os testes fora do jogo começam em `tests/run.ps1`; configure os caminhos das ferramentas nos scripts. Fontes dos recursos: [THIRD_PARTY.md](../THIRD_PARTY.md).

## Licença

SoraMod é distribuído sob a [GNU GPL v3 (GPL-3.0-only)](../LICENSE). Copyright © 2026 MinamotoDev. As fontes incluídas mantêm suas próprias licenças; veja [THIRD_PARTY.md](../THIRD_PARTY.md).
