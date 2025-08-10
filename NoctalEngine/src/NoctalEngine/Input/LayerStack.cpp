#include "LayerStack.h"
#include "Layer.h"

namespace NoctalEngine
{
	LayerStack::LayerStack()
	{
	}

	LayerStack::~LayerStack()
	{
		for (Layer* layer : m_Layers)
		{
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_LayerInsert = m_Layers.emplace(m_LayerInsert, layer);
	}

	void LayerStack::PopLayer(Layer* layer)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (iterator != m_Layers.end())
		{
			m_Layers.erase(iterator);
			m_LayerInsert--;
		}
	}

	void LayerStack::PushOverlay(Layer* layer)
	{
		m_Layers.emplace_back(layer);
	}

	void LayerStack::PopOverlay(Layer* layer)
	{
		auto iterator = std::find(m_Layers.begin(), m_Layers.end(), layer);

		if (iterator != m_Layers.end())
		{
			m_Layers.erase(iterator);
		}
	}
}

