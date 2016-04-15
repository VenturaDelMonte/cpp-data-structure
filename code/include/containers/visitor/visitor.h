#pragma once


namespace containers 
{
	template<typename ReturnType> class static_visitor 
	{
		public:
  			typedef ReturnType result_type;
	};

	template<typename VisitorType, typename VisitableType> VisitorType::result_type apply_visitor(VisitorType& visitor, VisitableType& visitable) 
	{
		return visitor(visitable);
	}
}

