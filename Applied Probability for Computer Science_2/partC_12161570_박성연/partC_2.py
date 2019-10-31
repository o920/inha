import numpy as np 
import math
def normpdf(x, mean, sd):
    var = float(sd)**2
    pi = 3.1415926
    denom = (2*pi*var)**.5
    num = math.exp(-(float(x)-float(mean))**2/(2*var))
    return num/denom

file=open("output.txt","r") 
lines=file. readlines() 
npa = np.array(lines) 
np.random.shuffle(npa)
lines = list(npa)
idx = int(len(lines)/5)

pre = np.zeros(5)
re = np.zeros(5)
ac= np.zeros(5)
f1=np.zeros(5)

count1, count2=0,0
red1_av, red2_av, red1_var, red2_var =0.,0.,0.,0.
green1_av, green2_av, green1_var, green2_var =0.,0.,0.,0.
blue1_av, blue2_av, blue1_var, blue2_var =0.,0.,0.,0.

for k in range(0,5) :
    for line in lines[:idx*k] :
        sline=line.split('\t')
        if sline[3][0]=='1':
            blue1_av += int(sline[0][0])
            blue1_var += int(sline[0][0])**2
            green1_av += int(sline[1][0])
            green1_var += int(sline[1][0])**2
            red1_av += int(sline[2][0])
            red1_var += int(sline[2][0])**2
            count1 += 1
        else :
            blue2_av += int(sline[0][0])
            blue2_var += int(sline[0][0])**2
            green2_av += int(sline[1][0])
            green2_var += int(sline[1][0])**2
            red2_av += int(sline[2][0])
            red2_var += int(sline[2][0])**2
            count2 += 1
    for line in lines[idx*(k+1):] :
        sline=line.split('\t')
        if sline[3][0]=='1':
            blue1_av += int(sline[0][0])
            blue1_var += int(sline[0][0])**2
            green1_av += int(sline[1][0])
            green1_var += int(sline[1][0])**2
            red1_av += int(sline[2][0])
            red1_var += int(sline[2][0])**2
            count1 += 1
        else :
            blue2_av += int(sline[0][0])
            blue2_var += int(sline[0][0])**2
            green2_av += int(sline[1][0])
            green2_var += int(sline[1][0])**2
            red2_av += int(sline[2][0])
            red2_var += int(sline[2][0])**2
            count2 += 1

    red1_av /= count1
    red2_av /= count2
    red1_var /= count1 
    red1_var = red1_var - red1_av**2
    red2_var /= count2
    red2_var = red2_var - red2_av**2

    green1_av /= count1
    green2_av /= count2
    green1_var /= count1 
    green1_var = green1_var - green1_av**2
    green2_var /= count2
    green2_var = green2_var - green2_av**2

    blue1_av /= count1
    blue2_av /= count2
    blue1_var /= count1 
    blue1_var = blue1_var - blue1_av**2
    blue2_var /= count2
    blue2_var = blue2_var - blue2_av**2

    red1, red2, green1, green2, blue1, blue2, classify = 0.,0.,0.,0.,0.,0.,0
    tp,fn,fp,tn=0,0,0,0
    for line in lines[idx*k:idx*(k+1)] :
        #피부일 때 또는 아닐 때 RGB 각각의 평균과 분선 그리고 red green blue의 특정값을
        #parameter로 넣어서 RGB 각각의 가우시안 함수에서 그 특정 값의 확률을 구한다.
        sline=line.split('\t')
        if sline[3][0]=='1':
            red1 = normpdf(int(sline[0][0]),red1_av,red1_var**0.5)
            green1 = normpdf(int(sline[1][0]),green1_av,green1_var**0.5)
            blue1 = normpdf(int(sline[2][0]),blue1_av,blue1_var**0.5)
        elif sline[3][0] =='2' :
            red2 = normpdf(int(sline[0][0]),red2_av,red2_var**0.5)
            green2 = normpdf(int(sline[1][0]),green2_av,green2_var**0.5)
            blue2 = normpdf(int(sline[2][0]),blue2_av,blue2_var**0.5)


        
        if (red1*green1*blue1*(count1/(count1+count2))) >= (red2*green2*blue2*(count2/(count1+count2))) :
            classify = 1
        else :
            classify = 2

        if sline[3][0]=='1':
            if classify==1 :
                tp+=1
            if classify==2 :
                fn+=1
        else :
            if classify==1 :
                fp+=1
            if classify==2 :
                tn+=1

    pre[k] = tp/(tp+fp)
    re[k] = tp/(tp+fn)
    ac[k] = (tp+tn)/(tp+fp+fn+tn)
    f1[k]=2*pre[k]*re[k]/(pre[k]+re[k])

precision,recall,accuracy,f1_ex=0.,0.,0.,0.
for i in range(0,4) :
    precision += pre[i]
    recall += re[i]
    accuracy += ac[i]
    f1_ex += f1[i]

precision /= 5
recall /= 5
accuracy /= 5
f1_ex /= 5

print(str(precision) + '\n' + str(recall) + '\n' + str(accuracy)+'\n'+str(f1_ex))