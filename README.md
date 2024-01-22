# SEHList

This list does not require the use of a mutex in a multi-threaded environment.  
Using virtualalloc and seh, I made it possible to use lists without critical section exceptions.  
Please turn on EHa in project properties.

mutex를 사용하지 않아도 멀티쓰레드 환경에서 익셉션이 발생하지 않도록, SEH와 alloc을 사용한 SEHList를 구성했습니다.  
프로젝트에서 EHa 세팅을 키셔야 정상적으로 사용 가능합니다.

# HOW IT WORKS
Usually, exceptions occur because the same variable is used by multiple threads.  
To prevent this, I allocated a dynamic memory address and write, read values ​​directly to the allocated memory address to avoid using the same variable.  
By using SEH, hardware exceptions are also prevented, preventing crashes in unexpected situations.

보통 익셉션은, 같은 변수를 여러 쓰레드에서 사용하기 때문에 발생합니다.  
이를 방지하기 위해, 동적 메모리 주소를 할당하고 할당된 메모리 주소에 직접 값을 write, read 하여 같은 변수를 사용하지 않도록 만들었습니다.  
SEH를 사용하여 하드웨어 익셉션 또한 방지하여, 예기치 못한 상황에서의 충돌 또한 막았습니다.

# HOW TO USE
```
SEHList<class or struct> listName(maxSize); //max size = max index
SEHList<class or struct> listUsingCodeCave(codeCave, sizeOfCodeCave); //using the codeCave (so to not use VirtualAlloc & VirtualFree)
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
bool isValid = !listName; //check is successfully alloced.

for(auto object : listName) // or you can use for(const auto object : listName)
{
//this list support iterator, so you can use begin() and end().
}
```

# LICENSE

ⓒ 2024 konmio616 <<konmio616@naver.com>>

# THANKS TO

chat-gpt helped me by suggesting iterator-related ideas and coding iterator class.
