import  ctypes
from pyspark import SparkConf, SparkContext, StorageLevel
import time
import random
import os
import sys

def readFasta(filename,filename2):
    f1= open(filename,'r')
    seq = ""
    name= ""
    seqs = []
    names= []
    for line in f1:
        if line.startswith('>'):
            if seq!="":
                seq+='\n'
                seqs.append(seq)
                name= ""
                name += line[:-10]
                names.append(name[1:])
                seq=""
                continue
            seq = ""
            name= ""
            name += line[:-10]
            continue
        if line.startswith('\n'):
            if seq!="":
                seq+='\n'
                seqs.append(seq)
                names.append(name[1:])
                seq=""
                continue
        seq += line[:-1]
    # seqs.append(seq)
    #print(seqs.__len__())
    #print(seqs)
    f1.close()
    f2=open(filename2,'w')
    f2.write(''.join(seqs))
    f2.close()

    return [seqs,names]

def findCentre(filename,strNum,basepath):
    optimal=ctypes.cdll.LoadLibrary(basepath+'/optimal.so')
    input_char_p_1=ctypes.c_char_p(str(filename).encode('utf-8'))
    input_char_p_2=(ctypes.c_char*500000)()
    input_int=ctypes.c_int(strNum)
    optimal.findResult.argtypes=(ctypes.c_char_p,ctypes.c_char*500000,ctypes.c_int)
    idx=optimal.findResult(input_char_p_1,input_char_p_2,input_int)
    centre=input_char_p_2.value.decode()
    return [idx,centre]

def compare(s1,centre,basepath):
    optimal=ctypes.cdll.LoadLibrary(basepath+'/optimal.so')
    optimal.calculate.argtypes=(ctypes.c_char_p,ctypes.c_char_p,ctypes.c_char*5000000,ctypes.c_char*5000000)
    input_char_p_1=ctypes.c_char_p(s1.encode('utf-8'))
    input_char_p_2=ctypes.c_char_p(centre.encode('utf-8'))
    input_char_p_3=(ctypes.c_char*5000000)()
    input_char_p_4=(ctypes.c_char*5000000)()
    optimal.calculate(input_char_p_1,input_char_p_2,input_char_p_3,input_char_p_4)
    result=[]
    result.append(str(input_char_p_3.value.decode()))
    result.append(str(input_char_p_4.value.decode()))
    # print(result)
    return result
    
def align(seqs,original,strNum,basepath):
    dna_input=((ctypes.c_char_p)*strNum)\
        (*[i.encode() for i in seqs])
    dna_original=(ctypes.c_char_p)(original.encode('utf-8'))
    dna_result=(ctypes.c_char*5000000)()
    optimal=ctypes.cdll.LoadLibrary(basepath+'/optimal.so')
    optimal.align_py.argtypes=(ctypes.c_char_p*strNum,ctypes.c_char_p,ctypes.c_char*5000000,ctypes.c_int)
    optimal.align_py(dna_input,dna_original,dna_result,\
        ctypes.c_int(strNum))
    centre=str(dna_result.value.decode())
    return  centre

def constructMatrix(seqs,strNum,basepath):
    dna_input=((ctypes.c_char_p)*strNum)\
        (*[i.encode() for i in seqs])
    distance_length=int((strNum**2-strNum)/2+1)
    distances_input=(ctypes.c_double\
        *int(distance_length))()
    optimal=ctypes.cdll.LoadLibrary(basepath+'/optimal.so')
    optimal.construct_matrix.argtypes=(ctypes.c_char_p*strNum,\
        ctypes.c_double*distance_length,ctypes.c_int)
    optimal.construct_matrix(dna_input,distances_input,\
        ctypes.c_int(strNum))
    return  distances_input

def compare_sec(seqs,centre):
    for seq in seqs:
        yield compare(seq,centre)
        pass


def sa_align(memory,application_name,filename,basepath,partition = 5,url = "local[*]"):
    conf= SparkConf().setMaster(url).set("spark.executor.memory", memory).setAppName(application_name)
    sc = SparkContext(conf=conf)
    time_start=time.time()
    [seqs,names]=readFasta(filename,'DNA.txt')
    strNum=len(seqs)
    print(strNum)
    for i in range(len(seqs)):
        seqs[i]=seqs[i].strip().upper()
        seqs[i]=seqs[i].replace('N','-')
        print(len(seqs[i]))
    idx=random.randint(0,strNum-1)
    centre_seq=seqs[idx]
    compare(centre_seq,centre_seq,basepath)
    seqs.pop(idx)
    words_new = sc.broadcast(centre_seq)
    data_heterogenous=sc.parallelize(seqs,partition).persist(StorageLevel.MEMORY_ONLY_SER)
    res=data_heterogenous.map(lambda p : compare(p,words_new.value,basepath))
    result=res.take(strNum)
    res.unpersist()
    data_heterogenous.unpersist()
    centres=[]
    seqs.clear()
    f2=open(filename[:-5]+"time_cost.txt",'a')
    for elem in result:
        seqs.append(elem[0])
        centres.append(elem[1])
        f2.write("".join(str(len(elem[0]))+'\n'))
    centre_seq_sec=align(centres,centre_seq,strNum-1,basepath)
    words_new_2 = sc.broadcast(centre_seq_sec)
    print(len(centre_seq_sec))
    data_heterogenous_sec=sc.parallelize(seqs,partition).persist(StorageLevel.MEMORY_ONLY_SER)
    res=data_heterogenous_sec.map(lambda p : compare(p,words_new_2.value,basepath)[0])
    seqs.clear()
    seqs=res.take(strNum)
    seqs.insert(idx,centre_seq)
    time_end=time.time()
    time_c= time_end - time_start
    print('time cost'+ str(time_c)+'s')
    f2.write(''.join('time cost'+ str(time_c)+'s'))
    f2.close()
    distances=constructMatrix(seqs,strNum,basepath)
    neigh=ctypes.cdll.LoadLibrary(basepath+'/libnneigh.so')
    result=(ctypes.c_char*1000000)()
    distance_length=int((strNum**2-strNum)/2+1)
    name_input=((ctypes.c_char_p)*strNum)\
        (*[i.encode() for i in names])
    neigh.main_prog.argtypes=(ctypes.c_int,ctypes.c_char_p*strNum,\
        ctypes.c_double*distance_length\
            ,ctypes.c_char*1000000)
    neigh.main_prog(\
        ctypes.c_int(strNum),name_input,distances,result)   
    pass



