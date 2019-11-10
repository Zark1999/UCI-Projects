import map_api
import time

#################################################################
class get_steps:
    def get_result(self,dict_response:dict):
        '''print the result of directions'''
        print('DIRECTIONS')
        for each_location in dict_response['route']['legs']:
            for each_step in each_location['maneuvers']:
                print(each_step['narrative'])

class get_totaldistance:
    def get_result(self,dict_response:dict):
        '''print the result of total distance'''
        print('TOTAL DISTANCE: ' ,end='')
        distance = dict_response['route']['distance']
        print(int(distance),'miles')

class get_totaltime:
    def get_result(self,dict_response:dict):
        '''print the result of total time'''
        print('TOTAL TIME: ',end='')
        time_in_sec = dict_response['route']['time']
        time_in_min = int(time_in_sec/60)
        print(time_in_min, 'minutes')

class get_latlong:
    def get_result(self,dict_response:dict) -> list:
        '''get the result(a list) of lat and long'''
        latlong_list = []
        for each_location in dict_response['route']['locations']:
            latlong_list.append([str(each_location['latLng']['lat']),str(each_location['latLng']['lng'])])
        return latlong_list
            
class get_elevation:
    def get_result(self,dict_response:dict):
        '''print the result of elevation'''
        try:
            print(int(dict_response['elevationProfile'][0]['height']))
        except:
            print('Unknown')
    
#################################################################
def copyright_statement():
    print('Directions Courtesy of MapQuest; Map Data Copyright OpenStreetMap Contributors')


def print_latlongs(latlong_list:list):
    '''takes a latlong_list as an input and prints the lat and long'''
    print('LATLONGS')
    for each_lat_lng in latlong_list:
        lat = float(each_lat_lng[0])
        if lat < 0:
            lat = 0 - lat
            print_lat = '{:4.2f}'.format(lat) + 'S'
        else:
            print_lat = '{:4.2f}'.format(lat) + 'N'
            
        lng = float(each_lat_lng[1])
        if lng < 0:
            lng = 0 - lng
            print_lng = '{:4.2f}'.format(lng) + 'W'
        else:
            print_lng = '{:4.2f}'.format(lng) + 'E'
                
        print(print_lat,print_lng)


def check_route(direction_dict:dict):
    '''check whether the route is found'''
    if direction_dict['route']['routeError']['errorCode'] == 2:
        print('NO ROUTE FOUND')
        exit()

    
def manage_output(address_list:list, info_list:list):
    '''manages the final output of the program'''
    direction_url = map_api.get_url_direction(address_list)
    direction_dict = map_api.get_dict_feedback(direction_url)

    check_route(direction_dict)
    latlong_list = get_latlong().get_result(direction_dict)
    elevation_dict_list = []
    for each_latlong in latlong_list:
        elevation_url = map_api.get_url_elevation(each_latlong)
        elevation_dict_list.append(map_api.get_dict_feedback(elevation_url))
    
    info_dict = {'STEPS': get_steps(), 'TOTALDISTANCE': get_totaldistance(),
                 'TOTALTIME': get_totaltime(), 'ELEVATION': get_elevation()}

    for each_info in info_list:       
        if each_info != 'LATLONG':
            
            if each_info == 'ELEVATION':
                print('ELEVATIONS')
                for each_elevation_dict in elevation_dict_list:
                    info_dict[each_info].get_result(each_elevation_dict)
                    
            else:
                info_dict[each_info].get_result(direction_dict)
                
        else:
            print_latlongs(latlong_list)
        print()
        
    copyright_statement()
    

