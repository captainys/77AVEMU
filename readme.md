# FM-7/FM77AV/FM77AV40 Emulator "Mutsu"
# FM-7/FM77AV/FM77AV40 エミュレータ 「陸奥」

<http://www.ysflight.com>

PEB01130@nifty.com

---

# Link to Releases
https://github.com/captainys/77AVEMU/releases/latest


---

# Introduction
It is yet another emulator for legendary 8-bit computer, Fujitsu FM-7 series computers.  Some of the features are:

- Cross Platform (Windows/Linux/macOS).
- Can choose Graphical and Command User Interface.
- Powerful Debugger
- Cooperation with 2D Retro Map Tool.

The goal is to emulate FM77AV40.  I never owned FM77AV40EX and AV40SX, and practically more than 99.9% of FM-7 series software runs on FM77AV.  Minimum goal is to support FM77AV.

In the future, I want to replicate copy-protection as is, rather than cracking it to run applications.

There are many excellent emulators for FM-7 series, so why another one?  Mainly because I like programming.  Well, I wanted to make one that is preservable.  Cross-platform is a must for a program to be preservable.  We never know if Windows exists 10 years from now.  Unless the software is written portable and cross-platform, the software dies with the platform.  When I wrote Tsugaru, I felt I should be able to write FM-7 series emulator, but I hesitated because there already were good emulators, but after talking with Game Preservation Society this (2022) summer that it would be nice to have an emulator that can replicate copy protection as is, I decided to go for it.

Let's look at the reality.  You can find a pirated copy of Thexder for FM-7 on the internet.  If you hear a beep on start up, it is most likely using an initial program loader written by a hacker called J.N. for evading the copy protection.  We don't know who he or she was.  But, real Thexder does not boot on start up.  I rather want to run the original by replicating its copy protection as is.

Those pirated copies eventually would have a role in the preservation, but the copyright law is not ready to discuss what to do with preservation.  Don't tell me 'abandonware'.  Many game developers are still in business.  There is no such thing called abandonware.  The current copyright law protects the creators, but it is incapable of protecting what's created.

The chicken race between the hackers and copy-protectors was a piece of technological history that I want to preserve.  The initial program loader by J.N. is part of this history.  Therefore, I want to preserve both.  How the original copy was working.  How it was cracked.



伝説の名8ビットコンピュータ富士通FM-7/FM77AV/FM77AV40エミュレータです。特徴としては、

- クロスプラットフォーム(Windows/Linux/macOS)対応
- グラフィカルユーザーインターフェース、コマンドユーザーインターフェース選択可能
- 強力なデバッガ
- 二次元マップツールとの連携機能

という点画あります。

目標はFM77AV40を再現することです。FM77AV40EX, AV40SXは所有したことがないですし、現実的に99.9%以上のFM-7シリーズのゲームはFM77AVで実行できるので、FM77AVの再現が最低目標です。いや、FM77AVが再現できればいいかな、と思ったけど一応手元にAV40もあって実機で実験できるから。

FM-7シリーズには既にいくつかの非常に高い再現性を持つエミュレータが存在しますが、なぜあらためて作るのか、というと、僕はプログラミングが好きだからです。いや、ひとつは、長期保存可能なエミュレータを作りたかったというのがあります。あるソフトウェアが長い時間生き残るためには、クロスプラットフォームであることが絶対条件です。今から10年後、Windowsはあるのでしょうか？誰にもわかりません。ポータブル、クロスプラットフォームに書かれていないプログラムは、プラットフォームと共に死にます。もうひとつは、FM TOWNSエミュレータ「津軽」の開発に成功して、これならFM77AVも行けそうだ、と、思っていたものの、でも需要無さそうだし、と、思っていたところが、この夏(2022)帰省したとき、ゲーム保存協会のジョセフさんと話しているとき、コピープロテクトをそのまま再現できるエミュレータがあるといいのにね、という話になって、じゃあやるか、と、思って作りました。（なお、既に津軽はコピープロテクトをそのまま再現するようになっています、というか、現在までのところFM TOWNS用ソフトに使われていたコピープロテクトで津軽で再現不可能なものは確認されていません。)

現実に目を向けてみましょう。違法コピーされたレトロゲームはネットで簡単に見つけることができます。例えば、FM-7用のテグザー、もしも、起動時にビープ音がしたならば、それはまず間違いなく、謎のハッカーJ.N.氏によって書かれたコピープロテクトを回避するInitial Program Loaderが作動しています。誰なのか、彼なのか、彼女なのか、まったくわからないこの人物ですが、ThexderのそのIPLには本人の署名が残っています。陸奥では、クラックして実行するのではなく、もとのプログラムをそのままコピープロテクションごと再現して実行したい、と、思っています。

今ネット上に存在する違法コピー、大半が日本国外のサーバーで見つかるので、日本国内の著作権法で取り締まろうとしても取り締まることはできないのが現実なのだと思います。が、この違法コピーは、いずれコンピュータ歴史・文化の保存になんらかの役割を持つ日が来るかもしれません。ただ、まだ著作権法はコンピュータやゲームの歴史・文化を保存するということに関して議論できる状態になっていません。今の著作権法は著作者を保護するものの、著作物を保護するという観点が決定的に欠落しています。

また、コピープロテクトをかける人、はずす人のチキンレース自体も非常に興味深い歴史の一部と言えます。だから、テグザーのコピープロテクトはどのように作動していたのか、これも保存したいし、それがどうやってクラックされたのか、それも技術史の一部として残したいと考えています。



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

# About Keyboard Layout

FM-7/77AV was from Japan.  Therefore its keyboard layout was Japanese.  If you are using Japanese keyboard, it should work as is, but if you are using US keyboard, you can use the following keys for typing keys that existed on FM77AV keyboard but does not exist (or different) in the US keyboard.

| US Keyboard | In Mutsu VM |
|-------------|-------------|
|~            |ESC          |
|'            |:            |
|Right Ctrl   |Double Quote |
|Left Alt     |Left Space   |
|Right Alt    |Right Space  |

For typing a longer text like F-BASIC program, you can also use keyboard translation mode, which translates what you type into FM-7 key code.  You can choose a translation mode from Virtual Machine -> Keyboard.


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

最低限必要なのは、実機FM-7,FM77AV,またはFM77AV40から抜き出したROMイメージがあるディレクトリの指定です。そのディレクトリを指定してSTARTボタンを押すと、F-BASICのプロンプトが表示されるでしょう。

バーチャルマシンからGUIに戻るには、（設定を変更していない限り）Scroll Lockキーを押すか、またはウィンドウ右下のMENUアイコンをクリックします。バーチャルマシンに戻るには、またScroll Lockを押すか、Virtual Machine->Resumeを選びます。

ディスクのゲームを起動するには、ほとんどの場合、ダイアログのFD0にイメージファイル名を指定して起動すれば自動的にゲームが始まります。

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
