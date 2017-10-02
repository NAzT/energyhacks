# Temp Guard
Temp Guard is the solution to an issue we found when moving into our own home.
There the warm water supply had a circulation pump that always ran to put hot
water to the outlets for pure convenience. This was a twofold mess:
  
* the warm water reservour was cooled down even though no warm water was needed
* the pump ran with 30W 24h a day

Switching to a time switch brought some relief, but the right condition was met
when letting the pump run only on necessity.

There were a few solutions of this kind in the internet, either as tinker work or
commercial.

As I wanted to have the freedom to change the configuration to whatever idea I would
come during operating the control, I chose to develop it on my own - and here is the
solution.

An Atmel ATmega8 is used to measure two temperatures via ADC (warm water outlet of the  heating and inlet of the warm water circulation) and to switch a pump on diverse temperature conditions as well as manually.
Switching is done via a solid-state relais and UI is done via LCD using a solution to drive it serially as shown in some Elektor magazine article; nevertheless the
corresponding routines are developed corresponding to the LCD spec - to
really understand what happens. User interaction is done via three buttons: Menu selection
and up and down.

The control is running since 2009 whitout any error (but some alterations on the
hysteresis of the temperature measurement - there is still some room for improvement)

Newly adapted is the Arduino version of the Temperature Guard that runs with a Pollin
AVR NetIO board using the Arduino bootloader and [Jean-Claude Wippler's ethercard library](http://github.com/jcw/ethercard).
This version features the capability to switch the circulation pump also via the LAN (for convenience to not run water to start the pump or go to the basement for manual action)

<img src="TempGuard_Webdisplay.jpg" width=400px>

Markus Gebhard, Copyright 2009-2013
