# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'project_apes.ui'
#
# Created by: PyQt5 UI code generator 5.12.1
#
# WARNING! All changes made in this file will be lost!

from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(637, 430)
        self.Poweron = QtWidgets.QPushButton(Dialog)
        self.Poweron.setGeometry(QtCore.QRect(500, 10, 106, 30))
        self.Poweron.setObjectName("Poweron")
        self.poweroff = QtWidgets.QPushButton(Dialog)
        self.poweroff.setGeometry(QtCore.QRect(500, 60, 106, 30))
        self.poweroff.setObjectName("poweroff")
        self.water = QtWidgets.QTextEdit(Dialog)
        self.water.setGeometry(QtCore.QRect(180, 250, 104, 31))
        self.water.setObjectName("water")
        self.label = QtWidgets.QLabel(Dialog)
        self.label.setGeometry(QtCore.QRect(60, 260, 101, 21))
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(Dialog)
        self.label_2.setGeometry(QtCore.QRect(40, 310, 131, 21))
        self.label_2.setObjectName("label_2")
        self.lux = QtWidgets.QTextEdit(Dialog)
        self.lux.setGeometry(QtCore.QRect(180, 300, 104, 31))
        self.lux.setObjectName("lux")
        self.motion = QtWidgets.QTextEdit(Dialog)
        self.motion.setGeometry(QtCore.QRect(180, 350, 104, 31))
        self.motion.setObjectName("motion")
        self.label_3 = QtWidgets.QLabel(Dialog)
        self.label_3.setGeometry(QtCore.QRect(40, 350, 131, 21))
        self.label_3.setObjectName("label_3")
        self.up = QtWidgets.QPushButton(Dialog)
        self.up.setGeometry(QtCore.QRect(420, 200, 106, 30))
        self.up.setObjectName("up")
        self.down = QtWidgets.QPushButton(Dialog)
        self.down.setGeometry(QtCore.QRect(420, 340, 106, 30))
        self.down.setObjectName("down")
        self.right = QtWidgets.QPushButton(Dialog)
        self.right.setGeometry(QtCore.QRect(520, 270, 106, 30))
        self.right.setObjectName("right")
        self.left = QtWidgets.QPushButton(Dialog)
        self.left.setGeometry(QtCore.QRect(330, 270, 106, 30))
        self.left.setObjectName("left")
        self.Auto = QtWidgets.QPushButton(Dialog)
        self.Auto.setGeometry(QtCore.QRect(90, 120, 106, 30))
        self.Auto.setObjectName("Auto")
        self.Manual = QtWidgets.QPushButton(Dialog)
        self.Manual.setGeometry(QtCore.QRect(420, 120, 106, 30))
        self.Manual.setObjectName("Manual")
        self.label_4 = QtWidgets.QLabel(Dialog)
        self.label_4.setGeometry(QtCore.QRect(110, 30, 361, 21))
        font = QtGui.QFont()
        font.setPointSize(14)
        font.setItalic(True)
        self.label_4.setFont(font)
        self.label_4.setObjectName("label_4")
        self.line = QtWidgets.QFrame(Dialog)
        self.line.setGeometry(QtCore.QRect(300, 120, 20, 291))
        self.line.setFrameShape(QtWidgets.QFrame.VLine)
        self.line.setFrameShadow(QtWidgets.QFrame.Sunken)
        self.line.setObjectName("line")

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.Poweron.setText(_translate("Dialog", "Power On"))
        self.poweroff.setText(_translate("Dialog", "Power Off"))
        self.label.setText(_translate("Dialog", "Water level"))
        self.label_2.setText(_translate("Dialog", "Lightness level"))
        self.label_3.setText(_translate("Dialog", "Robot moving"))
        self.up.setText(_translate("Dialog", "up"))
        self.down.setText(_translate("Dialog", "down"))
        self.right.setText(_translate("Dialog", "right"))
        self.left.setText(_translate("Dialog", "left"))
        self.Auto.setText(_translate("Dialog", "Auto"))
        self.Manual.setText(_translate("Dialog", "Manual"))
        self.label_4.setText(_translate("Dialog", "Floor Cleaner - Makes life easier!"))


