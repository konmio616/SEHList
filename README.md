# SEHList

This list does not require the use of a mutex in a multi-threaded environment.
Using virtualalloc and seh, I made it possible to use lists without critical section exceptions.
Please turn on EHa in project properties. This project is for x64 project.

mutex를 사용하지 않아도 멀티쓰레드 환경에서 익셉션이 발생하지 않도록, SEH와 alloc을 사용한 SEHList를 구성했습니다.
프로젝트에서, EHa 세팅을 키셔야 정상적으로 사용 가능합니다.

# HOW TO USE

  SEHList<class or struct> listName(maxSize); //max size = max index
  auto object = listName[index]; //get object
  listName.add(object); //add object
  listName.remove(index); //remove object
  listName.exchange(index, object); //exchange the object at that index
  listName.clear(); //clear the list
  size_t listSize = listName.size(); //get the size of list

  bool isEmpty = listName.empty(); //check is list empty
  bool isFound = listName.find(object); //find the object and return result
  bool isEqual = listName == otherList; //check list is equal or not
  listName = otherList; //copy otherList to list. original list will be cleared.

  for(auto object : listName) // or you can use for(const auto object : listName)
  {
    //this list support iterator, so you can use begin() and end().
  }

# LICENSE

ⓒ 2024 konmio616 <<konmio616@naver.com>>

# THANKS TO

chat-gpt helped me by suggesting iterator-related ideas and coding iterator class.
