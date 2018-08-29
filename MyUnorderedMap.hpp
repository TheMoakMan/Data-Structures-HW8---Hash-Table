const std::string EMPTY = "EMPTY";
const std::string TOMB = "DELETED";

/*~~Extra Functions~~*/
void get_identity(std::string my_id[])
{
  my_id[0] = "ebmy83";
  my_id[1] = "12422230";	
	
}

/*~~MyUnorderedMap Functions~~~*/
template <typename K, typename V>
MyUnorderedMap<K, V>::MyUnorderedMap()
{
  data_size = reserved_size = 0;
  m_data = NULL;  
}

template <typename K, typename V>		
MyUnorderedMap<K, V>::~MyUnorderedMap()
{
  delete[] m_data;
}

template <typename K, typename V>
MyUnorderedMap<K, V>::MyUnorderedMap(const MyUnorderedMap<K, V> &source)
{
  //Copy the maximum and data sizes of the source vector
  reserved_size = source.reserved_size;
  data_size = source.data_size;
  
  //Create an array to be copied into
  m_data = new MyPair<K, V>[reserved_size];
  
  //Assign each element
  for(int i = 0; i < reserved_size; i++){
	m_data[i].first = source.m_data[i].first;
	m_data[i].second = source.m_data[i].second;
  }	
	
}

template <typename K, typename V>
MyUnorderedMap<K, V> & MyUnorderedMap<K, V>::operator=(const MyUnorderedMap<K, V> &source)
{
  //Delete the current array before assigning
  delete[] m_data;
  
  //Copy and Max and Data size of the source
  reserved_size = source.reserved_size;
  data_size = source.data_size;
  
  //Create a new array to be copied into
  m_data = new MyPair<K, V>[reserved_size];
  
  //Assign each element
  for(int i = 0; i < reserved_size; i++){
	m_data[i].first = source.m_data[i].first;
    m_data[i].second = source.m_data[i].second;	
  }
  
  return *this;
}

template <typename K, typename V>
V & MyUnorderedMap<K, V>::at(const K &key)
{
  MyPair<K, V> *temp = find(key);	
  
  if(temp != nullptr){
	return (temp->second);
  } 
  else{
	throw std::out_of_range("KEY DOES NOT EXIST WITHIN BOUNDS");  
  }	
}

template <typename K, typename V>
V & MyUnorderedMap<K, V>::operator[](const K &key)
{
  MyPair<K, V> *temp = find(key);
  
  //If key maps to an element in the array, return reference to it's paired value
  //If key does not map to any element in the array but instead to an EMPTY element or TOMB
  //then insert that value 
  if(temp != nullptr)
	return (temp->second);
  else{
    MyPair<K, V> tempPair(key);
    insert(tempPair);
    return (*this)[key];
  }	
}

template <typename K, typename V>
bool MyUnorderedMap<K, V>::empty() const
{
  return !data_size;
}

template <typename K, typename V>
int MyUnorderedMap<K, V>::size() const
{
  return data_size;
}

template <typename K, typename V>
void MyUnorderedMap<K, V>::clear()
{
  data_size = 0;
  reserved_size = 0;
  
  delete[] m_data;
  m_data = NULL;
}

template <typename K, typename V>
void MyUnorderedMap<K, V>::insert(const MyPair<K, V> &init_pair)
{
  if(reserved_size == 0)
	reserve(2);  
  
  int startPos = hash(init_pair.first);
  int currPos = startPos;
    
  //Insert at the startPos based on the hash function output
  //If that position if already filled, then iterate to the next open position
  //using the probing sequence (Linear in this case) i = Linear Probe Counter
  //Duplicates are prohibited
  int i = 1; 
  while(m_data[currPos].first != EMPTY && m_data[currPos].first != TOMB && m_data[currPos].first != init_pair.first){
	currPos = (startPos + i) % reserved_size; 
	i++;
  }
  
  //Insert the Pair into the Table
  m_data[currPos].first = init_pair.first;
  m_data[currPos].second = init_pair.second;
  
  //Increase Data Size
  data_size++;
  
  //Check the data_size vs reserved_size and adjust if needed
  if(static_cast<double>(data_size) / reserved_size >= 0.6)
	reserve(reserved_size * 2);
  else if(static_cast<double>(data_size) / reserved_size <= 0.1)
	reserve(reserved_size * 0.3);
}

template <typename K, typename V>
void MyUnorderedMap<K, V>::erase(const K &key)
{	
  //Iterate through the Hash Table until the key is found or is not found.
  //In this case, Linear Probing is used. 
  MyPair<K, V> *tempPair = find(key);
  
  //If the key is found, remove it, if not then return.
  if(tempPair != nullptr){
	tempPair->first = TOMB;
    data_size--;	
  }
  else{
	return;  
  }
  
  //Check if data_size has gone under 10% of reserved_size and shrink if so.
  if(static_cast<double>(data_size) / reserved_size <= 0.1)
	reserve(reserved_size * 0.3);
}

template <typename K, typename V>
MyPair<K, V> * MyUnorderedMap<K, V>::find(const K &key) const
{
  if(reserved_size == 0)
    return nullptr;	  
	
  int startPos = hash(key);
  int currPos = startPos;
	
  //Iterate through the Hash Table until the key is found or is not found.
  //In this case, Linear Probing is used. 
  int i = 1;
  while(m_data[currPos].first != key && m_data[currPos].first != EMPTY){
	currPos = (startPos + i) % reserved_size;  
	i++;
  }
  
  //If the key is found, return a pointer to the MyPair, if not then return a Null Pointer.
  if(m_data[currPos].first == key){
    return &m_data[currPos];	
  }	
  
  return nullptr;
}

template <typename K, typename V>
void MyUnorderedMap<K, V>::print() const
{
  cout << "[";
  for(int i = 0; i < reserved_size; i++){
	if(m_data[i].first != EMPTY && m_data[i].first != TOMB){
	  cout << "(" << m_data[i].first << ": " << m_data[i].second << "), ";
	}
  }
  
  cout << "]";
}

template <typename K, typename V>
int MyUnorderedMap<K, V>::count(const K &key) const
{
  if(find(key) != nullptr)
    return 1;
  else
	return 0;
}

template <typename K, typename V>
void MyUnorderedMap<K, V>::reserve(int new_cap)
{
  int tempSize = 0;
  
  
  //Create a temp array to store the current hash table contents
  MyPair<K, V> *tempArray = new MyPair<K, V>[reserved_size];
  
  //Copy the hash table values into a temp array.
  for(int i = 0; i < reserved_size; i++){
  	if(m_data[i].first != EMPTY && m_data[i].first != TOMB){
	  tempArray[i].first = m_data[i].first;
	  tempArray[i].second = m_data[i].second;
	  
	  tempSize++;
    }
  }	
  
  //Delete the old array.
  reserved_size = new_cap;
  data_size = 0;
  delete[] m_data;
	
  //Allocate a new array with the updated size
  m_data = new MyPair<K, V>[reserved_size];
  
  //Set each unused bucket to EMPTY.
  for(int k = 0; k < reserved_size; k++){
	m_data[k].first = EMPTY;
  }
  
  //Rehash the old array into the new hash table.
  for(int l = 0; l < tempSize; l++){
    insert(tempArray[l]);
  }
  
  delete[] tempArray;
}

//Implemented for Strings
template <typename K, typename V>
int MyUnorderedMap<K, V>::hash(const K &key) const
{
  int sum = 0;
  
  //Adds the ACII value of each character to sum. 
  for(int i = 0; i < key.length(); i++){
	sum += key[i];         
  }
  
  return (sum % reserved_size);
}