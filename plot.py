import pyqtgraph as pg
from pyqtgraph.Qt import QtWidgets
import socket 

host = "10.197.211.196"
port = 1234

MAX_POINTS = 300
app = QtWidgets.QApplication([])

window = pg.GraphicsLayoutWidget(show=True)
window.setWindowTitle("live angle data")

plot = window.addPlot(title='angle')
plot.setLabel('left', 'angle')
plot.setLabel('bottom', 'time')


curve = plot.plot()

sock = socket.socket()
sock.connect((host, port))

angle = []
time = []

buff = "" 

def update():
	global angle, time, buff
	buff += sock.recv(1024).decode()
	
	while "\n" in buff:
		line, buff = buff.split("\n", 1)
		a, t = line.split(",")
		time.append(float(t))
		angle.append(float(a))
		if len(angle) > 300:
			del angle[:-300]
			
		if len(time) > 300:
			del time[:-300]
			
		curve.setData(time, angle)
		
timer = pg.QtCore.QTimer()
timer.timeout.connect(update)

timer.start(20)

app.exec()
        
        

        
