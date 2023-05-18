#!/usr/bin/python
from operator import itemgetter

address = []
prev_address = []
loc = []
xor_values = []
trace_result = []
frequency = []
unsorted_list = []
sorted_list = []

with open('random-loc.txt') as f:
    data = f.readlines()
    i = 0
for line in data:
    words = line.split()
    num = len(words)
    if i == 0:
        address.append(int(words[0]))
        a = int(words[0])
        prev_address.append(int(a/2))
        i = 1
    else:
        if num < 6:
            loc.append(int(words[2]))
        else:
            loc.append(float(words[2]) + 0.1)
        i = 0

with open("showmap-result.txt") as f:
    data = f.readlines()
for line in data:
    words = line.split(':')
    trace_result.append(int(words[0]))
    frequency.append(int(words[1]))

f = open("Table.txt", 'w')
f.write("Loc of Previous \tLoc of Current \tPrev Random  \tCurr Random \tTable Index \tNo of times visited\n")
j = -1
for val_1 in address:
    j += 1
    for val_2 in prev_address:
        z = val_1 ^ val_2
        xor_values.append(z)
        m = -1
        for x in trace_result:
            m += 1
            if x == z:
                i = -1
                for y in address:
                    i += 1
                    if val_2 == (y/2):
                        unsorted_list.append([loc[i],loc[j],y,val_1,x,frequency[m]])

unsorted_list.sort(key=itemgetter(0))

for item in unsorted_list:
    f.write("\t\t%s\t\t%s\t\t%x\t\t%x\t\t%u\t\t%u\n" % (item[0],item[1],item[2],item[3],item[4],item[5]))



