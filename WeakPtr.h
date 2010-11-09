// Copyright (C) 1993-2002 David R. Cheriton.  All rights reserved.

#ifndef FWK_WEAKPTR_H
#define FWK_WEAKPTR_H

namespace Fwk {

template <class T>
class WeakPtr
{
public:
    WeakPtr(T* p = 0) : ptr_(p) { }
    WeakPtr(const WeakPtr<T>& mp) : ptr_(mp.ptr_) { }
    ~WeakPtr() { }

    WeakPtr<T>& operator=( const WeakPtr<T>& mp );
    WeakPtr<T>& operator=( WeakPtr<T>& mp );
    WeakPtr<T>& operator=( const Ptr<T>& mp );
    WeakPtr<T>& operator=( Ptr<T>& mp );
    WeakPtr<T>& operator=( T* p );

    bool operator==( const WeakPtr<T>& mp ) const { return ptr_ == mp.ptr_; }
    bool operator!=( const WeakPtr<T>& mp ) const { return ptr_ != mp.ptr_; }
    bool operator==( const Ptr<T>& mp ) const { return ptr_ == mp.ptr(); }
    bool operator!=( const Ptr<T>& mp ) const { return ptr_ != mp.ptr(); }
    bool operator==( T* p ) const { return ptr_ == p; }
    bool operator!=( T* p ) const { return ptr_ != p; }

    const T * operator->() const { return ptr_; }
    T * operator->() { return ptr_; }
    T * ptr() const { return ptr_; }

    template <class OtherType>
    operator WeakPtr<OtherType>() const { return WeakPtr<OtherType>( ptr_ ); }

    struct PointerConversion { int valid; };
    operator int PointerConversion::*() const {
        return ptr_ ? &PointerConversion::valid : 0;
    }

protected:
    T *ptr_;
};

template<class T> WeakPtr<T>&
WeakPtr<T>::operator=( const WeakPtr<T>& mp ) {
    ptr_ = mp.ptr_;
    return *this;
}

template<class T> WeakPtr<T>&
WeakPtr<T>::operator=( WeakPtr<T>& mp ) {
    ptr_ = mp.ptr_;
    return *this;
}

template<class T> WeakPtr<T>&
WeakPtr<T>::operator=( const Ptr<T>& mp ) {
    ptr_ = mp.ptr();
    return *this;
}

template<class T> WeakPtr<T>&
WeakPtr<T>::operator=( Ptr<T>& mp ) {
    ptr_ = mp.ptr();
    return *this;
}

template<class T> WeakPtr<T>&
WeakPtr<T>::operator=( T* p ) {
    ptr_ = p;
    return *this;
}

template <class T, class U>
WeakPtr<T> ptr_cast(WeakPtr<U> mp) {
    return dynamic_cast<T*>(mp.ptr());
}

}

#endif /* FWK_PTR_H */
