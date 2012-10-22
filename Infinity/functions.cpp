#include "stdafx.h"
#include "functions.h"

namespace Functions
{
	long Monochrome(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().monochrome();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long MaxWidth(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().max_width();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long MaxHeight(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().max_height();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long Model(GetStringParams* p)
	{
		try
		{
			p->result = xop::convert::string_to_handle(Infinity::camera().model());
		}
		catch (xop::exception& e)
		{
			p->result = xop::convert::string_to_handle("");
			return e.code();
		}

		return 0;
	}

	long Exposure(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().exposure();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long Gain(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().gain();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long TargetLeft(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().target_left();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long TargetTop(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().target_top();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long TargetWidth(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().target_width();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long TargetHeight(GetNumberParams* p)
	{
		try
		{
			p->result = Infinity::camera().target_height();
		}
		catch (xop::exception& e)
		{
			SetNaN64(&p->result);
			return e.code();
		}

		return 0;
	}

	long Register(long index)
	{
		switch (index)
		{
		case 0:  return (long) Monochrome;
		case 1:  return (long) MaxWidth;
		case 2:  return (long) MaxHeight;
		case 3:  return (long) Model;
		case 4:  return (long) Exposure;
		case 5:  return (long) Gain;
		case 6:  return (long) TargetLeft;
		case 7:  return (long) TargetTop;
		case 8:  return (long) TargetWidth;
		case 9:  return (long) TargetHeight;
		default: return 0;
		}
	}
}