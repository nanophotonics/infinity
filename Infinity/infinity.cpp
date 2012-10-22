#include "stdafx.h"
#include "infinity.h"

Infinity& Infinity::camera()
{
	static Infinity camera;
	return camera;
}

Infinity::Infinity() : _handle(NULL)
{
}

Infinity::~Infinity()
{
	disconnect();
}

void Infinity::connect()
{
	if (connected()) return;

	_handle = LucamCameraOpen(1);
	if (!_handle) throw last_exception();

	_monochrome = get_property(LUCAM_PROP_COLOR_FORMAT) == LUCAM_CF_MONO;
	_max_width = (ULONG) get_property(LUCAM_PROP_MAX_WIDTH);
	_max_height = (ULONG) get_property(LUCAM_PROP_MAX_HEIGHT);
	_model = get_model();
	reset_target();
}

void Infinity::disconnect()
{
	if (connected())
	{
		LucamCameraClose(_handle);
		_handle = NULL;
	}
}

void Infinity::reconnect()
{
	disconnect();
	connect();
}

bool Infinity::connected() const
{
	return _handle != NULL;
}

HANDLE Infinity::handle()
{
	connect();
	return _handle;
}

bool Infinity::monochrome()
{
	connect();
	return _monochrome;
}

ULONG Infinity::max_width()
{
	connect();
	return _max_width;
}
	
ULONG Infinity::max_height()
{
	connect();
	return _max_height;
}
		
const std::string& Infinity::model()
{
	connect();
	return _model;
}

Infinity::Image Infinity::capture()
{
	LUCAM_SNAPSHOT settings;
	snapshot_settings(settings);

	Image image(settings.format.width, settings.format.height, monochrome());

	capture(image.data(), &settings);

	return image;
}

void Infinity::capture(void* image)
{
	LUCAM_SNAPSHOT settings;
	snapshot_settings(settings);

	capture(image, &settings);
}

void Infinity::capture(void* image, LUCAM_SNAPSHOT* settings)
{
	if (monochrome())
	{
		raw_capture(image, settings);
	}
	else
	{
		Image buffer(settings->format.width, settings->format.height);
		raw_capture(buffer.data(), settings);
		convert(buffer, image);
	}
}

void Infinity::raw_capture(void* buffer, LUCAM_SNAPSHOT* settings)
{
	if (!LucamTakeSnapshot(handle(), settings, (BYTE*) buffer)) throw last_exception();
}
	
void Infinity::convert(Image& buffer, void* image)
{
	LUCAM_CONVERSION settings;
	conversion_settings(settings);

	if (!LucamConvertFrameToRgb48(handle(), (WORD*) image, buffer.data(), buffer.width(), buffer.height(), LUCAM_PF_16, &settings)) throw last_exception();
}

float Infinity::exposure()
{
	return get_property(LUCAM_PROP_STILL_EXPOSURE);
}

void Infinity::set_exposure(float value)
{
	set_property(LUCAM_PROP_STILL_EXPOSURE, value);
}

void Infinity::set_exposure(double value)
{
	set_exposure((float) value);
}

float Infinity::gain()
{
	return get_property(LUCAM_PROP_GAIN);
}

void Infinity::set_gain(float value)
{
	set_property(LUCAM_PROP_GAIN, value);
}

void Infinity::set_gain(double value)
{
	set_gain((float) value);
}

ULONG Infinity::target_left() const
{
	return _actual_target.left;
}

void Infinity::set_target_left(ULONG value)
{
	_target.left = value;
	update_actual_target_x();
}

ULONG Infinity::target_top() const
{
	return _actual_target.top;
}

void Infinity::set_target_top(ULONG value)
{
	_target.top = value;
	update_actual_target_y();
}

ULONG Infinity::target_width() const
{
	return _actual_target.width;
}

void Infinity::set_target_width(ULONG value)
{
	_target.width = value;
	update_actual_target_x();
}

ULONG Infinity::target_height() const
{
	return _actual_target.height;
}

void Infinity::set_target_height(ULONG value)
{
	_target.height = value;
	update_actual_target_y();
}

void Infinity::reset_target()
{
	_target.left = 0;
	_target.top = 0;
	_target.width = max_width();
	_target.height = max_height();

	_actual_target = _target;
}

void Infinity::set_target(ULONG left, ULONG top, ULONG width, ULONG height)
{
	_target.left = left;
	_target.top = top;
	_target.width = width;
	_target.height = height;

	update_actual_target();
}

void Infinity::update_actual_target()
{
	update_actual_target_x();
	update_actual_target_y();
}

void Infinity::update_actual_target_x()
{
	_actual_target.left = _target.left /* - target.left % 2 */;
	_actual_target.width = min(max_width() - _actual_target.left, _target.width /* + _target.width % 2 */);
}

