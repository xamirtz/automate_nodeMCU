from machine import Pin
from machine import Timer

p_test = Pin(4, Pin.OUT)
p_test.on()
time.sleep(1)
p_test.off()
time.sleep(1)
