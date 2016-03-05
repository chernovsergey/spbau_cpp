#ifndef LINKED_PTR
#define LINKED_PTR

#include <algorithm>
#include <functional>

namespace aux
{
    struct Node
    {
            Node()
                : next( nullptr )
                , prev( nullptr )
            {}

            Node( Node const& node )
                : next( node.next )
                , prev( &node )
            {
                if( next ) next->prev = this;
                if( prev ) prev->next = this;
            }

            ~Node()
            {
                if( next ) next->prev = prev;
                if( prev ) prev->next = next;

                next = prev = nullptr;
            }

            bool unique() const
            {
                return next == nullptr && prev == nullptr;
            }

            void swap( Node& object )
            {
                bool with_l = prev == &object;
                bool with_r = next == &object;

                std::swap( prev, object.prev );
                std::swap( next, object.next );

                if( with_l )
                {
                    next = &object;
                    object.prev = this;
                }
                else if( with_r )
                {
                    prev = &object;
                    object.next = this;
                }

                if( prev ) prev->next = this;
                if( next ) next->prev = this;

                if( object.prev ) object.prev->next = &object;
                if( object.next ) object.next->prev = &object;
            }

        private:
            mutable Node const* next;
            mutable Node const* prev;

    }; // Node

} // aux


namespace smart_ptr
{
    using namespace aux;

    template<typename T>
    struct linked_ptr
    {
            using ptr_type = T* ;
            using dltr_type = std::function<void( ptr_type )>;
            #define del_func []( T* resource ) { delete resource; }

            explicit linked_ptr()
                : pntr( nullptr )
            {}

            explicit linked_ptr( std::nullptr_t resource )
                : pntr( resource )
                , dltr( del_func )
            {}

            template<typename U>
            explicit linked_ptr( U* resource )
                : pntr( resource )
                , dltr( del_func )
            {}

            template<typename U>
            linked_ptr( linked_ptr<U> const& resource )
                : pntr( resource.get() )
                , node( resource.get_node() )
                , dltr( del_func )
            {}

            ~linked_ptr()
            {
                if( unique() )
                    dltr( get() );
            }

            linked_ptr& operator=( linked_ptr const& resource )
            {
                return operator=<T>( resource );
            }

            template<typename U>
            linked_ptr<T>& operator=( linked_ptr<U> const& resource )
            {
                linked_ptr<T>( resource ).swap( *this );
                return *this;
            }

            explicit operator bool() const
            {
                return pntr != nullptr;
            }

            void reset()
            {
                linked_ptr( nullptr ).swap( *this );
            }

            template<typename U>
            void reset( U* resource )
            {
                linked_ptr<T>( resource ).swap( *this );
            }

            template<typename U>
            void reset( linked_ptr<U>const& resource )
            {
                linked_ptr<T>( resource ).swap( *this );
            }

            bool unique() const
            {
                if( pntr )
                    return node.unique();
                else
                    return false;
            }

            T* get() const
            {
                return pntr;
            }

            T& operator*() const
            {
                return *pntr;
            }

            T* operator->() const
            {
                return pntr;
            }

            void swap( linked_ptr<T>& obj )
            {
                if( pntr != obj.pntr )
                {
                    std::swap( pntr, obj.pntr );
                    std::swap( dltr, obj.dltr );
                }

                node.swap( obj.node );
            }

            Node const& get_node() const
            {
                return node;
            }

        private:
            ptr_type pntr;
            Node node;
            dltr_type dltr;

    }; // struct linked_ptr<>

    template<typename U>
    bool operator<( linked_ptr<U>const& lhs, linked_ptr<U>const& rhs )
    {
        return lhs.get() < rhs.get();
    }

    template<typename T, typename U>
    bool operator>( linked_ptr<T> const& lhs, linked_ptr<U> const& rhs )
    {
        return rhs < lhs;
    }

    template<typename T, typename U>
    bool operator==( linked_ptr<T> const& lhs, linked_ptr<U>const& rhs )
    {
        return ( lhs <= rhs ) && ( rhs <= lhs );
    }

    template<typename T, typename U>
    bool operator!=( linked_ptr<T> const& lhs, linked_ptr<U> const& rhs )
    {
        return !( lhs == rhs );
    }

    template<typename T, typename U>
    bool operator<=( linked_ptr<T> const& lhs, linked_ptr<U> const& rhs )
    {
        return !( lhs > rhs );
    }

    template<typename T, typename U>
    bool operator>=( linked_ptr<T>const& lhs, linked_ptr<U> const& rhs )
    {
        return !( lhs < rhs );
    }

} // namespace smart_ptr

#endif // LINKED_PTR

