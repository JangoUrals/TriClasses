#include<fstream>
#include<iostream>
#include<stdint.h>
#include<random>

using std::iostream;
using std::ios;
using std::fstream;

const uint32_t prime_gth_10deg9 = 1073676287;
const uint32_t base = 256;

template <class K> struct MyHashValue
{
    MyHashValue *next;
    K value;

    MyHashValue()
    {
        next = NULL;
    }

    MyHashValue(K obj)
    {
        value = obj;
        next = NULL;
    }
};

template <class K> class MyHash
{
    uint32_t table_size;
    uint32_t p;
    MyHashValue<K> ** table;
public:
    MyHash(uint32_t size, uint32_t prime)
    {
        table_size = size;
        p = prime;
        table = new MyHashValue<K>*[table_size];
        for (register uint32_t i = 0; i < table_size; ++i)
            table[i] = NULL;
    }
    ~MyHash()
    {
        delete[] table;
    }
    // 
    uint32_t h(K &obj)
    {
        return obj.hash() % table_size;
    }
    // 
    bool CheckAndPush(K &obj)
    {
        uint32_t idx = h(obj);
        MyHashValue<K> * val = table[idx];
        if (val == NULL)
        {
            table[idx] = new MyHashValue<K>(obj);
            return true;
        }
        while (val != NULL)
        {
            if (val->value == obj)
                return false;
            if (val->next == NULL)
            {
                val->next = new MyHashValue<K>(obj);
                return true;
            }
            val = val->next;
        }
        return true;
    }
};

class UniversalHash
{
    uint32_t a;
    uint32_t b;
    uint32_t p;
    uint32_t n;

public:
    UniversalHash(uint32_t max_val, uint32_t hash_lenghth)
    {
        p = max_val;
        n = hash_lenghth;
        std::random_device rd;
        std::default_random_engine generator(rd());
        std::uniform_int_distribution<uint32_t> distribution1(1, p - 1);
        a = distribution1(generator);
        std::uniform_int_distribution<uint32_t> distribution2(0, p - 1);
        b = distribution2(generator);
    }
    ~UniversalHash() {};

    uint32_t h(uint32_t k)
    {
        return ((a * k + b) % p) % n;
    }

};

template <class  K> void small_swap(K &num_a, K &num_b)
{
    K swap;
    swap = num_a; num_a = num_b; num_b = swap;
}

template <class  K> void sort_triple(K &num_a, K &num_b, K &num_c)
{
    if (num_a > num_b)
    {
        small_swap(num_a, num_b);
    }
    if (num_b > num_c)
    {
        small_swap(num_b, num_c);
        if (num_a > num_b)
        {
            small_swap(num_a, num_b);
        }
    }
}

template <class  K> K gcd(K m, K n)
{
    K r;
    while (n != 0)
    {
        r = m % n;
        m = n;
        n = r;
    }
    return m;
}

struct tri
{
    uint32_t a, b, c;

    static UniversalHash * h_a, *h_b, *h_c;

    tri()
    {
        a = 0; b = 0; c = 0;
    }

    tri(uint32_t a1, uint32_t b1, uint32_t c1)
    {
        a = a1; b = b1; c = c1;
        sort_triple(a, b, c);
        uint32_t gcd3 = gcd(gcd(a, b), c);
        a /= gcd3; b /= gcd3; c /= gcd3;
    }

    bool operator==(const tri & other)
    {
        return (a == other.a) && (b == other.b) && (c == other.c);
    }

    uint32_t hash()
    {
        return h_a->h(a) + h_b->h(b) * base + h_c->h(c) * base * base;
    }
};

UniversalHash* tri::h_a = new UniversalHash(prime_gth_10deg9, base);
UniversalHash* tri::h_b = new UniversalHash(prime_gth_10deg9, base);
UniversalHash* tri::h_c = new UniversalHash(prime_gth_10deg9, base);

int main()
{
    // open files
    fstream in("input.txt", ios::in);
    if (!in.is_open()) std::cout << "File input.txt is not open\n";
    fstream out("output.txt", ios::out);
    if (!out.is_open()) std::cout << "File output.txt is not open\n";

    // number of triangles 
    uint32_t tri_n;
    in >> tri_n;

    // number of distinct classes of triangles 
    uint32_t class_n = 0;

    // create hash table
    MyHash<tri> hash_table(0x1000000, prime_gth_10deg9);

    // cycle of reading and processing triangles
    uint32_t side_a, side_b, side_c;
    for (register uint32_t i = 0; i < tri_n; ++i)
    {
        in >> side_a >> side_b >> side_c;
        // sort sides of a triangle 
        //tri<uint32_t> triangle(side_a, side_b, side_c);
        tri triangle(side_a, side_b, side_c);
        //sort_sides(side_a, side_b, side_c);
        //simplify_tri(side_a, side_b, side_c);
        if (hash_table.CheckAndPush(triangle)) ++class_n;
        // 
        out << triangle.a << " " << triangle.b << " " << triangle.c << std::endl;
    }

    // write an answer
    out << class_n;

    // close files 
    in.close();
    out.close();
}

