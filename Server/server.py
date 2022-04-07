from flask import Flask
import serial
app = Flask(__name__)

ser = serial.Serial('/dev/ttyACM0')
print(ser.name) 

@app.route('/')
def main_interface():
    text = 'Baiasu&Badleh 2022'
    temp = '- Temperature is '
    temp_serial = ser.readline()

    buttonTags = \
        '<p>\
        <button onclick="document.location=\'led_off\'">LED OFF</button> \
        <button onclick="document.location=\'led_on\'">LED ON</button></p>'

    return text + temp + temp_serial.decode() + buttonTags
    
@app.route('/led_on')
def led_on():
    ser.write("1 A".encode())
    return "LED ON"

@app.route('/led_off')
def led_off():
    ser.write("1 S".encode())
    return "LED OFF"