#include <iostream>
#include <stdexcept>
// #include "String.h"
#include "Vector.h"

int main() {
    // std::cout << "--- 1. CONSTRUCTORS ---\n";
    // String default_str;
    // const String param_str("Google");
    // const String null_str(nullptr);
    // std::cout << "Param: " << param_str << " | Size: " << param_str.size() << "\n";
    // std::cout << "Null: " << null_str << " | Size: " << null_str.size() << "\n\n";
    //
    // std::cout << "--- 2. COPY SEMANTICS ---\n";
    // String copy_constructed(param_str);
    // String copy_assigned;
    // copy_assigned = param_str;
    // std::cout << "Copy Constructed: " << copy_constructed << "\n";
    // std::cout << "Copy Assigned: " << copy_assigned << "\n\n";
    //
    // std::cout << "--- 3. MOVE SEMANTICS ---\n";
    // const String move_constructed(std::move(copy_constructed));
    // String move_assigned;
    // move_assigned = std::move(copy_assigned);
    //
    // // Verifying moved-from states are valid and empty
    // std::cout << "Moved-into Constructed: " << move_constructed << "\n";
    // std::cout << "Moved-from Constructed Size: " << copy_constructed.size() << "\n";
    // std::cout << "Moved-into Assigned: " << move_assigned << "\n";
    // std::cout << "Moved-from Assigned Size: " << copy_assigned.size() << "\n\n";
    //
    // std::cout << "--- 4. ELEMENT ACCESS & EXCEPTIONS ---\n";
    // String access_str("Bustub");
    // access_str[0] = 'b'; // Non-const operator[]
    // std::cout << "Mutated string: " << access_str << "\n";
    //
    // try {
    //     const char c = access_str[10]; // Should throw
    //     std::cout << c << "\n";
    // } catch (const std::out_of_range& e) {
    //     std::cout << "Caught expected exception: " << e.what() << "\n\n";
    // }
    //
    // std::cout << "--- 5. RELATIONAL OPERATORS ---\n";
    // const String a("Apple");
    // const String b("Banana");
    // const String a2("Apple");
    // const String c("Apples");
    //
    // std::cout << "a == a2 (1 expected): " << (a == a2) << "\n";
    // std::cout << "a == b  (0 expected): " << (a == b) << "\n";
    // std::cout << "a < b   (1 expected): " << (a < b) << "\n";
    // std::cout << "b > a   (1 expected): " << (b > a) << "\n";
    // std::cout << "a < c   (1 expected): " << (a < c) << "\n\n";
    //
    // std::cout << "--- 6. STREAM I/O ---\n";
    // String input_str;
    // std::cout << "Type a test string without spaces: ";
    // if (std::cin >> input_str) {
    //     std::cout << "You entered via operator>> : " << input_str << "\n";
    //     std::cout << "Size: " << input_str.size() << "\n";
    // }

    // const Vector<int> vector1({1,2,3,4,5});
    // const int arr1[5] = {1,2,3,4,5};
    // const Vector<int> vector1(5, arr1);
    // const Vector<int> vector1({1,2,3,4,5});
    std::size_t size;
    std::cin >> size;
    Vector<int> vector1(size);
    for (std::size_t i = 0; i < size; ++i) {
        std::cin >> vector1[i];
    }
    Vector<int> vector2;
    vector2 = vector1;
    std::cout << vector2 << std::endl;

    return 0;
}
