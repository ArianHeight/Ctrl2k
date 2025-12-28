#include "tests.h"

#include <iostream>

#include "Core/OracleBone/obn.h"

struct ExpectedSize
{
    size_t size;
    size_t cap;
    size_t max_length;
    size_t length;
};

template <typename stringtype, typename chartype>
void testStringSize(const chartype* in, const ExpectedSize& expected)
{
    stringtype s(in);
    assert(sizeof(s) == expected.size);
    assert(s.capacity() == expected.cap);
    assert(s.max_length() == expected.max_length);
    assert(s.length() == expected.length);
}

void testSmallStringSize()
{
    std::cout << subtestPretext << "Testing Small String 8 Size\n";
    testStringSize<obn::small_string8, char>("blue", { 8 * sizeof(char), 7, 6, 4 });
    testStringSize<obn::small_string8, char>("bluetooth", { 8 * sizeof(char), 7, 6, 6 });
    std::cout << subtestPretext << "Testing Small String 16 Size\n";
    testStringSize<obn::small_string16, char>("bluetooth", { 16 * sizeof(char), 15, 14, 9 });
    testStringSize<obn::small_string16, char>("bluetooth headset", { 16 * sizeof(char), 15, 14, 14 });
    std::cout << subtestPretext << "Testing Small String 32 Size\n";
    testStringSize<obn::small_string32, char>("bluetooth headset", { 32 * sizeof(char), 31, 30, 17 });
    testStringSize<obn::small_string32, char>("bluetooth wireless headset charging", { 32 * sizeof(char), 31, 30, 30 });
    std::cout << subtestPretext << "Testing Small String 64 Size\n";
    testStringSize<obn::small_string64, char>("bluetooth wireless headset", { 64 * sizeof(char), 63, 62, 26 });
    testStringSize<obn::small_string64, char>("bluetooth wireless headset charging through the wall socket is at 50%",
        { 64 * sizeof(char), 63, 62, 62 });
    std::cout << subtestPretext << "Testing Small String 128 Size\n";
    testStringSize<obn::small_string128, char>("bluetooth wireless headset charging", { 128 * sizeof(char), 127, 126, 35 });
    testStringSize<obn::small_string128, char>(
        "bluetooth wireless headset charging through the wall socket is at 50%, the lamp is off, and the windows are open. The night is young and the protagonist does not want to sleep.",
        { 128 * sizeof(char), 127, 126, 126 });

    std::cout << subtestPretext << "Testing Wide Small String 8 Size\n";
    testStringSize<obn::wsmall_string8, wchar_t>(L"blue", { 8 * sizeof(wchar_t), 7, 6, 4 });
    testStringSize<obn::wsmall_string8, wchar_t>(L"bluetooth", { 8 * sizeof(wchar_t), 7, 6, 6 });
    std::cout << subtestPretext << "Testing Wide Small String 64 Size\n";
    testStringSize<obn::wsmall_string64, wchar_t>(L"bluetooth wireless headset", { 64 * sizeof(wchar_t), 63, 62, 26 });
    testStringSize<obn::wsmall_string64, wchar_t>(L"bluetooth wireless headset charging through the wall socket is at 50%",
        { 64 * sizeof(wchar_t), 63, 62, 62 });
    std::cout << subtestPretext << "Testing Wide Small String 128 Size\n";
    testStringSize<obn::wsmall_string128, wchar_t>(L"bluetooth wireless headset charging", { 128 * sizeof(wchar_t), 127, 126, 35 });
    testStringSize<obn::wsmall_string128, wchar_t>(
        L"bluetooth wireless headset charging through the wall socket is at 50%, the lamp is off, and the windows are open. The night is young and the protagonist does not want to sleep.",
        { 128 * sizeof(wchar_t), 127, 126, 126 });
}

void testFixedStringSize()
{
    std::cout << subtestPretext << "Testing Fixed String 12 Size\n";
    testStringSize<obn::stack_string<12>, char>("blue", { sizeof(obn::stack_string<12>), 12, 11, 4 }); // don't care about struct size
    testStringSize<obn::stack_string<12>, char>("bluetooth headset", { sizeof(obn::stack_string<12>), 12, 11, 11 }); // don't care about struct size

    std::cout << subtestPretext << "Testing Wide Fixed String 12 Size\n";
    testStringSize<obn::wstack_string<12>, wchar_t>(L"blue", { sizeof(obn::wstack_string<12>), 12, 11, 4 }); // don't care about struct size
    testStringSize<obn::wstack_string<12>, wchar_t>(L"bluetooth headset", { sizeof(obn::wstack_string<12>), 12, 11, 11 }); // don't care about struct size
}

