Sample Profiles

[LAYDOCK (T&E)]
Mutsu Profile allows relative-path from the location of the profile.  If you have Laydock disk images, and FM77AV40 ROM files, organize files as:

    Laydock.Mutsu (in this folder)
    ROMS/AV
        BOOT_BAS.ROM
        BOOT_DOS.ROM
        FBASIC30.ROM
        INITIATE.ROM
        KANJI.ROM
        SUBSYSCG.ROM
        SUBSYS_A.ROM
        SUBSYS_B.ROM
        SUBSYS_C.ROM
    DISK
        LAYDOCK1.D77
        LAYDOCK2.D77

Then, you can open Laydock.Mutsu, and start LAYDOCK by clicking on "START" button.

Also, this game really required two players, but many Laydock players (I believe) played by oneself with a game pad and a keyboard.  In fact, player 2 only had a role of shooting gun or a missile and was pretty boring.

How I did was holding gamepad to control the fighter 1, dock with fighter 2 as soon as the game starts, and then pressed the left space with side of my left hand while still controlling fighter 1 with the game pad.

What a pain!

But, with virtual keys, you don't have to use both a game pad and a keyboard.  You can assign LEFT_SPACE to your game pad in Keyboard 2 or Keyboard 3 tab.  Make sure to assign your USB or Bluetooth game pad to Gameport 0 of the VM.  Then,

(1) Start Laydock and load up players.
(2) Start the game with:
      Player 1  JOYSTICK 1
      Player 2  KEYBOARD
(3) Dock fighter 1 to fighter 2, and then you can do all the controls with your game pad.





[YS2 (Falcom)]
Back in late 1980s, games were released in multiple floppy disks, and very often we had to change disk as the game progresses.

It would be very inconvenient if you need to open file-selector and select a disk-image all the time.

To solve this problem, you can set up image-file aliases, like DISKA for DISK/YS2_A.D77, and DISKB for DISK/YS2_B.D77 etc.

This profile is an example of how to use aliases.  You can start YS2 by putting this profile and disk and ROM images as follows.

    YS2.Mutsu (in this folder)
    ROMS/AV
        BOOT_BAS.ROM
        BOOT_DOS.ROM
        FBASIC30.ROM
        INITIATE.ROM
        KANJI.ROM
        SUBSYSCG.ROM
        SUBSYS_A.ROM
        SUBSYS_B.ROM
        SUBSYS_C.ROM
    DISK
        YS2_A.D77
        YS2_B.D77
        YS2_P.D77
        YS2_USER.D77

