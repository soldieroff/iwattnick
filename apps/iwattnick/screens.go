#
# iWattnick screens
#

var PIN         # Код разблокировки компьютера
var CAPACITY    # Задать ёмкость батареи в А*ч

var SCREEN_NAME_0
var SCREEN_NAME_1
var SCREEN_NAME_2
var SCREEN_NAME_3
var SCREEN_NAME_4

var SPEED       # Скорость
var ACCEL       # Ускорение
var CURRENT     # Ток
var VOLTAGE     # Напряжение
var POWER       # Мощность
var CHARGE      # Заряд
var TIME        # Время
var DATE        # Дата
var TEMP_0      # Датчик температуры 1
var TEMP_1      # Датчик температуры 2
var TEMP_2      # Датчик температуры 3
var TEMP_3      # Датчик температуры 4
var CADENCE     # Каденс
var COMPASS     # Компас
var SLOPE       # Уклон
var THROTTLE    # Напряжение ручки газа
var ILLUM       # Освещённость
var BATTVOLT    # Напряжение батарейки
var BTENA       # Состояние Bluetooth

var SCREEN0     # Настройки экрана 1
var SCREEN1     # Настройки экрана 2
var SCREEN2     # Настройки экрана 3
var SCREEN3     # Настройки экрана 4
var SCREEN4     # Настройки экрана 5

var STAT_TRIP   # Статистика за поездку
var STAT_SEAS   # Статистика за сезон
var STAT_ALL    # Статистика за всё время использования

var CP0         # Контрольная точка 1
var CP1         # Контрольная точка 2
var CP2         # Контрольная точка 3

var ST_MAXSPEED # Максимальная скорость
var ST_MAXACCEL # Максимальное ускорение
var ST_MILEAGE  # Пробег
var ST_RIDETIME # Чистое время езды
var ST_MEDSPEED # Средняя скорость
var ST_MAXCURR  # Максимальный ток
var ST_MAXPOWER # Максимальная мощность
var ST_CHGSPENT # Израсходованный заряд
var ST_CHGGAIN  # Рекуперированный заряд
var ST_POWSPENT # Израсходованная энергия
var ST_POWGAIN  # Рекуперированная энергия
var ST_MAXTEMP1 # Максимальная температура 1
var ST_MAXTEMP2 # Максимальная температура 2
var ST_MAXTEMP3 # Максимальная температура 3
var ST_MAXTEMP4 # Максимальная температура 4
var ST_MAXCADEN # Максимальный каденс

var LIM_SPEED   # Ограничение скорости
var LIM_MILEAGE # Целевой пробег
var LIM_CURRENT # Ограничение тока разряда
var LIM_POWER   # Ограничение мощности

var SCR_NAME    # Название редактируемого экрана
var SCR_TMPL    # Название шаблона редактируемого экрана
var SCR_ENABLE  # Включить редактируемый экран в карусель

var CAP_MAX     # Максимальная ёмкость батареи
var CAP_LOW     # Низкий уровень заряда батареи
var CAP_LOWACT  # Действие при низком уровне заряда

var SP_PPR      # Если на вход датчика скорости подключён датчик Холла либо фазный провод, число пар магнитов. Если геркон - 1.
var SP_WHD      # Длина окружности колеса
var SP_ALDIST   # Интервал в километрах, через которые срабатывает кукушка
var SP_ALACT    # Действие при срабатывании кукушки

var CS_CALIB    # Ток для калибровки
var CS_SHRES    # Сопротивление шунта
var CS_MAXD     # Порог тока нагрузки
var CS_MAXC     # Порог тока заряда
var CS_MAXACT   # Реакция на выход тока за допустимые пределы

var VS_MAX      # Максимальный порог напряжения батареи для аварийной сигнализации
var VS_MAXACT   # Реакция на выход напряжения батареи выше заданного порога
var VS_MIN      # Минимальный порог напряжения батареи для аварийной сигнализации
var VS_MINACT   # Реакция на падение напряжения батареи ниже установленного порога

var CAP_MIN     # Минимальный порог уровня заряда
var CAP_MINACT  # Действие при достижении порога

