#include "reflect/signal/exit.hpp"
#include <stdlib.h>

using namespace zeno::reflect;

class ExitManager : public IExitManager {
    virtual void graceful_exit(uint8_t exit_code) override {
        // TODO: Trigger graceful exit event
        exit(exit_code);
    }
};

IExitManager &zeno::reflect::IExitManager::get()
{
    static ExitManager mgr{};
    return mgr;
}

zeno::reflect::IExitManager::~IExitManager()
{
}