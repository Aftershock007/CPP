#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include "String.h"
#include "Vector.h"

void test_string() {
    std::cout << "========== STRING TESTS ==========\n\n";

    std::cout << "--- S1. CONSTRUCTORS & SSO ---\n";
    const String default_str;
    std::cout << "Default: \"" << default_str << "\" | Size: " << default_str.size()
              << " | Capacity: " << default_str.capacity() << "\n";

    const String param_str("Google");
    std::cout << "Param(SSO): \"" << param_str << "\" | Size: " << param_str.size()
              << " | Capacity: " << param_str.capacity() << "\n";

    const String long_str("This is a long string that exceeds SSO");
    std::cout << "Param(heap): \"" << long_str << "\" | Size: " << long_str.size()
              << " | Capacity: " << long_str.capacity() << "\n";

    const String null_str(nullptr);
    std::cout << "Null: \"" << null_str << "\" | Size: " << null_str.size() << "\n";

    std::cout << "c_str(): " << param_str.c_str() << "\n";
    std::cout << "empty() default (1): " << default_str.empty() << "\n";
    std::cout << "empty() param (0): " << param_str.empty() << "\n\n";

    std::cout << "--- S2. COPY SEMANTICS ---\n";
    String copy_constructed(param_str);
    String copy_assigned;
    copy_assigned = param_str;
    std::cout << "Copy Constructed: \"" << copy_constructed << "\"\n";
    std::cout << "Copy Assigned: \"" << copy_assigned << "\"\n";

    // Deep copy: mutating copy shouldn't affect original
    copy_constructed[0] = 'g';
    std::cout << "After mutating copy: \"" << copy_constructed << "\" | Original: \"" << param_str << "\"\n";

    // Copy a heap string
    String long_copy(long_str);
    std::cout << "Heap copy: \"" << long_copy << "\"\n\n";

    std::cout << "--- S3. MOVE SEMANTICS ---\n";
    String move_src("Hello");
    std::cout << "Before move: \"" << move_src << "\" | Size: " << move_src.size() << "\n";
    const String move_constructed(std::move(move_src));
    std::cout << "Move constructed: \"" << move_constructed << "\"\n";
    std::cout << "Moved-from: \"" << move_src << "\" | Size: " << move_src.size() << "\n";

    // Move a heap string
    String heap_src("This is definitely a heap allocated string");
    const String heap_moved(std::move(heap_src));
    std::cout << "Heap move: \"" << heap_moved << "\"\n";
    std::cout << "Heap moved-from size: " << heap_src.size() << "\n";

    // Move assignment
    String move_assigned;
    String move_src2("World");
    move_assigned = std::move(move_src2);
    std::cout << "Move assigned: \"" << move_assigned << "\"\n\n";

    std::cout << "--- S4. ELEMENT ACCESS & EXCEPTIONS ---\n";
    String access_str("Bustub");
    access_str[0] = 'b';
    std::cout << "Mutated: \"" << access_str << "\"\n";

    const String const_str("CMU");
    std::cout << "Const access [1]: " << const_str[1] << "\n";

    try {
        const char c = access_str[100];
        std::cout << c << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught: " << e.what() << "\n\n";
    }

    std::cout << "--- S5. COMPARISONS (strcmp-based) ---\n";
    const String a("Apple");
    const String b("Banana");
    const String a2("Apple");
    const String c("Apples");

    std::cout << "a == a2 (1): " << (a == a2) << "\n";
    std::cout << "a == b  (0): " << (a == b) << "\n";
    std::cout << "a < b   (1): " << (a < b) << "\n";
    std::cout << "b > a   (1): " << (b > a) << "\n";
    std::cout << "a < c   (1): " << (a < c) << "\n\n";

    std::cout << "--- S6. APPEND / OPERATOR+= / OPERATOR+ ---\n";
    String hello("Hello");
    hello += String(", ");
    hello += "World";
    std::cout << "After +=: \"" << hello << "\" | Size: " << hello.size()
              << " | Capacity: " << hello.capacity() << "\n";

    hello.append(String("!"));
    std::cout << "After append: \"" << hello << "\"\n";

    const String left("Foo");
    const String right("Bar");
    const String concat = left + right;
    std::cout << "operator+: \"" << concat << "\"\n";

    // Append that triggers SSO -> heap transition
    String grow_str("Hi");
    grow_str += " there, this should exceed SSO capacity easily";
    std::cout << "SSO->heap: \"" << grow_str << "\" | Capacity: " << grow_str.capacity() << "\n\n";

    std::cout << "--- S7. SUBSTR / FIND ---\n";
    const String haystack("Hello, World!");

    const String sub1 = haystack.substr(0, 5);
    std::cout << "substr(0,5): \"" << sub1 << "\"\n";

    const String sub2 = haystack.substr(7);
    std::cout << "substr(7): \"" << sub2 << "\"\n";

    std::cout << "find('W'): " << haystack.find('W') << "\n";
    std::cout << "find('z') == npos (1): " << (haystack.find('z') == String::npos) << "\n";

    std::cout << "find(\"World\"): " << haystack.find(String("World")) << "\n";
    std::cout << "find(\"xyz\") == npos (1): " << (haystack.find(String("xyz")) == String::npos) << "\n";

    try {
        haystack.substr(100);
    } catch (const std::out_of_range& e) {
        std::cout << "Caught substr OOB: " << e.what() << "\n\n";
    }

    std::cout << "--- S8. ITERATORS ---\n";
    String iter_str("ABCDE");
    std::cout << "Range-for: ";
    for (const char ch : iter_str) {
        std::cout << ch << " ";
    }
    std::cout << "\n";

    // Mutate via iterator
    for (char& ch : iter_str) {
        ch = static_cast<char>(ch + 32); // lowercase
    }
    std::cout << "After lowercase: \"" << iter_str << "\"\n\n";

    std::cout << "--- S9. CLEAR / RESERVE ---\n";
    String res_str("Test");
    std::cout << "Before reserve: Capacity: " << res_str.capacity() << "\n";
    res_str.reserve(100);
    std::cout << "After reserve(100): Capacity: " << res_str.capacity()
              << " | Data intact: \"" << res_str << "\"\n";

    std::size_t cap = res_str.capacity();
    res_str.clear();
    std::cout << "After clear: \"" << res_str << "\" | Size: " << res_str.size()
              << " | Capacity: " << res_str.capacity()
              << " | Cap unchanged (1): " << (res_str.capacity() == cap) << "\n\n";

    std::cout << "--- S10. STREAM I/O (dynamic) ---\n";
    std::istringstream iss("HelloWorld");
    String stream_str;
    iss >> stream_str;
    std::cout << "Read from stream: \"" << stream_str << "\" | Size: " << stream_str.size() << "\n";

    // Read a long string
    std::istringstream iss2("ThisIsAVeryLongStringThatExceedsTheSmallStringOptimizationCapacity");
    String long_stream;
    iss2 >> long_stream;
    std::cout << "Long stream read: \"" << long_stream << "\" | Size: " << long_stream.size() << "\n\n";

    std::cout << "--- S11. STD::HASH ---\n";
    std::unordered_set<String> set;
    set.insert(String("Apple"));
    set.insert(String("Banana"));
    set.insert(String("Apple")); // duplicate
    std::cout << "Set size (2 expected): " << set.size() << "\n";
    std::cout << "Hash of \"Apple\": " << std::hash<String>{}(String("Apple")) << "\n";
    std::cout << "Hash of \"Banana\": " << std::hash<String>{}(String("Banana")) << "\n\n";

    std::cout << "--- S12. SELF-ASSIGNMENT ---\n";
    String self_str("SelfTest");
    self_str = self_str;
    std::cout << "After self-assignment: \"" << self_str << "\"\n";

    // Self-assignment with heap string
    String self_heap("This is a heap allocated string for self-assignment");
    self_heap = self_heap;
    std::cout << "Heap self-assignment: \"" << self_heap << "\"\n\n";

    std::cout << "--- S13. EDGE CASES ---\n";
    // Append to empty
    String empty_append;
    empty_append += "Hello";
    std::cout << "Append to empty: \"" << empty_append << "\"\n";

    // Append empty to non-empty
    String no_change("Keep");
    no_change += "";
    no_change.append(String());
    std::cout << "Append empty (unchanged): \"" << no_change << "\"\n";

    // Self-append
    String self_append("AB");
    self_append += self_append;
    std::cout << "Self-append: \"" << self_append << "\"\n";

    // Substr edge cases
    const String sub_src("Hello");
    const String sub_empty = sub_src.substr(5); // pos == size -> empty
    std::cout << "substr(size): \"" << sub_empty << "\" | Size: " << sub_empty.size() << "\n";
    const String sub_full = sub_src.substr(0); // full string
    std::cout << "substr(0): \"" << sub_full << "\"\n";

    // Find on empty string
    const String empty_find;
    std::cout << "find('a') on empty == npos (1): " << (empty_find.find('a') == String::npos) << "\n";

    // Find empty in non-empty
    std::cout << "find(\"\") at 0 (0): " << sub_src.find(String()) << "\n";

    // Copy/move empty strings
    const String empty_orig;
    const String empty_copy(empty_orig);
    std::cout << "Copy empty: \"" << empty_copy << "\" | Size: " << empty_copy.size() << "\n";
    String empty_move_src;
    const String empty_moved(std::move(empty_move_src));
    std::cout << "Move empty: \"" << empty_moved << "\" | Size: " << empty_moved.size() << "\n";

    // Operator+ with empty
    const String plus_empty = String("Hi") + String();
    std::cout << "\"Hi\" + \"\": \"" << plus_empty << "\"\n\n";

    std::cout << "========== STRING TESTS DONE ==========\n\n";
}

