import cv2 #open CV import

def func(i,j,num,minor,major,center_x,center_y):
    for l in range(0,num):
        if ((i-float(center_x[l]))**2)/(float(minor[l])**2)+((j-float(center_y[l]))**2)/(float(major[l])**2) <=1 :
            return 1
    return 2
    
f = open("output.txt", "w")
for k in range(1,11) :
    
    file = open("FDDB-fold-"+'%02d'%k +"-ellipseList.txt","r") #파일 열기~

    while True :
        imgfile01 = file.readline() #받아오는 변수

        imgfile01 = imgfile01[:-1] #엔터를 지워주고
        imgfile01 = imgfile01.replace("/","\\")+".jpg" #/를 \로 바꿔줌 (경로)
        img = cv2.imread(imgfile01)
        if img is None:
            break
        print(imgfile01)
        num = int(file.readline())
        
        major = []
        minor = []
        center_x = []
        center_y = []
        for i in range(0,num) :
            data = file.readline()
            datas = data.split(' ')
            print(datas)
            draw = cv2.ellipse(img, (int(float(datas[3])), int(float(datas[4]))), (int(float(datas[1])),int(float(datas[0]))), float(datas[2]),0,360, (255,0,0), 1)
            width = int(img.shape[1])
            height = int(img.shape[0])
            center_x.append(datas[3])
            center_y.append(datas[4])
            major.append(datas[0])
            minor.append(datas[1])
        for i in range(0,height) :
            for j in range(0,width) :
                detection = func(i,j,num,minor,major,center_x,center_y)
                f.write(str(img.item(i,j,2))+'\t'+str(img.item(i,j,1))+'\t'+str(img.item(i,j,0))+'\t' + str(detection) +'\n')
    
f.close()