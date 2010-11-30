#include <string>
#include <set>
#include <ostream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include <sys/wait.h>
#include <sys/signal.h>
#include <boost/lexical_cast.hpp>
#include "Instance.h"

using namespace std;

#define STRINGIFY(a) #a
#define STRING(a) STRINGIFY(a)

#define ASSERT(a) {\
    if (!(a)) {\
        throw std::string("Assertion failed: "__FILE__ ":" STRING(__LINE__) " " #a);\
    }\
}

#define EQUAL(a, b) {\
    if ((a) != (b)) {\
      throw std::string("Assertion failed: "__FILE__":" STRING(__LINE__) " " + a + " == " + b);\
    }\
} 

#define MAYBETHROW(a) {\
    try {\
        a;\
    } catch (...) {\
    }\
}
        
#define TEST(testfn, points) {\
    test(&(testfn), (points), #testfn);\
}


/* This function runs a single test by forking the process.  This way,
   if there is a segfault during a test, the test program won't crash */
void test(void (*testfn)(), int points, const char *name) {
    pid_t pid = fork();
    
    if (pid > 0) {
        /* Check the child's return value */
        int status = 0;
        wait(&status);
        
        /* Non-zero value = test failed */
        cout << name << " " << (status ? 0 : points) << endl;
    } else if (pid == 0) {
        /* Run the test in the child process.  Exit with 0 for success,
           non-zero for failure */
        try {
            testfn();
            exit(0);
        } catch (std::string& error) {
            std::cout << error << endl;
            exit(1);
        } catch (...) {
            exit(1);
        }
    } else {
        cout << "Fork failed!!" << endl;
    }
}

/* Tokenizes the multi-line input into a set of strings */
set<string> tokenize(const std::string& output) {
    set<string> tokens; 
    static const char *delim = "\n";
    size_t last = output.find_first_not_of(delim, 0);
    size_t pos = output.find_first_of(delim, last);
    
    /* Find all lines that are non-empty */
    while (string::npos != pos || string::npos != last) {
        string token = output.substr(last, pos - last);
        for (size_t i = 0; i < token.size(); i++) {
            if (!isspace(token[i])) {
                /* Only insert if non-empty */
                tokens.insert(output.substr(last, pos - last));
                break;
            }
        }

        last = output.find_first_not_of(delim, pos);
        pos = output.find_first_of(delim, last);
    }
    return tokens;
}

bool contains(const set<string>& s, const string& str) {
    return s.find(str) != s.end();
}

void testLocationAttributes() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
    Ptr<Instance> seg1 = m->instanceNew("seg1", "Truck segment");
    Ptr<Instance> seg2 = m->instanceNew("seg2", "Truck segment");
    Ptr<Instance> loc1 = m->instanceNew("loc1", "Port");
    Ptr<Instance> loc2 = m->instanceNew("loc2", "Truck terminal");

    seg1->attributeIs("source", "loc1");
    seg1->attributeIs("length", "10.5");
    seg1->attributeIs("return segment", "seg2");
    seg1->attributeIs("difficulty", "1.0");
    seg1->attributeIs("expedite support", "no");

    seg2->attributeIs("source", "loc2");
    seg2->attributeIs("length", "3.0");
    seg2->attributeIs("return segment", "seg1");
    seg2->attributeIs("difficulty", "5.0");
    seg2->attributeIs("expedite support", "yes");

    EQUAL(seg1->attribute("source"), "loc1");
    EQUAL(seg1->attribute("length"), "10.50");
    EQUAL(seg1->attribute("return segment"), "seg2");
    EQUAL(seg1->attribute("difficulty"), "1.00");
    EQUAL(seg1->attribute("expedite support"), "no");
    
    EQUAL(seg2->attribute("source"), "loc2");
    EQUAL(seg2->attribute("length"), "3.00");
    EQUAL(seg2->attribute("return segment"), "seg1"); 
    EQUAL(seg2->attribute("difficulty"), "5.00");
    EQUAL(seg2->attribute("expedite support"), "yes");

}

void testSegmentAttributes() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    Ptr<Instance> seg1 = m->instanceNew("seg1", "Boat segment");
    Ptr<Instance> seg2 = m->instanceNew("seg2", "Boat segment");
    Ptr<Instance> seg3 = m->instanceNew("seg3", "Boat segment");
    Ptr<Instance> loc = m->instanceNew("loc", "Boat terminal");

    seg1->attributeIs("source", "loc");
    seg2->attributeIs("source", "loc");
    seg3->attributeIs("source", "loc");

    EQUAL(loc->attribute("segment1"), "seg1");
    EQUAL(loc->attribute("segment2"), "seg2");
    EQUAL(loc->attribute("segment3"), "seg3");
    
}

