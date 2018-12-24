# Submitter: kuixir(Ren, Kuixi)
# from goody import type_as_str
import inspect


class Check_All_OK:
    """
    Check_All_OK class implements __check_annotation__ by checking whether each
      annotation passed to its constructor is OK; the first one that
      fails (by raising AssertionError) prints its problem, with a list of all
      annotations being tried at the end of the check_history.
    """
       
    def __init__(self,*args):
        self._annotations = args
        
    def __repr__(self):
        return 'Check_All_OK('+','.join([str(i) for i in self._annotations])+')'

    def __check_annotation__(self, check, param, value, check_history):
        for annot in self._annotations:
            check(param, annot, value, check_history+'Check_All_OK check: '+str(annot)+' while trying: '+str(self)+'\n')


class Check_Any_OK:
    """
    Check_Any_OK implements __check_annotation__ by checking whether at least
      one of the annotations passed to its constructor is OK; if all fail 
      (by raising AssertionError) this classes raises AssertionError and prints
      its failure, along with a list of all annotations tried followed by the
      check_history.
    """
    
    def __init__(self,*args):
        self._annotations = args
        
    def __repr__(self):
        return 'Check_Any_OK('+','.join([str(i) for i in self._annotations])+')'

    def __check_annotation__(self, check, param, value, check_history):
        failed = 0
        for annot in self._annotations: 
            try:
                check(param, annot, value, check_history)
            except AssertionError:
                failed += 1
        if failed == len(self._annotations):
            assert False, repr(param)+' failed annotation check(Check_Any_OK): value = '+repr(value)+\
                         '\n  tried '+str(self)+'\n'+check_history                 



