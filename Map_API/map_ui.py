import output_handling

def main_program():
    '''main part of the program'''
    address_list = query_address()
    info_list = query_info()
    check_query_info(info_list)
    print()
    output_handling.manage_output(address_list,info_list)
    print()
    


def query_address() -> list:
    '''get the user input about address and store them in a list, then return the list'''
    address_list = []
    try:
        num_of_address = int(input())
        if num_of_address < 2:
            print('You must specify at least two locations to run this experiment.')
            exit()
    except ValueError:
        print('The first line must specify a positive integer number of locations.')
        exit()
        
    for each_address in range(num_of_address):
        address = input()
        address_list.append(address)
    return address_list


def query_info() -> list:
    '''get the user input about info and store them in a list, then return the list'''
    info_list = []
    
    try:
        num_of_info = int(input())
    except ValueError:
        print('There must be a positive integer number of generators.')
        exit()
    
    for each_info in range(num_of_info):
        info = input()
        info_list.append(info)
    return info_list


def check_query_info(info_list:list):
    '''check whether the query list is valid'''
    for each_info in info_list:
        if each_info.strip() == '':
            print('Invalid output type: undefined')
            exit()
            
        elif each_info not in ['ELEVATION','LATLONG','STEPS','TOTALTIME','TOTALDISTANCE']:
            print('Invalid output type:',each_info)
            exit()


if __name__ == '__main__':
    main_program()
