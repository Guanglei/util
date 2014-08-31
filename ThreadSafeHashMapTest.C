#include "ThreadSafeHashMap.H"

#include <string>
#include <unordered_map>
#include <thread>

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ThreadSafeHashMapTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(ThreadSafeHashMapTest);
  
  CPPUNIT_TEST(test1);
  CPPUNIT_TEST(test2);

  CPPUNIT_TEST_SUITE_END();
  
  public:
    void test1();
    void test2();

  private:
    typedef util::ThreadSafeHashMap<std::string, std::string> HashMap;
    typedef std::unordered_map<std::string, std::string> StdMap;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadSafeHashMapTest);

void ThreadSafeHashMapTest::test1()
{
  HashMap hashMap;
  hashMap.insert(HashMap::ValueType(std::make_pair("a", "abc")));
  hashMap.insert(HashMap::ValueType(std::make_pair("b", "abc")));
  hashMap.insert(HashMap::ValueType(std::make_pair("c", "abc")));
  CPPUNIT_ASSERT(hashMap.size() == 3);
  CPPUNIT_ASSERT(hashMap.hasKey("a"));
  CPPUNIT_ASSERT(!hashMap.hasKey("d"));
  
  StdMap stdMap;
  for(auto& i:hashMap)
  {
    stdMap.insert(StdMap::value_type(std::make_pair(i.first, i.second)));
  }

  CPPUNIT_ASSERT(stdMap.size() == 3);
  CPPUNIT_ASSERT(stdMap.count("a") == 1);
  CPPUNIT_ASSERT(stdMap.count("d") == 0);
}

void ThreadSafeHashMapTest::test2()
{
  HashMap hashMap;
  auto l = [&hashMap](std::string k){hashMap.insert(HashMap::ValueType(std::make_pair(k, "abc")));}; 
  std::thread t1(l, "a");
  std::thread t2(l, "b");
  std::thread t3(l, "c");
  
  t1.join();
  t2.join();
  t3.join();

  CPPUNIT_ASSERT(hashMap.size() == 3);
  CPPUNIT_ASSERT(hashMap.hasKey("a"));
  CPPUNIT_ASSERT(hashMap.hasKey("b"));
  CPPUNIT_ASSERT(hashMap.hasKey("c"));
  CPPUNIT_ASSERT(!hashMap.hasKey("d"));
}

