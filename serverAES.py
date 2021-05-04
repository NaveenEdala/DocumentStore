from Crypto.Cipher import AES
import os
import tkinter as tk
import socket
import subprocess

def wlan_ip():
    result=subprocess.run('ipconfig',stdout=subprocess.PIPE,text=True).stdout.lower()
    scan=0
    for i in result.split('\n'):
        if 'wi-fi' in i: scan=1
        if scan:
            if 'ipv4' in i: return i.split(':')[1].strip()

def decryption(ciphertext):
    decryptor = AES.new(key, AES.MODE_CTR, counter=lambda: counter)
    return decryptor.decrypt(ciphertext)


window = tk.Tk()
window.geometry("800x400")
window.title("Server Endpoint for AES communication")

inputFrame = tk.Frame(master = window, relief=tk.RIDGE, borderwidth = 3)
outputFrame = tk.Frame(master = window, relief=tk.FLAT, borderwidth = 5)

welcomeMessage = tk.Label(text = "Hello! This is a Graphical Interface to use encrypted TCP socket communication via AES. You are running the **server** endpoint.", height = 3)
welcomeMessage.pack()

portnum = tk.IntVar(value=1024)

portLabel = tk.Label(master = inputFrame, text = "Port number to launch server on (1024 - 65535): ")
portEntry = tk.Entry(master = inputFrame, textvariable = portnum)

outputMessage = tk.Label(master = outputFrame, text="")

outputMessage.pack()

port = portnum.get()
counter = "SIXTEENCHARACTER"
key = "SIXTEENCHARACTER"*2
local_ip = wlan_ip()

def beginServer():
    outputMessage["text"] += "\n Listening for connections..."
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((local_ip, port))
        s.listen()
        conn, addr = s.accept()
        with conn:
            outputMessage["text"] += "\n Incoming connection from " + str(addr)
            while True:
                data = conn.recv(1024)
                if not data:
                    break
                outputMessage["text"] += str(decryption(data))

portSubmit = tk.Button(master = inputFrame, text = "Begin server", command = beginServer)

portLabel.pack()
portEntry.pack()
portSubmit.pack()


inputFrame.pack()
outputFrame.pack(pady = 15)
window.mainloop()