
import re, traceback, keyword

def pnamedtuple(type_name, field_names, mutable = False):
    def show_listing(s):
        for line_num,line_text in enumerate(s.split('\n'),  1):
            print(f' {line_num: >3} {line_text.rstrip()}')

    # put your code here
    # bind class_definition (used below) to the string constructed for the class
    def unique(iterable):
        iterated = set()
        for i in iterable:
            if i not in iterated:
                iterated.add(i)
                yield i
            
    _legal = re.compile(r'^[a-zA-Z]\w*$')
    if _legal.match(str(type_name)) == None:
        raise SyntaxError(str(type_name) + 'type_name must be a legal string')
    if type(field_names) != list:
        field_comp = re.compile(r' +|, *')
        field_list = field_comp.split(str(field_names))
    else:
        field_list = field_names
        
    for i in field_list:
        if _legal.match(i) == None or i in keyword.kwlist:
            raise SyntaxError('hmmm' + str(i) + 'field_names must be a legal string')
    field_list = [i for i in unique(field_list)]
        
    class_definition = '''\
class {}:
    def __init__(self, {}):
        {}
        self.fields = {}
        self.mutable = {}
    def __repr__(self):
        return '{}' + '(' + ','.join(('{}={}'.format(i, self.__dict__[i]) for i in self.fields)) + ')'
{}
    def __getitem__(self, target):
        if type(target) == int:
            if target >= len(self.fields):
                raise IndexError("__getitem__: index out of range")
            return self.__dict__[self.fields[target]]
        elif type(target) == str:
            if target in self.__dict__:
                return self.__dict__[target]
            raise IndexError("__getitem__: string index doesn't exist")
        raise IndexError("__getitem__: index must be a int or string")
        
    def __eq__(self, target):
        if type(target) == {} and len(self.fields) == len(target.fields):
            for i in range(len(self.fields)):
                if self[i] != target[i]:
                    return False
            return True
        else:
            return False
        
    def _replace(self, **kargs):
        if len(kargs) == 0:
            raise TypeError('_replace: no attributes entered')
        for k in kargs.keys():
            if k not in {}:
                raise TypeError('_replace:' + str(k) + ' is not an attribute')
        if self.mutable:
            for k in kargs:
                self.__dict__[k] = kargs[k]
        else:
            temp = list(self.__dict__.values())
            for k in kargs:
                ind = self.fields.index(k)
                temp[ind] = kargs[k]
            return {}(*temp[:-2])
    
    def __setattr__(self, name, value):
        if 'mutable' in self.__dict__:
            if self.mutable:
                self.__dict__[name] = value
            else:
                raise AttributeError("__setattr__: {} class is immutable")
        else:
            self.__dict__[name] = value
'''. format(type_name, ', '.join((i for i in field_list)), '\n        '.join(('self.{} = {}'.format(i, i) for i in field_list)), \
            field_list, mutable, type_name, '{}', '{}', '\n'.join(('    def get_{}(self):\n        return self.{}'.format(i,i) for i in field_list)), \
            type_name, field_list, type_name, type_name)
    # While debugging, remove comment to show the source code for the clas
    # show_listing(class_definition)
    
    # Execute the class_definition str in a local name space; later, bind the
    #   source_code attribute to the class_defintion; after try, return the
    #   class object created; if there is a syntax error, list the class and
    #   also show the error
    name_space = dict(__name__ = f'pnamedtuple_{type_name}')
    try:
        exec(class_definition,name_space)
        name_space[type_name].source_code = class_definition
    except (TypeError, SyntaxError) :
        show_listing(class_definition)
        traceback.print_exc()
    return name_space[type_name]


    
if __name__ == '__main__':
    # Test pnamedtuple in script below, use Point = pnamedtuple('Point', 'x y')
    Triple1 = pnamedtuple('Triple1', 'a b c')
    t1 = Triple1(1,2,3)
    t1._replace(a=2)
    #driver tests
    import driver
    driver.default_file_name = 'bscp3S18.txt'
#     driver.default_show_exception= True
#     driver.default_show_exception_message= True
#     driver.default_show_traceback= True
    driver.driver()
