#include "stdafx.h"
#include "operations.h"

namespace Operations
{
	long SetValues(const ValueParams& p)
	{
		try
		{
			if (p.has_exposure)
				Infinity::camera().set_exposure(p.exposure);
			if (p.has_gain)
				Infinity::camera().set_gain(p.gain);

			if (p.has_target)
			{
				if (p.left >= 0)
					Infinity::camera().set_target_left((ULONG) p.left);
				if (p.top >= 0)
					Infinity::camera().set_target_top((ULONG) p.top);
				if (p.width >= 0)
					Infinity::camera().set_target_width((ULONG) p.width);
				if (p.height >= 0)
					Infinity::camera().set_target_height((ULONG) p.height);
			}
		}
		catch (xop::exception& e)
		{
			return e.code();
		}
		return 0;
	}


	namespace Connect
	{
		long Execute(Params* p)
		{
			try
			{
				Infinity::camera().connect();
			}
			catch (xop::exception& e)
			{
				return e.code();
			}
			return 0;
		}

		long Register()
		{
			return RegisterOperation(Template, RuntimeVariables, RuntimeStrings, sizeof(Params), Execute, 0);
		}
	}

	namespace Disconnect
	{
		long Execute(Params* p)
		{
			Infinity::camera().disconnect();
			return 0;
		}

		long Register()
		{
			return RegisterOperation(Template, RuntimeVariables, RuntimeStrings, sizeof(Params), Execute, 0);
		}
	}

	namespace Reconnect
	{
		long Execute(Params* p)
		{
			try
			{
				Infinity::camera().reconnect();
			}
			catch (xop::exception& e)
			{
				return e.code();
			}
			return 0;
		}

		long Register()
		{
			return RegisterOperation(Template, RuntimeVariables, RuntimeStrings, sizeof(Params), Execute, 0);
		}
	}

	namespace Set
	{
		long Execute(Params* p)
		{
			return SetValues(p->values);
		}

		long Register()
		{
			return RegisterOperation(Template, RuntimeVariables, RuntimeStrings, sizeof(Params), Execute, 0);
		}
	}

	namespace Capture
	{
		long Execute(Params* p)
		{
			if (long error = SetValues(p->values)) return error;
						
			try
			{
				if (p->has_image)
				{
					Infinity& camera = Infinity::camera();
					bool monochrome = camera.monochrome();
					xop::wave<WORD> image(std::string(p->image.name), p->image.dfH, camera.target_width(), camera.target_height(), monochrome ? 0 : 3);
					
					if (monochrome)
					{
						camera.capture(image.data());
					}
					else
					{
						camera.capture().deinterleave(image.data());
					}
				}
			}
			catch (xop::exception& e)
			{
				return e.code();
			}
			return 0;
		}

		long Register()
		{
			return RegisterOperation(Template, RuntimeVariables, RuntimeStrings, sizeof(Params), Execute, 0);
		}
	}

	long Register(long index)
	{
		switch (index)
		{
		case 0:  return Connect::Register();
		case 1:  return Disconnect::Register();
		case 2:  return Reconnect::Register();
		case 3:  return Set::Register();
		case 4:  return Capture::Register();
		default: return -1;
		}
	}
}
