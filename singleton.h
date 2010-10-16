#ifndef SINGLETON_H
#define SINGLETON_H

namespace Shipping {

template <typename SingletonType>
class Singleton {
  public:
    static SingletonType* instance() {
        if(!m_pInstance) m_pInstance = new SingletonType;
        assert(m_pInstance !=NULL);
        return m_pInstance;
    }
  protected:
    Singleton();
    ~Singleton();
  private:
    Singleton(Singleton const&) {};
    Singleton& operator=(Singleton const&) {};
    static SingletonType* m_pInstance;
};

template <class SingletonType> SingletonType* Singleton<SingletonType>::m_pInstance=NULL;

#endif