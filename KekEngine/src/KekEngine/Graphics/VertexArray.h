#pragma once
#include <vector>
#include "Buffers.h"
#include "VertexLayout.h"

namespace Kek
{
    namespace Graphics
    {
        template <typename TVertex, typename TIndex = int>
        class VertexArray
        {
            VertexLayout layout;
            Buffer vertexBuffer;
            IndexBuffer indexBuffer;

            uint vertexCount;
            uint indexCount;

        public:
            uint shape;
            uint usage;
            VertexArray(){}
            VertexArray(std::initializer_list<VertexLayout::Attribute> attrbList, uint shape = TRIANGLES, uint usage = STATIC_DRAW)
                : layout(), vertexBuffer(), indexBuffer(), vertexCount(0), indexCount(0), shape(shape), usage(usage)
            {
                layout.SetAttributes(attrbList);
            }

            void ReserveVertices(uint count)
            {
                vertexCount = count;
                vertexBuffer.Bind();
                vertexBuffer.Reserve(vertexCount * sizeof(TVertex), usage);
            }
            void ReserveIndices(uint count)
            {
                indexCount = count;
                indexBuffer.Bind();
                indexBuffer.Reserve(indexCount * sizeof(TIndex), usage);
            }
            void AddVertices(std::vector<TVertex> vertices)
            {
                ReserveVertices(vertexCount + vertices.size());
                vertexBuffer.SubData(&vertices[0], vertices.size(), vertexCount - vertices.size());
            }
            void AddIndices(const std::vector<TIndex>& indices)
            {
                ReserveIndices(indexCount + indices.size());
                indexBuffer.SubData(&indices[0], indices.size(), indexCount - indices.size());
            }

            void SubVertices(const std::vector<TVertex> &vertices, uint offset)
            {
                vertexBuffer.Bind();
                vertexBuffer.SubData(&vertices[0], vertices.size(), offset);
            }
            void SubIndices(const std::vector<TIndex> &indices, uint offset)
            {
                indexBuffer.Bind();
                indexBuffer.SubData(&indices[0], indices.size(), offset);
            }

            uint VertexCount()
            {
                return vertexCount;
            }
            uint IndexCount()
            {
                return indexCount;
            }

            void Draw(uint startOffset = 0, uint endOffset = 0)
            {
                layout.Bind();
                if (indexCount == 0)
                {
                    layout.Draw(shape, startOffset, vertexCount - endOffset);
                }
                else
                {
                    layout.Draw(shape, startOffset, indexCount - endOffset, TtoType<TIndex>());
                }
            }
        };
    }
}