var ALARM       # Будильник
var ALARM_WDAYS # Дни недели, в которые срабатывает будильник
var TIMER       # Время срабатывания таймера
var TIMER_REPEAT # Многоразовое или одноразовое срабатывание
var TIMER_WHEN  # По какому событию включать таймер

var TEMP_TYPE   # Тип текущего датчика температуры: Нет, Встроенный, TMP36, LM35, KTY83, каденс
var TEMP        # Показания текущего датчика температуры
var TEMP_MAX    # Верхний порог текущего датчика температуры
var TEMP_MAXACT # Реакция на выход температуры выше заданного порога
var TEMP_MIN    # Нижний порог текущего датчика температуры
var TEMP_MINACT # Реакция на падение температуры ниже установленного предела

var THR_MIN     # Минимальное напряжение ручки газа
var THR_MAX     # Максимальное напряжение ручки газа
var THR_EXP     # Экспонента для режима трансляции ручки газа (1-8)
var THR_CRUTIME # Интервал срабатывания круиз-контроля. 0 - выключить

var ILLUM_MIN   # Минимальный уровень освещения
var ILLUM_ACT   # Реакция на низкий уровень освещения

var BT_TYPE     # Тип BlueTooth модуля
var BT_NAME     # Имя BlueTooth бортового компьютера
var BT_PIN      # PIN-код для соединения с устройством

var EA_ENABLE   # Включена ли защита в текущий момент
var EA_ONCE     # Одноразовое (отключение при наступлении события) или многоразовое срабатывание
var EA_THR0     # Сбрасывать ли газ в 0 при срабатывании защиты
var EA_DISPLAY  # Отображать ли на дисплее аварийную ситуацию
var EA_SIREN    # Количество писков звукового индикатора, 0-7
var EA_REPSIR   # Повторять ли сигнал через увеличивающиеся интервалы времени (от 5 минут до до 4 часов)
var EA_PERSIST  # Защита не отключается по нажатию любой кнопки

var EW_MON      # Разрешить будильник в понедельник
var EW_TUE      # Разрешить будильник во вторник
var EW_WED      # Разрешить будильник в среду
var EW_THU      # Разрешить будильник в четверг
var EW_FRI      # Разрешить будильник в пятницу
var EW_SAT      # Разрешить будильник в субботу
var EW_SUN      # Разрешить будильник в воскресенье

action NONE
action LOCK
action SCREEN L
action SCREEN_V LV
action SEL_INFO N
action EDIT_NUM V
action EDIT_BOOL V
action EDIT_PIN V
action EDIT_ACT V
action EDIT_TMACT V
action EDIT_BTYPE
action EDIT_TEMPL
action EDIT_STR V
action EDIT_TIME V
action EDIT_DATE V
action EDIT_WDAYS V
action EDIT_TMPSENT V
action SET_CP
action RESET_CP
action RESET V
action ST_RESETALL
action S_ADD_SENS V
action S_ADD_STAT V
action CAL_CUR0
action CAL_SHUNT
action CAL_VOLTAGE
action CAL_TEMP
action CAL_HORIZON
action CAL_ILLUM
action BT_INFO
action EA_TEST
action EA_RESET
action EW_WRKDAYS
action EW_WEND
action EW_NEVER

# Выбор экрана
# Не все экраны переключаются в карусели основного экрана, некоторые можно
# просмотреть только через это меню
menu SELECT 10 1
    SEL_INFO(0) 1 \{SCREEN_NAME_0}
    SEL_INFO(1) 1 \{SCREEN_NAME_1}
    SEL_INFO(2) 1 \{SCREEN_NAME_2}
    SEL_INFO(3) 1 \{SCREEN_NAME_3}
    SEL_INFO(4) 1 \{SCREEN_NAME_4}

# Главное меню
# Сюда попадаем с информационного экрана по ДПК
menu MENU 10 1
    SCREEN(STATISTICS)  STATISTICS
    SCREEN(SENSORS)     SENSORS
    SCREEN(LIMITS)      LIMITS
    SCREEN(CHECKPOINT)  CHECKPOINT
    SCREEN(SCREENS)     SCREENS
    SCREEN(PROTECT)     PROTECT

