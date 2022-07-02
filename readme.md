# FM-7/FM77AV/FM77AV40 Emulator "Mutsu"
# FM-7/FM77AV/FM77AV40 エミュレータ 「陸奥」

<http://www.ysflight.com>

PEB01130@nifty.com

#Introduction
It is yet another emulator for legendary Fujitsu FM-7 series computers.  The goal is to emulate FM77AV40.  I never owned FM77AV40EX and AV40SX, and practically more than 99.9% of FM-7 series software runs on FM77AV.  Minimum goal is to support FM77AV.

It can run only a few tape-based games with no sound.  Will add more features soon.

I'm re-using code from FM-TOWNS Emulator Tsugaru.  So, CMakeLists.txt assumes that you have TOWNSEMU and 77AVEMU at the same level.

Please run Mutsu_CUI -H for help.

伝説の明記富士通FM-7/FM77AV/FM77AV40エミュレータです。目標はFM77AV40を再現することです。FM77AV40EX, AV40SXは所有したことがないですし、現実的に99.9%以上のFM-7シリーズのゲームはFM77AVで実行できるので、FM77AVの再現が最低目標です。

現在、まだ一部のテープのゲームが実行できるだけで、音が鳴りませんが、順次サポートを広げていきます。

なお、FM TOWNSエミュレータ「津軽」のコードを再利用しているので、CMakeLists.Txtは、77AVEMUと同じレベルのディレクトリにTOWNSEMUが存在することを前提としています。

使い方はコンパイル後に Mutsu_CUI -H とコマンドをタイプしてください。