void testStatsAttributes() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
    Ptr<Instance> stats = m->instanceNew("stats", "Stats");

    /* Make some instanes of varying types, then test stats */
    m->instanceNew("seg1", "Boat segment");
    m->instanceNew("seg2", "Plane segment");
    m->instanceNew("seg3", "Plane segment");
    m->instanceNew("seg4", "Truck segment");
    m->instanceNew("seg5", "Truck segment");
    m->instanceNew("seg6", "Truck segment");
    m->instanceNew("seg7", "Truck segment");
    m->instanceNew("seg8", "Truck segment");

    m->instanceNew("loc1", "Customer");
    m->instanceNew("loc2", "Port");
    m->instanceNew("loc3", "Truck terminal");
    m->instanceNew("loc4", "Boat terminal");
    m->instanceNew("loc5", "Plane terminal");
    m->instanceNew("loc6", "Truck terminal");
    m->instanceNew("loc7", "Truck terminal");
    m->instanceNew("loc8", "Truck terminal");

    EQUAL(stats->attribute("Boat segment"), "1");
    EQUAL(stats->attribute("Plane segment"), "2");
    EQUAL(stats->attribute("Truck segment"), "5");
    EQUAL(stats->attribute("Customer"), "1");
    EQUAL(stats->attribute("Port"), "1");
    EQUAL(stats->attribute("Truck terminal"), "4");
    EQUAL(stats->attribute("Plane terminal"), "1");
    EQUAL(stats->attribute("Boat terminal"), "1");
}

void testStatsAttributes2() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Make instances of varying types, then test stats */
    m->instanceNew("seg1", "Boat segment");
    m->instanceNew("seg2", "Plane segment");
    m->instanceNew("seg3", "Plane segment");
    m->instanceNew("seg4", "Truck segment");
    m->instanceNew("seg5", "Truck segment");
    m->instanceNew("seg6", "Truck segment");
    m->instanceNew("seg7", "Truck segment");
    m->instanceNew("seg8", "Truck segment");

    m->instanceNew("loc1", "Customer");
    m->instanceNew("loc2", "Port");
    m->instanceNew("loc3", "Truck terminal");
    m->instanceNew("loc4", "Boat terminal");
    m->instanceNew("loc5", "Plane terminal");
    m->instanceNew("loc6", "Truck terminal");
    m->instanceNew("loc7", "Truck terminal");
    m->instanceNew("loc8", "Truck terminal");

    /* Stats should still work even if created after some
       segments and locations have been made */
    Ptr<Instance> stats = m->instanceNew("stats", "Stats");

    /* Delete an object, which should reduce the count */
    m->instanceDel("loc8");
    EQUAL(stats->attribute("Truck terminal"), "3");

    Ptr<Instance> seg1 = m->instance("seg1");
    seg1->attributeIs("expedite support", "yes");
    EQUAL(stats->attribute("expedite percentage"), "12.50");

    /* Delete some more objects */
    m->instanceDel("seg5");
    m->instanceDel("seg6");
    m->instanceDel("seg7");
    m->instanceDel("seg8");
    EQUAL(stats->attribute("expedite percentage"), "25.00");

    /* Make sure attributes are read-only */
    stats->attributeIs("Boat segment", "99");
    EQUAL(stats->attribute("Boat segment"), "1");
    stats->attributeIs("Truck segment", "293823");
    EQUAL(stats->attribute("Truck segment"), "1");
}

void testFleetAttributes() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
    
    /* Test reading and writing all fleet attributes */
    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Boat, speed", "1");
    fleet->attributeIs("Boat, cost", "2");
    fleet->attributeIs("Boat, capacity", "3");

    fleet->attributeIs("Plane, speed", "4");
    fleet->attributeIs("Plane, cost", "5");
    fleet->attributeIs("Plane, capacity", "6");

    fleet->attributeIs("Truck, speed", "7");
    fleet->attributeIs("Truck, cost", "8");
    fleet->attributeIs("Truck, capacity", "9");

    EQUAL(fleet->attribute("Boat, speed"), "1.00");
    EQUAL(fleet->attribute("Boat, cost"), "2.00");
    EQUAL(fleet->attribute("Boat, capacity"), "3");

    EQUAL(fleet->attribute("Plane, speed"), "4.00");
    EQUAL(fleet->attribute("Plane, cost"), "5.00");
    EQUAL(fleet->attribute("Plane, capacity"), "6");

    EQUAL(fleet->attribute("Truck, speed"), "7.00");
    EQUAL(fleet->attribute("Truck, cost"), "8.00");
    EQUAL(fleet->attribute("Truck, capacity"), "9");
}

