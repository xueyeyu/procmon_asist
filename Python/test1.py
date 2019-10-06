import pandas as pd
import csv

#定义跟踪程序名
Pname=input('程序名：')
#统一转换成小写，便于后面的比较
Pname=Pname.lower()
print(Pname) 


#csv读取
#csv_file = csv.reader(open(r'C:/Users/Public/Desktop/log.csv','r',errors='ignore')) #使用csv.reader读取csvfile中的文件,r为只读
#csv_file=pd.read_csv(r'C:/Users/Public/Desktop/log.csv')
csv_file=pd.read_csv(r'E:\github\procmon_asist\test_program\Log.csv')
#csv_header = next(csv_file)# 读取第一行每一列的标题
df=pd.DataFrame(csv_file)

#三步筛选
#series .str.lower() 将某一列数据变成str并转成小写字符
dfs=df.loc[df['Process Name'].str.lower() == Pname] #程序名称
print(dfs)
dfs=dfs.loc[dfs['Operation'] == 'CreateFile']#操作
print(dfs)
dfs=dfs.loc[dfs['Result'] == 'SUCCESS']#结果
print(dfs)

#筛选结果验证
print(dfs['Process Name'].all())
print(dfs['Operation'].all())
print(dfs['Result'].all())

#dfs.to_csv(r'E:\github\procmon_asist\test_program\temp_Log2.csv',index=False,encoding='ANSI')

#筛选后的 即程序操作 再次筛选
#筛选 写入操作 磁盘写入操作
wPath=[]
wDetail=[]
for i in range(len(dfs['Detail'])):
    if dfs.iloc[i].loc['Detail'].rfind('OpenResult: Created') != -1:
        wPath.append(dfs.iloc[i].loc['Path'])
        wDetail.append(dfs.iloc[i].loc['Detail'])
#print(wDetail)

#将写操作数据写入 csv
wPath=pd.DataFrame(wPath)
wDetail=pd.DataFrame(wDetail)
wcsv=pd.concat([wPath,wDetail], axis=1) 
wcsv.to_csv(r'E:\github\procmon_asist\test_program\temp_write.csv',index=False,encoding='ANSI')


#print(dfs['Operation'])
dfs.drop_duplicates(subset=['Path'], keep='last', inplace=True)
dfs_w=[] #存储写信息
#detail=dfs['Detail']  #获取detail信息

for x in range(len(dfs['Detail'])):
    #print(dfs['Detail'].iloc[x])
    if dfs.iloc[x].loc['Detail'].find('Write')!=-1: 
        dfs_w.append(dfs.iloc[x])
        #print(dfs.iloc[x])
#Desired Access: Generic Read/Write, Disposition: Open, Options: No Buffering, Synchronous IO Non-Alert, Non-Directory File, Attributes: N, ShareMode: None, AllocationSize: n/a, OpenResult: Opened

dfs_w=pd.DataFrame(dfs_w)
print(dfs_w['Path'])



#csv_data=[] #创建list对象存放数据
#for row in csv_file:  # 将csv 文件中的数据保存到birth_data中
#    csv_data.append(row)
#csv_data = np.array(csv_data) # 将list数组转化成array数组便于查看数据结构
#csv_header = np.array(csv_header)
