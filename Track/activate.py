#Importing Libraries
import cv2
import mediapipe as mp
import time
import numpy as np
import sys
select = sys.argv[0]

#To implements sockets
from socket import *
ip = input("Enter the IP from Serial Monitor")
#'192.168.137.237'
address = (ip, 5000)  # Bind Address and port, Same as Arduino address and port
client_socket = socket(AF_INET, SOCK_DGRAM)  # Set Up the Socket
client_socket.settimeout(0.01)  # only wait 1 second for a resonse

#Initialize capturing
cap = cv2.VideoCapture(0)

#use hands class
mpHands = mp.solutions.hands
hands = mpHands.Hands() # using default paramaters of 'Hands()'
mpDraw = mp.solutions.drawing_utils #to draw landmarls

while True:
	#A frame is captured
	success, img = cap.read()
	#converting colorspace to RGB
	imgRGB = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
	#process the converted results
	results = hands.process(imgRGB)

	#setup horizantal and diagonal lines
	cv2.line(img, (340, 0), (340, 480), (0, 0, 255), 1)
	cv2.line(img, (0, 240), (640, 240), (0, 255, 255), 1)

	#if palm is detected
	if results.multi_hand_landmarks:
		#for each landmarks
		for handLms in results.multi_hand_landmarks:
			for id, lm in enumerate(handLms.landmark):
				h, w, c = img.shape

				#getting x and y cordinates of the palm
				cx, cy = int(lm.x * w), int(lm.y * h)
				def translate(inpval, in_from, in_to, out_from, out_to):
						out_range = out_to - out_from
						in_range = in_to - in_from
						in_val = inpval - in_from
						val = (float(in_val) / in_range) * out_range
						out_val = out_from + val
						return int(out_val)
				yval = str(translate(cy,0,480,0,64))
				print("y:"+yval)
				data = yval # Set data to Blue Command
				client_socket.sendto(data.encode('utf-8'), address)  # send command to arduino
				try:
					rec_data, addr = client_socket.recvfrom(2048)  # Read response from arduino
					print(rec_data)
					# Print the response from Arduino
				except:
					pass
					
			#draw the lines for palm and fingers
			mpDraw.draw_landmarks(img, handLms, mpHands.HAND_CONNECTIONS)

	#display the captured frame
	cv2.imshow('Image', np.flip(img,axis=1))
	#for exiting the capture
	cv2.waitKey(1)