void testConn1() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Create a diamond-shaped graph with two alternate routes */
    m->instanceNew("a", "Truck terminal");
    m->instanceNew("b", "Truck terminal");
    m->instanceNew("c", "Truck terminal");
    m->instanceNew("d", "Truck terminal");
    m->instanceNew("e", "Truck terminal");

    Ptr<Instance> s;

    /* Branch #1 */
    s = m->instanceNew("1", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("1r", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "1");
    s->attributeIs("expedite support", "no");

    s = m->instanceNew("2", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "4");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("2r", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "4");
    s->attributeIs("return segment", "2");
    s->attributeIs("expedite support", "no");

    /* Branch #3 */
    s = m->instanceNew("3", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("3r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "3");
    s->attributeIs("expedite support", "no");

    s = m->instanceNew("4", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "4");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("4r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "4");
    s->attributeIs("return segment", "4");
    s->attributeIs("expedite support", "no");

    /* Branch #4 */
    s = m->instanceNew("5", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "20");
    s->attributeIs("difficulty", "4");
    s->attributeIs("expedite support", "yes");
    s = m->instanceNew("5r", "Truck segment");
    s->attributeIs("source", "e");
    s->attributeIs("length", "20");
    s->attributeIs("difficulty", "4");
    s->attributeIs("return segment", "5");
    s->attributeIs("expedite support", "yes");

    s = m->instanceNew("6", "Truck segment");
    s->attributeIs("source", "e");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "2");
    s->attributeIs("expedite support", "yes");
    s = m->instanceNew("6r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "20");
    s->attributeIs("difficulty", "2");
    s->attributeIs("return segment", "6");
    s->attributeIs("expedite support", "yes");

    /* Set up default values */
    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Truck, speed", "1");
    fleet->attributeIs("Truck, capacity", "20");
    fleet->attributeIs("Truck, cost", "30");

    Ptr<Instance> conn = m->instanceNew("conn", "Conn");  
    set<string> t = tokenize(conn->attribute("connect a : d"));
    ASSERT(contains(t, "60.00 2.00 no; a(1:1.00:1r) b(3:1.00:3r) d"));
    ASSERT(contains(t, "2400.00 20.00 no; a(2:10.00:2r) c(4:10.00:4r) d"));
    // REMOVED because spec was unclear
    //ASSERT(contains(t, "3000.00 30.00 no; a(5:20.00:5r) e(6:10.00:6r) d"));
    ASSERT(contains(t, "4500.00 23.08 yes; a(5:20.00:5r) e(6:10.00:6r) d"));

}

void testConn2() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Branching star-like shape */
    m->instanceNew("a", "Port");
    m->instanceNew("b", "Boat terminal");
    m->instanceNew("c", "Plane terminal");
    m->instanceNew("d", "Truck terminal");
    m->instanceNew("e", "Customer");

    Ptr<Instance> s;

    s = m->instanceNew("1", "Boat segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("1r", "Boat segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "1");

    s = m->instanceNew("2", "Plane segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("2r", "Plane segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "2");

    s = m->instanceNew("3", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("3r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "3");

    s = m->instanceNew("4", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s = m->instanceNew("4r", "Truck segment");
    s->attributeIs("source", "e");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "4");


    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Truck, speed", "1");
    fleet->attributeIs("Truck, capacity", "1");
    fleet->attributeIs("Truck, cost", "1");
    fleet->attributeIs("Boat, speed", "2");
    fleet->attributeIs("Boat, capacity", "2");
    fleet->attributeIs("Boat, cost", "2");
    fleet->attributeIs("Plane, speed", "3");
    fleet->attributeIs("Plane, capacity", "3");
    fleet->attributeIs("Plane, cost", "3");

    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    set<string> t;

    t = tokenize(conn->attribute("connect a : d"));
    ASSERT(contains(t, "10.00 10.00 no; a(3:10.00:3r) d"));
    ASSERT(t.size() == 1);

    t = tokenize(conn->attribute("connect c : e"));
    ASSERT(contains(t, "50.00 23.33 no; c(2r:10.00:2) a(3:10.00:3r) d(4:10.00:4r) e"));
    ASSERT(t.size() == 1);

    t = tokenize(conn->attribute("connect e : b"));
    ASSERT(contains(t, "40.00 25.00 no; e(4r:10.00:4) d(3r:10.00:3) a(1:10.00:1r) b"));
    ASSERT(t.size() == 1);

    /* Break some links here */
    s = m->instance("1");
    s->attributeIs("return segment", "");
    s = m->instance("2");
    s->attributeIs("return segment", "");
    s = m->instance("3");
    s->attributeIs("return segment", "");

    ASSERT(tokenize(conn->attribute("connect a : d")).empty());
    ASSERT(tokenize(conn->attribute("connect c : e")).empty());
    ASSERT(tokenize(conn->attribute("connect e : b")).empty());
}

void testConn3() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* This tests a network that has unconnected parts, and checks
     * to make sure that connect returns no paths */
    
    m->instanceNew("a", "Port");
    m->instanceNew("b", "Port");
    m->instanceNew("c", "Port");
    m->instanceNew("d", "Port");
    
    Ptr<Instance> s;
    s = m->instanceNew("1", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "2"); 
    s->attributeIs("expedite support", "no");

    s = m->instanceNew("1r", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "2"); 
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "1");

    s = m->instanceNew("2", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "2"); 
    s->attributeIs("expedite support", "no");

    s = m->instanceNew("2r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "10");
    s->attributeIs("difficulty", "2"); 
    s->attributeIs("expedite support", "no");
    s->attributeIs("return segment", "2");

    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    ASSERT(tokenize(conn->attribute("connect a : d")).empty());
}

void testExplore1() {

    /* Simple network, arranged in a line */
    Ptr<Instance::Manager> m = shippingInstanceManager();

    m->instanceNew("1", "Truck terminal");
    m->instanceNew("2", "Port");
    m->instanceNew("3", "Port");
    m->instanceNew("4", "Boat terminal");

    Ptr<Instance> s12 = m->instanceNew("s12", "Truck segment");
    Ptr<Instance> s21 = m->instanceNew("s21", "Truck segment");
    Ptr<Instance> s23 = m->instanceNew("s23", "Plane segment");
    Ptr<Instance> s32 = m->instanceNew("s32", "Plane segment");
    Ptr<Instance> s34 = m->instanceNew("s34", "Boat segment");
    Ptr<Instance> s43 = m->instanceNew("s43", "Boat segment");

    s12->attributeIs("length", "10");
    s21->attributeIs("length", "10");
    s23->attributeIs("length", "20");
    s32->attributeIs("length", "20");
    s34->attributeIs("length", "30"); 
    s43->attributeIs("length", "30");

    s12->attributeIs("source", "1");
    s12->attributeIs("return segment", "s21");
    s21->attributeIs("source", "2");

    s23->attributeIs("source", "2");
    s23->attributeIs("return segment", "s32");
    s32->attributeIs("source", "3");
    
    s34->attributeIs("source", "3");
    s34->attributeIs("return segment", "s43");
    s43->attributeIs("source", "4");

    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    set<string> t = tokenize(conn->attribute("explore 1 : distance 100000000"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:20.00:s32) 3"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:20.00:s32) 3(s34:30.00:s43) 4"));
}

void testExplore2() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Make a ring of segments that meets back around at the first port */
    for (int i = 0; i <= 3; i++) {
        string id = boost::lexical_cast<string>(i+1);
        Ptr<Instance> loc = m->instanceNew(id, "Port"); 
    }
    for (int i = 0; i <= 3; i++) {
        string from = boost::lexical_cast<string>(i + 1);
        string to = boost::lexical_cast<string>(((i + 1) % 4) + 1);

        string seg1name = "s" + from + to;
        string seg2name = "s" + to + from;
        Ptr<Instance> seg1 = m->instanceNew(seg1name, "Truck segment");
        Ptr<Instance> seg2 = m->instanceNew(seg2name, "Truck segment");

        seg1->attributeIs("length", "10");
        seg2->attributeIs("length", "10");
        
        seg1->attributeIs("source", from);
        seg1->attributeIs("return segment", seg2name);
        seg2->attributeIs("source", to);
    }
    
    /* Now query to see what happens when there is a cycle in 
       the graph */
    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    set<string> t = tokenize(conn->attribute("explore 1 : distance 100000000"));
    
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:10.00:s32) 3(s34:10.00:s43) 4"));
    ASSERT(contains(t, "1(s14:10.00:s41) 4(s43:10.00:s34) 3(s32:10.00:s23) 2"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2(s23:10.00:s32) 3"));
    ASSERT(contains(t, "1(s14:10.00:s41) 4(s43:10.00:s34) 3"));
    ASSERT(contains(t, "1(s12:10.00:s21) 2"));
    ASSERT(contains(t, "1(s14:10.00:s41) 4"));
}

void testExplore3() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
  
    /* Create a tree-shaped network that branches out from a 
       single root node */
    m->instanceNew("a", "Port");
    m->instanceNew("b", "Port");
    m->instanceNew("c", "Port");
    m->instanceNew("d", "Port");
    m->instanceNew("e", "Port");
    m->instanceNew("f", "Port");
    m->instanceNew("g", "Port");

    Ptr<Instance> s;

    s = m->instanceNew("1", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s = m->instanceNew("1r", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "1");
    s->attributeIs("expedite support", "yes");

    s = m->instanceNew("2", "Truck segment");
    s->attributeIs("source", "a");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "yes");
    s = m->instanceNew("2r", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "2");
    
    s = m->instanceNew("3", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s = m->instanceNew("3r", "Truck segment");
    s->attributeIs("source", "d");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "3");
    s->attributeIs("expedite support", "yes");

    s = m->instanceNew("4", "Truck segment");
    s->attributeIs("source", "b");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s = m->instanceNew("4r", "Truck segment");
    s->attributeIs("source", "e");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "4");

    s = m->instanceNew("5", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s = m->instanceNew("5r", "Truck segment");
    s->attributeIs("source", "f");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "5");

    s = m->instanceNew("6", "Truck segment");
    s->attributeIs("source", "c");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("expedite support", "yes");
    s = m->instanceNew("6r", "Truck segment");
    s->attributeIs("source", "g");
    s->attributeIs("length", "1");
    s->attributeIs("difficulty", "1");
    s->attributeIs("return segment", "6");

    Ptr<Instance> fleet = m ->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Truck, cost", "10");
    fleet->attributeIs("Truck, speed", "10");
    fleet->attributeIs("Truck, capacity", "10");

    /* The graph created above is a tree, with "a" at the root.
     * All links have length 1 and difficulty 1 */
    Ptr<Instance> conn = m->instanceNew("conn", "Conn");
    set<string> t = tokenize(conn->attribute("explore a : distance 1"));
    ASSERT(contains(t, "a(1:1.00:1r) b"));
    ASSERT(contains(t, "a(2:1.00:2r) c"));
    ASSERT(t.size() == 2);

    t = tokenize(conn->attribute("explore a : cost 20"));
    ASSERT(contains(t, "a(1:1.00:1r) b"));
    ASSERT(contains(t, "a(1:1.00:1r) b(3:1.00:3r) d"));
    ASSERT(contains(t, "a(1:1.00:1r) b(4:1.00:4r) e"));
    ASSERT(contains(t, "a(2:1.00:2r) c"));
    ASSERT(contains(t, "a(2:1.00:2r) c(5:1.00:5r) f"));
    ASSERT(contains(t, "a(2:1.00:2r) c(6:1.00:6r) g"));
    ASSERT(t.size() == 6);

    t = tokenize(conn->attribute("explore d : expedited"));
    ASSERT(contains(t, "d(3r:1.00:3) b(1r:1.00:1) a(2:1.00:2r) c(6:1.00:6r) g"));
}

