#pragma once
#include "IndexedTriangleList.h"

namespace NoctalEngine
{
	class Quad
	{
	public:
		template<class V>
		static IndexedTriangleList<V> Make()
		{
            float half = 0.5f;

            std::vector<V> vertices(4);
            vertices[0].Pos = { -half, -half, 0.0f };
            vertices[1].Pos = { half, -half, 0.0f };   
            vertices[2].Pos = { -half,  half, 0.0f }; 
            vertices[3].Pos = { half,  half, 0.0f }; 

            std::vector<unsigned int> indices = 
            {
                0, 1, 2,
                2, 3, 0 
            };

            return { std::move(vertices), std::move(indices) };
		}

        template<class V>
        static IndexedTriangleList<V> MakeTextured()
        {
            float half = 0.5f;

            std::vector<V> vertices(4);
            vertices[0].Pos =       { -half, -half, 0.0f };
            vertices[0].TexCoord =  {  0.0f,  0.0f };

            vertices[1].Pos =       {  half, -half, 0.0f };
            vertices[1].TexCoord =  {  1.0f,  0.0f };

            vertices[2].Pos =       {  half,  half, 0.0f };
            vertices[2].TexCoord =  {  1.0f,  1.0f };

            vertices[3].Pos =       { -half,  half, 0.0f };
            vertices[3].TexCoord =  {  0.0f,  1.0f };

            std::vector<unsigned int> indices =
            {
                0, 1, 2,
                2, 3, 0
            };

            return { std::move(vertices), std::move(indices) };
        }
	};
}