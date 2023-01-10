#pragma once
#include <initializer_list>
typedef unsigned int uint;
namespace Kek
{
	namespace Graphics
	{
		extern uint STREAM_DRAW;
		extern uint STREAM_READ;
		extern uint STREAM_COPY;
		extern uint STATIC_DRAW;
		extern uint STATIC_READ;
		extern uint STATIC_COPY;
		extern uint DYNAMIC_DRAW;
		extern uint DYNAMIC_READ;
		extern uint DYNAMIC_COPY;

		extern void CopyBuffer(uint readID, uint writeID, uint readByteOffset, uint writeByteOffset, uint byteSize);

		class Buffer
		{
			uint id;

		public:
			Buffer();
			Buffer(uint byteSize, uint usage = DYNAMIC_DRAW);
			template <typename T>
			Buffer(T *data, uint size, uint usage = DYNAMIC_DRAW) : Buffer()
			{
				Data(data, size, usage);
			}
			template <typename T>
			Buffer(std::initializer_list<T> data, uint usage = DYNAMIC_DRAW) : Buffer((T *)data.begin(), data.size())
			{
			}

			void Reserve(uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void Data(T *data, uint size, uint usage = DYNAMIC_DRAW)
			{
				Data((void *)data, sizeof(T) * size, usage);
			}
			void Data(void *data, uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void SubData(T *data, uint size, uint offset)
			{
				SubData((void *)data, sizeof(T) * size, sizeof(T) * offset);
			}
			void SubData(void *data, uint byteSize, uint byteOffset);

			uint ID();
			void Bind();
			void Delete();
		};

		class IndexBuffer
		{
			uint id;

		public:
			IndexBuffer();
			IndexBuffer(uint byteSize, uint usage = DYNAMIC_DRAW);
			template <typename T>
			IndexBuffer(T *data, uint size, uint usage = DYNAMIC_DRAW) : IndexBuffer()
			{
				Data(data, size, usage);
			}
			template <typename T>
			IndexBuffer(std::initializer_list<T> data, uint usage = DYNAMIC_DRAW) : IndexBuffer((T *)data.begin(), data.size())
			{
			}

			void Reserve(uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void Data(T *data, uint size, uint usage = DYNAMIC_DRAW)
			{
				Data((void *)data, sizeof(T) * size, usage);
			}
			void Data(void *data, uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void SubData(T *data, uint size, uint offset)
			{
				SubData((void *)data, sizeof(T) * size, sizeof(T) * offset);
			}
			void SubData(void *data, uint byteSize, uint byteOffset);

			uint ID();
			void Bind();
			void Delete();
		};

		class StorageBuffer
		{
			uint id;

		public:
			StorageBuffer();
			StorageBuffer(uint byteSize, uint usage = DYNAMIC_DRAW);
			template <typename T>
			StorageBuffer(T *data, uint size, uint usage = DYNAMIC_DRAW) : StorageBuffer()
			{
				Data(data, size, usage);
			}
			template <typename T>
			StorageBuffer(std::initializer_list<T> data, uint usage = DYNAMIC_DRAW) : StorageBuffer((T *)data.begin(), data.size())
			{
			}

			void Reserve(uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void Data(T *data, uint size, uint usage = DYNAMIC_DRAW)
			{
				Data((void *)data, sizeof(T) * size, usage);
			}
			void Data(void *data, uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void SubData(T *data, uint size, uint offset)
			{
				SubData((void *)data, sizeof(T) * size, sizoef(T) * offset);
			}
			void SubData(void *data, uint byteSize, uint byteOffset);

			uint ID();
			void Bind();
			void Delete();
		};

		class UniformBuffer
		{
			uint id;

		public:
			UniformBuffer();
			UniformBuffer(uint byteSize, uint usage = DYNAMIC_DRAW);
			template <typename T>
			UniformBuffer(T *data, uint size, uint usage = DYNAMIC_DRAW) : UniformBuffer()
			{
				Data(data, size, usage);
			}
			template <typename T>
			UniformBuffer(std::initializer_list<T> data, uint usage = DYNAMIC_DRAW) : UniformBuffer((T *)data.begin(), data.size())
			{
			}

			void Reserve(uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void Data(T *data, uint size, uint usage = DYNAMIC_DRAW)
			{
				Data((void *)data, sizeof(T) * size, usage);
			}
			void Data(void *data, uint byteSize, uint usage = DYNAMIC_DRAW);

			template <typename T>
			void SubData(T *data, uint size, uint offset)
			{
				SubData((void *)data, sizeof(T) * size, sizoef(T) * offset);
			}
			void SubData(void *data, uint byteSize, uint byteOffset);

			uint ID();
			void Bind();
			void Delete();
		};

		// Will be implemented as needed.
		class TextureBuffer
		{
		};
	}
}