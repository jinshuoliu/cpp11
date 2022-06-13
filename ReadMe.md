# C++2.0新特性

## 1. Variadic template

***可变参数模板***

***template关于：**

- function template
- class template

***变换的是template parameters***

- 参数个数
- 参数类型

**语法:** ...

```cpp
// 这里...表示数量任意，它就是一个包
template <typename T,  typename... Types>
void printX(const T& firstArg, const Type&... args)
{
  cout << first Arg << endl;
  printX(args...); // 这样就可以实现递归操作
}
// 关于它的调用函数的优先级
template <typename T,  typename... Types>
void printX(const Type&... args) {
  /*
  此时，它和上面是同名函数，而上面调用的printX函数要进入哪个函数呢？
  它要看给它的参数和这两个函数的参数的匹配程度，从目前的来看，这个函数更加匹配
  */
}
```

```cpp
// 类模板的例子
template <typename... Args>
ostream& operator<<(ostream& os, const tuple<Args...> &t) {
  os << "[";
      PRINT_TUPLE<0,sizeof...(Args),Args...>::print(os, t);
  return os << "]";
}

template <int IDX, int MAX, typename... Args>
struct PRINT_TUPLE {
  static void print(ostream& os, const tuple<Args...>& t) {
    os << get<IDX>(t)<<(IDX+1==MAX?"":",");
    PRINT_TUPLE<IDX+1, MAX, Args...>::print(os, t);
  }
};

template <int MAX, typename... Args>
struct PRINT_TUPLE<MAX, MAX, Args...> {
  static void print(std::ostream& os, const tuple<Args...>& t) {

  }
};
```

```cpp
template <typename... Values> class tuple;
template<> class tuple<> {};

// 一个非常好的递归继承的例子
template <typename Head, typename... Tail>
class tuple<Head, Tail...> : private tuple<Tail...> {
  typedef tuple<Tail...> inherited;
protected:
  Head m_head;
public:
  tuple() {}
  tuple(Head v, Tail... vtail):m_head(v), inherited(vtail...){ }

  auto head()->decltype(m_head) {
    return m_head;
  }

  inherited& tail() {
    return *this;
  }
};

// 一个递归复合的例子
template <typename Head, typename... Tail>
class tuple<Head, Tail...> {
  typedef tuple<Tail...> composited;
protected:
  composited m_tail;
  Head m_head;
public:
  tuple() {}
  tuple(Head v, Tail... vtail):m_tail(vtail...), m_head(v){ }

  Head head() {
    return m_head;
  }

  inherited& tail() {
    return m_tail;
  }
};

/*
这两个例子看上去有什么区别吗？
这两个类里面包含的东西是不一样的
  第一个只包含一个Head，但是它向上继承父类的东西(也就是一堆Head)
  第二个包含一个Head和一个tuple<Tail...>
*/
```

## 2. nullptr and std::nullptr_t

C++11允许使用nullptr代替0或者NULL

std::nullptr_t定义在< cstddef >中,它就是nullptr的一个别名

```cpp
typedef decltype(nullptr) nullptr_t;
```

## 3. auto

自动推导右值的类型，将左值的类型指定为它

```cpp
auto t = 15; // t has type int
double f();
auto d = f(); // d has type double
```

## 4. Uniform Initialization

一致性初始化，直接在变量的后面加上大括号，里面存放初值

```cpp
int values[] {1,2,3};
vector<int> v{1,2,3,4,5,6};
vector<string> cities{"shanghai", "guangzhou", "wuhan", "sichuan"};
```

它利用的规则：

- 编译器看到大括号的内容，就会做出一个initializer_list< T >
  - 它关联到array<T, n>
- 调用函数时，该array内的元素可能会被编译器分解传给函数

### 4.1. Initializer List

```cpp
void print(std::initializer_list<int> vals)
{
  for(auto vals.begin(); p!=vals.end(); ++p)
    std::cout << *p << "\n";
}
print({1,2,3,4,5,6,7}); // 参数个数不一定，还没使用容器(其实它和容器也很像了)
```

## 5. explicit

explicit for ctors taking more than one argument

它主要用在构造函数上面，意为只有明确的调用才行，编译器不能自己决定调用某个构造函数

## 6. =default,=delete

圣经中有对它们的详解

=delete:就是不要它

- 如果将相关的构造函数使用它来修饰，那么编译器就无法调用此类构造函数，会导致无法拷贝等现象

=default:就是要编译器给的默认的那个

## 7. Alias Template

别名模板

