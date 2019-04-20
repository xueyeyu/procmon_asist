import pandas as pd
import csv



#csv读取
#csv_file = csv.reader(open(r'C:/Users/Public/Desktop/log.csv','r',errors='ignore')) #使用csv.reader读取csvfile中的文件,r为只读
#csv_file=pd.read_csv(r'C:/Users/Public/Desktop/log.csv')
csv_file=pd.read_csv(r'C:\Users\JACK\Desktop\Logfile.csv')
#csv_header = next(csv_file)# 读取第一行每一列的标题
df=pd.DataFrame(csv_file)

dfs=df.loc[df['Process Name'] == 'EyeGuard1.0.3.0.exe'] #程序名称
dfs=dfs.loc[dfs['Operation'] == 'CreateFile']#操作
dfs=dfs.loc[dfs['Result'] == 'SUCCESS']#结果
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

