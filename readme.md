# FM-7/FM77AV/FM77AV40 Emulator "Mutsu"
# FM-7/FM77AV/FM77AV40 エミュレータ 「陸奥」

<http://www.ysflight.com>

PEB01130@nifty.com

---

# Introduction
It is yet another emulator for legendary Fujitsu FM-7 series computers.  The goal is to emulate FM77AV40.  I never owned FM77AV40EX and AV40SX, and practically more than 99.9% of FM-7 series software runs on FM77AV.  Minimum goal is to support FM77AV.

Why another FM-7 series emulator?  Primary reason is because I like programming.  I was able to write FM Towns emulator.  I thought I should be able to write an FM77AV emulator by myself, especially I know pretty good internal mechanism.  But, I also want to make it (1)have functionalities for help preserving actual hardware, (2) have symbolic debugger like Tsugaru, (3)natively run on mac/Linux/Windows, (4) replicate copy-protection as is so that the program runs without altering the code.  I don't think none of the existing emulators have all.

At this time it can run only a few games with no sound.  Will add more features soon.

I'm re-using code from FM-TOWNS Emulator Tsugaru.  So, CMakeLists.txt assumes that you have TOWNSEMU and 77AVEMU at the same level.

Please run Mutsu_CUI -H for help.

伝説の明機富士通FM-7/FM77AV/FM77AV40エミュレータです。目標はFM77AV40を再現することです。FM77AV40EX, AV40SXは所有したことがないですし、現実的に99.9%以上のFM-7シリーズのゲームはFM77AVで実行できるので、FM77AVの再現が最低目標です。いや、FM77AVが再現できればいいかな、と思ったけど一応手元にAV40もあって実機で実験できるから。

FM-7シリーズには既にいくつかの非常に高い再現性を持つエミュレータが存在しますが、なぜあらためて作るのか、というと、僕はプログラミングが好きなので、FM TOWNSエミュレータが作れたんだからFM77AVも中身は結構知ってるつもりだからエミュレータ作れそう、と、思ったというのが最大の要因ですが、(1)実機の動態保存に使いやすいエミュレータにする、(2)資料研究のためにシンボリックデバッガを搭載する、(3)mac/Linux/Windowsにネイティブ対応、(4)コピープロテクトを忠実に再現することでプロテクトをソフトウェアの修正により解除しなくてもソフトウェアを実行できるようにする、のすべてを備えたエミュレータはまだ無いと思うので、この辺りを目指してます。

現在、まだ限られた一部のゲームが実行できるだけで、音が鳴りませんが、順次サポートを広げていきます。

なお、FM TOWNSエミュレータ「津軽」のコードを再利用しているので、CMakeLists.Txtは、77AVEMUと同じレベルのディレクトリにTOWNSEMUが存在することを前提としています。

使い方はコンパイル後に Mutsu_CUI -H とコマンドをタイプしてください。

---

# ROM Files
It is an emulator.  I have another side-goal to write my own F-BASIC compatible ROM, but it's a long shot.  Until then, you need ROM images extracted from an actual hardware.  The ROMs are compatible with the best-known-to-the-day FM-7 series emulator XM7.  Place ROM files in one directory, and give the directory name as the first-parameter to Mutsu_CUI.exe (or Mutsu_CUI in Linux and mac).

このプログラムはエミュレータなので、実機から吸い出したROMイメージが必要です。F-BASIC互換ROMを書く、という壮大な目標もあるにはあるんですが、それはかなり長い道のりなので、それまでの間は実機から吸い出したROMイメージが必要です。ROMイメージは、現在知られている中で最も再現性が高いと思われるエミュレータXM7と互換性があります。ROMイメージを一か所のディレクトリに置いて、Mutsu_CUI.exeの第一パラメータにディレクトリの場所を指定してください。

---

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

Then, you get the executable in main_cui\Release\Mutsu_CUI.exe in Windows, main_cui/Mutsu_CUI in Linux, or main_cui/Mutsu_CUI.app/Contents/MacOS/Mutsu_CUI in macos.

---

# Command Line Example

./Mutsu_CUI path_to_FM7ROM -TAPE tape/PlazmaLine.t77

./Mutsu_CUI path_to_FM7ROM -FD0 disk/Thexder.d77




---

# GUI
Mutsu_GUI let you use GUI environment.  When you start Mutsu_GUI, you will see a pull-down menu and a dialog for configuring start-up condition.

In general, menu except File menu are while running the virtual machine.  All initial-condition parameters are in the dialog.

Minimum you need to specify the directory where you store ROM files.  If you have a set of ROM files extracted from the real FM-7, FM77AV, or FM77AV40, you will see F-BASIC prompt when you click on the "Start" button.

By default, you can come back to the GUI by pressing Scroll Lock key, or clicking on MENU icon at the lower right corner.  And, Virtual Machine -> Resume or pressing Scroll Lock again to go back to the VM.

Most of the disk-based games automatically starts if you specify FD0 and FD1 disk images and start.

My guess is many people do not know how to start a tape-based game.  In most cases, you need to type RUN"" or LOADM"",,R.  But, which one?  It depends on what file is stored at the beginning of the tape.  Mutsu can identify what type file is at the beginning of the tape and type a command for you.  Set a tape image, check "Auto Start Tape Program", do not specify disk images, and start.

