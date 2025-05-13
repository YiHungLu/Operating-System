import numpy as np
import math
import sys
from sympy import *
import matplotlib
import threading
import os
import time
import multiprocessing
import datetime   

inputList = []
m2List = []
m2tempM = []

if __name__ == '__main__':


	def BubbleSort(sort):
		i = 0
		j = 0
		n = len(sort)
		for i in range( 0, n - 1 ):
			for j in range( i + 1, n ):
				if sort[i] > sort[j]:
					sort[i], sort[j] = sort[j], sort[i]
		return sort

	def BubbleSortForThread(index):
		i = 0
		j = 0
		global m2List
		sort = m2List[index]
		n = len(sort)
		for i in range( 0, n - 1 ):
			for j in range( i + 1, n ):
				if sort[i] > sort[j]:
					sort[i], sort[j] = sort[j], sort[i]
		m2List[index] = sort

	def MergeSortForThread(index):
		global m2List
		global m2tempM
		sort = []
		a = m2List[index]
		b = m2List[index+1]
		i = 0
		j = 0
		done = False
		while(done==False):
			if a[i]<=b[j]:
				sort.append(a[i])
				i = i+1
			else:
				sort.append(b[j])
				j = j+1

			if i==len(a):
				while(j<len(b)):
					sort.append(b[j])
					j = j+1
				done = True
			elif j==len(b):
				while(i<len(a)):
					sort.append(a[i])
					i = i+1
				done = True
		m2tempM.append(sort)

	def MergeSort(a,b):
		sort = []
		i = 0
		j = 0
		done = False
		while(done==False):
			if a[i]<=b[j]:
				sort.append(a[i])
				i = i+1
			else:
				sort.append(b[j])
				j = j+1

			if i==len(a):
				while(j<len(b)):
					sort.append(b[j])
					j = j+1
				done = True
			elif j==len(b):
				while(i<len(a)):
					sort.append(a[i])
					i = i+1
				done = True

		
		return sort

	def Read(fileName):
		f = open(fileName,"r")
		readlist = f.readlines()
		f.close()
		i = 0
		for i in range(len(readlist)):
			inputList.append(int(readlist[i]))

	def Cut(k):
		n = len(inputList)
		l = int(n/k)
		x = n%k
		cutList = []
		j = 0
		for i in range(0,k):
			b = []
			if x>0:
				cutList.append(inputList[j:j+l+1])
				x = x-1
				j = j + l+1
			else:
				cutList.append(inputList[j:j+l])
				j = j + l

		return cutList

	def Mission1(fileName):
		start = time.time()
		result = BubbleSort(inputList)
		end = time.time()
		f = open(name + "_output1.txt", 'w')
		f.write("Sort : ")
		f.write("\n")
		for i in range(0, len(result)):
			f.write(str(result[i]))
			f.write("\n")
		f.write("CPU Time : ") 
		f.write(str(end-start))
		f.write("\n")
		f.write("Output Time : ")
		nowT = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())
		f.write(nowT)
		f.close()

	def Mission2(fileName,k):
		global m2List
		global m2tempM
		m2List.clear()
		m2tempM.clear()
		m2List = Cut(k)
		start = time.time()
		threads = []
		for i in range(0,len(m2List)):
			threads.append(threading.Thread(target = BubbleSortForThread, args = (i,)))
			threads[i].start()

		for j in range(0,k):
			threads[j].join()

		threadsM = []
		m2tempM.clear()
		while(len(m2List)!=1):
			index = 0
			count = 0
			while(index<len(m2List)):
				if index == len(m2List)-1:
					m2tempM.append(m2List[index])
				else:
					threadsM.append(threading.Thread(target = MergeSortForThread, args = (index,)))
					threadsM[count].start()
					count = count + 1

				index = index + 2
			for n in range(0,len(threadsM)):
				threadsM[n].join()
			threadsM.clear()
			m2List.clear()
			for i in range(0,len(m2tempM)):
				m2List.append(m2tempM[i])
			m2tempM.clear()

		end = time.time()
		result = m2List[0]
		f = open(name + "_output2.txt", 'w')
		f.write("Sort : ")
		f.write("\n")
		for i in range(0, len(result)):
			f.write(str(result[i]))
			f.write("\n")
		f.write("CPU Time : ") 
		f.write(str(end-start))
		f.write("\n")
		f.write("Output Time : ")
		nowT = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())
		f.write(nowT)
		f.close()    


	def Mission4(fileName,k):
		cutList = Cut(k)
		start = time.time()
		for i in range(0,k):
			BubbleSort(cutList[i])
		
		index = 0
		while(len(cutList)!=1):
			cutList.append(MergeSort(cutList[0], cutList[1]))
			cutList.remove(cutList[0])
			cutList.remove(cutList[0])

		end = time.time()
		result = cutList[0]
		f = open(name + "_output4.txt", 'w')
		f.write("Sort : ")
		f.write("\n")
		for i in range(0, len(result)):
			f.write(str(result[i]))
			f.write("\n")
		f.write("CPU Time : ") 
		f.write(str(end-start))
		f.write("\n")
		f.write("Output Time : ")
		nowT = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())
		f.write(nowT)
		f.close()

	def BubbleSortForProcess(sort):
		n = len(sort)
		for i in range( 0, n - 1 ):
			for j in range( i + 1, n ):
				if sort[i] > sort[j]:
					sort[i], sort[j] = sort[j], sort[i]

	def MergeSortForProcess(a, b, temp):
		i = 0
		j = 0
		sort = []
		done = False
		while(done==False):
			if a[i]<=b[j]:
				sort.append(a[i])
				i = i+1
			else:
				sort.append(b[j])
				j = j+1

			if i==len(a):
				while(j<len(b)):
					sort.append(b[j])
					j = j+1
				done = True
			elif j==len(b):
				while(i<len(a)):
					sort.append(a[i])
					i = i+1
				done = True
		temp.append(sort)

	def Mission3(name,k):
		m3List = Cut(k)
		processes = []
		start = time.time()     #第三題開始
		for i in range(0,k):
			temp = m3List[i]
			processes.append(multiprocessing.Process(BubbleSortForProcess(temp)))
			processes[i].start()
		for i in range(0,k):
			processes[i].join()

		processesM = []
		mid = time.time()
		print(mid-start)
		while(len(m3List)!=1):
			index = 0
			length = len(m3List)
			tempM = []
			count = 0
			while(index<length):
				if(index == length-1):
					tempM.append(m3List[index])
				else:
					processesM.append(multiprocessing.Process(target = MergeSortForProcess(m3List[index],m3List[index+1],tempM)))
					processesM[count].start()
					count = count + 1
				index = index + 2

			for i in range(0,len(processesM)):
				processesM[i].join()
			processesM.clear()
			m3List.clear()
			for i in range(0,len(tempM)):
				m3List.append(tempM[i])
			tempM.clear()

		end = time.time()
		result = m3List[0]
		f = open(name + "_output3.txt", 'w')
		f.write("Sort : ")
		f.write("\n")
		for i in range(0, len(result)):
			f.write(str(result[i]))
			f.write("\n")
		f.write("CPU Time : ") 
		f.write(str(end-start))
		f.write("\n")
		f.write("Output Time : ")
		nowT = time.strftime("%Y-%m-%d %H:%M:%S",time.localtime())
		f.write(nowT)
		f.close()

	cmd = input("請輸入指令 (0:離開, 1:方法一 ,2:方法二, 3:方法三, 4:方法四): ")
	while(cmd!='0'):
		inputList.clear()
		name = input("請輸入檔案名稱 :")
		fileName = name + ".txt"    
		Read(fileName)
		if(cmd == '1'):
			Mission1(name)          #第一題
		else:
			k = input("請輸入要切成幾分 :")
			k = int(k)
			while(k<=0):
				k = input("請輸入>0 :")
			if cmd == '2':
				Mission2(name,k)        #第二題
			elif cmd == '3':
				Mission3(name,k)
			elif cmd == '4':
				Mission4(name,k)        #第四題
			else:
				print("error command")
		cmd = input("請輸入指令 (0:離開, 1:方法一 ,2:方法二, 3:方法三, 4:方法四): ")