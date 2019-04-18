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
        Dialog.resize(420, 300)
        self.Poweron = QtWidgets.QPushButton(Dialog)
        self.Poweron.setGeometry(QtCore.QRect(50, 20, 106, 30))
        self.Poweron.setObjectName("Poweron")
        self.poweroff = QtWidgets.QPushButton(Dialog)
        self.poweroff.setGeometry(QtCore.QRect(200, 20, 106, 30))
        self.poweroff.setObjectName("poweroff")
        self.water = QtWidgets.QTextEdit(Dialog)
        self.water.setGeometry(QtCore.QRect(200, 70, 104, 31))
        self.water.setObjectName("water")
        self.label = QtWidgets.QLabel(Dialog)
        self.label.setGeometry(QtCore.QRect(50, 80, 101, 21))
        self.label.setObjectName("label")
        self.label_2 = QtWidgets.QLabel(Dialog)
        self.label_2.setGeometry(QtCore.QRect(40, 130, 131, 21))
        self.label_2.setObjectName("label_2")
        self.lux = QtWidgets.QTextEdit(Dialog)
        self.lux.setGeometry(QtCore.QRect(200, 130, 104, 31))
        self.lux.setObjectName("lux")
        self.motion = QtWidgets.QTextEdit(Dialog)
        self.motion.setGeometry(QtCore.QRect(200, 190, 104, 31))
        self.motion.setObjectName("motion")
        self.label_3 = QtWidgets.QLabel(Dialog)
        self.label_3.setGeometry(QtCore.QRect(40, 190, 131, 21))
        self.label_3.setObjectName("label_3")
        self.display = QtWidgets.QPushButton(Dialog)
        self.display.setGeometry(QtCore.QRect(140, 250, 106, 30))
        self.display.setObjectName("display")

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
        self.display.setText(_translate("Dialog", "update"))


