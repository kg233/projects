# Kuixi Ren 43838927
# This file creates the classes for the outputs

import api_proc


class STEPS:
    '''class for steps'''
    def fetch(self, json):
        return api_proc.get_steps(json)

class TOTALDISTANCE:
    '''class for totaldistance'''
    def fetch(self, json):
        return api_proc.get_t_distance(json)

class TOTALTIME:
    '''class for totaltime'''
    def fetch(self, json):
        return api_proc.get_t_time(json)

class LATLONG:
    '''class for latlong'''
    def fetch(self, json):
        return api_proc.get_latlong(json)


class ELEVATION:
    '''class for elevation'''
    def fetch(self, json):
        return api_proc.get_elevation(json)



def exec_fetch(L: ['fetch'], json):
    '''prints the return value of each object's fetch'''
    for item in L:
        print(item.fetch(json))



# json = api_proc.get_response_from_list(['4533 Campus Dr, Irvine, CA', '1111 Figueroa St, Los Angeles, CA', '3799 S Las Vegas Blvd, Las Vegas, NV'])
# x = [STEPS(), LATLONG(), ELEVATION()]
#
# exec_fetch(x, json)
