#ifndef LOG_H
#define LOG_H

#include <iostream>
#include <string>
#include "fwk/NamedInterface.h"
#include "fwk/Ptr.h"
#include "fwk/PtrInterface.h"

#define LOG_CRITICAL(f, x) CerrLog::instance()->entryNew(Log::Critical, this->name(), (f), (x), 0)
#define LOG_ERROR(f, x)    CerrLog::instance()->entryNew(Log::Error,    this->name(), (f), (x), 0)
#define LOG_WARNING(f, x)  CerrLog::instance()->entryNew(Log::Warning,  this->name(), (f), (x), 0)
#define LOG_INFO(f, x)     CerrLog::instance()->entryNew(Log::Info,     this->name(), (f), (x), 0)
#define LOG_DEBUG(f, x)    CerrLog::instance()->entryNew(Log::Debug,    this->name(), (f), (x), 0)

using namespace std;

class Log : public Fwk::NamedInterface {
  public:
    enum Priority {
        Null,
        Critical,
        Error,
        Warning,
        Info,
        Debug
    };
    virtual void entryNew(Priority p , Fwk::NamedInterface* ptr, const string &funcName,
        const string &cond, int arg) throw() {
        entryNew(p, ptr->name(), funcName, cond, arg);
    }
    virtual void entryNew(Priority, const string &name, const string &funcName,
        const string &cond, int arg) throw() = 0;
    void logLevelIs(Priority logLevel) { logLevel_ = logLevel; }

  protected:
    Log(string name) : Fwk::NamedInterface(name) {}
    Priority logLevel_;
};

class CerrLog : public Log {
  public:
    static Fwk::Ptr<CerrLog> instance() {
        if (logger == NULL)
            logger = new CerrLog("cerr_log");
        return logger;
    }
    virtual void entryNew(Priority p, const string &name, const string &funcName, const string &cond, int arg) throw() {
        if (p > logLevel_) return; 
        cerr << "[";
        switch (p) {
          case Critical:
            cerr << "CRITICAL"; break;
          case Error:
            cerr << "ERROR"; break;
          case Warning:
            cerr << "WARNING"; break;
          case Info:
            cerr << "INFO"; break;
          case Debug:
            cerr << "DEBUG"; break;
          default:
            cerr << "UNDEFINED"; break;
        }
        cerr << "] " << name << "." << funcName << "(): " << cond << endl;
    }
  protected:
    CerrLog(string name) : Log(name){}

  private:
    static Fwk::Ptr<CerrLog> logger;
};

#endif
