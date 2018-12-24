'''
Created on Mar 28, 2018

@author: kevin
'''
# def f (a,*b):     # typically *args and **kargs or **kwargs
#     print('f1:',a,b)
#     print('f2:',a,*b)
# 
# def g (a,*b,**c):  # typically *args and **kargs or **kwargs
#     print('g1:',a,b,c)
#     print('g2:',a,*b,**c)
#     
# # f(1)
# # f(1,2)
# # f(1,2,3,4,5)
# # print()
# # g(1)
# g(1,2,3,4,5,sep='-',end='cr\n')
# x = [i - 5 for i in range(1,5)]
# print(x)
# x = {c: c+1 for c in range(1,5)}
# print(x)
# x = {L for num in [1,213,214125,46234631,23,4124] for L in str(num) if L in '1234'}
# print(x)
# x = x = {word : {c for c in word} for word in ['i', 'love', 'new', 'york']}
# print(x)
# 
# x = {key:value for key,value in enumerate((i for i in range(1,4)))}
# print(x)


# def f(a,b,*args, **kargs):
#   print(a,b,args, kargs)
# 
# f(b=1,a=2, d= 5,c=2)

class C:
    def __init__(self): pass

o = C()
o.a = 1
print(o.a)           # prints 1

o.__dict__['a'] = 2
print(o.a)           # prints 2

o.__dict__['b'] = 3
print(o.a,o.b)       # prints 2 3