# Настроить датчики
# Выводит показания всех датчиков, можно войти в меню настроек соответствующего датчика
menu SENSORS 10 1
    NONE                S_SPEED
    NONE                S_ACCEL
    NONE                S_CURRENT
    NONE                S_VOLTAGE
    NONE                S_POWER
    NONE                S_CAPACITY
    NONE                S_TIME
    NONE                S_DATE
    NONE                S_TEMP_0
    NONE                S_TEMP_1
    NONE                S_TEMP_2
    NONE                S_TEMP_3
    NONE                S_CADENCE
    NONE                S_COMPASS
    NONE                S_SLOPE
    NONE                S_THROTTLE
    NONE                S_ILLUM
    NONE                S_BATTVOLT
    NONE                S_BTOOTH

# Просмотр статистики
# Статистика учитывается отдельно за весь период, за сезон, за поездку.
menu STATISTICS 10 1
    SCREEN_V(SHOWSTAT,STAT_TRIP) ST_TRIP
    SCREEN_V(SHOWSTAT,STAT_SEAS) ST_SEAS
    SCREEN_V(SHOWSTAT,STAT_ALL)  ST_ALL

# Статистика
# Вывод статистики, можно обнулять отдельные показатели либо все сразу
menu SHOWSTAT 10 1
    RESET(ST_MAXSPEED)  ST_MAXSPEED
    RESET(ST_MAXACCEL)  ST_MAXACCEL
    RESET(ST_MILEAGE)   ST_MILEAGE
    RESET(ST_RIDETIME)  ST_RIDETIME
    RESET(ST_MEDSPEED)  ST_MEDSPEED
    RESET(ST_MAXCURR)   ST_MAXCURR
    RESET(ST_MAXPOWER)  ST_MAXPOWER
    RESET(ST_CHGSPENT)  ST_CHGSPENT
    RESET(ST_CHGGAIN)   ST_CHGGAIN
    RESET(ST_POWSPENT)  ST_POWSPENT
    RESET(ST_POWGAIN)   ST_POWGAIN
    RESET(ST_MAXTEMP1)  ST_MAXTEMP1
    RESET(ST_MAXTEMP2)  ST_MAXTEMP2
    RESET(ST_MAXTEMP3)  ST_MAXTEMP3
    RESET(ST_MAXTEMP4)  ST_MAXTEMP4
    RESET(ST_MAXCADEN)  ST_MAXCADEN
    ST_RESETALL         ST_RESETALL


# Ограничения
menu LIMITS 10 1
    EDIT_NUM(LIM_SPEED)   LIM_SPEED
    EDIT_NUM(LIM_MILEAGE) LIM_MILEAGE
    EDIT_NUM(LIM_CURRENT) LIM_CURRENT
    EDIT_NUM(LIM_POWER)   LIM_POWER

# Настройки защиты
menu PROTECT 10 1
    LOCK                LOCK
    EDIT_PIN(PIN)       EDIT_PIN

# Контрольные точки
menu CHECKPOINT 10 1
    SET_CP                SET_CP
    SCREEN_V(SHOW_CP,CP0) SHOW_CP_0
    SCREEN_V(SHOW_CP,CP1) SHOW_CP_1
    SCREEN_V(SHOW_CP,CP2) SHOW_CP_2
    RESET_CP              RESET_CP

# Выберите экран
# Здесь можно настроить содержимое всех экранов
menu SCREENS 10 1
    SCREEN_V(S_SETUP,SCREEN0)  1 \{SCREEN_NAME_0}
    SCREEN_V(S_SETUP,SCREEN1)  1 \{SCREEN_NAME_1}
    SCREEN_V(S_SETUP,SCREEN2)  1 \{SCREEN_NAME_2}
    SCREEN_V(S_SETUP,SCREEN3)  1 \{SCREEN_NAME_3}
    SCREEN_V(S_SETUP,SCREEN4)  1 \{SCREEN_NAME_4}

# Настроить экран
# Задать какая информация будет выводиться на основные экраны, и как она будет форматирована
menu S_SETUP 10 1
    EDIT_STR(SCR_NAME)             SCR_NAME
    EDIT_TEMPL                     SCR_TEMPL
    EDIT_BOOL(SCR_ENABLE)          SCR_ENABLE
    SCREEN(S_ADD_SENS)             SCR_A_SENS
    SCREEN_V(S_ADD_STAT,STAT_TRIP) SCR_A_STT
    SCREEN_V(S_ADD_STAT,STAT_SEAS) SCR_A_STS
    SCREEN_V(S_ADD_STAT,STAT_ALL)  SCR_A_STA


