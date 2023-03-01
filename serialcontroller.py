#Import Libraries
from PIL import Image
import matplotlib.pyplot as plt
import numpy
import serial
import time

def printarr():
       print('{')
       for i in numpy.asarray(small_img):
              print('{',end='')
              k=0
              for j in i:
                     k=k+1
                     print(int(not(j)),end='')
                     if(k<32):print(',',end='')
              print('},')
       print('};')        

#Read image
img=Image.open('img.bmp').convert('1')

#show image (debug)
#plt.imshow(img)
#plt.show()

small_img=img.resize((32,32),Image.Resampling.BILINEAR)          

#resize
o_size=(100,100) #output size
res=small_img.resize(o_size,Image.NEAREST)

#printarr()

#display image
print(">image will be similar to the plot! ")
plt.imshow(res)
plt.show()

#send to printer
print(">Initializing the print! ")

ser = serial.Serial('COM6', 9800, timeout=1)
time.sleep(0.02)

while (ser.readline() == b''):pass

for i in numpy.asarray(small_img):
       v   = 0
       p = 7
       for j in i:      
              v=v + int(not(j))*pow(2,p) 
              p=p-1
              if (p == -1):
                     p = 7
                     ser.write(v.to_bytes())
                     time.sleep(0.01)
                     v=0

       

print("done sending data")
ser.close()
