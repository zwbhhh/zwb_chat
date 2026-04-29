#ifndef SIGNALTON_H
#define SIGNALTON_H
#include "global.h"
template <typename T>
class Signalton{
protected:
    Signalton() = default;
    Signalton(const Signalton<T>&) = delete;
    Signalton& operator =(const Signalton<T> st) = delete;
    static std::shared_ptr<T> _instance;
public:
    static std::shared_ptr<T> GetInstance(){
        static std::once_flag s_flag;
        std::call_once(s_flag, [&](){
            _instance = std::shared_ptr<T>(new T);
        });

        return _instance;
    }

    void PrintAddress(){
        std::cout<< _instance.get() << std::endl;
    }

    ~Signalton(){
        std::cout << "hide is singleton destruct" <<std::endl;
    }
};

template <typename T>  //静态成员类外初始化(有模板的情况)
std::shared_ptr<T> Signalton<T>::_instance = nullptr;


#endif // SIGNALTON_H
