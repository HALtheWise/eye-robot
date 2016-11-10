#! /usr/bin/env python2
from websocket_server import WebsocketServer

import glob
import serial
import time

# Reads characters from stdin
from getch import getch

def discoverArduino():
	# This code is taken from http://stackoverflow.com/a/14224477
	ports = glob.glob('/dev/tty[AU][CS][A-Za-z]*')
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

	forward = turn = pan = tilt = 0

	while True: # time.time() < startTime + 30:
		command = getch()

		if command == '\x03':
			# Ctrl-C cancels
			return

		driveCommands = {
		'w':[1, 0],
		's':[-1,0],
		'a':[0,-1],
		'd':[0, 1],
		' ':[0, 0],
		}

		if command in driveCommands:
			cmd = driveCommands[command]
			forward = cmd[0]
			turn = cmd[1]

		turnCommands = {
		'j':[-10, 0],
		'l':[10,  0],
		',':[0, -10],
		'm':[0, -10],
		'i':[0,  10]
		}

		if command in turnCommands:
			cmd = turnCommands[command]
			pan += cmd[0]
			tilt += cmd[1]

		sendArduinoData(ser, forward, turn, pan, tilt)


ser = discoverArduino()

# keyControl(ser)

# Called for every client connecting (after handshake)
def new_client(client, server):
	print("New client connected and was given id %d" % client['id'])
	# server.send_message_to_all("Hey all, a new client has joined us")


# Called for every client disconnecting
def client_left(client, server):
	print("Client(%d) disconnected" % client['id'])


# Called when a client sends a message
def message_received(client, server, message):
	# if len(message) > 200:
	# 	message = message[:200]+'..'
	print("Client(%d) said: %s" % (client['id'], message))

	if not ser:
		server.send_message(client, 'Arduino not connected')
		return

	parts = map(float, message.split(','))
	if len(parts) == 4:
		sendArduinoData(ser, *parts)
	else:
		print 'Recieved message has wrong length:', message
		server.send_message(client, 'Recieved message has wrong length:' + message)


PORT=9001
server = WebsocketServer(PORT)
server.set_fn_new_client(new_client)
server.set_fn_client_left(client_left)
server.set_fn_message_received(message_received)
server.run_forever()
