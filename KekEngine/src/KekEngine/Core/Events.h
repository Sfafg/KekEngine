#pragma once

namespace Kek
{
	template <typename TReturn = void, typename... TInputs>
	struct FunctionPointer
	{
		typedef TReturn(*Pointer)(TInputs...);

		FunctionPointer() :ptr(NULL) {}
		FunctionPointer(TReturn(*ptr)(TInputs...)) : ptr(ptr)
		{
		}
		TReturn operator()(TInputs... in) const
		{
			return ptr(in...);
		}
		operator Pointer() const { return ptr; }

		private:
		Pointer ptr;
	};

	template <typename... TInputs>
	class Event
	{
		FunctionPointer<void, TInputs...>* pointers;
		int size;
		void Realocate(int count)
		{
			if(size == count)return;
			size = count;
			void* ptr = realloc(pointers, size * sizeof(FunctionPointer<void, TInputs...>));
			if(ptr != NULL)
			{
				pointers = (FunctionPointer<void, TInputs...>*)ptr;
			}
		}

		public:
		Event() :pointers(NULL), size(0) {}
		~Event() { delete pointers; }
		Event(const Event<TInputs... >& o)
		{
			if(o.size(0))
			{
				size = 0;
				delete pointers;
				return;
			}
			Realocate(o.Size());
			memcpy(pointers, o.pointers, size * sizeof(FunctionPointer<void, TInputs...>));
		}

		void operator=(const FunctionPointer<void, TInputs...>& ptr)
		{
			Realocate(1);
			pointers[0] = ptr;
		}
		void operator+=(const FunctionPointer<void, TInputs...>& ptr)
		{
			Realocate(size + 1);
			pointers[size - 1] = ptr;
		}
		void operator-=(const FunctionPointer<void, TInputs...>& ptr)
		{
			int i = 0;
			for(i; i < size; i++) if(pointers[i] == ptr) break;
			for(int j = i; j < size - 1; j++) pointers[j] = pointers[j + 1];
			Realocate(size - 1);
		}
		void operator()(TInputs... in) const
		{
			for(int i = 0; i < size; i++)
			{
				pointers[i](in...);
			}
		}

		int Size() const { return size; }
	};
}
