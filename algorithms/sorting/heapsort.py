#!/usr/bin/env python

import random

LENGTH = 100
data = [random.randint(0,100) for _ in range(LENGTH)]

def heapsort(lst):
    
    def swap(lst, a, b):
        """given a list, swap elements at 'a' and 'b'"""
        temp = lst[a]
        lst[a] = lst[b]
        lst[b] = temp
        
    def siftdown(lst, start, end):
        """move an element to proper position"""
        root = start
        while root * 2 + 1 <= end:
            left_child = root * 2 + 1
            cand = root
            # is left_child a candidate for swapping?
            if lst[cand] < lst[left_child]:
                cand = left_child
            # given that swap is max(left_child, root),
            # is right_child a candidate for swapping?
            if left_child + 1 <= end and lst[cand] < lst[left_child + 1]:
                cand = left_child +1
            # set root to value to be swapped (or do nothing if same)
            if cand != root:
                swap(lst, root, cand)
                root = cand
            else:
                break
            
    def heapify(lst):
        """call siftdown on non-leaf elements"""
        size = len(lst)
        # begin at rightmost non-leaf element
        start = size/2 -1
        # call siftdown and move left (start -= 1)
        while start >= 0:
            siftdown(lst, start, size - 1)
            start -= 1

    def heapsort(lst):
        # create heap
        heapify(lst)
        last = len(lst) - 1
        # while there is a heap under consideration
        while last > 0:
            # zeroeth element is largest, move to last position
            swap(lst, last, 0)
            # shrink heap 
            last = last - 1
            # first element altered, move to proper position
            siftdown(lst, 0, last)

    heapsort(lst)

print "initial",  data
heapsort(data)
print "sorted", data
                