void testSegmentCompatibility() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Makes sure that hooking up conflicting segment/location 
       types is not allowed */

    Ptr<Instance> l = m->instanceNew("l", "Truck terminal");
    Ptr<Instance> s = m->instanceNew("s", "Boat segment");

    MAYBETHROW(s->attributeIs("source", "l"));
    EQUAL(s->attribute("source"), "");

    Ptr<Instance> l2 = m->instanceNew("l2", "Boat terminal");
    Ptr<Instance> s2 = m->instanceNew("s2", "Plane segment");

    MAYBETHROW(s2->attributeIs("source", "l2"));
    EQUAL(s2->attribute("source"), "");

    /* Make sure attributes are unmodified on error */
    s->attributeIs("source", "l2");
    MAYBETHROW(s->attributeIs("source", "l"));
    EQUAL(s->attribute("source"), "l2");
}

void testSingletons() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Three valid options: exception, return nil, or return the
       same stats object under a different name */
    Ptr<Instance> stats1 = m->instanceNew("stats1", "Stats");
    try {
        Ptr<Instance> stats2 = m->instanceNew("stats2", "Stats");
        ASSERT(stats1 == stats2 || !stats2);
    } catch (...) {
    }

    Ptr<Instance> fleet1 = m->instanceNew("fleet1", "Fleet");
    try {
        Ptr<Instance> fleet2 = m->instanceNew("fleet2", "Fleet");
        ASSERT(fleet1 == fleet2 || !fleet2);
    } catch (...) {
    }

    Ptr<Instance> conn1 = m->instanceNew("conn1", "Fleet");
    try {
        Ptr<Instance> conn2 = m->instanceNew("conn2", "Fleet");
        ASSERT(conn1 == conn2 || !conn2);
    } catch (...) {
    }
}

