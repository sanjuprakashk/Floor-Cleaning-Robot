import socket

HOST = '10.0.0.199'
PORT = 8003

with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
		#s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
		
		s.connect((HOST,PORT))
	
		s.sendall(b'display')

		lux_value = s.recv(10)
		print("lux value ", lux_value)
		#self.ui.lux.setText((lux_value))

		motion_value = s.recv(10)
		print("motion_value ", motion_value)
		#self.ui.motion.setText((motion_value))

		water_value = s.recv(10)
		print("water_value ", water_value)
		#self.ui.water.setText((water_value))