```cpp
template <typename T>
using Vec = std::vector<T, MyAlloc<T>>;

// 使用
Vec<int> coll;
// 相当于
std::vector<int, MyAlloc<int>> coll;

// 想使用define来代替？
#define Vec<T> template<typename T> std::vector<T,MyAlloc<T>>;
// 再使用的时候
Vec<int> coll;
// 相当于
template<typename int std::vector<int,MyAlloc<int>> coll;// 这个效果合适吗？
```

***显示一下它的功效***

```cpp
/* 1.提出问题 */
// 想要做出一个程序来查看效率
// 该函数接受一个容器和一个元素类型，然后通过它们定义出一个实际的容器，用来进行测试
void test_moveable(Container cntr, T elem)
{
  Container<T> c;

  for(long i=0;i<SIZE;++i)
    c.insert(c.end(),T());
  
  output_static_data(T());
  Container<T> c1(c);
  Container<T> c2(std::move(c));
  c1.swap(c2);
}
// 测试
test_moveable(list, MyString);
test_moveable(list, MyStrNoMove);
// 报错：函数声明无效、Container和T不在声明范围内

/* 2.改进1 */
// 使用函数模板，想要得到类型
template<typename Container, typename T>
void test_moveable(Container cntr, T elem)
{
  Container<T> c;

  for(long i=0;i<SIZE;++i)
    c.insert(c.end(),T());
  
  output_static_data(T());
  Container<T> c1(c);
  Container<T> c2(std::move(c));
  c1.swap(c2);
}
// 测试，传进去的都是类型
test_moveable(list(), MyString());
test_moveable(list(), MyStrNoMove());
// 报错：Container不是模板

/* 3.改进2 */
// 告诉编译器它就是模板
template<typename Container, typename T>
void test_moveable(Container cntr, T elem)
{
  typename Container<T> c;

  for(long i=0;i<SIZE;++i)
    c.insert(c.end(),T());
  
  output_static_data(T());
  Container<T> c1(c);
  Container<T> c2(std::move(c));
  c1.swap(c2);
}
// 测试
test_moveable(list(), MyString());
test_moveable(list(), MyStrNoMove());
// 报错：expected nested-name-specifier before 'Container'
// 就是typename后面需要有 xx::xx 的结构

/* 4.改进3 */
// 牺牲掉调用端的灵活性
template<typename Container>
void test_moveable(Container c)
{
  typedef typename iterator_traits<typename Container::iterator>::value_type Valuetype;

  for(long i=0;i<SIZE;++i)
    c.insert(c.end(),Valuetype());
  
  output_static_data(*(c.begin()));
  Container<T> c1(c);
  Container<T> c2(std::move(c));
  c1.swap(c2);
}
// 测试
test_moveable(list<MyString>());
test_moveable(list<MyStrNoMove>());
// 缺点：它的基础是必须有iterator和traits的情况(标准库中的都是符合这种情况的)

/* 5.改进4 */
// 模板模板参数
template<typename T, template<class> class Container>
class XCLs
{
private:
  Container<T> c;
public:
  XCLs() {
    for(long i=0;i<SIZE;++i)
      c.insert(c.end(),T());
  
    output_static_data(T());
    Container<T> c1(c);
    Container<T> c2(std::move(c));
    c1.swap(c2);
  }
};
// 测试
XCLs<MyString, vector> cl;
// 报错：vector需要两个模板参数，虽然空间配置器有默认的，但是作为模板模板参数的时候编译器无法自动推导

/* 6.改进5 */
// 使用Alias template
template<typename T, template<class> class Container>
class XCLs
{
private:
  Container<T> c;
public:
  XCLs() {
    for(long i=0;i<SIZE;++i)
      c.insert(c.end(),T());
  
    output_static_data(T());
    Container<T> c1(c);
    Container<T> c2(std::move(c));
    c1.swap(c2);
  }
};
// 使用alias template
template <typename T>
using Vec = vector<T, alloctor<T>>;
// 测试
XCLs<MyString, Vec> cl;

```

### 7.1. template template parameter

```cpp
template<typename T, template<class> class Container>
```

## 8. decltype

来得到一个表达式的类型

应用：

1. to declare return types
2. in metaprogramming
3. pass the type of a lambda

```cpp
// 应用1:(返回类型指定方式)
template <typename T1, typename T2>
auto add(T1 x, T2 y)->decltype(x+y);

// 应用2:(元编程)
template <typename T>
void test(T obj)
{
  typedef typename decltype(obj)::iterator iType;
  // 相当于
  typedef typename T::iterator iType;
}

// 应用3:
auto cmp = [](const Person& p1, const Person& p2){
  return p1.lastname()<p2.lastname()||(p1.lastname()==p2.lastname()&&p1.firstname()<p2.firstname());
}
std::set<Person, decltype(cmp)> coll(cmp);

```

