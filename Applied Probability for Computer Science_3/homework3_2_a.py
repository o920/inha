import math
import numpy as np 

def ncr(n, r) :
    if r in (0, n):
        return 1
    return ncr(n-1, r) + ncr(n-1,r-1)

def binomial(n,p):
    q = 1- p
    for i in range(n+1) : 
        dist = np.array(ncr(n,i)*(p**i)*(q**(n-i)))
    return dist

f=open("output_2_a.txt","w")
n=10
p=0.5
for i in range(0,1025) :
    s = sum(np.random.binomial(1,p,n)==1)/n
    f.write(str(s)+"\n")
f.close()