void testDynamicStringSize()
{
    std::cout << subtestPretext << "Testing Dynamic String 0 Size\n";
    testStringSize<obn::dyn::heap_string0, char>("", { 32, 1, SIZE_MAX - 1, 0 });
    testStringSize<obn::dyn::heap_string0, char>("blue", { 32, 5, SIZE_MAX - 1, 4 });
    std::cout << subtestPretext << "Testing Dynamic String Size\n";
    testStringSize<obn::dyn::heap_string, char>("", { 32, 32, SIZE_MAX - 1, 0 });
    testStringSize<obn::dyn::heap_string, char>("blue", { 32, 32, SIZE_MAX - 1, 4 });
    testStringSize<obn::dyn::heap_string, char>("bluetooth wireless headset charging", { 32, 36, SIZE_MAX - 1, 35 });

    std::cout << subtestPretext << "Testing Wide Dynamic String 0 Size\n";
    testStringSize<obn::dyn::wheap_string0, wchar_t>(L"", { 32, 1, SIZE_MAX - 1, 0 });
    testStringSize<obn::dyn::wheap_string0, wchar_t>(L"blue", { 32, 5, SIZE_MAX - 1, 4 });
    std::cout << subtestPretext << "Testing Wide Dynamic String Size\n";
    testStringSize<obn::dyn::wheap_string, wchar_t>(L"", { 32, 32, SIZE_MAX - 1, 0 });
    testStringSize<obn::dyn::wheap_string, wchar_t>(L"blue", { 32, 32, SIZE_MAX - 1, 4 });
    testStringSize<obn::dyn::wheap_string, wchar_t>(L"bluetooth wireless headset charging", { 32, 36, SIZE_MAX - 1, 35 });
}

// DO NOT use the same string for any of the inputs
template <typename stringtype, typename chartype>
void testStringGeneral(const chartype* small_in, const chartype* in, const chartype* large_in, bool can_resize)
{
    stringtype s1 = small_in;
    stringtype s = in;
    stringtype s2 = large_in; // truncates if cannot resize
    assert(!s1.has_error());
    assert(!s.has_error());
    assert(can_resize || s2.has_error());
    s = s1;
    assert(!s.has_error());
    s = s2; // error flag is copied
    assert(can_resize || s2.has_error());
    s.reset_error();
    s2.reset_error();
    assert(!s.has_error());
    assert(!s2.has_error());

    assert(s == s2);
    assert(s != s1);

    if(can_resize)
    {
        stringtype s3 = s2;
        s1 = std::move(s2);
        s.clear();
        assert(s2.c_str() != s1.c_str());
        assert(s == s2);
        assert(s1 == s3);
    }
    else
    {
        s1 = std::move(s2);
        assert(s1 == s2);
    }

    constexpr size_t cutPos = 3;
    stringtype ss1 = in;
    assert(cutPos < ss1.length());
    stringtype ss2 = ss1.substring(cutPos);
    assert(ss1.starts_with(ss2));
    stringtype ss3 = ss1.substring(cutPos, ss1.length() - cutPos);
    assert(ss1.ends_with(ss3));
    assert(ss2 != ss3);
}

template<typename stringtype, typename chartype>
void testViewStringGeneral(const chartype* in)
{
    constexpr size_t cutPos = 10;
    size_t len = obn::string_len(in);
    assert(len > cutPos);

    stringtype vs1(in, len);
    assert(vs1.length() == vs1.size());
    assert(vs1.length() == len);
    assert(vs1.starts_with(vs1[0]));
    assert(vs1.ends_with(vs1[len - 1]));
    
    stringtype vs2 = vs1.substring(cutPos);
    assert(vs1.starts_with(vs2));
    assert(vs1.starts_with(vs2.c_str(), vs2.length()));

    stringtype vs3 = vs1.substring(cutPos, len - cutPos);
    assert(vs1.ends_with(vs3));
    assert(vs1.ends_with(vs3.data(), vs3.size()));

    assert(!vs3.empty());
    vs3.clear();
    assert(vs3.empty());

    stringtype vs4;
    assert(vs4.empty());
    const obn::simple_string<obn::stack_string_data, chartype, 64> incopy = in;
    vs4 = incopy.view();
    assert(vs4 == vs1);
    assert(vs1 == vs1);
    assert(vs1 != vs2);
    assert(vs4 != vs3);

    stringtype vs5 = incopy.subview(cutPos);
    assert(vs5 == vs2);
    assert(incopy.subview(cutPos, len - cutPos) == vs1.substring(cutPos, len - cutPos));
}

void runStringBasicTests()
{
    std::cout << subtestPretext << "Testing Basic Dynamic String Functionality\n";
    testStringGeneral<obn::dyn::heap_string, char>("", "blue", "bluetooth wireless headset charging", true);
    std::cout << subtestPretext << "Testing Basic Wide Dynamic String Functionality\n";
    testStringGeneral<obn::dyn::wheap_string, wchar_t>(L"", L"blue", L"bluetooth wireless headset charging", true);
    std::cout << subtestPretext << "Testing Basic Fixed String Functionality\n";
    testStringGeneral<obn::stack_string<12>, char>("", "blue", "bluetooth wireless headset charging", false);
    std::cout << subtestPretext << "Testing Basic Wide Small String Functionality\n";
    testStringGeneral<obn::wsmall_string8, wchar_t>(L"", L"blue", L"bluetooth", false);

    std::cout << subtestPretext << "Testing Basic View String Functionality\n";
    testViewStringGeneral<obn::view_string, char>("bluetooth wireless headset charging");
    std::cout << subtestPretext << "Testing Basic Wide View String Functionality\n";
    testViewStringGeneral<obn::view_wstring, wchar_t>(L"bluetooth wireless headset charging");
}

