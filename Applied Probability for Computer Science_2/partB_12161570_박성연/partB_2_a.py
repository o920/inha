import numpy as np 
import math

#parameter를 확률을 알고싶은 point와 평균과 표준편차로 갖는
#normpdf 함수를 만든다.
#가우시안 분포에서 그 위치의 확률 알아내는 함수를 만든다.
def normpdf(size, mean, st):
    var = float(st)**2
    pi = 3.1415926
    denom = (2*pi*var)**.5
    num = math.exp(-(float(size)-float(mean))**2/(2*var)) 
    return num/denom
file=open("Skin_NotSkin.txt","r") 
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

for k in range(0,5) :
    #평균 = 전체 합 / 전체 갯수
    #분산 = 제곱의 평균 - 평균의 제곱
    for line in lines[:idx*k] :
        sline=line.split('\t')
        if sline[3][0]=='1':
            red1_av += int(sline[0][0])
            red1_var += int(sline[0][0])**2
            count1 += 1
        else :
            red2_av += int(sline[0][0])
            red2_var += int(sline[0][0])**2
            count2 += 1
    for line in lines[idx*(k+1):] :
        sline=line.split('\t')
        if sline[3][0]=='1':
            red1_av += int(sline[0][0])
            red1_var += int(sline[0][0])**2
            count1 += 1
        else :
            red2_av += int(sline[0][0])
            red2_var += int(sline[0][0])**2
            count2 += 1


    #training data는 피부일 때~N(red1_av, red1_var)
    #아닐 때~N(red2_av,red2_var)
    red1_av /= count1
    red2_av /= count2
    red1_var /= count1 
    red1_var = red1_var - red1_av**2
    red2_var /= count2
    red2_var = red2_var - red2_av**2

    #print(str(red1_av)+' '+str(red1_var)+' '+str(red2_av)+' '+str(red2_var)+'\n')


    red1, red2, classify = 0.,0.,0
    tp,fn,fp,tn=0,0,0,0
    for line in lines[idx*k:idx*(k+1)] :
        sline=line.split('\t')
        #피부일 때 또는 아닐 때 평균과 분산 그리고 그때의 red값을 parameter로 넣어서
        #가우시안 분포에서 그 red값이 피부일 때 또는 아닐 때의 확률을 구한다.
        if sline[3][0]=='1':
            red1 = normpdf(int(sline[0][0]),red1_av,red1_var**0.5)
        elif sline[3][0] =='2' :
            red2 = normpdf(int(sline[0][0]),red2_av,red2_var**0.5)


        #피부일 때 또는 아닐 때의 확률을 비교하고 더 큰 쪽으로 결정한다.
        #그리고 tp,fn,fp,tn의 갯수를 센다.
        if red1 >= red2 :
            classify = 1
        else :
            classify = 2
        #print(str(red1) + '\n' + str(red2) + '\n' + str(classify)+'\n')
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

        



        
