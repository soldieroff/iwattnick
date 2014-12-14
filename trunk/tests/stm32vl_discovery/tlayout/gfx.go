#
# Sample Graphics Objects for LCD graphics library testing
#

include bold-6x8-cyr-caps.go
include bold-6x8-cyr-lower.go
include bold-6x8-lat-caps.go
include bold-6x8-lat-lower.go
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

text test1 1    \{align,center}МЕНЮ 1 \{catrun}
text mi_speed 1 Скорость\{align,right}\{speed,proun_8x8_digits}
text mi_dist 1  Расстояние\{align,right}\{dist,proun_8x8_digits}
text mi_test 1  Значение\{align,right}\{test,proun_8x8_digits}

text test2 1    \{align,center}МЕНЮ 2 \{cycle_wheel}\{cyclist}\{cycle_wheel}
text mi_back 1  Назад

var speed
var dist
var test

action none
action back
action layout L
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
    setvar(test,1)  1 Another menu item
    setvar(test,2) 1 Yet another menu item
    setvar(test,3) 1 One more menu item
    setvar(test,123) 1 И ещё пункт меню
    setvar(test,1234) 1 И ещё один
    setvar(test,-1234) 1 И ещё...
    setvar(test,-99999) 1 И ещё дофига...
    setvar(test,2) 1 АБВГДЕЁЖЗИЙКЛМНО
    setvar(test,3) 1 ПРСТУФХЦЧШЩЬЪЭЮЯ
    setvar(test,4) 1 абвгдеёжзийклмно
    setvar(test,5) 1 прстуфхцчшщьъэюя
    setvar(test,10) 1 1234567890
    setvar(test,11) 1 !()?.,;*-/_~
    layout(test3)  1 Подменю 3
    setvar(test,6) 1 Меню ещё не закончилось
    setvar(test,7) 1 Истино говорю вам
    setvar(test,8) 1 А вот теперь закончилось
    setvar(test,9) 1 Теперь уже - точно

menu test2 10 1
    layout(test1) mi_back
    layout(test1) mi_test
    layout(test1) 1 Тоже назад

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


text test3 1 \{align,center}Наноменю
menu test3 8
    none              mi_test
    setvar(test,1)    1 Установить 1
    setvar(test,5)    1 Установить 5
    setvar(test,123)  1 Установить 123
    back              mi_back

layout test3 96 48
    color 1
    box 0 0 95 9
    line 0 10 0 47
    line 0 47 95 47
    line 95 47 95 10
    color 0
    text test3 0 1
    color 1
    menu test3 1 10 94 46
