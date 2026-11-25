#include <iostream>
#include <limits>
#include <cstring>
using namespace std;

class SSOStr {
private:
    char* arr;
    short size_;
    short cap;

    static int shortCap()
    {
        return sizeof(SSOStr)-2;
    }

    char* raw()
    {
        return reinterpret_cast<char*>(this);
    }

    const char* raw() const
    {
        return reinterpret_cast<const char*>(this);
    }

    bool isShort() const
    {
        return raw()[0]&1;
    }

    char* data()
    {
        if(isShort())return raw()+1;
        return arr;
    }

    const char* data() const
    {
        if(isShort())return raw()+1;
        return arr;
    }

    void init(int n)
    {
        if(n>std::numeric_limits<short>::max()) cerr<<"The string is too long";
        if(n<=shortCap())
        {
            raw()[0]=static_cast<char>((n<<1)|1);
        }
        else
        {
            arr=new char[n+1];
            size_=cap=static_cast<short>(n);
        }
        data()[n]='\0';
    }

    void release()
    {
        if(!isShort())delete[] arr;
    }

public:
    SSOStr(const char* str="")
    {
        int n=strlen(str);
        init(n);
        memcpy(data(),str,n); //strncpy zastanuva ako ima null terminator vo stringot
    }

    SSOStr(const SSOStr& other)
    {
        init(other.size());
        memcpy(data(),other.data(),other.size());
    }

    ~SSOStr()
    {
        release();
    }

    SSOStr& operator=(const SSOStr& other)
    {
        if(this==&other)return *this;
        int n=other.size();
        if(other.isShort())
        {
            release();
            init(n);
            memcpy(data(),other.data(),n);
        }
        else
        {
            char* p=new char[n+1];
            memcpy(p,other.data(),n+1);
            release();
            arr=p;
            size_=cap=static_cast<short>(n);
        }
        return *this;
    }

    int size() const
    {
        if(isShort()) return static_cast<char>(raw()[0])>>1;
        return size_;
    }

    int capacity() const
    {
        if(isShort()) return shortCap();
        return cap;
    }

    char& operator[](int i)
    {
        if(i>=0&&i<size())return data()[i];
        cerr<<"Out of range"<<i<<endl;
    }

    const char& operator[](int i) const
    {
        if(i>=0&&i<size())return data()[i];
        cerr<<"Out of range"<<i<<endl;
    }

    friend SSOStr operator+(const SSOStr& a,const SSOStr& b)
    {
        SSOStr res;
        res.init(a.size()+b.size());
        memcpy(res.data(),a.data(),a.size());
        memcpy(res.data()+a.size(),b.data(),b.size());
        return res;
    }

    friend ostream& operator<<(ostream& out,const SSOStr& s)
    {
        return out<<s.data();
    }
};

int main() {
    SSOStr a("Hello");
    SSOStr b("World");

    SSOStr c = a + b;
    SSOStr d = c + c;

    std::cout << sizeof(SSOStr) << '\n';

    std::cout << c << ' '
              << c.size() << ' '
              << c.capacity() << '\n';

    std::cout << d << ' '
              << d.size() << ' '
              << d.capacity() << '\n';

    d = a;
    d[0] = 'F';

    std::cout << d << '\n';
}
