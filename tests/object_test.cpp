#include  "../stactiverecord/stactive_record.h"

using namespace stactiverecord;
using namespace std;

Sar_Dbi * Sar_Dbi::dbi;

void assert(bool v, string msg) {
  if(v) return;
  throw Sar_AssertionFailedException("Failed when testing " + msg);
}

class Test : public Record {
public:
  Test() : Record("test") {};
  Test(int id) : Record("test", id) {};
};

class TestTwo : public Record {
public:
  TestTwo() : Record("testtwo") {};
  TestTwo(int id) : Record("testtwo", id) {};
};

int main(int argc, char* argv[]) {
  if(argc != 2) {
    std::cout << "Usage: ./object_test <db config string>\n";
    return 1;
  }
  Sar_Dbi::dbi = Sar_Dbi::makeStorage(std::string(argv[1]));

  Test t;
  t.set("number", 40);
  t.set("boolean", true);
  t.save();

  // test some queries
  ObjGroup<Test> og = Record::find<Test>(Q("number", between(30,50)) && Q("boolean", true));
  assert(og.size() == 1 && og[0].id == t.id, "performing find query");
  og = Record::find<Test>(Q("number", nequals(40)));
  assert(og.size() == 0, "performing find query with no results");
    
  TestTwo tt;
  tt.set("number", 10);
  tt.save();
  t.setOne<TestTwo>(tt);
  t.save();

  // test reverse relationship
  Test q(t.id);
  ObjGroup<TestTwo> ogtt = q.getMany<TestTwo>();
  assert(ogtt.size() == 1 && ogtt[0].id == t.id, "testing reverse relationship between objects");

  // test complicated query
  Test o;
  o.set("stringone", "one");
  o.set("stringtwo", "two");
  o.set("integer", 40);
  o.save();

  Test oo;
  oo.set("stringone", "another");
  oo.set("stringtwo", "andanother");
  oo.set("integer", 50);
  oo.save();  

  og = Record::find<Test>((Q("stringone", startswith("ano")) || Q("stringone", endswith("ne"))) && Q("integer", between(39, 51)));
  assert(og.size() == 2, "testing a complicated query");

  //test lots and lots of relationships
  Test popular;
  ObjGroup<TestTwo> friends;
  int number = 50;
  for(int i=0; i<number; i++) {
    TestTwo unpop_friend;
    unpop_friend.save();
    friends << unpop_friend;
  }
  popular.setMany<TestTwo>(friends);
  popular.save();

  ogtt = popular.getMany<TestTwo>();
  assert(ogtt.size() == number, "testing object relationship with lots of objects");

  delete Sar_Dbi::dbi;
  cout << "No errors were found.\n";
  return 0;
}