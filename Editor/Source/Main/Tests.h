#pragma once

#include <cstring>

#include "Oniun/Core/Defines.h"
#include "Oniun/Core/Logger.h"
#include "Oniun/Core/String/Format.h"
#include "Oniun/Core/String/String.h"
#include "Oniun/Core/Templates/Array.h"
#include "Oniun/Core/Templates/HashMap.h"

namespace Test
{
    inline void StringType()
    {
        // Default constructor
        {
            String str;
            ASSERT(str.Length() == 0);
        }

        // Resize function
        {
            String str;
            str.Resize(20);
            ASSERT(str.Length() == 20);
        }

        // Clear function
        {
            String str;
            str.Resize(20);
            str.Clear();
            ASSERT(str.Length() == 0);
        }

        // Constructor with capacity
        {
            String str;
            String strWithCapacity(10);
            ASSERT(strWithCapacity.Length() == 0);
        }

        // constructor with C-string
        {
            String str("Hello");
            ASSERT(str.Length() == 5);
            ASSERT(std::strcmp(str.Data(), "Hello") == 0);
        }

        // constructor with C-string and length
        {
            String str("Hello, World!", 5);
            ASSERT(str.Length() == 5);
            ASSERT(std::strcmp(str.Data(), "Hello") == 0);
        }

        // copy constructor
        {
            String original("Hello");
            String str(original);
            ASSERT(str.Length() == original.Length());
            ASSERT(std::strcmp(str.Data(), original.Data()) == 0);
        }

        // move constructor
        {
            String original("Hello");
            String str(Memory::Move(original));
            ASSERT(str.Length() == 5);
            ASSERT(std::strcmp(str.Data(), "Hello") == 0);
        }

        // assignment operator
        {
            String str;
            String source("Hello");
            str = source;
            ASSERT(str.Length() == 5);
            ASSERT(std::strcmp(str.Data(), "Hello") == 0);
        }

        // Append function
        {
            String str("Hello");
            str.Append(" World World World World");
            ASSERT(str.Length() == 29);
            ASSERT(std::strcmp(str.Data(), "Hello World World World World") == 0);
        }

        // concatenation operator
        {
            String str("Hello World");
            String result = str + "!";
            ASSERT(result.Length() == 12);
            ASSERT(std::strcmp(result.Data(), "Hello World!") == 0);
        }

        // / operator for concatenation
        {
            String str("Hello World");
            String path = str / "path";
            ASSERT(path.Length() == 16); // Length should include the separator
            ASSERT(std::strcmp(path.Data(), "Hello World/path") == 0); // Assuming Unix path separator
        }

        // using const Char* + String
        {
            String str("Hello");
            String result = "Greetings, " + str;
            ASSERT(result.Length() == 16);
            ASSERT(std::strcmp(result.Data(), "Greetings, Hello") == 0);
        }

        // Substring function
        {
            String str("Hello World");
            String substring = str.Substring(1, 2);
            ASSERT(substring.Length() == 2);
            ASSERT(std::strcmp(substring.Data(), "el") == 0);
        }

        // Find substring
        {
            String str("Hello World, Hello");
            ASSERT(str.Find(String("Hello")) == 0);
            ASSERT(str.FindLast(String("Hello")) == 13);
        }

        // TrimTrailing function
        {
            String str("Hello   ");
            String trimmed = str.TrimTrailing();
            ASSERT(trimmed.Length() == 5);
            ASSERT(std::strcmp(trimmed.Data(), "Hello") == 0);
        }

        // {
        //     String str("Hello World, Hello");
        //
        //     // Find and replace first
        //     {
        //         String findReplaceFirst = str.ReplaceFirst("Hello", "Replaced");
        //         ASSERT(std::strcmp(findReplaceFirst.Data(), "Replaced World, Hello") == 0);
        //     }
        //
        //     // Find and replace last
        //     {
        //         String findReplaceLast = str.ReplaceLast("Hello", "word");
        //         ASSERT(std::strcmp(findReplaceLast.Data(), "Hello World, word") == 0);
        //     }
        //
        //     // Find and replace all
        //     {
        //         String findReplaceAll = str.Replace("Hello", "yipeee");
        //         ASSERT(std::strcmp(findReplaceAll.Data(), "yipeee World, yipeee") == 0);
        //     }
        // }

        // Parse function
        {
            int64 res1;
            double res2;
            String::Parse("-123123", &res1);
            String::Parse("321.123", &res2);
            ASSERT(res1 == -123123);
            ASSERT(res2 == 321.123);
        }

        // Format
        {
            FixedArray<int32, 10> arr = { 1, 2, 3, 4, 5, 6, 7, 8 };
            String formatted(*Format("{}: {}", "Fixed array", arr));
            ASSERT(formatted == "Fixed array: [ 1, 2, 3, 4, 5, 6, 7, 8 ]");
        }

        LOG(Info, "All String tests passed");
    }

