#ifndef DETAILS
#define DETAILS

namespace details
{

    template<class KEY, class VALUE>
    struct node
    {
        private:
            KEY key_;
            VALUE val_;
            node* parent_;
            std::unique_ptr<node> left_ptr_;
            std::unique_ptr<node> right_ptr_;

        public:
            node( const KEY& key, const VALUE& value )
                : key_( key )
                , val_( value )
                , parent_( nullptr )
            {}

            ~node() noexcept
            {}

            node( node&& src ) = default;
            node& operator=( node && src ) = default;

            node( const node& src ) = delete;
            node& operator=( const node& src ) = delete;

            void putLeft( std::unique_ptr<node>&& nptr ) noexcept
            {
                left_ptr_ = std::move( nptr );
            }

            std::unique_ptr<node> takeLeft() noexcept
            {
                return std::move( left_ptr_ );
            }

            void putRight( std::unique_ptr<node>&& nptr ) noexcept
            {
                right_ptr_ = std::move( nptr );
            }

            std::unique_ptr<node> takeRight() noexcept
            {
                return std::move( right_ptr_ );
            }

            node*& parent() noexcept
            {
                return parent_;
            }

            node* left() noexcept
            {
                return left_ptr_.get();
            }

            node* right() noexcept
            {
                return right_ptr_.get();
            }

            KEY& key() noexcept
            {
                return key_;
            }

            VALUE& value() noexcept
            {
                return val_;
            }
    };

} // namespace details


#endif // DETAILS