void testSegmentSourceChange() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* This tests changing the source of a segment */
    Ptr<Instance> loc1 = m->instanceNew("loc1", "Plane terminal");
    Ptr<Instance> loc2 = m->instanceNew("loc2", "Plane terminal");
    Ptr<Instance> seg1 = m->instanceNew("seg1", "Plane segment");
    Ptr<Instance> seg2 = m->instanceNew("seg2", "Plane segment");
    Ptr<Instance> seg3 = m->instanceNew("seg3", "Plane segment");

    seg1->attributeIs("source", "loc1");
    seg2->attributeIs("source", "loc1");
    seg3->attributeIs("source", "loc1");
    seg2->attributeIs("source", "loc2");

    EQUAL(loc1->attribute("segment1"), "seg1");
    EQUAL(loc1->attribute("segment2"), "seg3");
    MAYBETHROW(EQUAL(loc1->attribute("segment3"), ""));
    EQUAL(loc2->attribute("segment1"), "seg2");
}

void testDeleteLocation() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* This tests instanceDel */
    Ptr<Instance> loc1 = m->instanceNew("loc1", "Port");
    Ptr<Instance> loc2 = m->instanceNew("loc2", "Port");
    Ptr<Instance> seg4 = m->instanceNew("seg4", "Boat segment");
    Ptr<Instance> seg5 = m->instanceNew("seg5", "Boat segment");
    
    seg4->attributeIs("source", "loc1");
    seg4->attributeIs("return segment", "seg5");
    seg5->attributeIs("source", "loc2");

    /* Delete location 2.  This should either a) make the source
       of seg5 equal to the empty string, or b) destroy seg5 */
    m->instanceDel("loc2");
    loc2 = 0;
    ASSERT(!m->instance("seg5") || seg5->attribute("source") == "");
}

