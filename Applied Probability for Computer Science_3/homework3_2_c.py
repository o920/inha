import math
import numpy as np 

f=open("output_2_c.txt","w")
n=1000
p=0.01
for i in range(0,1025) :
    s = sum(np.random.binomial(1,p,n)==1)/n
    f.write(str(s)+"\n")
f.close()