import re
import prompt

def place_call(area_code,exchange,number):
    """
    returns a sequence of digits to dial
    Here are three examples of different type of numbers
      long distance: (949)824-2704
      local        : 824-2704
      inter-office : 4-2704
    The area_code is None, if the call is local or inter-office
    
    For long distance or local, we must dial a 9 first (to get an outside line)
    For a long-distance call, we must dial a 1 after the 9
    Here are three examples
      phone_call("949","824","2704") returns '919498242704'
      phone_call(None,"824","2704")  returns '98242704'
      phone_call(None,"4","2704")    returns '42704'
    """
    prefix = ("9" if area_code or len(exchange)>1 else "") + ("1" if area_code else "")
    area = (area_code if area_code else "")
    return prefix+area+exchange+number
   

def parse_phone_numbered(number):
    """
    Uses numbered groups: use this pattern in retester to see what the 5 groups do
    Notice how the arguments to place_call are specified: m.group(#)
    """
    pat = r"(\((\d{3})\))?((\d\d)?\d)-(\d{4})$"
    m = re.match(pat,number)
    assert m, number + " is not a legal phone number"
    return place_call(m.group(2), m.group(3),m.group(5))
      
def parse_phone_named(number):
    """
    Uses named groups: explicitly names the three groups needed
    Notice how the arguments to place_call are specified
      m.groupdict() puts the groups in a dict with their names and values
      ** says use the keys and values as parameter names and their arguments
      e.g., for f(**{'b':2, 'a':1, 'c':3} ) is translated into f(b=2,a=1,c=3)
    """
    pat = r"(\((?P<area_code>\d{3})\))?(?P<exchange>(\d\d)?\d)-(?P<number>\d{4})$"
    m = re.match(pat,number)
    assert m, number + " is not a legal phone number"
    return place_call(**m.groupdict())

parse_test = eval(prompt.for_string_via_index('Enter parse method', 'parse_phone_named',['parse_phone_numbered', 'parse_phone_named']))      
while True:
    try:
        phone = prompt.for_string("\nEnter phone number",default="(949)824-2704")
        print("  Dialing =",parse_test(phone))
    except:
        import traceback
        traceback.print_exc()
