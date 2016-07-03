import socket
from struct import *

histo = {}

def main():
    UDP_IP = "127.0.0.1"
    UDP_PORT = 19000
    
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM) 
    sock.bind((UDP_IP, UDP_PORT))
    
    bucketList = {}
    for i in range(0,51):
        bucketList[i] = {'time': 0, 'count': 0}
    
    while True:
        data, addr = sock.recvfrom(1024) 
        timePacket = unpack('II',data)
        histoId = timePacket[0]
        timeTaken = timePacket[1]
    
        bucket = 50
        if timeTaken < 100:
            bucket = timeTaken / 5
        elif timeTaken < 1000:
            bucket = (timeTaken / 100) + 20
        elif timeTaken <= 1000000:
            timeTaken /= 1000
            bucket = (timeTaken / 50) + 30
        else:
            timeTaken /= 1000000
    
        if not histoId in histo:
            histo[histoId] = bucketList
            histo[histoId]['totalCount'] = 0
    
        histo[histoId][bucket]['time'] += timeTaken
        histo[histoId][bucket]['count'] += 1
        histo[histoId]['totalCount'] += 1
        printHisto()


def printHisto():
    usString = "{:3d} -{:4d}us: {:2d}% {:5d} {:5.1f}us"
    msString = usString.replace('us', 'ms')
    sString = "  1s -   inf: {:2d}% {:5d} {:5.1f}ms"
    
    for h in histo:
        print("Histogram {:d}:   % Total AvgTime".format(h))
        for b in range(0,20):
            count = histo[h][b]['count']
            if count > 0:
                print(usString.format( (b*5)+1, (b+1)*5, count * 100 / histo[h]['totalCount'], count, float(histo[h][b]['time']) / float(count)))
                #print("{:>3d} - {:>3d}us: {:>2d} \n".format((b*5)+1, (b+1)*5, count * 100 ))

	x = 0
	for b in range(20, 30):
            count = histo[h][b]['count']
            if count > 0:
		print(usString.format(x+1, x+100, count * 100 / histo[h]['totalCount'], count, float(histo[h][b]['time']) / float(count)))
	    x+=100

	x = 0
	for b in range(30, 50):
            count = histo[h][b]['count']
            if count > 0:
		print(msString.format(x+1, x+50, count * 100 / histo[h]['totalCount'], count, float(histo[h][b]['time']) / float(count)))
	    x += 50
		
        count = histo[h][50]['count']
        if count > 0:
    	    print(sString.format(count * 100 / histo[h]['totalCount'], count, float(histo[h][b]['time']) / float(count)))
        print("---\n")
        #print("Histogram {:d}:   % Total AvgTime".format(histo[h]['totalCount']))


if __name__ == '__main__':
        main()
