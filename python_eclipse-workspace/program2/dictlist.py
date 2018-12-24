# Submitter: kuixir(Ren, Kuixi)
from goody import type_as_str  # Useful regarding some exceptions

class DictList:
    def __init__(self, *args):
        self.dl = []
        assert len(args) != 0, 'DictList.__init__: argument cannot be empty'
        for i in args:
            assert type(i) == dict, 'DictList.__init__: ' + str(i) + ' is not a dictionary.'
            assert len(i) != 0, 'DictList.__init__: ' + str(i) + ' is an empty dictionary.' 
            self.dl.append(i)
    
    def __len__(self):
        return len({keys for dic in self.dl for keys in dic})
    
    def __bool__(self):
        return len(self.dl) > 1
    
    def __repr__(self):
        return 'DictList(' + ', '.join((str(x) for x in self.dl)) + ')'
    
    def __contains__(self, target):
        return any((target in dic for dic in self.dl))
    
    def __getitem__(self, key):
        for adict in reversed(self.dl):
            if key in adict:
                return adict[key]
        raise KeyError('DictList.__getitem__: ' + str(key) + 'is not in any dictionary')
    
    def __setitem__(self, key, value):
        if key in self:
            for adict in reversed(self.dl):
                if key in adict:
                    adict[key] = value
                    break
        else:
            self.dl.append({key:value})
            
    def __delitem__(self, key):
        if key in self:
            for adict in reversed(self.dl):
                if key in adict:
                    del adict[key]
                    if len(adict) == 0:
                        self.dl.remove(adict)
                    break
        else:
            raise KeyError('DictList.__delitem__: ' + str(key) + 'is not in any dictionary.')
    
    def __call__(self, key):
        result = []
        for ind in range(len(self.dl)):
            if key in self.dl[ind]:
                result.append((ind, self.dl[ind][key]))
        return result
    
    def __iter__(self):
        #should it change when the object is changed??
        def gen(alist):
            unique = {key for adict in alist for key in adict}
            for adict in reversed(alist):
                for key in sorted(adict.keys()):
                    if key in unique:
                        yield key
                        unique.remove(key)
#         return gen(self.dl) #Mutable generator
        return gen(list(self.dl)) #Unmmutable gen
    
    def items(self):
        for key in self:
            yield (key, self[key]) 
                    
    def collapse(self):
        return {key: self[key] for key in self}
    
    def __eq__(self, right):
        left = self.collapse()
        if type(right) == DictList:
            return left == right.collapse()
        elif type(right) == dict:
            return left == right
        else:
            raise TypeError('DictList.__eq__: DictList object can only compare with dicts or DictLists, not ' + type_as_str(right))
                
    def __lt__(self, right_d):
        left = {keys for keys in self}
        if type(right_d) == DictList:
            right = {keys for keys in right_d}
            return left.issubset(right) and all((self[key] == right_d[key] for key in self)) and len(left) < len(right)
        elif type(right_d) == dict:
            right = right_d
            return left.issubset(right) and all((self[key] == right_d[key] for key in self)) and len(left) < len(right)
        else:
            raise NotImplemented
        
    def __gt__(self, right_d):
        right = {keys for keys in right_d}
        if type(right_d) == DictList:
            return right.issubset(self) and all((self[key] == right_d[key] for key in right_d)) and len(right) < len(self)
        elif type(right_d) == dict:
            return right.issubset(self) and all((self[key] == right_d[key] for key in right_d)) and len(right) < len(self)
        else:
            raise NotImplemented
        
    def __add__(self, right):
        left = eval(repr(self))
        if type(right) == DictList:
            right = eval(repr(right))
            return DictList(*(left.dl + right.dl))
        elif type(right) == dict:
            right = right.copy()
            left.dl.append(right)
            return eval(repr(left))
        else:
            raise NotImplemented
        
    def __radd__(self, left):
        if type(left) != dict:
            raise NotImplemented
        else:
            return DictList(left) + self
        
    def __setattr__(self, name, value):
        if name == 'dl' and name not in self.__dict__:
            self.__dict__[name] = value
        else:
            raise AssertionError('DictList.__setattr__: cannot create new attribute after initialization' + str(name))
    #if your __settattr__ does not work, comment it out so that it won't affect
    #  other tests passing

if __name__ == '__main__':
    #Simple tests before running driver
    #Put your own test code here to test DictList before doing bsc tests
    #Debugging problems with these tests is simpler

#     d = DictList(dict(a=1,b=2), dict(b=12,c=13))
#     print('len(d): ', len(d))
#     print('bool(d):', bool(d))
#     print('repr(d):', repr(d))
#     print(', '.join("'"+x+"'" + ' in d = '+str(x in d) for x in 'abcx'))
#     print("d['a']:", d['a'])
#     print("d['b']:", d['b'])
#     print("d('b'):", d('b'))
#     print('iter results:', ', '.join(i for i in d))
#     print('item iter results:', ', '.join(str(i) for i in d.items()))
#     print('d.collapse():', d.collapse())
#     print('d==d:', d==d)
#     print('d+d:', d+d)
#     print('(d+d).collapse():', (d+d).collapse())
#     print()
    
    import driver
    driver.default_file_name = 'bscp22S18.txt'
#     driver.default_show_exception= True
#     driver.default_show_exception_message= True
#     driver.default_show_traceback= True
    driver.driver()
