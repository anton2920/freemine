# FreeMine
## FreeMine is a clone of the famous Microsoft Windows game

## Brief description
FreeMine is a minesweeper game written on the C programming language. Graphics performed using SLD2. I've tried to make the exact copy, but still much work has to be done. 

## Installation guide
To install this program you need `libSDL2.so`, `libSDL2_image.so`, `libSDL2_ttf.so` and `libSDL2_mixer.so` dynamic libraries. Then perform:
``` bash
sh ./autogen.sh
./configure
make
sudo make install
```
After that, run `ldconfig(8)`. Do `sudo ldconfig`, there's no harm in it.

## Licence
It distributes only under GPLv2 licence. Go to [https://www.gnu.org/licenses/] for more information

## Copyright
© 2019 Anton
