from numpy import random
import numpy as np
import os

class Set:
    lines=[]
    words=[]
    
    def __init__(self, path):
        try:
            file = open(path, mode='rt', encoding = 'utf-8')
            lines=file.read().splitlines()
            for i in lines:
                if i.startswith('#'):
                    continue
                line = i.split(';')
                if len(line)!=2:
                    continue

                w=[]
                for j in line[0].split(','):
                    w.append(j.strip())
            
                self.words.append((w, line[1].strip()))
        finally:
            file.close()
    #Mode: 0 - all, 1 - any, 2 - first; TODO: implement in file
    def train(self, mode = 0, randomly = True, repeat = True):
        count = len(self.words)
        correct = 0
        n=0
        if randomly:
            random.shuffle(self.words)
        for i in self.words:
            ans = input(i[1] + '\n').split(',')

            cr = True
            if mode==0:
                if len(i[0])==len(ans):
                    for j in ans:
                        if not j.strip() in i[0]:
                            cr = False
                            break
                else:
                    cr = False
            elif mode==1:
                cr = False
                for j in ans:
                    if j.strip() in i[0]:
                        cr = True
                        break
            elif mode==2:
                if ans[0].strip() != i[0][0]:
                    cr=False
                else:
                    for j in ans:
                        if not j.strip() in i[0]:
                            cr=False
                            break
                
            if cr:
                print("Correct\n", i[0])
                if input("Write \"!\" if answer was incorrect\n").strip() == "!":
                    if repeat:
                        self.words.append(i)
                else:
                    if n<count:
                        correct=correct+1
            else:
                print("Incorrect\n", i[0])
                if input("Write \"!\" if answer was correct\n").strip() == "!":
                    print("Correct")
                    if n<count:
                        correct=correct+1
                else:
                    if repeat:
                        self.words.append(i)
            os.system("pause")
            os.system("cls")
            n=n+1
        print(correct, "/", count, "\n", int(round(correct/count, 2)*100), "%")
        os.system("pause")
    def save(self):
        return
    
inp=input("File path:\n")
os.system("cls")

set = Set(inp)
set.train(mode = 2)
