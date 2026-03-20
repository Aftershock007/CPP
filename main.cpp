#include <iostream>
#include <sstream>
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

    std::cout << "--- 1. CONSTRUCTORS ---\n";
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
    std::cout << "Empty InitList: " << empty_init_vec << " | Size: " << empty_init_vec.size() <<
        "\n";

    const Vector<int> zero_vec(0);
    std::cout << "Sized(0): " << zero_vec << " | Size: " << zero_vec.size() << "\n";

    const Vector<int> null_arr_vec(5, nullptr);
    std::cout << "Null Array: " << null_arr_vec << " | Size: " << null_arr_vec.size() << "\n\n";

    std::cout << "--- 2. COPY SEMANTICS ---\n";
    const Vector<int> copy_constructed(init_vec);
    std::cout << "Copy Constructed: " << copy_constructed << " | Size: " << copy_constructed.size()
        << "\n";

    Vector<int> copy_assigned;
    copy_assigned = init_vec;
    std::cout << "Copy Assigned: " << copy_assigned << " | Size: " << copy_assigned.size() << "\n";

    // Verify deep copy — modifying original shouldn't affect copies
    init_vec[0] = 99;
    std::cout << "Original after mutation: " << init_vec << "\n";
    std::cout << "Copy Constructed (unchanged): " << copy_constructed << "\n";
    std::cout << "Copy Assigned (unchanged): " << copy_assigned << "\n\n";

    std::cout << "--- 3. MOVE SEMANTICS ---\n";
    Vector<int> move_src({1, 2, 3});
    std::cout << "Before move: " << move_src << " | Size: " << move_src.size() << "\n";

    const Vector<int> move_constructed(std::move(move_src));
    std::cout << "Move Constructed: " << move_constructed << " | Size: " << move_constructed.size()
        << "\n";
    std::cout << "Moved-from Size: " << move_src.size() << "\n";

    Vector<int> move_src2({4, 5, 6});
    Vector<int> move_assigned;
    move_assigned = std::move(move_src2);
    std::cout << "Move Assigned: " << move_assigned << " | Size: " << move_assigned.size() << "\n";
    std::cout << "Moved-from Size: " << move_src2.size() << "\n\n";

    std::cout << "--- 4. ELEMENT ACCESS & EXCEPTIONS ---\n";
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

    std::cout << "--- 5. STREAM I/O ---\n";
    std::cout << "Output test: " << Vector<int>({1, 2, 3}) << "\n";

    std::istringstream iss("10 20 30 40");
    Vector<int> stream_vec;
    iss >> stream_vec;
    std::cout << "Read from stream: " << stream_vec << " | Size: " << stream_vec.size() << "\n\n";

    std::cout << "--- 6. SELF-ASSIGNMENT ---\n";
    Vector<int> self_vec({5, 10, 15});
    self_vec = self_vec;
    std::cout << "After self-assignment: " << self_vec << " | Size: " << self_vec.size() << "\n\n";

    std::cout << "--- 7. DESTRUCTORS (watch output below) ---\n";

    return 0;
}
