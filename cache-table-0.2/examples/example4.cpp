
#include <iostream>

#include <mm/cache_set.hpp>

/** 
 * This example shows how to use the discard function.
 * 
 * The discard function is used when there is a key collision, in 
 * this case the old element is removed and passed to the discard
 * function.
 * 
 * The function can be used to verify discarded items, save them 
 * into another level of cache (eg: on disk) or just ignore them. 
 * 
 */
 
struct DiscardLog
{
    void operator() ( const std::string& old_value, 
                      const std::string& new_value )
    {
        std::cout << "Replacing '" << old_value 
                  << "' ==> '" << new_value << "'" << std::endl;
    }
};

typedef mm::cache_set< std::string,                // Value
                       mm::hash<std::string>,      // HashFunction
                       std::equal_to<std::string>, // KeyEqual
                       DiscardLog                  // DiscardFunction
                     > Set;

int main()
{
    Set set;
    set.resize( 10 );
    
    set.insert( "kiwi" );
    set.insert( "plum" );
    set.insert( "apple" );
    set.insert( "mango" );
    set.insert( "apricot" );
    set.insert( "banana" );
    set.insert( "peer" );
    set.insert( "melon" );
    set.insert( "passion fruit" );
    set.insert( "pineapple" );

    std::cout << std::endl << " - All the elements - "  << std::endl;

    Set::const_iterator it;
    for ( it = set.begin(); it != set.end(); ++it )
        std::cout << *it << " ";

    std::cout << std::endl;
}