void Infinity::update_actual_target_y()
{
	_actual_target.top = _target.top /* - target.top % 2 */;
	_actual_target.height = min(max_height() - _actual_target.top, _target.height /* + _target.height % 2 */);
}

float Infinity::get_property(ULONG code)
{
	long flags;
	return get_property(code, &flags);
}

float Infinity::get_property(ULONG code, long *flags)
{
	float value;
	LucamGetProperty(handle(), code, &value, flags);
	return value;
}

void Infinity::set_property(ULONG code, float value, long flags)
{
	if (!LucamSetProperty(handle(), code, value, flags)) throw last_exception();
}

std::string Infinity::get_model()
{
	ULONG type;
	if (!LucamGetCameraId(handle(), &type)) throw last_exception();

	switch (type)
	{
	case 0x0A0: return "Infinity X-21";
	case 0x0A1: return "Infinity 1";
	case 0x1AC: return "Infinity 1-5";
	case 0x1A6: return "Infinity 1-6";
	case 0x0A2: return "Infinity 2";
	case 0x1A2: return "Infinity 2-1";
	case 0x1A7: return "Infinity 2-2";
	case 0x1A4: return "Infinity 2-3";
	case 0x0A3: return "Infinity 3";
	case 0x1A5: return "Infinity 3-1";
	case 0x0A4: return "Infinity 4";
	case 0x1AB: return "Infinity 4-4";
	case 0x1A8: return "Infinity 4-11";
	default:    return "Unknown";
	}
}

void Infinity::snapshot_settings(LUCAM_SNAPSHOT& s)
{
	s.exposure = exposure();
	s.gain = gain();

	s.gainRed = get_property(LUCAM_PROP_GAIN_RED);
	s.gainBlue = get_property(LUCAM_PROP_GAIN_BLUE);
	s.gainGrn1 = get_property(LUCAM_PROP_GAIN_GREEN1);
	s.gainGrn2 = get_property(LUCAM_PROP_GAIN_GREEN2);

	s.shutterType = LUCAM_SHUTTER_TYPE_GLOBAL;
	s.timeout = s.exposure + 10000;
	frame_format(s.format);
}

void Infinity::frame_format(LUCAM_FRAME_FORMAT& f)
{
	f.xOffset = _actual_target.left;
	f.yOffset = _actual_target.top;
	f.width = _actual_target.width;
	f.height = _actual_target.height;

	f.pixelFormat = LUCAM_PF_16;
	f.subSampleX = 1;
	f.subSampleY = 1;
}

void Infinity::conversion_settings(LUCAM_CONVERSION& c)
{
	c.CorrectionMatrix = LUCAM_CM_FLUORESCENT;
	c.DemosaicMethod = LUCAM_DM_HIGH_QUALITY;
}

Infinity::Exception Infinity::last_exception()
{
	ULONG code = 0;
	if (_handle) code = LucamGetLastErrorForCamera(_handle);
	if (code == 0) code = LucamGetLastError();
	return Exception((long) code);
}


#pragma region Infinity::Exception

Infinity::Exception::Exception(long code) : xop::exception(code > 0 ? FIRST_XOP_ERR + code : WM_UNKNOWN_ERROR)
{
}

#pragma endregion


#pragma region Infinity::Image

Infinity::Image::Image(ULONG width, ULONG height, bool monochrome) : _width(width), _height(height), _monochrome(monochrome)
{
	_size = _width * _height * (monochrome ? 1 : 3);
	_data = new WORD[_size];
}

Infinity::Image::~Image()
{
	delete [] _data;
}

ULONG Infinity::Image::width() const
{
	return _width;
}

ULONG Infinity::Image::height() const
{
	return _height;
}

bool Infinity::Image::monochrome() const
{
	return _monochrome;
}

WORD* Infinity::Image::data()
{
	return _data;
}

// In-place deinterleave
void Infinity::Image::deinterleave()
{
	if (_monochrome) return;

	WORD* copy = new WORD[_size];
	memcpy(copy, _data, _size);

	deinterleave(copy, _data);

	delete [] copy;
}

// Deinterleave into external buffer
void Infinity::Image::deinterleave(void* image)
{
	if (_monochrome)
	{
		memcpy(image, _data, _size);
		return;
	}
	
	deinterleave(_data, (WORD*) image);
}

void Infinity::Image::deinterleave(const WORD* source, WORD* destination)
{
	ULONG size = _width * _height;
	WORD* r = destination;
	WORD* g = r + size;
	WORD* b = g + size;

	for (ULONG i = 0; i < size; i++)
	{
		*b++ = *source++;
		*g++ = *source++;
		*r++ = *source++;
	}
}

#pragma endregion
