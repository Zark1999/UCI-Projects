import urllib.parse
import urllib.request
import json

#   http://open.mapquestapi.com/directions/v2/route?
#   key=UZb0GShXqFizBKMWgEq5LDlU5WT7aBJ2
#   &from=Irvine%2CCA&to=Los+Angeles%2CCA

BASE_DIRECTION_URL = 'http://open.mapquestapi.com/directions/v2'

#   http://open.mapquestapi.com/elevation/v1/profile?
#   key=UZb0GShXqFizBKMWgEq5LDlU5WT7aBJ2
#   &shapeFormat=raw&latLngCollection=39.74012,-104.9849,39.7995,-105.7237,39.6404,-106.3736

BASE_ELEVATION_URL = 'http://open.mapquestapi.com/elevation/v1'

MAP_API_KEY = 'UZb0GShXqFizBKMWgEq5LDlU5WT7aBJ2'

def get_url_direction(address_list:list) -> str:
    '''returns the url of the direction part'''
    parameters = [('key', MAP_API_KEY)]
    parameters.append(('from', address_list[0]))
    
    for address_num in range(1,len(address_list)):
        parameters.append(('to', address_list[address_num]))
        
    return BASE_DIRECTION_URL + '/route?' + urllib.parse.urlencode(parameters)


def get_url_elevation(latlong_list:list) -> str:
    '''returns the url of the elevation part'''
    parameters = [('key',MAP_API_KEY),('inFormat','kvp'),('shapeFormat','raw'),('unit','f')]
    latlong_str = ','.join(latlong_list)
    parameters.append(('latLngCollection',latlong_str))

    elevation_url =  BASE_ELEVATION_URL + '/profile?' + urllib.parse.urlencode(parameters)
    return elevation_url
    
def get_dict_feedback(url:str) -> dict:
    '''connnect the url and decode json text, returns a dictionary'''
    feedback = None    
    try:
        feedback = urllib.request.urlopen(url)
        plain_text = feedback.read().decode(encoding = 'utf-8')
        decoded_text = json.loads(plain_text)
        return decoded_text

    except:
        print('MAPQUEST ERROR')
        exit()
    
    finally:
        if feedback != None:
            feedback.close()

    
