from random import randint,random
import datetime
PHY_MEM = 32
VR_MEM = 64
result=[]
class replace(object):
    def __init__(self,access_len=32,block_len=3):
        #设置定长的访问序列
        self.access_len=access_len
        self.access = [None]*access_len
        #设置定长内存
        self.block = []
    
    def create_access(self,m=8,e=8,t=0.5):
        p = randint(0,VR_MEM-1)
        #设置一个大小为32的访问序列
        for i in range(self.access_len):
            temp = randint(p,p+7) % VR_MEM
            self.access[i]=temp
            if i%8==0:
                r = random()
                if(r<t):
                    p = randint(0,VR_MEM-1)
                else:
                    p = (p+1)%VR_MEM

    def special_access(self):
        self.access=[7,0,1,2,0,3,0,4,2,3,0,3,2,1,2,0,1,7,0,1]

    def find_block_index(self,index):
        max = -1
        if index==len(self.access):
            return self.block[0]
        for i in self.block:
            try:
                temp = self.access[index:].index(i)
            except ValueError:
                return i
            if temp>max:
                max =temp
                max_index = i
        return max_index

    def optimal(self,block_len):
        self.block = []
        miss_count = 0
        index = 1
        #print(self.access)
        while index<=len(self.access):
            if self.access[index-1] in self.block:
                index+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                continue
            else:
                if len(self.block)<block_len:
                    self.block.append(self.access[index-1])
                else:
                    need_replace = self.find_block_index(index)
                    self.block.remove(need_replace)
                    self.block.append(self.access[index-1])
                    miss_count+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                
                index+=1
        print("OPT缺页数为%d"%miss_count)
        result.append(miss_count)


    def FIFO(self,block_len):
        self.block=[]
        miss_count = 0
        index = 1
        while index<=len(self.access):
            if self.access[index-1] in self.block:
                
                index+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                continue
            else:
                if len(self.block)<block_len:
                    self.block.insert(0,self.access[index-1])
                else:
                    self.block.pop()
                    self.block.insert(0,self.access[index-1])
                    miss_count+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)               
                index+=1
        print("FIFO缺页数为%d"%miss_count)
        result.append(miss_count)


    def LRU(self,block_len):
        self.block=[]
        record_stack=[]#记录当前block中过去未使用状态排序，最久未使用排在栈底，最近使用排在栈顶。
        miss_count = 0
        index = 1
        #print(self.access)
        while index<=len(self.access):
            
            if self.access[index-1] in self.block:
                #更新record栈内情况
                record_stack.remove(self.access[index-1])
                record_stack.insert(0,self.access[index-1])
                index+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                #print("stack:"+str(record_stack))
                continue
            else:
                if len(self.block)<block_len:
                    self.block.append(self.access[index-1])
                    record_stack.insert(0,self.access[index-1])
                else:
                    self.block.remove(record_stack[-1])
                    self.block.append(self.access[index-1])
                    record_stack.pop()
                    record_stack.insert(0,self.access[index-1])
                    miss_count+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                #print("stack:"+str(record_stack))
                index+=1
        print("LRU缺页数为%d"%miss_count)
        result.append(miss_count)


    def check_clock(self):
        for i in self.block:
            if i[1]==0 and i[2]==0:
                return i
        for i in self.block:
            if i[1]==0 and i[2]==1:
                return i
            i[1]=0
        return self.check_clock()
        

    def CLOCK(self,block_len):
        self.block=[]
        miss_count = 0
        index = 1
        while index<=len(self.access):
            page_num=[]
            simulate_mod = random()
            for i in self.block:
                page_num.append(i[0])
            if self.access[index-1] in page_num:
                
                index+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                continue
            else:
                new = [self.access[index-1],1,0]
                if simulate_mod<0.5:
                    new[2]=1
                if len(self.block)<block_len:
                    self.block.append(new)
                else:
                    need_replace = self.check_clock()
                    self.block.remove(need_replace)
                    self.block.append(new)
                    miss_count+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)               
                index+=1
        print("改进CLOCK缺页数为%d"%miss_count)
        
        result.append(miss_count)
    
    
    def PBA(self,block_len):
        self.block=[]
        free_list=[]
        free_len=2
        miss_count = 0
        index = 1
        while index<=len(self.access):
            if self.access[index-1] in self.block:
                
                index+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+'     ' +str(self.block))
                #print(self.block)
                continue
            else:
                if len(self.block)<block_len:
                    self.block.insert(0,self.access[index-1])
                elif self.access[index-1] in free_list:
                    temp = self.block.pop()
                    free_list.remove(self.access[index-1])
                    self.block.insert(0,self.access[index-1])
                    free_list.insert(0,temp)
                    
                else:
                    temp = self.block.pop()
                    self.block.insert(0,self.access[index-1])
                    if(len(free_list)<=free_len):
                        free_list.append(temp)
                    else:
                        free_list.remove(free_list[0])
                        free_list.append(temp)
                    miss_count+=1
                #print("index="+str(index)+",miss_count="+str(miss_count)+"    " +str(self.block))
                #print(self.block)               
                index+=1
        print("PBA缺页数为%d"%miss_count)
        result.append(miss_count)
        


for i in [16,32,64,128,256]:
    result=[]
    VR_MEM=i
    opt = replace()
    opt.create_access()
    print("序列长度为：%d" %i)
    #print("生成随机序列如下：")
    #print(opt.access)
    #print("\n\n******************OPTI*********************\n\n")
    oldtime= datetime.datetime.now()
    opt.optimal(3)
    nowtime = datetime.datetime.now()
    time = (nowtime-oldtime).microseconds
    #print("\n\n****************FIFO**********************\n\n")
    opt.FIFO(3)     
    #print("\n\n****************LRU**********************\n\n")
    opt.LRU(3)
    #print("\n\n****************CCLOCK**********************\n\n")
    opt.CLOCK(3)
    #print("\n\n****************PBA**********************\n\n")
    #opt.PBA(3)
    print('\n')
    print("最佳置换算法缺页率为：%f" % (result[0]/len(opt.access)))
    print("先进先出算法缺页率为：%f" % (result[1]/len(opt.access)))
    print("最近最久未使用算法缺页率为：%f" % (result[2]/len(opt.access)))
    print("改进CLOCK算法缺页率为：%f" % (result[3]/len(opt.access)))
    #print("页面缓冲算法缺页率为：%f" % (result[4]/len(opt.access)))   
    print('\n\n')            
        

