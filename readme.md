# FM-7/FM77AV/FM77AV40 Emulator "Mutsu"
# FM-7/FM77AV/FM77AV40 エミュレータ 「陸奥」

<http://www.ysflight.com>

PEB01130@nifty.com

# Introduction
It is yet another emulator for legendary Fujitsu FM-7 series computers.  The goal is to emulate FM77AV40.  I never owned FM77AV40EX and AV40SX, and practically more than 99.9% of FM-7 series software runs on FM77AV.  Minimum goal is to support FM77AV.

Why another FM-7 series emulator?  Primary reason is because I like programming.  I was able to write FM Towns emulator.  I thought I should be able to write an FM77AV emulator by myself, especially I know pretty good internal mechanism.  But, I also want to make it (1)have functionalities for help preserving actual hardware, (2) have symbolic debugger like Tsugaru, (3)natively run on mac/Linux/Windows, (4) replicate copy-protection as is so that the program runs without altering the code.  I don't think none of the existing emulators have all.

At this time it can run only a few tape-based games with no sound.  Will add more features soon.

I'm re-using code from FM-TOWNS Emulator Tsugaru.  So, CMakeLists.txt assumes that you have TOWNSEMU and 77AVEMU at the same level.

Please run Mutsu_CUI -H for help.

伝説の明記富士通FM-7/FM77AV/FM77AV40エミュレータです。目標はFM77AV40を再現することです。FM77AV40EX, AV40SXは所有したことがないですし、現実的に99.9%以上のFM-7シリーズのゲームはFM77AVで実行できるので、FM77AVの再現が最低目標です。いや、FM77AVが再現できればいいかな、と思ったけど一応手元にAV40もあって実機で実験できるから。

FM-7シリーズには既にいくつかの非常に高い再現性を持つエミュレータが存在しますが、なぜあらためて作るのか、というと、僕はプログラミングが好きなので、FM TOWNSエミュレータが作れたんだからFM77AVも中身は結構知ってるつもりだからエミュレータ作れそう、と、思ったというのが最大の要因ですが、(1)実機の動態保存に使いやすいエミュレータにする、(2)資料研究のためにシンボリックデバッガを搭載する、(3)mac/Linux/Windowsにネイティブ対応、(4)コピープロテクトを忠実に再現することでプロテクトを解除しなくてもソフトウェアを実行できるようにする、のすべてを備えたエミュレータはまだ無いと思うので、この辺りを目指してます。

現在、まだ一部のテープのゲームが実行できるだけで、音が鳴りませんが、順次サポートを広げていきます。

なお、FM TOWNSエミュレータ「津軽」のコードを再利用しているので、CMakeLists.Txtは、77AVEMUと同じレベルのディレクトリにTOWNSEMUが存在することを前提としています。

使い方はコンパイル後に Mutsu_CUI -H とコマンドをタイプしてください。



# ROM Files
It is an emulator.  I have another side-goal to write my own F-BASIC compatible ROM, but it's a long shot.  Until then, you need ROM images extracted from an actual hardware.  The ROMs are compatible with the best-known-to-the-day FM-7 series emulator XM7.  Place ROM files in one directory, and give the directory name as the first-parameter to Mutsu_CUI.exe (or Mutsu_CUI in Linux and mac).

このプログラムはエミュレータなので、実機から吸い出したROMイメージが必要です。F-BASIC互換ROMを書く、という壮大な目標もあるにはあるんですが、それはかなり長い道のりなので、それまでの間は実機から吸い出したROMイメージが必要です。ROMイメージは、現在知られている中で最も再現性が高いと思われるエミュレータXM7と互換性があります。ROMイメージを一か所のディレクトリに置いて、Mutsu_CUI.exeの第一パラメータにディレクトリの場所を指定してください。



# Building the program
It uses cmake and Visual Studio in Windows, clang in Linux (need X11, OpenGL, and ALSA), or XCode in macOS.

In the working directory, do:

1. Open PowerShell with Visual Studio support in Windows, or Terminal in Linux or mac.
2. git clone https://github.com/captainys/TOWNSEMU.git
3. git clone https://github.com/captainys/77AVEMU.git
4. cd 77AVEMU
5. mkdir build
6. cd build
7. cmake ../src
8. cmake --build . --config Release

Then, you get Mutsu_CUI.exe in main_cui\Release in Windows, Mutsu_CUI in main/Release in Linux, or 

