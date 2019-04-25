from PyQt5 import QtWidgets, QtGui,QtCore
from console import Ui_Dialog #importing the generated python class from.ui
from PyQt5.QtWidgets import QTableWidgetItem
import sys, os
import socket

HOST = '10.0.0.199'
PORT = 8005

#inheriting mywindow class from the class QDialog
class mywindow(QtWidgets.QDialog):
    def __init__(self):
        super(mywindow, self).__init__() #initiating the parent class
        self.ui = Ui_Dialog() 
        self.ui.setupUi(self) #calling the function generated in the .ui
        self.establish_socket(self)

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



    def establish_socket(self):
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            #s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))

            print("Established connection\n")


    def Auto_mode(self):
        print("Auto mode\n")
        s.sendall(b'auto')

    def Manual_mode(self):
        print("Manual_mode\n")
        s.sendall(b'manual')

    def up_move(self):
        print("up_move\n")
        s.sendall(b'up')

    def down_move(self):
        print("down_move\n")
        s.sendall(b'down')

    def left_move(self):
        print("left_move\n")
        s.sendall(b'left')

    def right_move(self):
        print("right_move\n")
        s.sendall(b'right')

    def stop_move(self):
        print("stop\n")
        s.sendall(b'stop')



    def update_display(self):
        print("Display update")
        


        s.sendall(b'display')

        lux_value = s.recv(10)
        print("lux value ", lux_value)
        self.ui.lux.setText(str(lux_value))

        motion_value = s.recv(10)
        print("motion_value ", motion_value)
        self.ui.motion.setText(str(motion_value))
        
        water_value = s.recv(10)
        print("water_value ", water_value)
        self.ui.water.setText(str(water_value))


    

    
  
def main():
    

    app = QtWidgets.QApplication([])
    application = mywindow()
    application.show()
    app.exec() # to execute the application

    
if __name__ == '__main__':
    main()