void testDeleteSegment() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    Ptr<Instance> loc = m->instanceNew("loc", "Port");
    Ptr<Instance> seg1 = m->instanceNew("seg1", "Truck segment");
    seg1->attributeIs("source", "loc");
    Ptr<Instance> seg2 = m->instanceNew("seg2", "Boat segment");
    seg2->attributeIs("source", "loc");
    Ptr<Instance> seg3 = m->instanceNew("seg3", "Plane segment");
    seg3->attributeIs("source", "loc");

    /* This should remove seg2 and shift seg3 down.  Then, 
       asking for segment3 should throw an exception or
       return the empty string. */
    seg2->attributeIs("source", "");
    EQUAL(loc->attribute("segment1"), "seg1");
    EQUAL(loc->attribute("segment2"), "seg3");
    MAYBETHROW(EQUAL(loc->attribute("segment3"), ""));

    /* This should remove seg3 only. */
    seg3->attributeIs("source", "");
    EQUAL(loc->attribute("segment1"), "seg1");
    MAYBETHROW(EQUAL(loc->attribute("segment2"), ""));
    MAYBETHROW(EQUAL(loc->attribute("segment3"), ""));
}

void testErrorInvalidAttribute() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* Invalid attributes shouldn't crash the program.  They
       should throw an exception or maybe print to stderr. */
    Ptr<Instance> seg = m->instanceNew("seg", "Truck segment");
    MAYBETHROW(seg->attributeIs("foo", "hello"));
    MAYBETHROW(seg->attributeIs("speed", "1.0"));
    MAYBETHROW(seg->attribute("bar"));
    
    Ptr<Instance> loc = m->instanceNew("loc", "Port");
    MAYBETHROW(seg->attributeIs("source", "loc"));
    MAYBETHROW(seg->attributeIs("length", "1.0"));
    MAYBETHROW(seg->attributeIs("returnSegment", "seg"));
    MAYBETHROW(seg->attributeIs("difficulty", "1.0"));
    MAYBETHROW(seg->attributeIs("expedite support", "yes"));
}

