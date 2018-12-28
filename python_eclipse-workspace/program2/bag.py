
from collections import defaultdict
from goody import type_as_str        # Useful regarding some exceptions

class Bag:
    def __init__(self, *args):
        self._bag = defaultdict(int)
        if len(args) == 1:
            args = args[0]
        for item in args:
            self._bag[item] += 1
    def __repr__(self):
        result = []
        for key in self._bag:
            result += [key] * self._bag[key]
        return 'Bag(' + str(result) + ')'
    def __str__(self):
        return 'Bag(' + ', '.join(('{}[{}]'.format(key, self._bag[key]) for key in self._bag)) + ')'

    def __len__(self):
        return sum(self._bag.values())
    
    def unique(self):
        return len(self._bag.keys())
    
    def __contains__(self, sub):
        return sub in self._bag.keys()
    
    def count(self, item):
        if item not in self:
            return 0
        return self._bag[item]
    
    def add(self, right):
        self._bag[right] += 1
        
    def __add__(self, right):
        if type(right) != Bag:
            return NotImplemented
        result = Bag()
        for key in set(right._bag.keys()) | set(self._bag.keys()):
            for _times in range(right.count(key) + self.count(key)):
                result.add(key)
        return result

    def remove(self, target):
        if target not in self._bag:
            raise ValueError('Bag.remove: ' + str(target) + ' is not in the Bag object ' + str(self))
        self._bag[target] -= 1
        if self._bag[target] == 0:
            del self._bag[target]
            
    def __eq__(self, right):
        if type(right) != Bag:
            return False
        return self._bag == right._bag
    
    def __iter__(self):
        def gen(bag):
            self.tlist = []
            for key in bag:
                self.tlist += [key] * bag[key]
            for item in self.tlist:
                yield item
        return gen(self._bag.copy())
         


if __name__ == '__main__':
    
        
    #Simple tests before running driver
    #Put your own test code here to test Bag before doing bsc tests
    #Debugging problems with these tests is simpler

    b = Bag(['d','a','b','d','c','b', 1])
    print(repr(b))
    print(all((repr(b).count('\''+v+'\'')==c for v,c in dict(a=1,b=2,c=1,d=3).items())))
    for i in b:
        print(i)
  
    b2 = Bag(['a','a','b','x','d'])
    print(repr(b2+b2))
    print(str(b2+b2))
    print([repr(b2+b2).count('\''+v+'\'') for v in 'abdx'])
    b = Bag(['a','b','a'])
    print(repr(b))
    print()
    
    import driver
    driver.default_file_name = 'bscp21S18.txt'
#     driver.default_show_exception=True
#     driver.default_show_exception_message=True
#     driver.default_show_traceback=True
    driver.driver()
