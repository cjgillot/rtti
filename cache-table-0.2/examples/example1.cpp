
#include <iostream>

#include <mm/cache_map.hpp>

typedef mm::cache_map<std::string,double> Map;

int main ( int argc, char * const argv[] ) 
{
    // Create the map.
    Map map;

    // Insert an item in the map. Map::value_type is an alias to, in
    // this case, std::pair<std::string,double>.
    map.insert( Map::value_type( "ciao", 11.0 ) );    
    
    // Like std::map, this is the alternate form for inserting items
    // in the map.
    map[ "prova" ] = 1.5;
    map[ "hola" ] = 3.1;
    map[ "2" ] = 2.2;

    // Be careful that, as in std::map, if you look for a key that
    // doesn't exists, it will be inserted with default value.
    // To avoid this behaviour, use the insert() method to add item,
    // and find() to retrieve them.
    std::cout << "Printing a not existing item: "
              << map[ "not existant key" ] << std::endl;

    Map::iterator it = map.find( "test_key" );
    std::cout << "Looking for 'test_key': "
              << ( it == map.end() ? "not found" : "found" )
              << std::endl;

    for ( Map::iterator it = map.begin(); it != map.end(); ++it  )
    {
        std::cout << "map[ '" << it->first
                  << "' ] = " << it->second << std::endl;
    }

    std::cout << "Items: " << map.size() << std::endl;
    std::cout << "Key collisions: " << map.num_collisions() << std::endl;

    std::cout << std::endl
              << "Map empty key: '" << map.get_empty_key() << "'" << std::endl;
}