void testErrorInvalidValues() {
    Ptr<Instance::Manager> m = shippingInstanceManager();
    Ptr<Instance> seg = m->instanceNew("seg", "Plane segment");
    seg->attributeIs("length", "1.0");
    seg->attributeIs("difficulty", "1.0");
    seg->attributeIs("expedite support", "yes");
    Ptr<Instance> fleet = m->instanceNew("fleet", "Fleet");
    fleet->attributeIs("Boat, capacity", "10");
    fleet->attributeIs("Boat, cost", "10");
    fleet->attributeIs("Boat, speed", "10");
    
    /* Try some out-of-range/invalid values */
    MAYBETHROW(seg->attributeIs("length", "-10"));
    MAYBETHROW(seg->attributeIs("difficulty", "0.9"));
    MAYBETHROW(seg->attributeIs("difficulty", "6.0"));
    MAYBETHROW(seg->attributeIs("difficulty", "-1"));
    MAYBETHROW(fleet->attributeIs("Boat, capacity", "-1")); 
    MAYBETHROW(fleet->attributeIs("Boat, cost", "-1")); 
    MAYBETHROW(fleet->attributeIs("Boat, speed", "-1")); 
    MAYBETHROW(seg->attributeIs("expedite support", "whatever"));

    EQUAL(seg->attribute("length"), "1.00");
    EQUAL(seg->attribute("difficulty"), "1.00");     
    EQUAL(seg->attribute("expedite support"), "yes");
    EQUAL(fleet->attribute("Boat, capacity"), "10");
    EQUAL(fleet->attribute("Boat, cost"), "10.00");
    EQUAL(fleet->attribute("Boat, speed"), "10.00");
}

void testErrorExistingInstance() {
    Ptr<Instance::Manager> m = shippingInstanceManager();

    /* This should either throw an exception, return null, or
       return the same instance.  It should NOT return another
       new instance, which would overwrite the old one. */
    try {
        Ptr<Instance> seg1 = m->instanceNew("seg", "Truck segment");
        Ptr<Instance> seg2 = m->instanceNew("seg", "Truck segment");
        if (!seg2 || seg1 == seg2) {
            return;
        }
    } catch (...) {
        return;
    }

    ASSERT(false);
}

void testStressTest() {
  Ptr<Instance::Manager> m = shippingInstanceManager();

  for (int i = 0; i < 100000; i++) {

    m->instanceNew("conn", "Conn");
    m->instanceDel("conn");

    m->instanceNew("fleet", "Fleet");
    m->instanceDel("fleet");
  
    m->instanceNew("seg", "Truck segment");
    m->instanceDel("seg");
  }
}

void sigsegv(int signal) {
    cerr << "Segmentation fault" << endl;
    exit(1);
}

int main(int argc, char *argv[]) {
    signal(SIGSEGV, sigsegv);

    TEST(testLocationAttributes, 1);
    TEST(testSegmentAttributes, 1);
    TEST(testStatsAttributes, 1);
    TEST(testStatsAttributes2, 1);
    TEST(testFleetAttributes, 1);
    TEST(testConn1, 2);
    TEST(testConn2, 2);
    TEST(testConn3, 1);
    TEST(testExplore1, 2);
    TEST(testExplore2, 2);
    TEST(testExplore3, 2);
    TEST(testSegmentCompatibility, 1);
    TEST(testSingletons, 1);
    TEST(testSegmentSourceChange, 1);
    TEST(testDeleteLocation, 1);
    TEST(testDeleteSegment, 1);
    TEST(testErrorInvalidAttribute, 1);
    TEST(testErrorInvalidValues, 1);
    TEST(testErrorExistingInstance, 1); 
    TEST(testStressTest, 1);

    return 0;
}


