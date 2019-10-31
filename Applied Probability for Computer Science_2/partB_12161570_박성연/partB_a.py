import numpy as np #배열을 쓰기 위해서 numpy를 import해준다. numpy를 짧게 쓰려고 np라고 부르게 해준다.

#Skin_NotSkin.txt를 받아와서 lines에 넣고, 그 배열을 random하게 배열한 후 5개로 나눈다.
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


for k in range(0,5) :
    #skin일 때와 아닐 때를 나눠서 red가 특정 값(0~255)일 때 
    #피부인 값의 갯수와 아닌 값의 갯수를 저장할 배열을 선언한다.
    red1 = np.zeros(256)
    red2 = np.zeros(256)

    count1 =0
    count2 =0

    #lines(Skin_NotSkin의 데이터) 5분의 2번째 인덱스부터 끝까지 for문으로
    #피부인 데이터의 갯수와 아닌 데이터의 갯수를 세고, 그 red 값의 수를 하나 더해준다.
    for line in lines[:idx*k]:
        sline=line.split('\t')
        #print(sline)
        if sline[3][0]=='1':
            red1[int(sline[0])]+=1.
            count1 +=1
        else :
            red2[int(sline[0])]+=1.
            count2 +=1
    for line in lines[idx*(k+1):]:
        sline=line.split('\t')
        #print(sline)
        if sline[3][0]=='1':
            red1[int(sline[0])]+=1.
            count1 +=1
        else :
            red2[int(sline[0])]+=1.
            count2 +=1

    #피부일 때 red가 어떤 값일 확률과 피부가 아닐 때 
    #red가 어떤 값일 확률을 저장할 reds1,2 배열을 만든다.
    reds1 = np.zeros(256)
    reds2 = np.zeros(256)

    for i in range(0,256):
        reds1[i] = red1[i]/count1
        reds2[i] = red2[i]/count2

    #red가 어떤 값일 때, 1일 때 red일 확률과 2일 때 red일 확률을 비교해서, 
    #그 값이 피부인지 아닌지를 결정해서 저장할 배열 redclass를 만든다.
    redclass = np.zeros(256)
    for i in range(0,256):
        if reds1[i]>=reds2[i] : 
            redclass[i] = 1
        else :
            redclass[i] = 2

    #그 데이터를 output.txt에 출력한다.
    outputFile = open('output.txt','w')
    outputFile.write(str(redclass)+'\n')

    # output.txt를 읽기 모드로 불러온다.
    file=open("output.txt","r")
    tp,fn,fp,tn=0,0,0,0
    #testing data를 대입해서 tp, fn, fp, tn의 갯수를 센다.
    for line in lines[idx*k:idx*(k+1)]:  
        sline=line.split('\t')
        if int(sline[3])==1:
            if redclass[int(sline[0])]==1 :
                tp+=1
            if redclass[int(sline[0])]==2 :
                fn+=1
        else :
            if redclass[int(sline[0])]==1 :
                fp+=1
            if redclass[int(sline[0])]==2 :
                tn+=1
    #precision, recall, accuaracy, f1을 구해서 배열에 넣어준다.
    pre[k] = tp/(tp+fp)
    re[k] = tp/(tp+fn)
    ac[k] = (tp+tn)/(tp+fp+fn+tn)
    f1[k]=2*pre[k]*re[k]/(pre[k]+re[k])
    #print(str(pre[k]) + '\n' + str(re[k]) + '\n' + str(ac[k])+'\n'+str(f1[k]))

#다섯개의 precision, recall, accuracy, f1의 평균을 내서 출력한다.
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




