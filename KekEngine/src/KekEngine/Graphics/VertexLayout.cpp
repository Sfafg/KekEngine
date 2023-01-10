#include "VertexLayout.h"
#include <map>
#include "glad/glad.h"

namespace Kek
{
    namespace Graphics
    {
        uint CHAR = GL_BYTE;
        uint UCHAR = GL_UNSIGNED_BYTE;
        uint SHORT = GL_SHORT;
        uint USHORT = GL_UNSIGNED_SHORT;
        uint INT = GL_INT;
        uint UINT = GL_UNSIGNED_INT;
        uint FLOAT = GL_FLOAT;
        uint HFLOAT = GL_HALF_FLOAT;
        uint DOUBLE = GL_DOUBLE;

        extern uint TypeSize(uint type)
        {
            static const std::map<uint, uint> sizeMap = {
                {CHAR, sizeof(char)},
                {UCHAR, sizeof(unsigned char)},
                {SHORT, sizeof(short)},
                {USHORT, sizeof(unsigned short)},
                {INT, sizeof(int)},
                {UINT, sizeof(unsigned int)},
                {FLOAT, sizeof(float)},
                {HFLOAT, sizeof(float) / 2},
                {DOUBLE, sizeof(double)}};

            return sizeMap.at(type);
        }
        template <>
        uint TtoType<char>() { return CHAR; }
        template <>
        uint TtoType<unsigned char>() { return UCHAR; }
        template <>
        uint TtoType<short>() { return SHORT; }
        template <>
        uint TtoType<unsigned short>() { return USHORT; }
        template <>
        uint TtoType<int>() { return INT; }
        template <>
        uint TtoType<unsigned int>() { return UINT; }
        template <>
        uint TtoType<float>() { return FLOAT; }
        template <>
        uint TtoType<double>() { return DOUBLE; }

        uint POINTS = GL_POINTS;
        uint LINES = GL_LINES;
        uint LINE_LOOP = GL_LINE_LOOP;
        uint LINE_STRIP = GL_LINE_STRIP;
        uint TRIANGLES = GL_TRIANGLES;
        uint TRIANGLE_STRIP = GL_TRIANGLE_STRIP;
        uint TRIANGLE_FAN = GL_TRIANGLE_FAN;

        VertexLayout::VertexLayout()
        {
            glGenVertexArrays(1, &id);
            Bind();
        }
        VertexLayout::VertexLayout(std::initializer_list<Attribute> attributeList) : VertexLayout()
        {
            SetAttributes(attributeList);
        }

        void VertexLayout::SetAttributes(std::initializer_list<Attribute> attributeList)
        {
            uint byteStride = 0;
            for (int i = 0; i < attributeList.size(); i++)
            {
                const Attribute &attribute = attributeList.begin()[i];
                byteStride += attribute.count * TypeSize(attribute.type);
            }

            uint byteOffset = 0;
            for (int i = 0; i < attributeList.size(); i++)
            {
                const Attribute &attribute = attributeList.begin()[i];
                glVertexAttribPointer(i, attribute.count, attribute.type, GL_FALSE, byteStride, (void *)byteOffset);
                glEnableVertexAttribArray(i);
                byteOffset += attribute.count * TypeSize(attribute.type);
            }
        }

        void VertexLayout::Draw(uint shape, uint first, uint count)
        {
            glDrawArrays(shape, first, count);
        }

        void VertexLayout::Draw(uint shape, uint first, uint count, uint indexType)
        {
            glDrawElements(shape, count, indexType, (void *)first);
        }

        void VertexLayout::Bind()
        {
            glBindVertexArray(id);
        }

    };
}