
import api_proc
import class_obj

def run():
    location_list = _create_list_of_location()
    request_list = _create_list_of_request()
    obj_request_list = _transform(request_list)
    print()
    try:
        json = api_proc.get_response_from_list(location_list)
        if api_proc.get_status(json) != 0:
            print(api_proc.analyze_s_code(api_proc.get_status(json)), end = '')
            return
        class_obj.exec_fetch(obj_request_list, json)
    except: # Any error that occurs in the try statement would be considered Mapquest error, so I think there is
            # no need to specify errors.
        print('MAPQUEST ERROR', end = '')
        return
    _copyright()
    return

def _create_list_of_request() -> list:
    '''starts with empty list, prompts user to enter a number n, then adds n requests chosen by the suer to the list'''
    result = []
    number = _ask_how_many()
    for time in range(number):
        result = _ask_for_strings(result)
    return result

def _create_list_of_location() -> list:
    '''starts with empty list, prompts user to enter a number n, then adds n locations chosen by the user to the list'''
    result = []
    number = _ask_how_many()
    for time in range(number):
        result = _ask_for_strings(result)
    return result

def _ask_how_many() -> int:
    '''returns the the number the user enters'''
    return int(input())


def _ask_for_strings(L: list) -> list:
    '''ask for a string, returns a list with the location stored in as a string'''
    location = str(input())
    l = L
    l.append(location)
    return l



def _transform(L: list) -> list:
    '''returns a list of fetch object from a list of strings'''
    result = []
    words = ['STEPS', 'LATLONG', 'TOTALDISTANCE', 'TOTALTIME', 'ELEVATION']
    classes = [class_obj.STEPS(), class_obj.LATLONG(), class_obj.TOTALDISTANCE(), class_obj.TOTALTIME(), class_obj.ELEVATION()]
    for item in L:
        num = words.index(item)
        result.append(classes[num])
    return result

def _copyright():
    '''prints a copyright statement'''
    print('Directions Courtesy of MapQuest; Map Data Copyright OpenStreetMap Contributors.', end = '')
    

if __name__ == '__main__':
    run()