menu S_ADD_SENS 10 1
    S_ADD_SENS(SPEED)       S_SPEED
    S_ADD_SENS(ACCEL)       S_ACCEL
    S_ADD_SENS(CURRENT)     S_CURRENT
    S_ADD_SENS(VOLTAGE)     S_VOLTAGE
    S_ADD_SENS(POWER)       S_POWER
    S_ADD_SENS(CAPACITY)    S_CAPACITY
    S_ADD_SENS(TIME)        S_TIME
    S_ADD_SENS(DATE)        S_DATE
    S_ADD_SENS(TEMP_0)      S_TEMP_0
    S_ADD_SENS(TEMP_1)      S_TEMP_1
    S_ADD_SENS(TEMP_2)      S_TEMP_2
    S_ADD_SENS(TEMP_3)      S_TEMP_3
    S_ADD_SENS(CADENCE)     S_CADENCE
    S_ADD_SENS(COMPASS)     S_COMPASS
    S_ADD_SENS(SLOPE)       S_SLOPE
    S_ADD_SENS(THROTTLE)    S_THROTTLE
    S_ADD_SENS(ILLUM)       S_ILLUM
    S_ADD_SENS(BATTVOLT)    S_BATTVOLT
    S_ADD_SENS(BTENA)       S_BTOOTH

menu S_ADD_STAT 10 1
    S_ADD_STAT(ST_MAXSPEED) ST_MAXSPEED
    S_ADD_STAT(ST_MAXACCEL) ST_MAXACCEL
    S_ADD_STAT(ST_MILEAGE)  ST_MILEAGE
    S_ADD_STAT(ST_RIDETIME) ST_RIDETIME
    S_ADD_STAT(ST_MEDSPEED) ST_MEDSPEED
    S_ADD_STAT(ST_MAXCURR)  ST_MAXCURR
    S_ADD_STAT(ST_MAXPOWER) ST_MAXPOWER
    S_ADD_STAT(ST_CHGSPENT) ST_CHGSPENT
    S_ADD_STAT(ST_CHGGAIN)  ST_CHGGAIN
    S_ADD_STAT(ST_POWSPENT) ST_POWSPENT
    S_ADD_STAT(ST_POWGAIN)  ST_POWGAIN
    S_ADD_STAT(ST_MAXTEMP1) ST_MAXTEMP1
    S_ADD_STAT(ST_MAXTEMP2) ST_MAXTEMP2
    S_ADD_STAT(ST_MAXTEMP3) ST_MAXTEMP3
    S_ADD_STAT(ST_MAXTEMP4) ST_MAXTEMP4
    S_ADD_STAT(ST_MAXCADEN) ST_MAXCADEN

# Настройка датчика скорости
menu S_SPEED 10 1
    EDIT_NUM(SP_PPR)    SP_PPR
    EDIT_NUM(SP_WHD)    SP_WHD
    EDIT_NUM(SP_ALDIST) SP_ALDIST
    EDIT_ACT(SP_ALACT)  SP_ALACT

# Настройка датчика тока
menu S_CURRENT 10 1
    CAL_CUR0            CS_CALIB0
    CAL_SHUNT           CS_CALIB
    EDIT_NUM(CS_SHRES)  CS_SHRES
    EDIT_NUM(CS_MAXD)   CS_MAXD
    EDIT_NUM(CS_MAXC)   CS_MAXC
    EDIT_ACT(CS_MAXACT) CS_MAXACT

# Датчик напряжения
menu S_VOLTAGE 10 1
    CAL_VOLTAGE         VS_CALIB
    EDIT_NUM(VS_MAX)    VS_MAX
    EDIT_ACT(VS_MAXACT) VS_MAXACT
    EDIT_NUM(VS_MIN)    VS_MIN
    EDIT_ACT(VS_MINACT) VS_MINACT

# Ёмкость батареи
menu S_CAPACITY 10 1
    EDIT_NUM(CAPACITY)   CAP_SET
    EDIT_NUM(CAP_MAX)    CAP_SETMAX
    EDIT_NUM(CAP_LOW)    CAP_LOW
    EDIT_ACT(CAP_LOWACT) CAP_LOWACT

