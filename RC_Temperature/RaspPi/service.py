import serial
import socket  
import signal  
import errno 
from time import sleep    
import re

def HttpResponse(header,whtml):  
    f = file(whtml)  
    contxtlist = f.readlines()  
    context = ''.join(contxtlist)  
    response = "%s%s" % (header,context)  
    return response  
  
def sigIntHander(signo,frame):  
    print 'get signo# ',signo  
    global runflag  
    runflag = False  
    global lisfd  
    lisfd.shutdown(socket.SHUT_RD)  

def get_temp():
	file = open("config","r");
	temp_h = file.read()
	file.close()
	return temp_h

def set_temp(temp_h):
	file = open("config","w");
	file.write(temp_h);
	file.flush()
	file.close()

httpheader = '''
HTTP/1.1 200 OK 
Context-Type: text/html 
Server: RJ version 0.1
Content-Type: text/html\r\n\r\n'''  

strHost = "0.0.0.0"  
HOST = strHost #socket.inet_pton(socket.AF_INET,strHost)  
PORT = 9999 
ADDR = ( HOST, PORT )
  
lisfd = socket.socket(socket.AF_INET,socket.SOCK_STREAM)  
lisfd.bind(ADDR)
lisfd.listen(2)
  
signal.signal(signal.SIGINT,sigIntHander)  

temp_data = ''
recv_string = ''
# open serial 
ser = serial.Serial("/dev/ttyAMA0", 115200)  
def main():  
    global recv_string, temp_data
    while True:
        recv_fd = ser.inWaiting()  # wait recv
        if recv_fd != 0:
			recv_string = ''
			recv_string = ser.read(recv_fd)
			if '\r\n' in recv_string:
				temp_data = recv_string.split("\r\n", 1)[0]
        ser.flushInput()  # clean buffer
        sleep(0.1)  # necessary delay  
        try:
            confd,addr = lisfd.accept()  
        except socket.error as e:  
            if e.errno == errno.EINTR:  
                print 'get a except EINTR'  
            else:  
                raise  
            continue
        #print "connect by ",addr  
        data = confd.recv(1024)  
        print data
        if "GET /index.html" in data or "GET / HTTP/1.1" in data:
            confd.send( HttpResponse(httpheader, "index.html") )
        if "/?action=get_val" in data:
            confd.send( "%s%s\r\n" % (httpheader, temp_data) )
        if "GET /alarm.mp3" in data:
            confd.send( HttpResponse(httpheader, "alarm.mp3") )
        if "GET /?action=get_temp_h" in data:
            confd.send( "%s%s\r\n" % (httpheader, get_temp()) )
        if "POST /?set_temp_h=" in data:
			reg = re.compile(r"(?<==)\d+")
			temp_h = reg.search(data)
			set_temp( temp_h.group(0) )
			#print "\r\n Recv:" + temp_h.group(0) + "\r\n"
			confd.send( "%s%s\r\n" % (httpheader, "set ok") )
        confd.close()
if __name__ == '__main__':  
    try:  
        main()  
    except KeyboardInterrupt:  
        if ser != None:  
            ser.close() 
