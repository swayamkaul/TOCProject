import numpy as np
from PIL import  Image

img = Image.open("img1.jpeg")
data = np.array(img)

shape = (len(data),len(data[0]),3)

r = data[:,:,0].flatten()
g = data[:,:,1].flatten()
b = data[:,:,2].flatten()

t0, t1, t2 = open("tape0","w"), open("tape1","w"), open("tape2","w")
for i in r:
    t0.write("{0:024b} ".format(i))

for i in g:
    t1.write("{0:024b} ".format(i))

for i in b:
    t2.write("{0:024b} ".format(i))
t0.close()
t1.close()
t2.close()