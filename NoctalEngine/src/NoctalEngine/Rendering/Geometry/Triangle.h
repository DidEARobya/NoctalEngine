#pragma once
#include "IndexedTriangleList.h"

namespace NoctalEngine
{
    class Triangle
    {
    public:
        template<class V>
        static IndexedTriangleList<V> Make()
        {
            float half = 0.5f;

            std::vector<V> vertices(3);
            vertices[0].Pos = { -half, -half, 0.0f };
            vertices[1].Pos = { half, -half, 0.0f };
            vertices[2].Pos = { 0.0f,  half, 0.0f };

            std::vector<unsigned int> indices =
            {
                0, 1, 2
            };

            return { std::move(vertices), std::move(indices) };
        }

        template<class V>
        static IndexedTriangleList<V> MakeTextured()
        {
            float half = 0.5f;

            std::vector<V> vertices(3);
            vertices[0].Pos =       { -half, -half, 0.0f };
            vertices[0].TexCoord =  {  0.0f,  0.0f };

            vertices[1].Pos =       {  half, -half, 0.0f };
            vertices[1].TexCoord =  {  1.0f,  0.0f };

            vertices[2].Pos =       {  0.0f,  half, 0.0f };
            vertices[2].TexCoord =  {  0.5f,  1.0f };

            std::vector<unsigned int> indices =
            {
                0, 1, 2
            };

            return { std::move(vertices), std::move(indices) };
        }
    };
}