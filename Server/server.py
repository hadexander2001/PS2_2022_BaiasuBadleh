<<<<<<< HEAD
from crypt import methods
from flask import Flask, render_template
from flask import request
=======
from flask import Flask, render_template
from requests import request
>>>>>>> 189d2b6ee93b6c11768c6b72b4450e35e9bae7b6
import serial
app = Flask(__name__)

ser = serial.Serial('/dev/ttyACM0')
print(ser.name) 

@app.route('/')
<<<<<<< HEAD
def main_interface():
    return render_template('home.html')
=======
def index():

    return render_template('index.html')

if __name__ == '__main__':
    app.run()
    
    # text = 'Baiasu&Badleh 2022'
    # temp = ' - Temperature is '
    # temp_serial = ser.readline()


    # buttonTags = \
    #     '<p>\
    #     <button onclick="document.location=\'led_off\'">LED OFF</button> \
    #     <button onclick="document.location=\'led_on\'">LED ON</button> \
    #     <button onclick="document.location=\'set_rgb_led\'">RGB LED</button> \
    #     <button onclick="document.location=\'EEPROMmessages\'">EEPROM Messages</button> \
    #     </p>'
        
    # return text + temp + temp_serial.decode()  + buttonTags
>>>>>>> 189d2b6ee93b6c11768c6b72b4450e35e9bae7b6
    
@app.route('/led_on')
def led_on():
    ser.write("1 A".encode())
    return "LED ON"

@app.route('/led_off')
def led_off():
    ser.write("1 S".encode())
    return "LED OFF"

@app.route('/set_rgb_led', methods=['GET'])
def RGBScalingInterface():
    return render_template('set_rgb_led.html')

@app.route('/set_rgb_led', methods=['POST'])
def set_rgb_led():
<<<<<<< HEAD
    red = request.form['RED']
    green = request.form['GREEN']
    blue = request.form['BLUE']
    print("2 " + red + green + blue)
    ser.write(("2 " + red + green + blue).encode())
    return render_template('set_rgb_led.html')
=======
    input_tags = \
        '<br><br><label for="RED">Red scale: </label> \
        <input type="text" id="RED" name="RED"><br><br> \
        <label for="GREEN">Green scale: </label> \
        <input type="text" id="GREEN" name="GREEN"><br><br> \
        <label for="BLUE">Blue scale: </label> \
        <input type="text" id="BLUE" name="BLUE"><br><br>'

    red = request.input['RED']
    ser.write(("2 " + red).encode())
    return "Scale RGB LED\n" + input_tags
>>>>>>> 189d2b6ee93b6c11768c6b72b4450e35e9bae7b6

@app.route('/EEPROMmessages')
def readEEPROM():
    return "Currently Unavailable"

if __name__ == '__main__':
    app.run()