void test_vector() {
    std::cout << "========== VECTOR TESTS ==========\n\n";

    std::cout << "--- V1. CONSTRUCTORS ---\n";
    const Vector<int> default_vec;
    std::cout << "Default: " << default_vec << " | Size: " << default_vec.size() << "\n";

    const Vector<int> sized_vec(5);
    std::cout << "Sized(5): " << sized_vec << " | Size: " << sized_vec.size() << "\n";

    Vector<int> init_vec({10, 20, 30, 40, 50});
    std::cout << "InitList: " << init_vec << " | Size: " << init_vec.size() << "\n";

    constexpr int arr[] = {100, 200, 300};
    const Vector<int> arr_vec(3, arr);
    std::cout << "Array: " << arr_vec << " | Size: " << arr_vec.size() << "\n";

    const Vector<int> empty_init_vec({});
    std::cout << "Empty InitList: " << empty_init_vec << " | Size: " << empty_init_vec.size() << "\n";

    const Vector<int> zero_vec(0);
    std::cout << "Sized(0): " << zero_vec << " | Size: " << zero_vec.size() << "\n";

    const Vector<int> null_arr_vec(5, nullptr);
    std::cout << "Null Array: " << null_arr_vec << " | Size: " << null_arr_vec.size() << "\n\n";

    std::cout << "--- V2. COPY SEMANTICS ---\n";
    const Vector<int> copy_constructed(init_vec);
    std::cout << "Copy Constructed: " << copy_constructed << " | Size: " << copy_constructed.size() << "\n";

    Vector<int> copy_assigned;
    copy_assigned = init_vec;
    std::cout << "Copy Assigned: " << copy_assigned << " | Size: " << copy_assigned.size() << "\n";

    // Verify deep copy — modifying original shouldn't affect copies
    init_vec[0] = 99;
    std::cout << "Original after mutation: " << init_vec << "\n";
    std::cout << "Copy Constructed (unchanged): " << copy_constructed << "\n";
    std::cout << "Copy Assigned (unchanged): " << copy_assigned << "\n\n";

    std::cout << "--- V3. MOVE SEMANTICS ---\n";
    Vector<int> move_src({1, 2, 3});
    std::cout << "Before move: " << move_src << " | Size: " << move_src.size() << "\n";

    const Vector<int> move_constructed(std::move(move_src));
    std::cout << "Move Constructed: " << move_constructed << " | Size: " << move_constructed.size() << "\n";
    std::cout << "Moved-from Size: " << move_src.size() << "\n";

    Vector<int> move_src2({4, 5, 6});
    Vector<int> move_assigned;
    move_assigned = std::move(move_src2);
    std::cout << "Move Assigned: " << move_assigned << " | Size: " << move_assigned.size() << "\n";
    std::cout << "Moved-from Size: " << move_src2.size() << "\n\n";

    std::cout << "--- V4. ELEMENT ACCESS & EXCEPTIONS ---\n";
    Vector<int> access_vec({10, 20, 30, 40, 50});
    access_vec[0] = 99; // Non-const operator[]
    std::cout << "Mutated vec: " << access_vec << "\n";

    const Vector<int> const_vec({7, 8, 9});
    std::cout << "Const access [1]: " << const_vec[1] << "\n";

    try {
        const int val = access_vec[10]; // Should throw
        std::cout << val << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception: " << e.what() << "\n";
    }

    try {
        const int val = const_vec[5]; // Should throw on const version
        std::cout << val << "\n";
    } catch (const std::out_of_range& e) {
        std::cout << "Caught expected exception (const): " << e.what() << "\n\n";
    }

    std::cout << "--- V5. STREAM I/O ---\n";
    std::cout << "Output test: " << Vector<int>({1, 2, 3}) << "\n";

    std::istringstream iss("10 20 30 40");
    Vector<int> stream_vec;
    iss >> stream_vec;
    std::cout << "Read from stream: " << stream_vec << " | Size: " << stream_vec.size() << "\n\n";

    std::cout << "--- V6. SELF-ASSIGNMENT ---\n";
    Vector<int> self_vec({5, 10, 15});
    self_vec = self_vec;
    std::cout << "After self-assignment: " << self_vec << " | Size: " << self_vec.size() << "\n\n";

    std::cout << "--- V7. RESERVE ---\n";
    Vector<int> res_vec;
    std::cout << "Before reserve: Size: " << res_vec.size() << " | Capacity: " << res_vec.capacity() << "\n";
    res_vec.reserve(10);
    std::cout << "After reserve(10): Size: " << res_vec.size() << " | Capacity: " << res_vec.capacity() << "\n";
    res_vec.reserve(5); // should be no-op
    std::cout << "After reserve(5) (no-op): Size: " << res_vec.size() << " | Capacity: " << res_vec.capacity() << "\n\n";

    std::cout << "--- V8. PUSH_BACK ---\n";
    Vector<int> pb_vec;
    std::cout << "Empty: Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    pb_vec.push_back(10);
    std::cout << "After push_back(10): " << pb_vec << " | Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    pb_vec.push_back(20);
    std::cout << "After push_back(20): " << pb_vec << " | Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    pb_vec.push_back(30);
    std::cout << "After push_back(30): " << pb_vec << " | Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    // Verify growth: push enough to trigger multiple reallocations
    for (int i = 4; i <= 10; ++i) {
        pb_vec.push_back(i * 10);
    }
    std::cout << "After 10 pushes: " << pb_vec << " | Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    // push_back with move (rvalue)
    int val = 999;
    pb_vec.push_back(std::move(val));
    std::cout << "After push_back(move 999): " << pb_vec << " | Size: " << pb_vec.size() << " | Capacity: " << pb_vec.capacity() << "\n";

    // push_back self-reference: vec.push_back(vec[0])
    Vector<int> self_ref({10, 20, 30});
    self_ref.push_back(self_ref[0]);
    std::cout << "Self-ref push_back: " << self_ref << " | Size: " << self_ref.size() << "\n";

    // push_back on a pre-reserved vector (no reallocation expected)
    Vector<int> pre_vec;
    pre_vec.reserve(5);
    std::cout << "\nPre-reserved(5): Size: " << pre_vec.size() << " | Capacity: " << pre_vec.capacity() << "\n";
    for (int i = 1; i <= 5; ++i) {
        pre_vec.push_back(i);
    }
    std::cout << "After 5 pushes: " << pre_vec << " | Size: " << pre_vec.size() << " | Capacity: " << pre_vec.capacity() << "\n";
    pre_vec.push_back(6); // triggers growth
    std::cout << "After 6th push (growth): " << pre_vec << " | Size: " << pre_vec.size() << " | Capacity: " << pre_vec.capacity() << "\n\n";

    std::cout << "--- V9. EMPLACE_BACK ---\n";
    Vector<int> eb_vec;
    eb_vec.emplace_back(42);
    std::cout << "After emplace_back(42): " << eb_vec << " | Size: " << eb_vec.size() << " | Capacity: " << eb_vec.capacity() << "\n";

    eb_vec.emplace_back(100);
    eb_vec.emplace_back(200);
    std::cout << "After 2 more emplaces: " << eb_vec << " | Size: " << eb_vec.size() << " | Capacity: " << eb_vec.capacity() << "\n";

    // emplace_back returns a reference to the inserted element
    int& ref = eb_vec.emplace_back(999);
    std::cout << "Returned ref value: " << ref << "\n";
    ref = 0;
    std::cout << "After modifying ref to 0: " << eb_vec << "\n";

    // Test with a non-trivial type to show perfect forwarding
    Vector<std::string> str_vec;
    str_vec.emplace_back("hello");           // const char* -> string(const char*)
    str_vec.emplace_back(5, 'x');            // string(size_t, char) -> "xxxxx"
    std::string s = "world";
    str_vec.emplace_back(std::move(s));      // move construct
    std::cout << "String vec: ";
    for (std::size_t i = 0; i < str_vec.size(); ++i) {
        std::cout << "[" << str_vec[i] << "] ";
    }
    std::cout << "| Size: " << str_vec.size() << "\n";
    std::cout << "Moved-from string empty (expected): \"" << s << "\"\n\n";

    std::cout << "--- V10. ITERATORS ---\n";
    Vector<int> iter_vec({10, 20, 30, 40, 50});

    // Range-based for (uses begin()/end())
    std::cout << "Range-for: ";
    for (const auto& x : iter_vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Modify via iterators
    for (auto& x : iter_vec) {
        x *= 2;
    }
    std::cout << "After doubling: " << iter_vec << "\n";

    // STL algorithms
    std::sort(iter_vec.begin(), iter_vec.end(), std::greater<int>());
    std::cout << "Sorted descending: " << iter_vec << "\n";

    auto it = std::find(iter_vec.begin(), iter_vec.end(), 60);
    std::cout << "find(60): " << (it != iter_vec.end() ? "found" : "not found") << "\n";

    // const iteration
    const Vector<int> const_iter_vec({1, 2, 3});
    std::cout << "Const range-for: ";
    for (const auto& x : const_iter_vec) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // Reverse iterators
    Vector<int> rev_vec({1, 2, 3, 4, 5});
    std::cout << "Reverse: ";
    for (auto rit = rev_vec.rbegin(); rit != rev_vec.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    // Const reverse iterators
    const Vector<int> const_rev_vec({10, 20, 30});
    std::cout << "Const reverse: ";
    for (auto rit = const_rev_vec.rbegin(); rit != const_rev_vec.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n\n";

    std::cout << "--- V11. CLEAR / EMPTY / DATA ---\n";
    Vector<int> util_vec({1, 2, 3, 4, 5});
    std::cout << "empty() (0 expected): " << util_vec.empty() << "\n";
    std::cout << "data() != nullptr (1 expected): " << (util_vec.data() != nullptr) << "\n";

    // Direct data() pointer access
    int* raw = util_vec.data();
    raw[0] = 99;
    std::cout << "After data()[0] = 99: " << util_vec << "\n";

    // const data()
    const Vector<int> cdata_vec({7, 8, 9});
    const int* craw = cdata_vec.data();
    std::cout << "Const data()[0]: " << craw[0] << "\n";

    // clear — size goes to 0, capacity stays
    std::size_t cap_before = util_vec.capacity();
    util_vec.clear();
    std::cout << "After clear(): Size: " << util_vec.size() << " | Capacity: " << util_vec.capacity() << "\n";
    std::cout << "Capacity unchanged (1 expected): " << (util_vec.capacity() == cap_before) << "\n";
    std::cout << "empty() (1 expected): " << util_vec.empty() << "\n";

    // push_back after clear reuses existing capacity
    util_vec.push_back(42);
    std::cout << "After push_back(42): " << util_vec << " | Size: " << util_vec.size() << " | Capacity: " << util_vec.capacity() << "\n";

    // empty on default-constructed
    const Vector<int> empty_vec;
    std::cout << "Default vec empty() (1 expected): " << empty_vec.empty() << "\n\n";

    std::cout << "--- V12. POP_BACK / RESIZE / ASSIGN ---\n";
    Vector<int> pr_vec({10, 20, 30, 40, 50});
    std::cout << "Initial: " << pr_vec << " | Size: " << pr_vec.size() << "\n";

    pr_vec.pop_back();
    std::cout << "After pop_back(): " << pr_vec << " | Size: " << pr_vec.size() << "\n";

    pr_vec.pop_back();
    pr_vec.pop_back();
    std::cout << "After 2 more pop_backs: " << pr_vec << " | Size: " << pr_vec.size() << "\n";

    // resize grow — new elements value-initialized to 0
    pr_vec.resize(6);
    std::cout << "After resize(6): " << pr_vec << " | Size: " << pr_vec.size() << " | Capacity: " << pr_vec.capacity() << "\n";

    // resize grow with fill value
    pr_vec.resize(8, 99);
    std::cout << "After resize(8, 99): " << pr_vec << " | Size: " << pr_vec.size() << "\n";

    // resize shrink
    pr_vec.resize(3);
    std::cout << "After resize(3): " << pr_vec << " | Size: " << pr_vec.size() << " | Capacity: " << pr_vec.capacity() << "\n";

    // assign — replaces all content
    pr_vec.assign(5, 42);
    std::cout << "After assign(5, 42): " << pr_vec << " | Size: " << pr_vec.size() << "\n";

    // assign with smaller count (reuses capacity)
    pr_vec.assign(2, 7);
    std::cout << "After assign(2, 7): " << pr_vec << " | Size: " << pr_vec.size()
              << " | Capacity: " << pr_vec.capacity() << "\n";

    // pop_back on size 1
    Vector<int> single({7});
    single.pop_back();
    std::cout << "Pop single element: Size: " << single.size() << " | empty: " << single.empty() << "\n";

    // pop_back on empty — should be no-op
    single.pop_back();
    std::cout << "Pop on empty: Size: " << single.size() << "\n\n";

    std::cout << "--- V13. INSERT / ERASE ---\n";
    Vector<int> ie_vec({10, 20, 30, 40, 50});
    std::cout << "Initial: " << ie_vec << "\n";

    // Insert at beginning
    ie_vec.insert(ie_vec.begin(), 5);
    std::cout << "Insert 5 at begin: " << ie_vec << "\n";

    // Insert in middle
    ie_vec.insert(ie_vec.begin() + 3, 25);
    std::cout << "Insert 25 at [3]: " << ie_vec << "\n";

    // Insert at end
    ie_vec.insert(ie_vec.end(), 60);
    std::cout << "Insert 60 at end: " << ie_vec << "\n";

    // Erase from beginning
    ie_vec.erase(ie_vec.begin());
    std::cout << "Erase begin: " << ie_vec << "\n";

    // Erase from middle
    ie_vec.erase(ie_vec.begin() + 2);
    std::cout << "Erase [2]: " << ie_vec << "\n";

    // Erase last element
    ie_vec.erase(ie_vec.end() - 1);
    std::cout << "Erase last: " << ie_vec << "\n";

    // Insert returns iterator to inserted element
    int* ins_it = ie_vec.insert(ie_vec.begin() + 1, 999);
    std::cout << "Insert returned: " << *ins_it << "\n";

    // Erase returns iterator to next element
    int* er_it = ie_vec.erase(ie_vec.begin() + 1);
    std::cout << "Erase returned next: " << *er_it << "\n";

    // Insert on empty vector
    Vector<int> empty_ie;
    empty_ie.insert(empty_ie.begin(), 42);
    std::cout << "Insert on empty: " << empty_ie << "\n";

    // Erase/Insert OOB exceptions
    try {
        ie_vec.erase(ie_vec.end());
    } catch (const std::out_of_range& e) {
        std::cout << "Erase OOB caught: " << e.what() << "\n";
    }

    try {
        ie_vec.insert(ie_vec.end() + 5, 0);
    } catch (const std::out_of_range& e) {
        std::cout << "Insert OOB caught: " << e.what() << "\n\n";
    }

    std::cout << "--- V14. SHRINK_TO_FIT ---\n";
    Vector<int> sf_vec;
    sf_vec.reserve(100);
    sf_vec.push_back(1);
    sf_vec.push_back(2);
    sf_vec.push_back(3);
    std::cout << "Before shrink: Size: " << sf_vec.size() << " | Capacity: " << sf_vec.capacity() << "\n";

    sf_vec.shrink_to_fit();
    std::cout << "After shrink: Size: " << sf_vec.size() << " | Capacity: " << sf_vec.capacity() << "\n";
    std::cout << "Data intact: " << sf_vec << "\n";

    // shrink_to_fit on empty (after clear)
    sf_vec.clear();
    sf_vec.shrink_to_fit();
    std::cout << "After clear + shrink: Size: " << sf_vec.size() << " | Capacity: " << sf_vec.capacity() << "\n";

    // shrink_to_fit when already fitting — no-op
    Vector<int> already_fit({1, 2, 3});
    std::size_t cap = already_fit.capacity();
    already_fit.shrink_to_fit();
    std::cout << "Already fitting (no-op): Capacity unchanged: " << (already_fit.capacity() == cap) << "\n\n";

    std::cout << "--- V15. SWAP ---\n";
    Vector<int> swap_a({1, 2, 3});
    Vector<int> swap_b({10, 20});
    std::cout << "Before: a=" << swap_a << " b=" << swap_b << "\n";
    swap_a.swap(swap_b);
    std::cout << "After:  a=" << swap_a << " b=" << swap_b << "\n";

    // Swap with empty
    Vector<int> swap_empty;
    swap_a.swap(swap_empty);
    std::cout << "Swap with empty: a=" << swap_a << " (size " << swap_a.size()
              << ") empty=" << swap_empty << " (size " << swap_empty.size() << ")\n\n";

    std::cout << "--- V16. VECTOR OF CUSTOM STRING ---\n";
    Vector<String> sv;
    sv.push_back(String("Hello"));
    sv.push_back(String("World"));
    sv.emplace_back("CMU");
    std::cout << "Vector<String> size: " << sv.size() << "\n";
    for (std::size_t i = 0; i < sv.size(); ++i) {
        std::cout << "  [" << i << "] \"" << sv[i] << "\"\n";
    }
    // Copy Vector<String>
    Vector<String> sv_copy(sv);
    std::cout << "Copy size: " << sv_copy.size() << " | [0]=\"" << sv_copy[0] << "\"\n";
    // Move Vector<String>
    Vector<String> sv_moved(std::move(sv_copy));
    std::cout << "Moved size: " << sv_moved.size() << " | Moved-from size: " << sv_copy.size() << "\n\n";

    std::cout << "========== VECTOR TESTS DONE ==========\n\n";
}

int main() {
    test_string();
    test_vector();
    return 0;
}
