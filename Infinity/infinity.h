#pragma once

class Infinity
{
#pragma region class Exception

public:
	class Exception : public xop::exception
	{
	public:
		Exception(long code = 0);
	};

#pragma endregion

#pragma region class Image

public:
	class Image
	{
	public:
		Image(ULONG width, ULONG height, bool monochrome = true);
		virtual ~Image();

		ULONG width() const;
		ULONG height() const;
		bool monochrome() const;
		WORD* data();

		void deinterleave();
		void deinterleave(void* image);

	private:
		ULONG _width, _height, _channels, _size;
		bool _monochrome;
		WORD* _data;

		void deinterleave(const WORD* source, WORD* destination);
	};

#pragma endregion

public:
	static Infinity& camera();

	virtual ~Infinity();

	void connect();
	void disconnect();
	void reconnect();
	bool connected() const;

	bool monochrome();
	ULONG max_width();
	ULONG max_height();		
	const std::string& model();
	
	float exposure();
	void set_exposure(float value);
	void set_exposure(double value);

	float gain();
	void set_gain(float value);
	void set_gain(double value);
	
	ULONG target_left() const;
	void set_target_left(ULONG value);

	ULONG target_top() const;
	void set_target_top(ULONG value);

	ULONG target_width() const;
	void set_target_width(ULONG value);

	ULONG target_height() const;
	void set_target_height(ULONG value);
	
	void reset_target();
	void set_target(ULONG left, ULONG top, ULONG width, ULONG height);
	
	Image capture();
	void capture(void* image);
	void capture(void* image, LUCAM_SNAPSHOT* settings);

private:
	struct Target
	{
		ULONG left, top, width, height;
	};

	HANDLE _handle;
	bool _monochrome;
	std::string _model;
	ULONG _max_width;
	ULONG _max_height;
	Target _target;
	Target _actual_target;
	
	Infinity();

	HANDLE handle();

	Exception last_exception();

	float get_property(ULONG code);
	float get_property(ULONG code, long *flags);
	void set_property(ULONG code, float value, long flags = 0);

	void apply_snapshot_settings(LUCAM_SNAPSHOT& s);
	void apply_frame_format(LUCAM_FRAME_FORMAT& f);
	void apply_conversion_settings(LUCAM_CONVERSION& c);

	std::string get_model();

	void raw_capture(void* buffer, LUCAM_SNAPSHOT* settings);
	void convert(Image& buffer, void* image);

	void update_actual_target();
	void update_actual_target_x();
	void update_actual_target_y();

	// singleton: not implemented
	Infinity(const Infinity&);
	void operator=(const Infinity&);
};