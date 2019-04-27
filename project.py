from PyQt5 import QtWidgets, QtGui,QtCore
from console import Ui_Dialog #importing the generated python class from.ui
from PyQt5.QtWidgets import QTableWidgetItem
import sys, os
import socket
from twilio.rest import Client

HOST = '10.0.0.199'
PORT = 8005

#inheriting mywindow class from the class QDialog
class mywindow(QtWidgets.QDialog):
    def __init__(self):
        super(mywindow, self).__init__() #initiating the parent class
        self.ui = Ui_Dialog() 
        self.ui.setupUi(self) #calling the function generated in the .ui
        

        self.ui.Auto.clicked.connect(self.Auto_mode) 

        self.ui.Manual.clicked.connect(self.Manual_mode) 

        self.ui.up.clicked.connect(self.up_move)

        self.ui.down.clicked.connect(self.down_move)

        self.ui.left.clicked.connect(self.left_move)

        self.ui.right.clicked.connect(self.right_move) 

        self.ui.stop.clicked.connect(self.stop_move) 

        self._update_timer = QtCore.QTimer()
        self._update_timer.timeout.connect(self.update_display)
        self._update_timer.start(1000) # milliseconds





    def Auto_mode(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Auto mode\n")
            s.sendall(b'auto')
            # # Your Account SID from twilio.com/console
            # account_sid = "AC4395720c534e1be2cae59ad58fcfb8d3"
            # # Your Auth Token from twilio.com/console
            # auth_token  = "69133ef1e8bdda9079022e898aacb519"

            # client = Client(account_sid, auth_token)

            # message = client.messages.create(
            #     to="+17209175161", 
            #     from_="+17205730808",
            #     body="Floor cleaning Robot: Auto mode turned on! Welcome back to your cleaner home")

            # print(message.sid)



    def Manual_mode(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Manual_mode\n")
            s.sendall(b'manual')

    def up_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("up_move\n")
            s.sendall(b'up')

    def down_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("down_move\n")
            s.sendall(b'down')

    def left_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("left_move\n")
            s.sendall(b'left')

    def right_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("right_move\n")
            s.sendall(b'right')

    def stop_move(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("stop\n")
            s.sendall(b'stop')



    def update_display(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")
            print("Display update")
            


            s.sendall(b'display')

            lux_value = s.recv(10)
            lux_string = str(lux_value)
            lux_string.replace('\x00',' ')
            print("lux value ", lux_string)
            self.ui.lux.setText(lux_string)

            motion_value = s.recv(10)
            motion_string = str(motion_value)
            motion_string.replace('\x00',' ')
            print("motion_value ", motion_string)
            self.ui.motion.setText(motion_string)
            
            water_value = s.recv(10)
            water_string = str(water_value)
            water_string.replace('\x00',' ')
            print("water_value ", water_value)
            self.ui.water.setText(water_string)

            mode_value = s.recv(10)
            mode_string = str(mode_value)
            mode_string.replace('\x00',' ')
            print("mode ",mode_string)
            self.ui.mode.setText(mode_string)


    

    
  
def main():
    

    app = QtWidgets.QApplication([])
    application = mywindow()
    application.show()
    app.exec() # to execute the application

    
if __name__ == '__main__':
    main()






