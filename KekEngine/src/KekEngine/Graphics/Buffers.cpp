#include "Buffers.h"
#include "glad/glad.h"

namespace Kek
{
    namespace Graphics
    {
        uint STREAM_DRAW = GL_STREAM_DRAW;
        uint STREAM_READ = GL_STREAM_READ;
        uint STREAM_COPY = GL_STREAM_COPY;
        uint STATIC_DRAW = GL_STATIC_DRAW;
        uint STATIC_READ = GL_STATIC_READ;
        uint STATIC_COPY = GL_STATIC_COPY;
        uint DYNAMIC_DRAW = GL_DYNAMIC_DRAW;
        uint DYNAMIC_READ = GL_DYNAMIC_READ;
        uint DYNAMIC_COPY = GL_DYNAMIC_COPY;

        void CopyBuffer(uint readID, uint writeID, uint readByteOffset, uint writeByteOffset, uint byteSize)
        {
            glBindBuffer(GL_COPY_READ_BUFFER, readID);
            glBindBuffer(GL_COPY_WRITE_BUFFER, writeID);
            glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readByteOffset, writeByteOffset, byteSize);
        }

        Buffer::Buffer()
        {
            glGenBuffers(1, &id);
            Bind();
        }
        Buffer::Buffer(uint byteSize, uint usage) : Buffer()
        {
            Reserve(byteSize, usage);
        }

        void Buffer::Reserve(uint byteSize, uint usage)
        {
            int currentSize = 0;
            Bind();
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, (GLint *)&currentSize);

            uint tempBufferID = 0;
            glGenBuffers(1, &tempBufferID);
            glBindBuffer(GL_COPY_WRITE_BUFFER, tempBufferID);
            glBufferData(GL_COPY_WRITE_BUFFER, currentSize, nullptr, GL_STATIC_COPY);

            CopyBuffer(id, tempBufferID, 0, 0, currentSize);

            Bind();
            Data(nullptr, byteSize, usage);
            CopyBuffer(tempBufferID, id, 0, 0, currentSize);

            glDeleteBuffers(1, &tempBufferID);
        }
        void Buffer::Data(void *data, uint byteSize, uint usage) { glBufferData(GL_ARRAY_BUFFER, byteSize, data, usage); }
        void Buffer::SubData(void *data, uint byteSize, uint byteOffset) { glBufferSubData(GL_ARRAY_BUFFER, byteOffset, byteSize, data); }

        uint Buffer::ID() { return id; }
        void Buffer::Bind() { glBindBuffer(GL_ARRAY_BUFFER, id); }
        void Buffer::Delete() { glad_glDeleteBuffers(1, &id); }

        IndexBuffer::IndexBuffer()
        {
            glGenBuffers(1, &id);
            Bind();
        }
        IndexBuffer::IndexBuffer(uint byteSize, uint usage) : IndexBuffer()
        {
            Reserve(byteSize, usage);
        }

        void IndexBuffer::Reserve(uint byteSize, uint usage) { Data(nullptr, byteSize, usage); }
        void IndexBuffer::Data(void *data, uint byteSize, uint usage) { glBufferData(GL_ELEMENT_ARRAY_BUFFER, byteSize, data, usage); }
        void IndexBuffer::SubData(void *data, uint byteSize, uint byteOffset) { glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, byteOffset, byteSize, data); }

        uint IndexBuffer::ID() { return id; }
        void IndexBuffer::Bind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id); }
        void IndexBuffer::Delete() { glad_glDeleteBuffers(1, &id); }

        StorageBuffer::StorageBuffer()
        {
            glGenBuffers(1, &id);
            Bind();
        }
        StorageBuffer::StorageBuffer(uint byteSize, uint usage) : StorageBuffer()
        {
            Reserve(byteSize, usage);
        }

        void StorageBuffer::Reserve(uint byteSize, uint usage) { Data(nullptr, byteSize, usage); }
        void StorageBuffer::Data(void *data, uint byteSize, uint usage) { glBufferData(GL_SHADER_STORAGE_BUFFER, byteSize, data, usage); }
        void StorageBuffer::SubData(void *data, uint byteSize, uint byteOffset) { glBufferSubData(GL_SHADER_STORAGE_BUFFER, byteOffset, byteSize, data); }

        uint StorageBuffer::ID() { return id; }
        void StorageBuffer::Bind() { glBindBuffer(GL_SHADER_STORAGE_BUFFER, id); }
        void StorageBuffer::Delete() { glad_glDeleteBuffers(1, &id); }

        UniformBuffer::UniformBuffer()
        {
            glGenBuffers(1, &id);
            Bind();
        }
        UniformBuffer::UniformBuffer(uint byteSize, uint usage) : UniformBuffer()
        {
            Reserve(byteSize, usage);
        }

        void UniformBuffer::Reserve(uint byteSize, uint usage) { Data(nullptr, byteSize, usage); }
        void UniformBuffer::Data(void *data, uint byteSize, uint usage) { glBufferData(GL_UNIFORM_BUFFER, byteSize, data, usage); }
        void UniformBuffer::SubData(void *data, uint byteSize, uint byteOffset) { glBufferSubData(GL_UNIFORM_BUFFER, byteOffset, byteSize, data); }

        uint UniformBuffer::ID() { return id; }
        void UniformBuffer::Bind() { glBindBuffer(GL_UNIFORM_BUFFER, id); }
        void UniformBuffer::Delete() { glad_glDeleteBuffers(1, &id); }
    }
}