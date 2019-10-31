import numpy as np #배열을 쓰기 위해서 numpy를 import해준다. numpy를 짧게 쓰려고 np라고 부르게 해준다.

#Skin_NotSkin.txt를 받아와서 lines에 넣고, 그 배열을 random하게 배열한 후 5개로 나눈다.
file=open("Skin_NotSkin.txt","r")
lines=file.readlines()
npa = np.array(lines)
np.random.shuffle(npa)
lines = list(npa)
idx = int(len(lines)/5)
#print(len(lines), len(lines[idx:]), len(lines[:idx]))
pre = np.zeros(5)
re = np.zeros(5)
ac = np.zeros(5)
f1 = np.zeros(5)
for k in range(0,5) :
    #피부일 떄 또는 아닐 때, RGB 세 가지 색이 어떤 값을 갖는 갯수를 
    #저장할 배열 255개 선언 후 0으로 초기화 
    red1 = np.zeros(256)
    red2 = np.zeros(256)
    green1 = np.zeros(256)
    green2 = np.zeros(256)
    blue1 = np.zeros(256)
    blue2 = np.zeros(256)


    count1 =0
    count2 =0
    #traing data를 피부일 때와 아닐 때를 나눠서 
    #RGB 세 가지 색의 갯수를 더해서 저장한다.
    for line in lines[:idx*k]:
        sline=line.split('\t')
        if sline[3][0]=='1':
            red1[int(sline[0])]+=1.
            green1[int(sline[1])]+=1.
            blue1[int(sline[2])]+=1.
            count1 +=1
        else :
            red2[int(sline[0])]+=1.
            green2[int(sline[1])]+=1.
            blue2[int(sline[2])]+=1.
            count2 +=1
    for line in lines[idx*(k+1):]:
        sline=line.split('\t')
        #print(sline)
        if sline[3][0]=='1':
            red1[int(sline[0])]+=1.
            green1[int(sline[1])]+=1.
            blue1[int(sline[2])]+=1.
            count1 +=1
        else :
            red2[int(sline[0])]+=1.
            green2[int(sline[1])]+=1.
            blue2[int(sline[2])]+=1.
            count2 +=1

    #피부일 때 또는 아닐 때 RGB 각각 어떤 값을 가질 확률을 저장할
    #배열을 255개 선언 후 0으로 초기화
    reds1 = np.zeros(256)
    reds2 = np.zeros(256)
    greens1 = np.zeros(256)
    greens2 = np.zeros(256)
    blues1 = np.zeros(256)
    blues2 = np.zeros(256)

    for i in range(0,256):
        reds1[i] = red1[i]/count1
        reds2[i] = red2[i]/count2
        greens1[i] = green1[i]/count1
        greens2[i] = green2[i]/count2
        blues1[i] = blue1[i]/count1
        blues2[i] = blue2[i]/count2
        
    #print(count1, count2)
    
    #print(red)
    #testing data를 testing 해줌
    tp,fn,fp,tn=0,0,0,0
    for line in lines[idx*k:idx*(k+1)]:
        sline=line.split('\t')
        #result1은 베이지안 분류기 공식에 따라 
        #피부일 때 red가 어떤 값을 가질 확률 * green * blue * 피부일 확률 이고,
        #result2는 피부가 아닐 때의 확률이다.
        #이 때 result1이 result2보다 크면 피부이고, 그 반대면 피부가 아니다.
        result1= reds1[int(sline[0])]*greens1[int(sline[1])]*blues1[int(sline[2])]*(count1/(count1+count2))
        result2= reds2[int(sline[0])]*greens2[int(sline[1])]*blues2[int(sline[2])]*(count2/(count1+count2))
        classify=0
        if result1 >= result2 : 
            classify = 1
        else :
            classify = 2

        #실제 데이터 값과 training을 통해 얻은 위의 classify값을 비교해서
        #tp,fn,fp,tn의 갯수를 센다.
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
    #print(str(tp) + '\n' + str(fn) + '\n' + str(fp)+'\n'+str(tn)) 
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


