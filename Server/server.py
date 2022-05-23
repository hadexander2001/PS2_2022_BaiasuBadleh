from crypt import methods
from flask import Flask, render_template
from flask import request
from flask_mail import Mail, Message
import serial
app = Flask(__name__)

ser = serial.Serial('/dev/ttyACM0')
print(ser.name) 

# app.config['MAIL_SERVER']='smtp.gmail.com'
# app.config['MAIL_PORT'] = 465
# app.config['MAIL_USERNAME'] = 'hadexander2001@gmail.com'
# app.config['MAIL_PASSWORD'] = '' # generate password
# app.config['MAIL_USE_TLS'] = False
# app.config['MAIL_USE_SSL'] = True
# mail = Mail(app)


@app.route('/')
def main_interface():

    
    distOutput = ser.readline()
    # distOutputAdapted = int(distOutput.decode())
    # print(distOutputAdapted)
    # if distOutputAdapted <= 10:
    #     msg = Message(
    #         'Flooding danger!',
    #         sender='hadexander2001@gmail.com',
    #         recipients=['hadexander2001@gmail.com'])

    #     msg.body = "Your room is about to get flooded!"
    #     mail.send(msg)

    return render_template(
        'home.html', 
        distOutput = distOutput.decode())
    
@app.route('/led_on')
def led_on():
    ser.write("1 A".encode())
    return "LED ON" + \
        "<style> \
            body {background-color: rgb(37, 253, 37)} \
        <style>"

@app.route('/led_off')
def led_off():
    ser.write("1 S".encode())
    return "LED OFF" + \
        "<style> \
            body {background-color: red} \
        <style>"

@app.route('/set_rgb_led', methods=['GET'])
def RGBScalingInterface():
    return render_template('set_rgb_led.html')

@app.route('/set_rgb_led', methods=['POST'])
def set_rgb_led():
    red = request.form['RED']
    green = request.form['GREEN']
    blue = request.form['BLUE']
    print("2 " + red + green + blue)
    ser.write(("2 " + red + green + blue).encode())
    return render_template('set_rgb_led.html')

@app.route('/EEPROMmessages')
def readEEPROM():
    return "Currently Unavailable"

if __name__ == '__main__':
    app.run()