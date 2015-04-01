#ifndef DOUBLE_HASH_TABLE_H
#define DOUBLE_HASH_TABLE_H



 #include <exception>
 #include <typeinfo>

using namespace std;
enum state { EMPTY, OCCUPIED, DELETED };

template<typename T>
class DoubleHashTable {
	private:
		int count;
		int power;
		int array_size;
		T *array;
		state *array_state;

		int h1( T const & ) const; // first hash function
		int h2( T const & ) const; // second hash function

	public:
		DoubleHashTable( int = 5 );
		~DoubleHashTable();
		int size() const;
		int capacity() const;		
		bool empty() const;
		bool member( T const & ) const;
		T bin( int ) const;

		void print() const;

		void insert( T const & );
		bool remove( T const & );
		void clear();
};

template<typename T >
DoubleHashTable<T >::DoubleHashTable( int m ):				                           //Constructor 
count( 0 ), power( m ),
array_size( 1 << power ),
array( new T [array_size] ),
array_state( new state[array_size] ) {

	for ( int i = 0; i < array_size; ++i ) {
		array_state[i] = EMPTY;
	}
}

template<typename T >
DoubleHashTable<T >::~DoubleHashTable() {					                        //Deleted both the arry and array_state arrays to free up memory
	
	delete[] array ; 
	delete[] array_state; 
}

template<typename T >
int DoubleHashTable<T >::size() const {						                        // Returns count, the number of elemnts currently in the table
    
	return count;
}

template<typename T >
int DoubleHashTable<T >::capacity() const {					                        // Returns the size of the array, ie the max number of elemnets that can be stored
    
	return array_size;
}



template<typename T >
bool DoubleHashTable<T >::empty() const {				                        	//checks count if it is under 1, then the array is empty 
    
	if (count<1){
		return true;
	}
	return false;
}

template<typename T >
int DoubleHashTable<T >::h1( T const &obj ) const {			                        //First hash function 
	

	if((obj) <0){
		return (static_cast<int>(obj)%array_size +array_size)%array_size; 	        // If the initial value of obj is negative apply the given hash function + array_size
	}   
	return static_cast<int>(obj)%array_size; 								        //If the intial value is postivie, just apply the hash function 

}

template<typename T >
int DoubleHashTable<T >::h2( T const &obj ) const {							        // Ssecond hash function
	
	int hash2 = 0 ; 
	if(static_cast<int>(obj) < 0){
		hash2= ((int)((static_cast<int>(obj)/array_size))%array_size +array_size); 	// If the obj is negative apply the second hash function + array_size
	}else{ 
		hash2 =((int)((static_cast<int>(obj))/array_size)%array_size) ; 			// If the obj is positive, apply the second hash function . 
	}

	if(hash2%2 == 0 ){ 																// If the hash ends up being even, add one to the hash 
		hash2++;
	}
	return hash2; 

}

template<typename T >
bool DoubleHashTable<T >::member( T const &obj ) const {							//Checks if a value is in the array. 
	
	if(count<1){																	// If the array is empty, return 0 
		return false;
	}else if (array[h1(obj)]==obj){													// If the hash1 location stores the value, return true
		return true;
	}else if(array[h1(obj)] != obj){
		int offset = h2(obj);
		int counter = 0 ; 															// If the hash1 location does not store the value, check all possible hash2 locations 
		while(counter< array_size){
			if(array[(h1(obj)+offset)%array_size]== obj && array_state[(h1(obj)+offset)%array_size]==OCCUPIED){
				return true;
			}
			offset= offset + h2(obj);
			counter++;
		}
	}
	return false;																	// If the value cant be found, return false 
}

template<typename T >
T DoubleHashTable<T >::bin( int n ) const {                    						//return the contents of the desired bin
	return  array[n];
}

template<typename T >
void DoubleHashTable<T >::insert( T const &obj ) {									//Inserts an object into the array 
	 
	if (count >= array_size) { 														//If the array is full, throw an overflow exception 
		throw ; 
	}else if(array_state[h1(obj)]==OCCUPIED){										//If the hash1 location is occupied, use the hash2 and 
		int loc=0; 																	//the principles of double hash to find an appropriate location 
		while( array_state[(h1(obj)+loc)%array_size]==OCCUPIED){
			loc = loc + h2(obj) ;
		}
		array[(h1(obj)+loc)%array_size] = obj;
		array_state[(h1(obj)+loc)%array_size] = OCCUPIED;
		count++; 

		
	}else{
		array[h1(obj)]=obj;															// If the hash1 value is not occupied, place the obj into the hash1 location. 
		count++;
		array_state[h1(obj)]= OCCUPIED; 
	}


	 return ; 
}

template<typename T >
bool DoubleHashTable<T >::remove( T const &obj ) {									// Remove an object from the table 
	
	
	if(array_size<1){																//if the table is empty, return false
		return false;
	}else if (array[h1(obj)]==obj && array_state[h1(obj)]==OCCUPIED){				// if the hash1 locatino of the obj contains obj, delete obj 
		array_state[h1(obj)]=DELETED;
		count --; 
		array[h1(obj)] = -666;
		return true;
	}else if(array[h1(obj)] != obj){												//If the hash1 location of the obj does not contain obj use hash2 to continue the search
		int offset = h2(obj);
		int counter = 0;
		
		while(counter<= array_size){
			if(array[(h1(obj)+offset)%array_size]== obj && array_state[(h1(obj)+offset)%array_size]==OCCUPIED){
				array_state[(h1(obj)+offset)%array_size]= DELETED;
				array[(h1(obj)+offset)%array_size]= -66666; 
				count --; 
				return true;
			}
			offset= offset + h2(obj);
			counter++;
		}
	}

	return false;


}

template<typename T >
void DoubleHashTable<T >::clear() {                                                 //Creates new array state to clear everything 
	 delete[] array_state;
	 array_state= new state[array_size];
	 count = 0; 	
	 return ; 
}

#endif
