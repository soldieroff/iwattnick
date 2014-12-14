include bold-6x8-cyr-caps.go
include bold-6x8-lat-caps.go
include bold-6x8-digits.go
include bold-6x8-punct.go

include impact-8x16-digits.go

var speed
var dist

action layout L
action setvar VN

text test1 1 \{align,center}TEST 1
text mi_speed 1 SPEED IS\{align,right}\{speed,impact_8x16_digits}
text mi_dist 1  DISTANCE IS \{dist,impact_8x16_digits}

text helloworld 1 HELLO, WORLD!
text privetmir 1  ЗДРАВСТВУЙ, МИР!

menu test1 9
    layout(test1)   mi_speed
    layout(test1)   mi_dist
    layout(test1)   1 ANOTHER MENU ITEM
    setvar(speed,0) 1 YET ANOTHER MENU ITEM
    setvar(speed,1) 1 ONE MORE MENU ITEM
    setvar(speed,2) 1 И ЕЩЁ ПУНКТ МЕНЮ
    setvar(speed,3) 1 И ЕЩЁ
    setvar(speed,4) 1 И ЕЩЁ...
    setvar(speed,5) 1 И ЕЩЁ ДОФИГА...
    setvar(speed,6) 1 МЕНЮ ЕЩЁ НЕ ЗАКОНЧИЛОСЬ
    setvar(speed,7) 1 ИСТИНО ГОВОРЮ ВАМ
    setvar(speed,8) 1 А ВОТ ТЕПЕРЬ ЗАКОНЧИЛОСЬ
    setvar(speed,9) 1 ТЕПЕРЬ УЖЕ - ТОЧНО

layout test1 128 64
    color 1
    box 0 0 127 9
    line 0 10 0 63
    line 0 63 127 63
    line 127 63 127 10
    color 0
    text test1 0 0
    color 1
    menu test1 1 10 126 62
