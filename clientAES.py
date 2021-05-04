from Crypto.Cipher import AES
import os
import tkinter as tk
import socket

counter = "SIXTEENCHARACTER"
key = "SIXTEENCHARACTER"*2
def encryption(ciphertext):
    encryptor = AES.new(key, AES.MODE_CTR, counter=lambda: counter)
    return encryptor.encrypt(ciphertext)

window = tk.Tk()
window.geometry("800x400")
window.title("Client Endpoint for AES communication")

inputFrame = tk.Frame(master = window, relief=tk.RIDGE, borderwidth = 3)
outputFrame = tk.Frame(master = window, relief=tk.FLAT, borderwidth = 5)

welcomeMessage = tk.Label(text = "Hello! This is a Graphical Interface to use encrypted TCP socket communication via AES. You are running the **client** endpoint.", height = 3)
welcomeMessage.pack()

ipStore = tk.StringVar(value="192.168.0.1")
ipLabel = tk.Label(master = inputFrame, text = "IP address of server to connect: ")
ipEntry = tk.Entry(master = inputFrame, textvariable = ipStore)

portStore = tk.IntVar(value=1024)
portLabel = tk.Label(master = inputFrame, text = "Port number to connect on server (1024 - 65535): ")
portEntry = tk.Entry(master = inputFrame, textvariable = portStore)

messageStore = tk.StringVar(value="sample")
messageLabel = tk.Label(master = inputFrame, text = "Enter your message here: ")
messageEntry = tk.Entry(master = inputFrame, textvariable = messageStore)

outputMessage = tk.Label(master = outputFrame, text="")
outputMessage.pack()

def beginClient():
    serverip = ipStore.get()
    port = portStore.get()
    message = encryption(messageStore.get())
    outputMessage["text"] += "\n Listening for connections..."
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((serverip, port))
        s.sendall(message)
    

totalSubmit = tk.Button(master = inputFrame, text = "Connect to server", command = beginClient)

ipLabel.pack()
ipEntry.pack()
portLabel.pack()
portEntry.pack()
messageLabel.pack()
messageEntry.pack()
totalSubmit.pack()

inputFrame.pack()
outputFrame.pack(pady = 15)
window.mainloop()