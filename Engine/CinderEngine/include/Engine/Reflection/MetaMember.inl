namespace Reflection
{

	template <typename MemberType, typename ParentType >
	MetaMember::MetaMember( const NameType & memberName,
							MemberType ParentType::*member,
							const char * description ) :

							mName( memberName ),
							mType( Reflection::MetaTypeToHash< std::remove_pointer< RemQual< MemberType >::Type >::type >( ) ),
							mOffset( offsetof( ParentType, *member ) ),
							mIsPointer( std::is_pointer< MemberType >::value ),
							mContainer( Reflection::CreateContainer( ( ( ParentType* ) 0 )->*member ) ),
							mDescription( description ? description : "" )
	{

	}

	inline void * MetaMember::GetPtr( void * obj ) const
	{
		return reinterpret_cast< void * >( reinterpret_cast< char * >( obj ) +mOffset );
	}

	inline const void * MetaMember::GetPtr( const void * obj ) const
	{
		return reinterpret_cast< const void * >( reinterpret_cast< const char * >( obj ) +mOffset );
	}

	template <typename T>
	T *	MetaMember::GetPtr( void * obj ) const
	{
		return reinterpret_cast< T * >( reinterpret_cast< char * >( obj ) +mOffset );
	}

	template <typename T>
	const T * MetaMember::GetPtr( const void * obj ) const
	{
		return reinterpret_cast< const T * >( reinterpret_cast< const char * >( obj ) +mOffset );
	}

}