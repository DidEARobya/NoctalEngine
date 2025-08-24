#pragma once

namespace NoctalEngine
{
	class Bindable
	{
	public:
		virtual void Bind() = 0;
		virtual ~Bindable() = default;
	};
}