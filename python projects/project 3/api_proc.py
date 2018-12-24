# Kuixi Ren 43838927
'''
consumer key: dMnJqiNHaEJGFPLqXyUTnsvyc8hC3KT5
consumer secret: HkmgqmqPJFl8nPML
&outFormat=json&ambiguities=ignore&routeType=fastest&doReverseGeocode=false&enhancedNarrative=false&avoidTimedConditions=false
'''

import urllib.request
import urllib.parse
import json
import datetime

API_KEY = 'dMnJqiNHaEJGFPLqXyUTnsvyc8hC3KT5'
D_BASE_URL = 'http://open.mapquestapi.com/directions/v2/route?'
E_BASE_URL = 'http://open.mapquestapi.com/elevation/v1/profile?'

def get_response_from_list(L: list) -> dict:
    """Construct json dict from a list of locations"""
    return _get_response(make_D_url(L))


def make_D_url(locations: list) -> str:
    '''locations is a list of destinations in order, returns the DIRECTION web url for api'''
    return D_BASE_URL + urllib.parse.urlencode(_make_location_parameters(locations))


def make_E_url(longlat: tuple) -> str:
    '''returns url for ELEVATION, input is one location pair'''
    return E_BASE_URL + 'key=' + API_KEY + '&latLngCollection=' + str(longlat[0]) + ',' + str(longlat[1]) + '&unit=f'


# PARSINGJSON

def get_t_distance(json:dict) -> str:
    '''return a formatted string about the total distance'''
    return 'TOTAL DISTANCE: {} miles\n'.format(round(json['route']['distance']))

def get_t_time(json:dict) -> str:
    """return a formatted string about the total time"""
    format_time = json['route']['formattedTime']
    h, m, s = format_time.split(':')
    return 'TOTAL TIME: {} minutes\n'.format(round(datetime.timedelta(hours=int(h), minutes=int(m), seconds=int(s)).total_seconds() / 60))



def get_latlong(json: dict) -> str:
    '''return a formatted string about latlong of each location'''
    latlong_list = _read_latlong(json)
    string = 'LATLONGS\n'
    for pair in latlong_list:
        NorS = 'N'
        EorW = 'E'
        if pair[0] < 0:
            NorS = 'S'
        if pair[1] < 0:
            EorW = 'W'
        string += '{:.2f}{} {:.2f}{}\n'.format(abs(pair[0]), NorS, abs(pair[1]), EorW)
    return string

def get_elevation(json:dict) -> str:
    """return a formatted string about each locations elevation"""
    string = 'ELEVATIONS\n'
    L = _read_latlong(json)
    for pair in L:
        try:
            e_API = _get_response(make_E_url(pair))
            string += '{}\n'.format(round(e_API['elevationProfile'][-1]['height']))
        except:
            string += 'UNKNOWN\n'
    return string

def get_status(json:dict) -> int:
    '''returns the status code'''
    return json['info']['statuscode']


def analyze_s_code(num: int) -> str:
    '''interpret the MAPQUEST status code'''
    invalid = [400,401,402,605,606,610,609,612]
    if num in invalid:
        return 'NO ROUTE FOUND'
    elif num == 611:
        return 'You must specify at least two locations to run this experiment.'
    else:
        return 'MAPQUEST ERROR'



def get_steps(json:dict) -> str:
    '''returns a formatted string about the how to get from location to location'''
    string = 'DIRECTIONS\n'
    for leg in json['route']['legs']:
        for maneuver in leg['maneuvers']:
            string += '{}\n'.format(maneuver['narrative'])

    return string



# private

def _read_latlong(json: dict) -> list:
    """from a json return the list of latlong pairs of each destination"""
    result = []
    for item in json['route']['locations']:
        result.append((item['latLng']['lat'],item['latLng']['lng']))
    return result


def _make_location_parameters(locations: list) -> [tuple]:
    '''from a list of locations return a list of tuples with keys and locations'''
    result = []
    result.append(('key', API_KEY))
    result.append(('from', locations[0]))
    for ind in range(len(locations)-1):
        result.append(('to', locations[ind+1]))
    return result

def _get_response(url: str) -> dict:
    """from the url returns the result of the webpage"""
    response = urllib.request.urlopen(url)
    json_text = response.read().decode(encoding = 'utf-8')
    response.close()
    return json.loads(json_text)




#print(make_D_url(['4533 Campus Dr, Irvine, CA', '1111 Figueroa St, Los Angeles, CA', '3799 S Las Vegas Blvd, Las Vegas, NV']))
#x = get_response(make_D_url(['4533 Campus Dr, Irvine, CA', '1111 Figueroa St, Los Angeles, CA', '3799 S Las Vegas Blvd, Las Vegas, NV']))
#print(type(x))

#print(get_elevation(x))
#print(get_status(x))
# print(_read_latlong(get_response_from_list(['4533 Campus Dr, Irvine, CA', '1111 Figueroa St, Los Angeles, CA', '3799 S Las Vegas Blvd, Las Vegas, NV'])))
#print(make_E_url((33.64864, -117.831271)))
