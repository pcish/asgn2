#ifndef LOG_H
#define LOG_H

#include    <iostream>
#include    <string>
#include    "fwk/NamedInterface.h"
#include    "fwk/Ptr.h"
#include    "fwk/PtrInterface.h"

using namespace std;

class Log : public Fwk::NamedInterface{
public:
    enum Priority{
        Null,
        Critical,
        Error,
        Warning,
        Status,
        Debug
    };
    virtual void entryNew(Priority, Fwk::NamedInterface*, const string &funcName,
        const string &cond, int arg) throw() = 0;
protected:
    Log(string name) : Fwk::NamedInterface(name) {}
};

class CerrLog : public Log{
public:
    static Fwk::Ptr<CerrLog> instance(string name) {
        if (logger == NULL) 
            logger = new CerrLog(name);
        return logger;
    }
    virtual void entryNew(Priority p, Fwk::NamedInterface* ptr, const string &funcName, const string &cond, int arg) throw() {
        cerr << "In " << funcName << ": " << cond << endl;
    }
protected:
    CerrLog(string name) : Log(name){}

private:
    static Fwk::Ptr<CerrLog> logger;
};

#endif
