#pragma once
#include <initializer_list>

typedef unsigned int uint;
namespace Kek
{
    namespace Graphics
    {
        extern uint CHAR;
        extern uint UCHAR;
        extern uint SHORT;
        extern uint USHORT;
        extern uint INT;
        extern uint UINT;
        extern uint FLOAT;
        extern uint HFLOAT;
        extern uint DOUBLE;

        extern uint TypeSize(uint type);
        template <typename T>
        uint TtoType();

        extern uint POINTS;
        extern uint LINES;
        extern uint LINE_LOOP;
        extern uint LINE_STRIP;
        extern uint TRIANGLES;
        extern uint TRIANGLE_STRIP;
        extern uint TRIANGLE_FAN;

        class VertexLayout
        {
            uint id;

        public:
            struct Attribute
            {
                uint count;
                uint type;
                Attribute(uint type, uint count) : type(type), count(count){};
            };

            VertexLayout();
            VertexLayout(std::initializer_list<Attribute> attributeList);

            void SetAttributes(std::initializer_list<Attribute> attributeList);

            void Draw(uint shape, uint first, uint count);
            void Draw(uint shape, uint first, uint count, uint indexType);
            void Bind();
        };
    };
}