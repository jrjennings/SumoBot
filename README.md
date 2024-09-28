# SumoBot

Description:
Differential Drive Robot based on the Zumo Robot by Pololu  
Distance sensor detects left edge of object   
Timer to switch between roving and scanning modes  

COMPONENTS:  
3D Printed Chassis  
Controller - Baby Orangutan B328 Pololu #1220  
Adjustable Boost Regulator 2.5-9.5V Pololu #791  
silicone tires (home made)
2ea 150:1 Micro Metal Gearmotors MP Pololu #2368  
Sharp GP2Y0A21YK0F Analog Distance Sensor 10-80cm Pololu #136  
3-Pin Female JST PH-Style Cable (30cm) for Sharp Distance Sensors Pololu #117  
2ea QTR-1A Reflectance Sensors Pololu # 2458    
dip switch (double) for turning off motors Philmore # 30-1002  
electrolytic capacitor 33uF 25v (on power rail near sensor leads)
electrolytic Capacitor .47uF 50v(for low pass filter on distance sensor)
electrolytic capacitor 47uF 10v (on controller pwr supply) to smooth voltage spikes
resistor 12Kohm 1/4 watt (for low pass filter on distance sensor)
NI-MH Battery 4 AA Cells 2200mAh 4.8V  
Mini Slide Switch 3.Pin SPDT, 0.3A Pololu # 1408
2ea Heavy Duty nylon hinges DU-BRO Cat. No. 257 (for Dozers' blade)
Dozers' blade made from scrap sheet metal.


Basic MAZE Logic  
#########################################  
Formula for creating direction value dVal:  
dVal = irV[0] * 100 + irV[3] * 10 + irV[5] * 1  

| qty  Name | Discription | Vendor | part # |
|---|---|---|---|---|---|
| 1 | Chassis | 3D Printed PLA | Home Made | |
| 1 | Baby Orangutang B328 | Robot Controller | Pololu | #1220 |
| 000 | L | * 4Way |  
| 110 | R | Right Turn (no straight path) |  
| 001 | S | * Left Branch |
| 101 | L | Left turn (no straight path) |     
| 111 | B | * End of line |
| 011 | S | Tracking Line |
