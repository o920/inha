import math
import numpy as np 

f = open("output_2_c.txt","r")
lines = f.readlines()
p = np.zeros(26)
count = 0
for i in lines : 
    line = i.split('\n')
    if float(line[0])==0.000 : p[0] += 1 
    elif float(line[0])==0.001 : p[1] += 1 
    elif float(line[0])==0.002 : p[2] += 1 
    elif float(line[0])==0.003 : p[3] += 1
    elif float(line[0])==0.004 : p[4] += 1
    elif float(line[0])==0.005 : p[5] += 1
    elif float(line[0])==0.006 : p[6] += 1
    elif float(line[0])==0.007 : p[7] += 1
    elif float(line[0])==0.008 : p[8] += 1
    elif float(line[0])==0.009 : p[9] += 1
    elif float(line[0])==0.010 : p[10] += 1
    elif float(line[0])==0.011 : p[11] += 1
    elif float(line[0])==0.012 : p[12] += 1
    elif float(line[0])==0.013 : p[13] += 1
    elif float(line[0])==0.014 : p[14] += 1
    elif float(line[0])==0.015 : p[15] += 1
    elif float(line[0])==0.016 : p[16] += 1
    elif float(line[0])==0.017 : p[17] += 1
    elif float(line[0])==0.018 : p[18] += 1
    elif float(line[0])==0.019 : p[19] += 1
    elif float(line[0])==0.020 : p[20] += 1
    elif float(line[0])==0.021 : p[21] += 1
    elif float(line[0])==0.022 : p[22] += 1
    elif float(line[0])==0.023 : p[23] += 1
    elif float(line[0])==0.024 : p[24] += 1
    elif float(line[0])==0.025 : p[25] += 1
    count += 1

for i in range(0,26) :
    print("P(X = 0.0"+'%02d'%i+") ="+str(p[i]/count)+"\t")
f=open("ouput_2_d_2.txt","w")
for i in range(0,26) :
    f.write(str(p[i]/count)+"\n")
f.close()
f=open("output_2_d.txt","w")
a = np.random.poisson(10,1000)
for i in range(0,1000) : 
    f.write(str(a[i])+"\n")
f.close()



