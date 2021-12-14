

```
lv_font_conv --font OpenSans-Regular.ttf --symbols ãÃâÂçÇôÔõÕéèÉÈíìÍÌúùÙÚáÁàÀ° -r '0x20-0x7f,0x2020,0xB0' --font NotoEmoji-Regular.ttf -r '0x1F600-0x1F64F' --font icons_sys_20.ttf -r '0xe920,0xe924,0xe921,0xe922,0xe923,0xe906,0xe907,0xe901,0xe903,0xe905,0xe904,0xe902,0xe900,0xe908,0xe909,0xe90a,0xe90b,0xe90c' --size 24 --format lvgl --bpp 2 --no-compress -o lv_font_24.c
```

```
lv_font_conv --font icons_sys_20.ttf -r '0xe920,0xe924,0xe921,0xe922,0xe923,0xe906,0xe907,0xe901,0xe903,0xe905,0xe904,0xe902,0xe900,0xe908,0xe909,0xe90a,0xe90b,0xe90c' --size 24 --format lvgl --bpp 2 --no-compress -o lv_font_sys_24.c
```


```
lv_font_conv --font icons_sys_48.ttf -r 0xe908 -r 0xe901 -r 0xe900 -r 0xe903 -r 0xe909 -r 0xe904 -r 0xe905 -r 0xe906 -r 0xe907 -r 0xe90b -r 0xe90c -r 0xe902 -r 0xe910 -r 0xe914 --size 48 --format lvgl --bpp 2 --no-compress -o lv_font_sys_48.c
```


```
lv_font_conv --font 071MKSDBoldB.TTF --symbols %-/0123456789: -r 0x20 --size 76 --format lvgl --bpp 2 --no-compress -o lv_font_clock_76.c
```



-------------------------------------------------------------------------------
        batteryFull          = "\xEE\xA4\xA0"; //e920
        batteryEmpty         = "\xEE\xA4\xA4"; //e924
        batteryThreeQuarter  = "\xEE\xA4\xA1"; //e921
        batteryHalf          = "\xEE\xA4\xA2"; //e922
        batteryOneQuarter    = "\xEE\xA4\xA3"; //e923
        batteryError         = "\xEE\xA4\x86"; //e906
        batteryUnknown       = "\xEE\xA4\x87"; //e907
        heartBeat            = "\xEE\xA4\x81"; //e901
        bluetooth            = "\xEE\xA4\x83"; //e903
        plug                 = "\xEE\xA4\x85"; //e905
        shoe                 = "\xEE\xA4\x84"; //e904                           
        dot                  = "\xEE\xA4\x82"; //e902
        info                 = "\xEE\xA4\x80"; //e900
        phone                = "\xEE\xA4\x88"; //e908
        clock                = "\xEE\xA4\x89"; //e909
        backLight            = "\xEE\xA4\x8A"; //e90a
        firmware             = "\xEE\xA4\x8B"; //e90b
        about                = "\xEE\xA4\x8C"; //e90c
        wface                = "\xEE\xA4\x8F"; //e90f

        expandLess           = "\xEE\xA4\x90"; //e910
        expandMore           = "\xEE\xA4\x91"; //e911

        // font sys 48
        
        tennisRacket1        = "\xEE\xA4\x81";//e901
        tennisRacket2        = "\xEE\xA4\x80";//e900
        tennisRacket3        = "\xEE\xA4\x88";//e908

        settings             = "\xEE\xA4\x82";//e902

        brightnessHigh       = "\xEE\xA4\x84";//e904
        brightnessLow        = "\xEE\xA4\x85";//e905
        brightnessMedium     = "\xEE\xA4\x86";//e906

        notificationsOff     = "\xEE\xA4\x8B";//e90b
        notificationsOn      = "\xEE\xA4\x8C";//e90c

        highlight            = "\xEE\xA4\x87";//e907

        qrCode               = "\xEE\xA4\x94";//e914

        PhoneOff             = "\xEE\xA4\x83";//e903        
        phoneOn              = "\xEE\xA4\x89";//e909
        phoneDisabled        = "\xEE\xA4\x90";//e910