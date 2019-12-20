#include <iostream>
#include <functional>

template <typename T>
class optional
{
public:
    optional() : m_valid(false), m_value() {}
    optional(const T& t) : m_valid(true), m_value(t) {}
    optional(T&& t) : m_valid(true), m_value(std::move(t)) {}
    optional(const optional<T>& ot) : m_valid(ot.valid()), m_value(*ot) {}
    optional(optional<T>&& ot) : m_valid(ot.valid()), m_value(std::move(*ot)) {}

    T& operator*() { return m_value; }
    const T& operator*() const { return m_value; }

    bool valid() const { return m_valid; }
    
    template<typename F>
    optional<F> operator&(optional<F> f(T))
    {
        std::cout << "inside bind operation" << std::endl;
        return m_valid ? f(m_value) : optional<F>();
    }
    
    template<typename F>
    auto operator&(F f) -> optional<decltype(f(**this))>
    {
        typedef decltype(f(**this)) Rtype;
        std::cout << "inside fmap operation" << std::endl;
        return m_valid ? optional<Rtype>(f(m_value)) : optional<Rtype>();
    }
    
    operator bool() const
    {
        return m_valid;
    }
    
private:
    bool m_valid;
    T m_value;
};


double func(int a)
{
    return double(a);
}

template<typename T>
bool print(T t)
{
    std::cout << t << std::endl;
        
    return true;
}

optional<int> optional_func(int val)
{
    return optional<int>(val*2);
}

int main()
{
    int x = 10;
    optional<int> a;
    optional<int> b(3);
    optional<int> c(x);
    optional<int> d(b);
    optional<int> e(a);
    
    auto y = a & ([](int val) { std::cout << "a = " << val << std::endl; return val; });
    auto z = b & ([](int val) { std::cout << "b = " << val << std::endl; return val; });
    c & ([](int val) { std::cout << "c = " << val << std::endl; return val; });
    d & ([](int val) { std::cout << "d = " << val << std::endl; return val; });
    e & ([](int val) { std::cout << "e = " << val << std::endl; return val; });
    
    if(y) std::cout << "y = " << *y << std::endl;
    if(z) std::cout << "z = " << *z << std::endl;

    b & func & print<double>;
    
    c & optional_func & print<int>;
    
    

    return 0;
}

