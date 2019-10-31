import math
import numpy as np

f = open("output_2_a.txt","r")
lines = f.readlines()
count = 0 
sum = 0
for i in lines : 
    line = i.split("\n")
    count += 1
    sum += float(line[0])

print(sum/count)