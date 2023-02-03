import serial
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import numpy as np
import datetime as dt

fig = plt.figure()
voltage = []
time = []

"""
f = open("ecg_tony.txt","r")
next(f)
for line in f:
	p = line.split()
	time.append(float(p[0]))
	voltage.append(float(p[1]))
"""

ser = serial.Serial('COM4',9600)

def realtime_graph(i):
	data = int(str(ser.readline()).replace('b\'','').replace('\\r\\n\'',''))
	time.append(dt.datetime.now().strftime('%S.%f%f%f%f'))
	value = (data*5.0)/1023
	voltage.append(float(value))
	print(data)

	plt.cla()
	plt.ylim([0,5])
	plt.xlim([0,100])
	

ani = animation.FuncAnimation(plt.gcf(),realtime_graph,interval=10)


plt.plot(time,voltage)
plt.tight_layout()
plt.show()



