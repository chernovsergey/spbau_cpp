#ifndef FN
#define FN

#include <tuple>

namespace fn
{
    template <size_t idx>
    struct placeholder
    {
        enum { id = idx };
    };

    static const placeholder<0> _1;
    static const placeholder<1> _2;

    namespace aux
    {
        template <typename fun_t, typename... args_t>
        class binder
        {

            private:
                // argument getters
                template <typename arg_t, typename tuple_t>
                arg_t&& fetch( arg_t&& arg, tuple_t&& )
                {
                    return std::forward<arg_t>( arg );
                }

                template <size_t N, typename tuple_t>
                typename std::tuple_element<N, tuple_t>::type&& fetch( placeholder<N>& placeholder, tuple_t&&  args )
                {
                    return std::forward<typename std::tuple_element<N, tuple_t>::type>( std::get<placeholder.id>( args ) );
                }
                //

                template <typename tuple_t, std::size_t... id>
                decltype( auto ) fetch_and_run( tuple_t&&  args, std::index_sequence<id...> )
                {
                    return f( ( fetch( std::get<id>( call_args ), std::forward<tuple_t>( args ) ) )... );
                }

                template <typename tuple_t>
                decltype( auto ) invoke( tuple_t&& args )
                {
                    constexpr auto T = std::tuple_size<std::tuple<args_t...>>::value;
                    return fetch_and_run( std::forward<tuple_t>( args ), std::make_index_sequence<T> {} );
                }

                fun_t f;
                std::tuple<args_t...> call_args;

            public:
                binder( fun_t const& foo, args_t&&  ... arg )
                    : f( foo )
                    , call_args( std::forward<args_t>( arg )... )
                {}

                template <typename... arguments>
                decltype( auto ) operator()( arguments&&  ... args )
                {
                    return invoke( std::forward_as_tuple( std::forward<arguments>( args )... ) );
                }
        };
    }

    template <typename fun_t, typename ... args_t>
    auto bind( fun_t f, args_t... args )
    {
        return aux::binder<typename std::decay<fun_t>::type, typename std::decay<args_t>::type...>(
                   std::forward<fun_t>( f ),
                   std::forward<args_t>( args )...
               );
    }

    class bad_function_call : public std::exception {};

    template<typename>
    class function;

    template<typename R, typename... args_t>
    class function<R( args_t... )>
    {
            struct base
            {
                virtual ~base() = default;
                virtual base* clone() = 0;
                virtual R operator()( args_t&& ... ) = 0;
            };

            template<typename fun_t>
            struct invocable : base
            {

                invocable( const fun_t& func )
                    : fun( func )
                {}

                invocable( const invocable& rhs )
                    : fun( rhs.fun )
                {}

                invocable* clone() override
                {
                    return new invocable( fun );
                }

                R operator()( args_t&& ...args ) override
                {
                    return fun( std::forward<args_t>( args )... );
                }

                fun_t fun;
            };


            base* invoker;


        public:

            ~function()
            {
                delete invoker;
            }

            template<typename fun_t>
            function( fun_t func )
                : invoker( new invocable<fun_t>( func ) )
            {}

            function( std::nullptr_t )
                : invoker( nullptr )
            {}

            function()
                : invoker( nullptr )
            {}

            function( const function& rhs )
                : invoker( rhs.clone() )
            {}

            function( function&& rhs )
                : function()
            {
                swap( rhs );
            }

            bool empty() const
            {
                return invoker == nullptr;
            }

            operator bool() const
            {
                return !empty();
            }

            function& operator=( function rhs )
            {
                swap( rhs );
                return *this;
            }

            template<typename Functor>
            function& operator=( const Functor& func )
            {
                function tmp( func );
                swap( tmp );
                return *this;
            }

            R operator()( args_t&& ... args ) const
            {
                if( empty() )
                    throw bad_function_call();

                return invoker->operator()( std::forward<args_t>( args )... );
            }

            void swap( function& rhs )
            {
                std::swap( invoker, rhs.invoker );
            }

            base* clone() const
            {
                if( !invoker )
                    return nullptr;

                return invoker->clone();
            }


    };

    template<typename R, typename... args_t>
    void swap( function<R( args_t... )>& lhs, function<R( args_t... )>& rhs )
    {
        lhs.swap( rhs );
    }
}
#endif
