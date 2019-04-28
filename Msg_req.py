from flask import Flask, request
from twilio.twiml.messaging_response import MessagingResponse
import socket

HOST = '10.0.0.199'
PORT = 8005

app = Flask(__name__)

@app.route("/sms", methods =['POST'])
def sms():
    number = request.form['From']
    message_body = request.form['Body']

    
    if (message_body == "Clean"):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Sent signal to start\n")
            s.sendall(b'auto')
            s.sendall(b'on')

        resp = MessagingResponse()
        response_message = ('Hello, Your robot is activated. Cleaning in progress')
        resp.message(response_message)
        return str(resp)

if __name__ == "__main__":
    app.run(debug=True)