
namespace linq
{
	// Base interface for all enumerable sequences of type T.
	template<typename T>
	class isequence
	{
	public:
		typedef T value_type;

		// Rewinds the sequence to the beginning.
		// Returns true iff there is a value in the sequence.
		// May be called at any time.
		// May throw (but generally returns false on failure).
		// This must be the first call to this interface.
		virtual bool move_first() const=0;

		// Gets the current value of the sequence.
		// Always succeeds.
		// This method may only be called after move_first() or move_next() return true,
		// otherwise the results are undefined.
		// May throw (but should generally succeed. Use the return value of move_first/next
		// to signal that the sequence has come to an end, and exceptions if the
		// sequence is invalid.)
		virtual const value_type & get_value() const=0;

		// Moves the sequence to the next item.
		// Returns true iff there is a value in the sequence.
		// This may only be called after a true return from move_first() or move_next(),
		// otherwise the results are undefined.
		// May throw (but generally should return false on failure)
		virtual bool move_next() const=0;
	};

	// A reference to a sequence.
	template<typename T>
	class sequence_ref : public methods<sequence_ref<T>, sequence<T>>
	{
	public:
		sequence_ref(const isequence<T> & ref) : m_ref(ref)
		{
		}

		const T & get_value() const { return m_ref.get_value(); }
		bool move_next() const { return m_ref.move_next(); }
		bool move_first() const { return m_ref.move_first(); }

	private:
		const isequence<T> & m_ref;
	};

	// An abstract (pure virtual) class representing a sequence of type T.
	template<typename T>
	class sequence : public methods<sequence<T>, isequence<T>, sequence_ref<T>>
	{
	};

	// !! Split up this file
	template<typename T>
	class sequence_ptr : public methods<sequence_ptr<T>, sequence<T>>
	{
	public:
		typedef std::shared_ptr<isequence<T>> ptr_type;
		typedef T value_type;

		template<typename Derived, typename Base>
		sequence_ptr(const methods<Derived, Base> &e) : m_ptr(std::make_shared<Derived>(e.get()))
		{
		}

		template<typename Derived, typename Base>
		sequence_ptr(methods<Derived, Base> &&e) : m_ptr(std::make_shared<Derived>(std::move(e)))
		{
		}

		//sequence_ptr(const ptr_type & p) : m_ptr(p) { }
		//sequence_ptr(ptr_type && p) : m_ptr(std::move(p)) { }

		bool move_first() const { return m_ptr->move_first(); }
		bool move_next() const { return m_ptr->move_next(); }
		const value_type & get_value() const { return m_ptr->get_value(); }
	private:
		std::shared_ptr<isequence<T>> m_ptr;
	};


	template<typename T>
	bool operator==(const isequence<T> &e1, const isequence<T> &e2)
	{
		bool v1, v2;
		for(v1=e1.move_first(), v2=e2.move_first(); v1&&v2; v1=e1.move_next(), v2=e2.move_next())
		{
			if(e1.get_value() != e2.get_value()) return false;
		}

		return v1==v2;
	}

	template<typename T>
	std::ostream & operator<<(std::ostream & os, const isequence<T> &e)
	{
		os << '{';
		int pos=0;
		for(bool valid = e.move_first(); valid; valid = e.move_next())
		{
			if(pos>0) os << ',';
			os << ' ';
			++pos;
			os << e.get_value();
		}
		if(pos>0) os << ' ';
		os << '}';
		return os;
	}
	
}