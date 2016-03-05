#ifndef EVENT
#define EVENT

#include <functional>
#include <map>
#include <utility>

namespace utils
{

    template<class>
    struct event;


    template<class retval, class ... args>
    struct event<retval( args... )>
    {
        using event_handler_func = std::function<retval( args... )>;
        using event_handler_dltr = std::function<void()>;

        event(){}

        event( event& )  = delete;
        event( event&& ) = default;

        event& operator =( event& )   = delete;
        event& operator =( event && ) = default;

        event_handler_dltr add( event_handler_func handler )
        {
            ++counter;
            store.insert(std::make_pair(counter, handler));
            return [&](){return store.erase(counter);};
        }

        void fire(args... call_arguments)
        {
            for(auto handler : store)
            {
                if(handler.first)
                    handler.second(call_arguments...);
            }
        }

        private:
            size_t counter = 0;
            std::map<size_t, event_handler_func> store;
    };


}

#endif // EVENT

