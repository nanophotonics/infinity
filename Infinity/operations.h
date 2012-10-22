#pragma once
#include "infinity.h"
#pragma pack(2)

namespace Operations
{
	typedef const char* const const_string;

	struct CallParams
	{
		int from_function;
		int from_macro;
	};

	struct VoidParams
	{
		CallParams called;
	};

	struct ValueParams
	{
		int has_exposure;
		double exposure;
		int exposure_set[1];

		int has_gain;
		double gain;
		int gain_set[1];

		int has_target;
		double left;
		double top;
		double width;
		double height;
		int target_set[4];
	};

	long SetValues(const ValueParams& p);

	namespace Connect
	{
		const_string Template = "Infinity_Connect";
		const_string RuntimeVariables = "";
		const_string RuntimeStrings = "";
		typedef VoidParams Params;
		long Execute(Params* p);
		long Register();
	}

	namespace Disconnect
	{
		const_string Template = "Infinity_Disconnect";
		const_string RuntimeVariables = "";
		const_string RuntimeStrings = "";
		typedef VoidParams Params;
		long Execute(Params* p);
		long Register();
	}

	namespace Reconnect
	{
		const_string Template = "Infinity_Reconnect";
		const_string RuntimeVariables = "";
		const_string RuntimeStrings = "";
		typedef VoidParams Params;
		long Execute(Params* p);
		long Register();
	}

	namespace Set
	{
		const_string Template = "Infinity_Set exposure=number:exposure, gain=number:gain, target={number:left, number:top, number:width, number:height}";
		const_string RuntimeVariables = "";
		const_string RuntimeStrings = "";
		struct Params
		{
			ValueParams values;
			CallParams called;	
		};
		long Execute(Params* p);
		long Register();
	}

	namespace Capture
	{
		const_string Template = "Infinity_Capture image=dataFolderAndName:{image,real}, exposure=number:exposure, gain=number:gain, target={number:left, number:top, number:width, number:height}";
		const_string RuntimeVariables = "";
		const_string RuntimeStrings = "";
		struct Params
		{
			int has_image;
			DataFolderAndName image;
			int image_set[1];

			ValueParams values;
			CallParams called;	
		};
		long Execute(Params* p);
		long Register();
	}

	long Register(long index);
}

#pragma pack()