    inline void HeapArrayType()
    {
        // Default constructor
        {
            Array<uint64> arr;
            ASSERT(arr.Count() == 0);
            ASSERT(arr.Capacity() == 0);
        }

        // Constructor with capacity
        {
            Array<uint64> arr(5);
            ASSERT(arr.Count() == 0);
            ASSERT(arr.Capacity() == 5);
        }

        // Adding elements
        {
            Array<uint64> another;
            another.Add(10);
            another.Add(20);
            another.Add(30);
            ASSERT(another.Count() == 3);
            ASSERT(another[0] == 10);
            ASSERT(another[1] == 20);
            ASSERT(another[2] == 30);
        }

        // Accessing elements
        {
            Array<uint64> arr;
            ASSERT(arr.Count() == 0); // arr should still be empty
        }

        // Inserting an element
        {
            Array<uint64> arr(5);
            for (uint64 i = 0; i < 30; ++i)
                arr.Add(i);
            arr.Insert(15, 1);
            ASSERT(arr.Count() == 31);
            ASSERT(arr[1] == 15);
        }

        // Removing an element
        {
            Array<uint64> arr(5);
            for (uint64 i = 0; i < 30; ++i)
                arr.Add(i);
            arr.Remove(20);
            ASSERT(arr.Count() == 29); // 20 should be removed
        }

        // Popping an element
        {
            Array<uint64> arr(5);
            for (uint64 i = 0; i < 30; ++i)
                arr.Add(i);
            uint64 poppedValue = arr.Pop();
            ASSERT(poppedValue == 29); // Assuming 29 is the last element
            ASSERT(arr.Count() == 29);
        }

        // Clearing the array
        {
            Array<uint64> arr(5);
            for (uint64 i = 0; i < 30; ++i)
                arr.Add(i);
            arr.Clear();
            ASSERT(arr.Count() == 0);
            ASSERT(arr.Capacity() == 32);
        }

        // Using initializer list
        {
            Array arr = {1, 2, 3, 4, 5};
            ASSERT(arr.Count() == 5);
            ASSERT(arr[0] == 1);
            ASSERT(arr[1] == 2);
            ASSERT(arr[2] == 3);
            ASSERT(arr[3] == 4);
            ASSERT(arr[4] == 5);
        }

        LOG(Info, "All Heap Array tests passed");
    }

    inline void HeapHashMapType()
    {
        // Default constructor
        {
            HashMap<StringView, int32> map;
            ASSERT(map.Count() == 0);
            ASSERT(map.IsEmpty());
        }

        // Adding a key-value pair
        {
            HashMap<StringView, int32> map;
            map.Add("key1", 42);
            ASSERT(map.Count() == 1);
            ASSERT(!map.IsEmpty());

            // Verify that the value can be retrieved
            int32 value = map.Get("key1");
            ASSERT(value == 42);

            // Verify that trying to get a non-existent key returns nullptr
            ASSERT(map.TryGet("key5") == nullptr);
        }

        // Adding multiple key-value pairs
        {
            HashMap<StringView, int32> map;
            map.Add("key1", 10);
            map.Add("key2", 20);
            map.Add("key3", 30);

            ASSERT(map.Count() == 3);
            ASSERT(map.Get("key1") == 10);
            ASSERT(map.Get("key2") == 20);
            ASSERT(map.Get("key3") == 30);
        }

        // Clearing the map
        {
            HashMap<StringView, int32> map;
            map.Add("key1", 100);
            map.Clear();
            ASSERT(map.Count() == 0);
            ASSERT(map.IsEmpty());
        }

        // Iterator functionality
        {
            HashMap<StringView, int32> map;
            map.Add("key1", 1);
            map.Add("key2", 2);
            map.Add("key3", 3);

            // iterator
            int32 sum = 0;
            for (const auto& bucket : map)
                sum += bucket.Value;
            ASSERT(sum == 6); // 1 + 2 + 3 = 6
        }

        // Iterator with empty map
        {
            HashMap<StringView, int32> map;
            ASSERT(map.begin() == map.end()); // Iterator should be at end for an empty map
        }

        // Resize and rehash
        {
            // Required checks for this test to work
            ASSERT(Hash<StringView>{}.Get("Key32") % 10 == 7);
            ASSERT(Hash<StringView>{}.Get("Key3") % 10 == 7);
            ASSERT(Hash<StringView>{}.Get("Key32") % 20 == 7);
            ASSERT(Hash<StringView>{}.Get("Key3") % 20 == 17);

            HashMap<StringView, int32> map(10);

            map.Add("Key32", 1);
            ASSERT(map.Capacity() == 10);;
            ASSERT(map.TryGet("Key32"));
            ASSERT(map.Get("Key32") == 1);

            map.Add("Key3", 2);
            ASSERT(map.Capacity() == 20);
            ASSERT(map.TryGet("Key32"));
            ASSERT(map.Get("Key32") == 1);
            ASSERT(map.TryGet("Key3"));
            ASSERT(map.Get("Key3") == 2);
        }

        // Operator [] adding and getting
        {
            HashMap<StringView, int32> map;
            map["Key"] = 32;
            ASSERT(map.Count() == 1);
            ASSERT(map.Get("Key") == 32);
        }

        {
            HashMap<StringView, int32> map;
            map.Add("Key1", 1);
            map.Add("Key2", 2);
            map.Add("Key3", 3);

            HashMap<StringView, int32> copyed(map);

            ASSERT(copyed.TryGet("Key1"));
            ASSERT(copyed.Get("Key1") == 1);

            ASSERT(copyed.TryGet("Key2"));
            ASSERT(copyed.Get("Key2") == 2);

            ASSERT(copyed.TryGet("Key3"));
            ASSERT(copyed.Get("Key3") == 3);
        }

        LOG(Info, "All HashMap tests passed");
    }
}