## 9. Lambdas

可以定义inline函数(就是写一些小的仿函数),当作一个函数对象

```cpp
// 简单格式
[]{
  std::cout << "hello lambdas" << std::endl;
};

[]{
  std::cout << "hello lambdas" << std::endl;
}();

// 常用形式
auto I = [](){
  std::cout << "hello lambdas" << std::endl;
};

// 调用它
I();

// 格式
[1](2)3{...}
// 1:捕获以访问 lambda 内的非静态外部对象。 可以使用 std:cout 等静态对象。
// 可以指定捕获以访问未作为参数传递的外部范围的数据：
// [=]:表示外部作用域按值传递给 lambda。
// [&]:表示外部作用域通过引用传递给 lambda。
// 2:参数列表，如果没有参数可以不加小括号
// 3:可选
// mutable:一个对象是按值传递的，但是在 lambda 定义的函数对象内部，您可以对传递的值进行写访问。不加就是传进来的值只可读
// throwSpec:
// ->retType:返回类型没有任何具体定义，是从返回值推导出来的。

/* 例 */
int id = 0;
auto f = [id]() mutable {
  std::cout << "id:" << id << std::endl;
  ++id;
}

// 相当于
class Functor {
private:
  int id;
public:
 void operator()(){
   std::cout << "id:" << id << std::endl;
   ++id;
 }
};
Functor f;

id = 42;
f(); // 0
f(); // 1
f(); // 2
std::cout << id << std::endl; // 42

```

## 10. Rvalue references

***右值引用*** 可以帮助解决一些不必要的拷贝(要拷贝的那个是个右值,就可以不拷贝,直接把它的值偷过来)

右值：

- 表达式
- 临时对象
- ...

```cpp
// 写一个move aware class
class MyString {
public:
  static size_t DCtor; // 累计default-ctor呼叫次数
  static size_t Ctor; // 累计ctor呼叫次数
  static size_t CCtor; // 累计copy-ctor呼叫次数
  static size_t CAsgn; // 累计copy-asgn呼叫次数
  static size_t MCtor; // 累计move-ctor呼叫次数
  static size_t MAsgn; // 累计move-asgn呼叫次数
  static size_t Dtor; // 累计dtor呼叫次数
private:
  char* _data;
  size_t _len;

  void _init_data(const char *s) {
    _data = new char[_len+1];
    memcpy(_data, s, _len);
    _data[_len] = '\0';    
  }

public:
  // default constructor
  MyString() : _data(NULL),_len(0) {
    ++DCtor;
  }

  // constructor
  MyString(const char* p) : _len(strlen(p)) {
    ++Ctor;
    _init_data(p);
  }

  // copy constructor
  MyString(const MyString& str) : _len(str._len) {
    ++CCtor;
    _init_data(str._data);
  }

  // move constructor
  MyString(MyString&& str) noexcept : _data(str._data),_len(str._len) {
    ++MCtor;
    // 前面初始化了_data和_len，下面就把str对原来区域的指向消除
    str._len = 0;
    str._data = NULL;
  }

  // copy assignment
  MyString& operator=(const MyString& str) {
    ++CAsgn;
    // 自我赋值检查
    if(this != &str){
      if(_data) delete _data; // 如果自己之前就有数据，先delete掉它
      _len = str._len; // 深拷贝
      _init_data(str._data);
    }
    else {
    }
    return *this;
  }

  // move assignment
  MyString& operator=(const MyString& str) noexcept {
    ++MAsgn;
    // 自我赋值检查
    if(this != &str){
      if(_data) delete _data; // 如果自己之前就有数据，先delete掉它
      _len = str._len; // 深拷贝
      _data = str._data;
      str._len = 0;
      str._data = NULL;
    }
    return *this;
  }

  // dtor
  virtual ~MyString() {
    ++Dtor;
    if(_data) {
      delete _data;
    }
  }

  bool operator<(const MyString& rhs) const {
    return string(this->_data)<string(rhs._data);
  }

  bool operator==(const MyString& rhs) const {
    return string(this->_data)==string(rhs._data);
  }

  char* get() const {
    return _data;
  }
};
// 静态的要在外面给它初值
size_t MyString::DCtor=0;
size_t MyString::Ctor=0;
size_t MyString::CCtor=0;
size_t MyString::CAsgn=0;
size_t MyString::MCtor=0;
size_t MyString::MAsgn=0;
size_t MyString::Dtor=0;
```
