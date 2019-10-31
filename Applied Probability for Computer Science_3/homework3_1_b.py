import random

def head_P() :
    front = 0
    for i in range(0,1000):
        result = random.randrange(0,10)
        if(result < 6) :
            front += 1
    return (front / 1000)


f=open("output_b.txt","w")
for i in range(1,1001):
    
    f.write(str(head_P())+"\n")

f.close()