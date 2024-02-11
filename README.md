# MatrixPatterns
Using Arduino to render patterns on a 16x16 LED Matrix

I wanted a night light to very slowly rotate through a variety of great patterns. So I have it rotate through 9 different patterns, one per hour. Most of the patterns were borrowed from other places. Many were tweaked by me. 
![IMG_7229](https://github.com/krampster/MatrixPatterns/assets/20178922/bb029a59-ad3b-4666-abb7-430f85018361)

# Patterns:
HeartDust - pixel heart with interactive confetti. Based on Adafruit PixelDust sample  
TwinkleFox - FastLED sample. Thanks Mark Kriegsman.  
Scene16 - downloaded pixel art  
Zenyatta16 - my son's pixel art  
Pacifica (blue waves) - FastLED sample. Thanks Mark Kriegsman and Mary Corey March.   
LavaLamp - Modified FastLED sample: NoisePlusPalette   
Fire - Modified: https://github.com/toggledbits/MatrixFireFast Thanks Patrick H. Rigney. I scoured the internet for the best Fire examples, and improved on this one.  
DemoReel100: (Rainbow with glitter and Confetti) - FastLED sample. Thanks Mark Kriegsman  
Cylon (color changing snake) - FastLED sample  

# How to:
The parts cost about $70.  
Thanks to Adafruit for the guide and the 3d printed case.  
https://learn.adafruit.com/sqaure-neopixel-display-with-black-led-acrylic/code  
https://www.thingiverse.com/thing:4812805


![IMG_7221](https://github.com/krampster/MatrixPatterns/assets/20178922/0328c519-4b77-4898-8f44-7806b884c2c2)

My hardware includes a Adafruit Feather M0 Basic Proto. (I recommend a M4 express instead)  
https://www.adafruit.com/product/2772  
And a prop maker  
https://www.adafruit.com/product/3988 - Allows me to use the accelerometer and maybe up-voltages the data out.  
I added a button to cycle through different modes, and holding it enters demo mode.  

Image conversions to code done via LCD Image Converter  
https://sourceforge.net/projects/lcd-image-converter/45c8-b796-d359f187648e)  
