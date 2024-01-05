# bbq10

![bbq10](https://i.imgur.com/64zl30s.jpeg)

Legendary keyboard from Blackberry Q10 smartphone.

* Keyboard Maintainer: [Viktar Simanenka](https://github.com/viteo)
* Hardware Supported: 
   * Blackberry Q10 keyboard
   * STM32F103 BluePill
* Hardware Availability:
   * BBQ10 around 3$ on AliExpress
   * BM14C(0.8)-24DS receptacle connector

Make example for this keyboard (after setting up your build environment):

    qmk compile -kb handwired/bbq10 -km default

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader:

* **Bootmagic reset**: Hold down the key at (4,4) in the matrix ('$' Dollar sign near the Enter key) and plug in the keyboard (reset MCU)
