#pragma once

template<typename R, typename... Args>
class CCFunction : public cocos2d::CCObject {
protected:
    std::function<R(Args...)> m_function;
public:
    static CCFunction* create(std::function<R(Args...)> function) {
        auto ret = new CCFunction();
        ret->m_function = function;
        ret->autorelease();
        return ret;
    }

    std::function<R(Args...)> getFunction() const {
        return m_function;
    }

    R execute(Args... args) const {
        return m_function(args...);
    }
};