# Настройка часов
menu S_CLOCK 10 1
    EDIT_TIME(TIME)         CL_SET_TIME
    EDIT_DATE(DATE)         CL_SET_DATE
    EDIT_TIME(ALARM)        CL_AL_SET
    EDIT_WDAYS(ALARM_WDAYS) CL_AL_WDAYS
    EDIT_TIME(TIMER)        CL_TM_SET
    EDIT_BOOL(TIMER_REPEAT) CL_TM_REPEAT
    EDIT_TMACT(TIMER_WHEN)  CL_TM_START

# Датчик температуры
menu S_TEMP 10 1
    EDIT_TMPSENT(TEMP_TYPE) TS_TYPE
    CAL_TEMP                TS_CALIB
    EDIT_NUM(TEMP_MAX)      TS_MAX
    EDIT_ACT(TEMP_MAXACT)   TS_MAXACT
    EDIT_NUM(TEMP_MIN)      TS_MIN
    EDIT_ACT(TEMP_MINACT)   TS_MINACT

# Датчик уклона
menu S_SLOPE 10 1
    CAL_HORIZON CAL_HORIZON

# Ручка газа
menu S_THROTTLE 10 1
    EDIT_NUM(THR_MIN)     THR_MIN
    EDIT_NUM(THR_MAX)     THR_MAX
    EDIT_NUM(THR_EXP)     THR_EXP
    EDIT_NUM(THR_CRUTIME) THR_CRUTIME

# Датчик света
menu S_ILLUM 10 1
    CAL_ILLUM           ILLUM_CAL
    EDIT_NUM(ILLUM_MIN) ILLUM_MIN
    EDIT_ACT(ILLUM_ACT) ILLUM_ACT

# Модуль Bluetooth
menu S_BTOOTH 10 1
    EDIT_BTYPE          BT_TYPE
    EDIT_STR(BT_NAME)   BT_NAME
    EDIT_PIN(BT_PIN)    BT_PIN
    BT_INFO             BT_INFO

# Контрольная точка N
# Просмотр статистики в контрольной точке N
menu SHOW_CP 10 1
    NONE        ST_MAXSPEED
    NONE        ST_MAXACCEL
    NONE        ST_MILEAGE
    NONE        ST_RIDETIME
    NONE        ST_MEDSPEED
    NONE        ST_MAXCURR
    NONE        ST_MAXPOWER
    NONE        ST_CHGSPENT
    NONE        ST_CHGGAIN
    NONE        ST_POWSPENT
    NONE        ST_POWGAIN
    NONE        ST_MAXTEMP1
    NONE        ST_MAXTEMP2
    NONE        ST_MAXTEMP3
    NONE        ST_MAXTEMP4
    NONE        ST_MAXCADEN

# Меню редактирования действия (EDIT_ACT)
menu E_ACTION 10 1
    EDIT_BOOL(EA_ENABLE)  EA_ENABLE
    EDIT_BOOL(EA_ONCE)    EA_ONCE
    EDIT_BOOL(EA_THR0)    EA_THR0
    EDIT_BOOL(EA_DISPLAY) EA_DISPLAY
    EDIT_NUM(EA_SIREN)    EA_SIREN
    EDIT_BOOL(EA_REPSIR)  EA_REPSIR
    EDIT_BOOL(EA_PERSIST) EA_PERSIST
    EA_TEST               EA_TEST
    EA_RESET              EA_RESET

# Меню редактирования дней недели
menu E_WDAYS 10 1
    EDIT_BOOL(EW_MON)   EW_MON
    EDIT_BOOL(EW_TUE)   EW_TUE
    EDIT_BOOL(EW_WED)   EW_WED
    EDIT_BOOL(EW_THU)   EW_THU
    EDIT_BOOL(EW_FRI)   EW_FRI
    EDIT_BOOL(EW_SAT)   EW_SAT
    EDIT_BOOL(EW_SUN)   EW_SUN
    EW_WRKDAYS          EW_WRKDAYS
    EW_WEND             EW_WEND
    EW_NEVER            EW_NEVER
