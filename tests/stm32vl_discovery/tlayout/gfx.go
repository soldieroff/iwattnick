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

text test1 1    \{align,center}МЕНЮ 1
text mi_speed 1 SPEED IS\{align,right}\{speed,proun_8x8_digits}
text mi_dist 1  DISTANCE IS\{align,right}\{dist,proun_8x8_digits}
text mi_test 1  VALUE IS\{align,right}\{test,proun_8x8_digits}

text test2 1    \{align,center}МЕНЮ 2
text mi_back 1  НАЗАД

var speed
var dist
var test

action setvar VN

# A generic menu layout - the frame, title etc
layout menuframe 128 64
    color 1
    box 0 0 127 9
    line 0 10 0 63
    line 0 63 127 63
    line 127 63 127 10

menu test1 10 1
    layout(test2)   mi_speed
    layout(test2)   mi_dist
    layout(test2)   mi_test
    layout(test2)  1 ANOTHER MENU ITEM
    setvar(test,0) 1 YET ANOTHER MENU ITEM
    setvar(test,1) 1 ONE MORE MENU ITEM
    setvar(test,123) 1 И ЕЩЁ ПУНКТ МЕНЮ
    setvar(test,1234) 1 И ЕЩЁ ОДИН
    setvar(test,-1234) 1 И ЕЩЁ...
    setvar(test,-99999) 1 И ЕЩЁ ДОФИГА...
    setvar(test,6) 1 МЕНЮ ЕЩЁ НЕ ЗАКОНЧИЛОСЬ
    setvar(test,7) 1 ИСТИНО ГОВОРЮ ВАМ
    setvar(test,8) 1 А ВОТ ТЕПЕРЬ ЗАКОНЧИЛОСЬ
    setvar(test,9) 1 ТЕПЕРЬ УЖЕ - ТОЧНО

menu test2 10 1
    layout(test1) mi_back
    layout(test1) mi_speed
    layout(test1) mi_dist
    layout(test1) mi_test
    layout(test1) 1 ТОЖЕ НАЗАД

layout test1 128 64
    draw menuframe 0 0
    color 0
    text test1 0 1
    color 1
    menu test1 1 10 126 62

layout test2 128 64
    draw menuframe 0 0
    color 0
    text test2 0 1
    color 1
    menu test2 1 10 126 62
