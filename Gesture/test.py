# -*- coding: utf-8 -*-
"""
@author: sain saji
"""
#importing libraries
import cv2
import os
import time
import handTrackingModule as htm
import sys
from datetime import datetime


# To implements sockets and socket initialization
from socket import *
ip = input("Enter the IP from Serial Monitor")
# '192.168.137.237'
address = (ip, 5000)  # Bind Address and port, Same as Arduino address and port
client_socket = socket(AF_INET, SOCK_DGRAM)  # Set Up the Socket
client_socket.settimeout(0.07)  # only wait 1 second for a resonse


#setting numbers for each gestures
def getNumber(ar):
    s = ""
    for i in ar:
        s += str(ar[i]);

    if(s == "00000"):
        return (0)
    elif(s == "01000"):
        return(1)
    elif(s == "01100"):
        return(2)
    elif(s == "01110"):
        return(3)
    elif(s == "01111"):
        return(4)
    elif(s == "11111"):
        return(5)
    elif(s == "01001"):
        return(6)
    elif(s == "01011"):
        return(7)


#webcam initialization
wcam, hcam = 640, 480
cap = cv2.VideoCapture(0)
cap.set(3, wcam)
cap.set(4, hcam)
pTime = 0
detector = htm.handDetector(detectionCon=0.75)

#capture current time before starting detection
now = datetime.now()

#start capture and detection
while True:
    # capture time after each frame
    later = datetime.now()
    difference = (later - now).total_seconds()

    #if difference is above 60 seconds stop capture and detection
    if(difference>60):
        data = "STOP"
        client_socket.sendto(data.encode('utf-8'), address)
        break

    #capture cordinates
    success, img = cap.read()
    img = detector.findHands(img, draw=True)
    lmList = detector.findPosition(img, draw=False)
    # print(lmList)
    tipId = [4, 8, 12, 16, 20]
    if(len(lmList) != 0):
        fingers = []
        # thumb
        if(lmList[tipId[0]][1] > lmList[tipId[0]-1][1]):
            fingers.append(1)
        else:
            fingers.append(0)
        # 4 fingers
        for id in range(1, len(tipId)):

            if(lmList[tipId[id]][2] < lmList[tipId[id]-2][2]):
                fingers.append(1)

            else:
                fingers.append(0)

        #draw landmarks,text for each gestures
        cv2.rectangle(img, (20, 255), (170, 425), (0, 255, 0), cv2.FILLED)
        cv2.putText(img, str(getNumber(fingers)), (45, 375), cv2.FONT_HERSHEY_PLAIN,
                                     10, (255, 0, 0), 20)
        
        #get the number for gesture
        print(getNumber(fingers))
        data = str(getNumber(fingers))  # Set data to Blue Command
        # send command to arduino
        client_socket.sendto(data.encode('utf-8'), address)  # send command to arduino
        try:
            rec_data, addr = client_socket.recvfrom(2048)  # Read response from arduino
            print(rec_data)
        except:
            pass
            


    #to calcualate fps
    cTime = time.time()
    fps = 1/(cTime-pTime)
    pTime = cTime

    #to display fps
    cv2.putText(img, f'FPS: {int(fps)}', (400,70),cv2.FONT_HERSHEY_COMPLEX,1,(255,255,0),3)
    cv2.imshow("image", img)
    if(cv2.waitKey(1) & 0xFF == ord('q')):
        break