class Check_Annotation:
    # setting the class attribute to True below allows checking to occur
    checking_on  = True

    # set self._checking_on to True too, for function checking to occur 
    def __init__(self, f):
        self._f = f
        self._checking_on = True
        
    # Check whether param's annot is correct for value, adding to check_history
    #    if recurs; defines many local function which use it parameters.  
    def check(self,param,annot,value,check_history=''):
        
        # Define local functions for checking, list/tuple, dict, set/frozenset,
        #   lambda/functions, and str (str for extra credit)
        # Many of these local functions called by check, call check on their
        #   elements (thus are indirectly recursive)
        def annot_None():
            pass
        
        def annot_type():
            if inspect.isfunction(annot):
                if len(annot.__code__.co_varnames) != 1:
                    assert False, "AssertionError: '{}' annotation inconsistency: predicate should have 1 parameter but had {}\n  predicate = {}{}".format(param,len(annot.__code__.co_varnames), annot,check_history)
                
            assert type(value) == annot, "'{}' failed annotation check(wrong type): value = {}\n  was type {} ...should be type {}{}".format(param, repr(value), type(value).__name__, annot.__name__, check_history)
        def annot_list(check_history=check_history):
            self.check(param, list, value)
            if len(annot) == 1:
                for ind in range(len(value)):
                    og = check_history
                    check_history += '\nlist[{}] check: {}'.format(ind, annot[0])
                    self.check(param, annot[0], value[ind], check_history)
                    check_history = og
            else:
                assert len(annot) == len(value), "'"+str(param)+"' failed annotation check(wrong number of elements): value = "+str(value)+"\n  annotation had "+str(len(annot))+ " elements"+ str(annot)
       
                for ind in range(len(annot)):
                    check_history += '\nlist[{}] check: {}\n'.format(ind, annot[ind])
                    self.check(param, annot[ind], value[ind], check_history)
                    check_history = ''
                    
                    
        def annot_tuple(check_history=check_history):
            self.check(param, tuple, value)
            if len(annot) == 1:
                for ind in range(len(value)):
                    og = check_history
                    check_history += '\ntuple[{}] check: {}'.format(ind, annot[0])
                    self.check(param, annot[0], value[ind], check_history)
                    check_history = og
            else:
                assert len(annot) == len(value), "'"+str(param)+"' failed annotation check(wrong number of elements): value = "+str(value)+"\n  annotation had "+str(len(annot))+ " elements"+ str(annot)
       
                for ind in range(len(annot)):
                    check_history += '\ntuple[{}] check: {}\n'.format(ind, annot[ind])
                    self.check(param, annot[ind], value[ind], check_history)
                    check_history = ''
        
            
        def annot_set(check_history=check_history):
            self.check(param, set, value)
            if len(annot) == 1:
                for ind in range(len(value)):
                    og = check_history
                    check_history += '\nset value check: {}'.format(list(annot)[0])
                    self.check(param, list(annot)[0], list(value)[ind], check_history)
                    check_history = og 
            else:
                assert False, "'{}' annotation inconsistency: set should have 1 value but had {}\n  annotation = {}".format(param, len(annot), annot)
                    
        def annot_dict(check_history = check_history):
            if not len(annot.items()) == 1:
                assert False, "'{}' annotation inconsistency: dict should have 1 item but had {}\n  annotation = {}".format(param, len(annot.items()), annot)
            
            assert isinstance(value, dict), "'{}' failed annotation check(wrong type): value = {}\n  was type {} ...should be type dict{}".format(param, repr(value), type(value).__name__, check_history)

            
            for k in value.keys():
                og = check_history
                check_history += '\ndict key check: {}'.format(list(annot.keys())[0])
                self.check(param, list(annot.keys())[0], k, check_history)
                check_history = og
                check_history += '\ndict value check: {}'.format(list(annot.values())[0])
                self.check(param, list(annot.values())[0], value[k], check_history)
                check_history = og
            
        def annot_frozenset(check_history=check_history):
            self.check(param, frozenset, value)
            if len(annot) == 1:
                for ind in range(len(value)):
                    og = check_history
                    check_history += '\nfrozenset value check: {}'.format(list(annot)[0])
                    self.check(param, list(annot)[0], list(value)[ind], check_history)
                    check_history = og
            else:
                assert False, "'{}' annotation inconsistency: frozenset should have 1 value but had {}\n  annotation = {}".format(param, len(annot), annot)
        
        def annot_lambda(check_history = check_history):

            if len(annot.__code__.co_varnames) != 1:
                assert False, "'{}' annotation inconsistency: predicate should have 1 parameter but had {}\n  predicate = {}{}".format(param,len(annot.__code__.co_varnames), annot,check_history)
            
            try:
                assert annot(value)
            except AssertionError:
                raise AssertionError("'{}' failed annotation check: value = {}\n  predicate = {}{}".format(param, repr(value), annot,check_history))
            except Exception as e:
                assert False, "'{}' annotation predicate({}) raised exception\n  exception = {}: {}{}"\
                .format(param, annot,type(e).__name__,e, check_history)
                
        def annot_other(check_history = check_history):
            try:
                annot.__check_annotation__
            except AttributeError:
                assert False, "'{}' annotation undecipherable: {}".format(param, annot)
                
            else:
                annot.__check_annotation__(self.check, param, value, check_history)
                    
        def annot_str(check_history=check_history):
            try:
                assert eval(annot, None, value), "'{}' failed annotation check(str predicate: {})\n  args for evaluation: {}{}"\
                .format(param, annot, ', '.join((str(x)+'->'+str(value[x]) for x in value)), check_history)
            except AssertionError:
                raise
            except Exception as e:
                assert False, "'{}' annotation check(str predicate: {}) raised exception\n  exception = {}: {}{}".format(param, annot, type(e).__name__, e, check_history)
         
        if annot == None:
            annot_None()
        elif type(annot) == type:
            annot_type()
            
        elif type(annot) == list:
            annot_list()
        elif type(annot) == tuple:
            annot_tuple()
            
        elif isinstance(annot, dict):
            annot_dict()
        
        elif type(annot) == set:
            annot_set()
            
        elif type(annot) == frozenset:
            annot_frozenset()
            
        elif inspect.isfunction(annot):
            annot_lambda()
            
        elif type(annot) == str:
            annot_str()
            
        else:
            annot_other()
        # Decode check function's annotation below; check it against arguments
        
        
    # Return result of calling decorated function call, checking present
    #   parameter/return annotations if required
    def __call__(self, *args, **kargs):
        
        # Return a dictionary of the parameter/argument bindings (actually an
        #    ordereddict: it stores the function header's parameters in order)
        def param_arg_bindings():
            f_signature  = inspect.signature(self._f)
            bound_f_signature = f_signature.bind(*args,**kargs)
            for param in f_signature.parameters.values():
                if not (param.name in bound_f_signature.arguments):
                    bound_f_signature.arguments[param.name] = param.default
            return bound_f_signature.arguments

        # If annotation checking is turned off at the class or function level
        #   just return the result of calling the decorated function
        # Otherwise do all the annotation checking
        if not self._checking_on and not self.checking_on:
            return self._f(*args, **kargs)
        try:
            param_arg_dict = param_arg_bindings()
            comp_result = self._f(*args, **kargs)
            if 'return' in self._f.__annotations__:
                param_arg_dict['_return'] = comp_result
            
            for k in self._f.__annotations__:
                if type(self._f.__annotations__[k]) == str:
                    self.check(k,self._f.__annotations__[k], param_arg_dict)
                
                elif k == 'return':
                    self.check(k, self._f.__annotations__[k], param_arg_dict['_return'])
                    
                else:
                    self.check(k, self._f.__annotations__[k], param_arg_dict[k])
            return comp_result
             
            # Check the annotation for each parameter (if there are any)
            
            # Compute/remember the value of the decorated function
            
            # If 'return' is in the annotation, check it
            
            # Return the decorated answer
            
            #remove after adding real code in try/except
            
        # On first AssertionError, print the source lines of the function and reraise 
        except AssertionError:
#             print(80*'-')
#             for l in inspect.getsourcelines(self._f)[0]: # ignore starting line #
#                 print(l.rstrip())
#             print(80*'-')
            raise




  
if __name__ == '__main__':     
    # an example of testing a simple annotation  
#     def f(x:[[str]]): pass
#     f = Check_Annotation(f)
#     f([['a',1],['c','d']]) 
# #     f('a')
#     def f(x : [[str]]): 
#         pass
#     f = Check_Annotation(f)
#     f([['a'],['c','d']])
#     def f(x : [str,int]): 
#         pass
#     f = Check_Annotation(f)
#     f(['a','b'])
#     def f(x : [(str,)]): pass
#     f = Check_Annotation(f)
#     f([(1,'b'),('c','d')])

#     def f(x:{str : int}): pass
#     f = Check_Annotation(f)
#     f({'a':'e', 'c': '23'})
#     f([('a',1),('b',2)])
 
#     def f(x:{str}): pass
#     f = Check_Annotation(f)
#     f({'a', 1})


#     def f(x:[lambda x : isinstance(x,int) and x>0]): pass
#     f = Check_Annotation(f)
#     f([1,'a'])
#     def f(x : {Check_All_OK(str,lambda x : len(x)<=3):Check_Any_OK(str,int)}): pass
#     f = Check_Annotation(f)
#     f({'a' : 1, 'b': 2, 'c':'c'})

#     def f(x : int) -> str: return 0
#     f = Check_Annotation(f)
#     f(2)

#     def f(x : 'x>=0'): pass
#     f = Check_Annotation(f)
#     f(1)
    class NotSupportProtocol: pass
    def f(x : NotSupportProtocol()): pass
    f = Check_Annotation(f)
    f(3)
    #driver tests

    import driver
    driver.default_file_name = 'bscp4S18.txt'
#     driver.default_show_exception= True
#     driver.default_show_exception_message= True
#     driver.default_show_traceback= True
    driver.driver()
