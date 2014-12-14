#
# Sample Graphics Objects for LCD graphics library testing
#

include bold-6x8-cyr-caps.go
include bold-6x8-lat-caps.go
include bold-6x8-digits.go
include bold-6x8-punct.go

include proun-8x8-digits.go
include impact-8x16-digits.go
include large-10x16-digits.go
include impact-12x24-digits.go

anim clearance_turn 16
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........
@@@@@@@@ ........

bitmap clearance_moving_boundary
@@@..@@@
@......@
@......@
........
........
@......@
@......@
@@@..@@@

bitmap clearance_moving_fill
......
..@@..
.@@@@.
@@@@@@
@@@@@@
.@@@@.
..@@..
......

anim cycle_ants 4
........ ........ ........ ........ ........ ........ ........ ........
..@@@@.. ...@@@.. ....@@.. .....@.. ........ ..@..... ..@@.... ..@@@...
........ ......@. ......@. ......@. .@....@. .@....@. .@...... .@......
........ ........ ......@. .@....@. .@....@. .@....@. .@...... ........
........ ........ .@...... .@....@. .@....@. .@....@. ......@. ........
........ .@...... .@...... .@...... .@....@. ......@. ......@. ......@.
..@@@@.. ..@@@... ..@@.... ..@..... ........ .....@.. ....@@.. ...@@@..
........ ........ ........ ........ ........ ........ ........ ........

anim arrow_left 4
........ ........ ........ ........ ........ ........ ........ ........
....@... ...@.... ..@..... .@...... @....... .......@ ......@. .....@..
...@@... ..@@.... .@@..... @@...... @......@ ......@@ .....@@. ....@@..
..@@@... .@@@.... @@@..... @@.....@ @.....@@ .....@@@ ....@@@. ...@@@..
...@@... ..@@.... .@@..... @@...... @......@ ......@@ .....@@. ....@@..
....@... ...@.... ..@..... .@...... @....... .......@ ......@. .....@..
........ ........ ........ ........ ........ ........ ........ ........
........ ........ ........ ........ ........ ........ ........ ........

anim arrow_right 4
........ ........ ........ ........ ........ ........ ........ ........
...@.... ....@... .....@.. ......@. .......@ @....... .@...... ..@.....
...@@... ....@@.. .....@@. ......@@ @......@ @@...... .@@..... ..@@....
...@@@.. ....@@@. .....@@@ @.....@@ @@.....@ @@@..... .@@@.... ..@@@...
...@@... ....@@.. .....@@. ......@@ @......@ @@...... .@@..... ..@@....
...@.... ....@... .....@.. ......@. .......@ @....... .@...... ..@.....
........ ........ ........ ........ ........ ........ ........ ........
........ ........ ........ ........ ........ ........ ........ ........

anim clearance_forward 16
.....@..... @@@@@.@@@@@
....@@@.... @@@@...@@@@
...@@@@@... @@@.....@@@
..@@@@@@@.. @@.......@@
.@@@@@@@@@. @.........@
....@@@.... @@@@...@@@@
....@@@.... @@@@...@@@@
....@@@.... @@@@...@@@@

anim arrow_forward 4
...... ...... ...... ...... ...... ..@@.. .@@@@. @@@@@@
...... ...... ...... ...... ..@@.. .@@@@. @@@@@@ ......
...... ...... ...... ..@@.. .@@@@. @@@@@@ ...... ......
...... ...... ..@@.. .@@@@. @@@@@@ ...... ...... ......
...... ..@@.. .@@@@. @@@@@@ ...... ...... ...... ......
..@@.. .@@@@. @@@@@@ ...... ...... ...... ...... ......
.@@@@. @@@@@@ ...... ...... ...... ...... ...... ..@@..
@@@@@@ ...... ...... ...... ...... ...... ..@@.. .@@@@.

bitmap cyclist
@@.........
@@.@@@@@...
...@....@..
..@....@.@.
.@....@..@.
.......@.@.
.........@.
...........

anim cycle_wheel 2
..... ..... ..... ..... ..... .....
..... ..... ..... ..... ..... .....
..... ..... ..... ..... ..... .....
.@@.. .@... ...@. ..@@. .@@@. .@@@.
@.... @...@ @...@ ....@ ....@ @....
@...@ @...@ @...@ @...@ ..... .....
....@ @...@ @...@ @.... @.... ....@
..@@. ...@. .@... .@@.. .@@@. .@@@.

anim catrun 4
....@............@@@ ..........@@@@@...@. .................... ...@@............... ...@@@.............. ...@@@..............
..@@@.....@@@.@@@... ...@@@..@@@@@@@@@@.. ....@....@@@.....@@. ..@@@@@............. ..@@@@@............. ..@@@@@.............
..@@@@@@@@@@@@@..... ..@@@@@@@@@@@@...... ..@@@@@@@@@@@@@@@@.. ..@@@@@@@@@......... ..@@@@@@@@.......... ...@@@@@@@@@@@@@@@..
....@@@@@@@@@@@@@@@. ..@@@@@@@@@@@@@..... ..@@@@@@@@@@@@...... .....@@@@@@@....@... ....@@@@@@@@@@...... ....@@@@@@@@@@@.@@@@
..@@@@@@@@@...@@@@@. .....@@@@....@.@.... .....@@@@@@@........ .....@@@@@@@@.@@@@.. ...@@@@@@@@@@@@@@@@. .@@@@@@@@@@@@@@.....
@@.@@@.............. ......@@@@...@...... .......@@@@@........ .....@@.@@@@@@@..... ..@@.@@..@@@@@...... @@.@@.......@@@@@...
..@@................ ......@@............ ........@@@......... ......@...@@........ .....@.....@.@@..... ...@.........@@.@@..
.@.................. ......@@............ .................... .........@@@........ ..........@@..@@.... .............@......

# guess what's this
bitmap huge
@@@@@@@@@@@@@@@@
.@.@.@.@.@.@.@.@
..@.@.@.@.@.@.@@
...@.@.@.@.@.@@@
....@.@.@.@.@@@@
.....@.@.@.@@@@@
......@.@.@@@@@@
.......@.@@@@@@@
.......@@@@@@@@@
......@..@@@@@@@
.....@....@@@@@@
....@......@@@@@
...@........@@@@
..@..........@@@
.@............@@
@@@@@@@@@@@@@@@@


text hello 1    ПРИ\{dir,down}ВЕТ
text sobaka 1   ШЛА СО\{catrun}БА\{align,right}КА
text royal 5    \{align,center}ПО РОЯЛЮ

var speed
var dist

text speed 1    \{speed,impact_8x16_digits}КМ/Ч
text dist 1     \{dist,proun_8x8_digits}КМ

used glyph \{cycle_wheel}\{cyclist}\{huge}
