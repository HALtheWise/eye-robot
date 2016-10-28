#! /usr/bin/env python2
# from websocket_server import WebsocketServer

import glob
import serial
import time

# Reads characters from stdin
from getch import getch

# Called for every client connecting (after handshake)
# def new_client(client, server):
# 	print("New client connected and was given id %d" % client['id'])
# 	server.send_message_to_all("Hey all, a new client has joined us")


# # Called for every client disconnecting
# def client_left(client, server):
# 	print("Client(%d) disconnected" % client['id'])


# # Called when a client sends a message
# def message_received(client, server, message):
# 	if len(message) > 200:
# 		message = message[:200]+'..'
# 	print("Client(%d) said: %s" % (client['id'], message))


def discoverArduino():
	# This code is taken from http://stackoverflow.com/a/14224477
	ports = glob.glob('/dev/tty[AU][A-Za-z]*')
	print("Discovered ports:", ports)
	if ports:
		ser = serial.Serial(ports[0], 115200)
		time.sleep(1) # Allow the port to initialize
		return ser

def sendArduinoData(ser, forward, turn, pan, tilt):
	ser.write(str(forward))
	ser.write(b',')
	ser.write(str(turn))
	ser.write(b',')
	ser.write(str(pan))
	ser.write(b',')
	ser.write(str(tilt))
	ser.write(b'\n')

	ser.flush()

	print('Sent data: ({}, {}, {}, {})'.format(forward, turn, pan, tilt))

def keyControl(ser):
	startTime = time.time()
	while True: # time.time() < startTime + 30:
		command = getch()

		whattodo = {
		'a':[0,-90],
		' ':[0, 0]
		}

		if command not in whattodo:
			continue

		forward = 0
		turn = 0
		pan = whattodo[command][0]
		tilt = whattodo[command][1]

		sendArduinoData(ser, forward, turn, pan, tilt)


ser = discoverArduino()

sendArduinoData(ser, 0, 0, 20, 30)

keyControl(ser)

# PORT=9001
# server = WebsocketServer(PORT)
# server.set_fn_new_client(new_client)
# server.set_fn_client_left(client_left)
# server.set_fn_message_received(message_received)
# server.run_forever()
