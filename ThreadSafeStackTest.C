#include "ThreadSafeStack.H"

#include <cppunit/TestFixture.h>
#include <cppunit/extensions/HelperMacros.h>

class ThreadSafeStackTest: public CPPUNIT_NS::TestFixture
{
  CPPUNIT_TEST_SUITE(ThreadSafeStackTest);
  
  CPPUNIT_TEST(test1);

  CPPUNIT_TEST_SUITE_END();
  
  public:
    void test1();

  private:
    struct MyNode:public util::NodeBase<MyNode>
    {
      MyNode(int i):i(i) {}
      int i;
    };

    util::ThreadSafeStack<MyNode> _stack;
};

CPPUNIT_TEST_SUITE_REGISTRATION(ThreadSafeStackTest);

void ThreadSafeStackTest::test1()
{
  MyNode* n1 = new MyNode(1);
  MyNode* n2 = new MyNode(2);
  MyNode* n3 = new MyNode(3);

  _stack.push(n1);
  _stack.push(n2);
  _stack.push(n3);

  CPPUNIT_ASSERT(_stack.pop()->i == 3);
  CPPUNIT_ASSERT(_stack.pop()->i == 2);
  CPPUNIT_ASSERT(_stack.pop()->i == 1);
  CPPUNIT_ASSERT(_stack.pop() == NULL);
}
