from flask import Flask
import serial
app = Flask(__name__)

ser = serial.Serial('/dev/ttyACM0')
print(ser.name) 

@app.route('/')
def main_interface():
    text = 'Baiasu&Badleh 2022'
    temp = ' - Temperature is currently unavailable'
    # temp_serial = ser.readlines()

    buttonTags = \
        '<p>\
        <button onclick="document.location=\'led_off\'">LED OFF</button> \
        <button onclick="document.location=\'led_on\'">LED ON</button> \
        <button onclick="document.location=\'set_rgb_led\'">RGB LED</button> \
        <button onclick="document.location=\'EEPROMmessages\'">EEPROM Messages</button> \
        </p>'
        
    return text + temp  + buttonTags #+temp_serial.decode()
    
@app.route('/led_on')
def led_on():
    ser.write("1 A".encode())
    return "LED ON"

@app.route('/led_off')
def led_off():
    ser.write("1 S".encode())
    return "LED OFF"

# Improvable
@app.route('/set_rgb_led')
def set_rgb_led():
    ser.write("2 FF0000".encode())
    return "RGB LED SET TO RED"

@app.route('/EEPROMmessages')
def readEEPROM():
    return "Currently Unavailable"