from socket import *
import time
ip = input("Enter the IP from Serial Monitor")
#'192.168.137.237'
address = (ip, 5000)  # Bind Address and port, Same as Arduino address and port
client_socket = socket(AF_INET, SOCK_DGRAM)  # Set Up the Socket
client_socket.settimeout(1)  # only wait 1 second for a resonse

while (1):  # Main Loop

    data = "Blue" # Set data to Blue Command
    client_socket.sendto(data.encode('utf-8'), address)  # send command to arduino
    try:
        rec_data, addr = client_socket.recvfrom(2048)  # Read response from arduino
        print(rec_data)
         # Print the response from Arduino
    except:
        pass

    time.sleep(0) # delay before sending next command
