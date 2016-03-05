#ifndef AU_LIST
#define AU_LIST

#include <stdexcept>

namespace containers
{
    template<typename T>
    struct au_list
    {
            typedef T value_type;
            typedef value_type& reference;
            typedef value_type const& const_reference;
            typedef value_type* pointer;
            typedef value_type const* const_pointer;
            typedef size_t size_type;

        public:
            struct node
            {
                private:
                    value_type value_;
                    node* prev_;
                    node* next_;

                    friend class au_list;

                public:
                    node( const_reference value, node* prev, node* next )
                        : value_( value )
                        , prev_( prev )
                        , next_( next )
                    {}

                    node( const_reference new_value )
                        : node( new_value, nullptr, nullptr )
                    {}

                    node()
                        : node( 0 )
                    {}

                    reference get_value()
                    {
                        return value_;
                    }

                    const_reference get_value() const
                    {
                        return value_;
                    }

                    void set_value( const_reference new_value )
                    {
                        value_ = new_value;
                    }

                    node* get_prev()
                    {
                        return prev_;
                    }

                    node const* get_prev() const
                    {
                        return prev_;
                    }

                    node* get_next()
                    {
                        return next_;
                    }

                    node const* get_next() const
                    {
                        return next_;
                    }
            };

        public:
            au_list()
            {
                begin_ = new node();
                end_ = begin_;
                size_ = 0;
            }

            ~au_list()
            {
                clear();
                delete begin(); //clear fict. head after all
            }

            au_list( au_list const& rhs )
                : au_list()
            {
                node* ptr = rhs.begin();

                while( ptr != rhs.end() )
                {
                    insert( end(), ptr->get_value() );
                    ptr = ptr->get_next();
                }
            }


            au_list& operator=( au_list rhs )
            {
                return au_list( rhs );
            }


            node* insert( node* position, const_reference value )
            {
                //memorize to be returned later
                node* ptr = new node( value, position->get_prev(), position );

                if( position->get_prev() != nullptr )
                {
                    position->get_prev()->next_ =  ptr;
                }

                position->prev_ = ptr;

                if( position == begin_ )
                {
                    begin_ = ptr;
                }

                ++size_;
                return ptr;
            }


            template<class V>
            node* insert( node* position, V* first, V* last )
            {
                //memorize ptr to be returned later
                node* ptr = insert( position, value_type( *first ) );

                while( first != last )
                {
                    ++first;
                    insert( position, value_type( *first ) );
                }

                return ptr;
            }


            node* erase( node* position )
            {
                //TODO
                //may be wrong logic here..

                if (empty())
                {
                    throw new std::runtime_error("removing from empty list");
                }

                node* ptr_next = position->get_next();
                node* ptr_prev = position->get_prev();

                if( ptr_next != nullptr )
                {
                    ptr_next->prev_ = ptr_prev;
                }

                if( ptr_prev != nullptr )
                {
                    ptr_prev->next_ = ptr_next;
                }
                else
                {
                    //?
                    begin_ = ptr_next;
                }

                delete position;


                size_--;
                return ptr_next;

            }

            node* erase( node* first, node* last )
            {
                while( first != last )
                {
                    first = erase( first );
                }

                return last;
            }


            node* begin() const
            {
                return begin_;
            }

            node* end() const
            {
                return end_;
            }

            bool empty() const
            {
                return begin_ == end_;
            }

            size_type size() const
            {
                return size_;
            }

            void clear()
            {
                node* ptr = begin();

                while( ptr != nullptr )
                {
                    node* temp = ptr;
                    ptr = ptr->get_next();

                    delete temp;
                }

                begin_ = new node();
                end_ = begin_;
                size_ = 0;

            }

            friend std::ostream& operator<<( std::ostream& output, const au_list& list )
            {
                if( !list.empty() )
                {
                    node* ptr = list.begin();

                    while( ptr != nullptr )
                    {
                        output << ptr->get_value() << " ";
                        ptr = ptr->get_next();
                    }
                    output << std::endl;
                }

                return output;
            }

        private:
            node* begin_;
            node* end_;
            size_type size_;
    };



}
#endif // AU_LIST