void runDynamicStringMemTest()
{
    std::cout << subtestPretext << "Testing Dynamic String Memory\n";
    obn::dyn::wheap_string0 wd = L"blue";
    obn::dyn::wheap_string wd1 = std::move(wd);
    assert(wd.c_str() != wd1.c_str());
    assert(wd.length() == 0);
    assert(wd1.length() == 4);
    wd = wd1;
    assert(wd == wd1);
    obn::wsmall_string64 s = L"bluetooth";
    wd = s;
    wd1 = std::move(s);
    assert(wd == wd1);
    obn::wsmall_string64 s1 = std::move(wd);
    assert(wd.length() > 0);
    assert(s1 == wd1);
    wd1.clear();
    assert(wd1 == L"");
}

void runStringAppendTest()
{
    std::cout << subtestPretext << "Testing String Append Functionality\n";
    obn::small_string8 s = "blue";
    s.append('t');
    assert(s.length() == 5);
    s.append("ooth");
    assert(s.length() == 6);
    assert(s.has_error());
    obn::stack_string<7> f = "blue";
    f += "to";
    assert(f.length() == 6);
    assert(!f.has_error());
    assert(f == s);
    obn::dyn::heap_string d = s;
    obn::stack_string<32> f1 = "oth wireless headset charging";
    d += f1;
    assert(d == "bluetooth wireless headset charging");
    d.shrink_to_fit();
    assert(d.capacity() == 36);
}

void runStringFindTest()
{
    std::cout << subtestPretext << "Testing String Find Functionality\n";
    obn::small_string32 s = "bluetooth";
    assert(s.starts_with('b'));
    assert(s.starts_with("blue"));
    assert(s.ends_with('h'));
    assert(s.ends_with("tooth"));
    obn::dyn::heap_string d = s;
    assert(s.starts_with(d));
    assert(s.ends_with(d));

    assert(s.substring(4) == "blue");
    assert(s.substring(4, 5) == "tooth");
    assert(d.substring(4) == "blue");
    assert(d.substring(4, 5) == "tooth");
}

void runStringMiscTest()
{
    std::cout << subtestPretext << "Testing String Misc Functionality\n";
    const obn::small_string8 s = "blue";
    assert(s[0] == 'b' && s[1] == 'l' && s[2] == 'u' && s[3] == 'e');
    obn::small_string8 s1 = s;
    s1[3] = 'd';
    assert(s1 == "blud");
    obn::small_string16 s2 = "bluetooth";
    s2.replace('o', 'e');
    assert(s2 == "blueteeth");

    const obn::dyn::heap_string d = "blue";
    assert(d[0] == 'b' && d[1] == 'l' && d[2] == 'u' && d[3] == 'e');
    obn::dyn::heap_string d1 = d;
    d1[3] = 'd';
    assert(d1 == "blud");
    obn::dyn::heap_string d2 = "bluetooth";
    d2.replace('o', 'e');
    assert(d2 == "blueteeth");
}

template<typename T>
void testStringRegistry(T& test)
{
    std::string val = "testing";
    assert(!test.is_registered(val.c_str(), val.size()));
    obn::string_registry_id val_ptr = test.register_string(val.c_str(), val.size());
    assert(val_ptr != obn::INVALID_STRING_REGISTRY_ID);
    assert(test.is_registered(val.c_str(), val.size()));
    assert(test.find_registered_string(val.c_str(), val.size()) == val_ptr);
    std::string val2 = "bluey";
    obn::string_registry_id val_ptr2 = test.register_string(val2.c_str(), val2.size());
    std::string val3 = "Hello, this string needs to be longer thatn 32 bytes";
    obn::string_registry_id val_ptr3 = test.register_string(val3.c_str(), val3.size());
    assert(test.find_registered_string(val.c_str(), val.size()) == val_ptr);
    assert(test.find_registered_string(val2.c_str(), val2.size()) == val_ptr2);
    assert(test.find_registered_string(val3.c_str(), val3.size()) == val_ptr3);
}

void runStringRegistryTests()
{
    {
        std::cout << subtestPretext << "Testing String Registry Functionality\n";
        obn::dyn::string_registry testRegistry(16);
        testStringRegistry(testRegistry);
    }
    {
        std::cout << subtestPretext << "Testing Fixed String Registry Functionality\n";
        obn::string_registry_default testRegistry;
        testStringRegistry(testRegistry);
    }
}

void runStringTests()
{
    std::cout << "\n***********************************\nRunning Tests For OracleBone...\n";
    testSmallStringSize();
    testFixedStringSize();
    testDynamicStringSize();
    runStringBasicTests();
    runDynamicStringMemTest();
    runStringAppendTest();
    runStringFindTest();
    runStringMiscTest();
    runStringRegistryTests();
}