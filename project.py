from PyQt5 import QtWidgets, QtGui,QtCore
from console import Ui_Dialog #importing the generated python class from.ui
from PyQt5.QtWidgets import QTableWidgetItem
import sys
import socket

HOST = '127.0.0.1'
PORT = 8003
#inheriting mywindow class from the class QDialog
class mywindow(QtWidgets.QDialog):
    def __init__(self):
        super(mywindow, self).__init__() #initiating the parent class
        self.ui = Ui_Dialog() 
        self.ui.setupUi(self) #calling the function generated in the .ui

        #connecting the poweron button
        self.ui.Poweron.clicked.connect(self.poweron_call) 

        self.ui.poweroff.clicked.connect(self.poweroff_call) 

        self.ui.display.clicked.connect(self.update_display) 

    def update_display(self):
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST,PORT))


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

    def poweron_call(self):
        print ("Powered on\n")

    def poweroff_call(self):

        print ("Powered off\n")
        self.ui.lux.setText(str(" "))
        self.ui.motion.setText(str(" "))
        self.ui.water.setText(str(" "))

    


  
def main():
    

    app = QtWidgets.QApplication([])
    application = mywindow()
    application.show()
    app.exec() # to execute the application

    
if __name__ == '__main__':
    main()