In late 1980s, games came with many floppy disks.  You had to change a disk frequently during the game play.  You can do it by selecting a file from FD0->Image and FD1->Image.  But, selecting a file from the file dialog every time you change a disk is annoying.  To ease disk change, you can specify aliases for disk images.  In the Aliases tab, you can give a name to disk-image files, which will show up in FD0->Image, FD1->Image, and Tape->Image.

OK.  For those who are new to FM-7, I need to mention about the "Auto Stop" tab.  FM-7 series until FM77AV could not sense key-release.  You could start moving a character by pressing a key, but since FM-7 cannot sense key release, there is no way to stop a character by releasing a key.  We usually used num keys to move a character around, and pressed num-5 to stop.  Which was not too bad.  When you had to move long distance in a game, I simply pressed a key, wait until the character reaches where it should be, and then pressed 5 to stop.  I didn't have to press and hold the key.  But, for some reason, it was not very intuitive for gamers of other platforms.

If you don't want to press num-5 to stop your character, Mutsu does it for you.  Select when you want Mutsu to virtually press a key for stopping, and which key should be virtually pressed to stop a character in Auto-Stop tab.


Mutsu_GUIを実行すると、GUIからエミュレータを使うことができます。原則として、File以外のメニューはバーチャルマシン起動後の設定変更で、ダイアログが初期設定です。

最低限必要なのは、実機FM-7,FM77AV,またはFM77AV40から抜き出したROMイメージがあるディレクトリの指定です。そのディレクトリを指定し手STARTボタンを押すと、F-BASICのプロンプトが表示されるでしょう。

バーチャルマシンからGUIに戻るには、（設定を変更していない限り）Scroll Lockキーを押すか、またはウィンドウ右下のMENUアイコンをクリックします。バーチャルマシンに戻るには、またScroll Lockを押すか、Virtual Machine->Resumeを選びます。

ディスクのゲームを起動するには、ほとんどの場合、ダイアロブのFD0にイメージファイル名を指定して起動すれば自動的にゲームが始まります。

テープのゲームの始め方がわからない人が多いかもしれません。昔はみんな知ってたんですがね。ほとんどの場合、RUN""またはLOADM"",,Rというコマンドをタイプすればいいのですが、どちらのコマンドを使うのかはテープの先頭に入っているファイルの種類によります。陸奥はテープの先頭のファイルを識別して正しいコマンドを代わりにタイプする機能があります。"Auto Start Tape Program"をチェックして、テープイメージファイル名を指定するのを忘れずにバーチャルマシンを起動すると、その機能が自動的にテープのプログラムを読み込んで開始します。

1980年代後半は、ゲームが何枚ものフロッピーディスクに収められていて、プレイ中頻繁にディスクの交換が必要になりました。ファイルダイアログで選んでもいいのですが、ディスク交換のたびにファイルを選ぶのはちょっと不便です。そこで、エイリアス機能があります。Aliasesタブで名前とその名前に対応するディスクイメージファイル名を指定しておくと、ファイルを選ぶ代わりにエイリアス名を選ぶことでディスクやテープを交換することができます。

FM-7を初めて使う人は、"Auto Stop"タブの説明が必要ですね。FM77AVよりも前のFM-7シリーズは、なんとキーリリースを検出できませんでした。例えば、ゲーム中でキーを押してキャラクターの移動を開始することはできたのですが、キーリリースを検出できないので、キーを放してもキャラクターは止まりませんでした。そんなわけで、我々は別のキーを押すことでキャラクターを止めるしかありませんでした。大体テンキーを使って移動だったので、真ん中の5キーを押して止めたもんでした。長い距離を移動するときなんかはキーを押しっぱなしにしなくても良かったので楽だったんですけどね。しかし、他の機種に慣れた人はなぜかこの便利な自動移動機能は不評でした。

もしも、わざわざテンキーの5を押してキャラクターを止めるのが嫌という人のために、陸奥が代わりにキーをリリースしたときに5キーを押す機能があります。"Auto Stop"メニューでどのタイミングで停止キーを押したことにするのか、どのキーを停止キーに使うのかを指定してください。



---

# Tested
## 2022/08/13
F-BASIC V3.3 L10  Started and tested a few commands.

F-BASIC V3.4 L20 for AV40  Started and tested a few commands.

Thexder  Played first 3 stages.

Polar Star III  Played first 2 stages.

Ys  Played through all th eway.

Ys2  Played through all the way.

MAGUS  Started and played a few times.

Xanadu Scenario 1  Played through all the way.

Laptick  Started ok.

Pro Baseball Fan  Started ok.

Laydock  Started ok.

Death Force  Started ok.

Valis  Opening anime and game started ok.

Plazma Line (Tape)  Started ok.

Delphis (Tape)  Started ok.

Dragon Buster  Played first 2 stages.

Space Harrier  Played first stage.

Silpheed  Opening demo.

Hydlide (Tape)  Started ok.

Tritorn (Tape)  Started ok.

Jelda (Tape)  Started ok.

Jelda2 (Tape)  Started ok.

Dig Dug (Tape)  Started ok.
