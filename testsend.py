import serial
import string
import random


def random_string_generator():
    allowed_chars = string.ascii_letters + string.digits
    str_size = random.randrange(3, 40)
    return ''.join(random.choice(allowed_chars) for x in range(str_size))


ser = serial.Serial('COM12')
print(ser.portstr)


while(1):
    s = random_string_generator()
    s = s+"\r\n"
    ser.write(s.encode())
    #print(s)
