'''
Created on Apr 15, 2018

@author: kevin
'''
import re

x = re.match('a(?P<lol>b*)(?P<third_number>c)','abbbbc')
print(x.groups())
print(x.groupdict())